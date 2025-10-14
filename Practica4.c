// sortbench.c — Bench de Merge Sort vs Quick Sort con saltos en tamaños grandes
// - Lee enteros de un .txt (uno por línea o separados por espacios)
// - Prepara MEDIO (aleatorio), MEJOR (ascendente) y PEOR (descendente) una sola vez por N
// - Reutiliza buffers y puede ejecutar escenarios en paralelo (OpenMP opcional)
// - Omite QUICK en MEJOR a partir de 1e6 (por costo O(n^2) con pivote final)
// - Omite QUICK en PEOR a partir de 1e6 (opcional, ajustable)
// Compila (Windows MinGW): gcc -O3 -march=native -DNDEBUG -fopenmp -o sortbench.exe sortbench.c
// Compila (Linux/macOS):  gcc -O3 -march=native -DNDEBUG -fopenmp -o sortbench     sortbench.c
// Ejecuta: ./sortbench numeros.txt   (o sin arg: buscará numeros.txt en el directorio)

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/* ===== Temporizador cross-platform ===== */
#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
typedef struct { LARGE_INTEGER f, t0, t1; } Timer;
static void timer_start(Timer* tm){ QueryPerformanceFrequency(&tm->f); QueryPerformanceCounter(&tm->t0); }
static double timer_ms(Timer* tm){ QueryPerformanceCounter(&tm->t1); return (tm->t1.QuadPart - tm->t0.QuadPart)*1000.0/(double)tm->f.QuadPart; }
#else
#include <time.h>
typedef struct { struct timespec t0, t1; } Timer;
static void timer_start(Timer* tm){ clock_gettime(CLOCK_MONOTONIC, &tm->t0); }
static double timer_ms(Timer* tm){
    clock_gettime(CLOCK_MONOTONIC, &tm->t1);
    return (tm->t1.tv_sec - tm->t0.tv_sec)*1000.0 + (tm->t1.tv_nsec - tm->t0.tv_nsec)/1e6;
}
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

typedef struct { unsigned long long comps, moves; } Metrics;

/* ===== Parámetros de flujo (puedes ajustar) ===== */
#define SKIP_WORST_AFTER       1000000ULL   /* omite QUICK en PEOR >= 1e6 */
#define SKIP_QSORT_BEST_AFTER  1000000ULL   /* omite QUICK en MEJOR >= 1e6 */

/* ===== Utilidades de casos ===== */
static int cmp_int_asc(const void* a, const void* b){
    int x = *(const int*)a, y = *(const int*)b;
    return (x>y) - (x<y);
}
static void reverse_ints(int *A, size_t n){
    for(size_t i=0,j=n? n-1:0; i<j; ++i,--j){ int t=A[i]; A[i]=A[j]; A[j]=t; }
}
static void shuffle_ints(int *A, size_t n, unsigned int seed){
    if(n<=1) return;
    srand(seed);
    for(size_t i=n-1; i>0; --i){
        size_t j=(size_t)(rand()%(int)(i+1));
        int t=A[i]; A[i]=A[j]; A[j]=t;
    }
}

/* ===== Merge Sort ===== */
static void merge_core(int *A, int *aux, size_t l, size_t m, size_t r, Metrics *mt){
    size_t i=l, j=m, k=l;
    while(i<m && j<r){
        mt->comps++;
        if(A[i] <= A[j]){ aux[k++]=A[i++]; mt->moves++; }
        else            { aux[k++]=A[j++]; mt->moves++; }
    }
    while(i<m){ aux[k++]=A[i++]; mt->moves++; }
    while(j<r){ aux[k++]=A[j++]; mt->moves++; }
    for(size_t t=l; t<r; ++t){ A[t]=aux[t]; mt->moves++; }
}
static void mergesort_rec(int *A, int *aux, size_t l, size_t r, Metrics *mt){
    if(r - l <= 1) return;
    size_t m = l + ((r - l) >> 1);
    mergesort_rec(A, aux, l, m, mt);
    mergesort_rec(A, aux, m, r, mt);
    merge_core(A, aux, l, m, r, mt);
}
static void mergesort_count(int *A, size_t n, Metrics *mt, int *aux){
    mergesort_rec(A, aux, 0, n, mt);
}

/* ===== Quick Sort (Lomuto iterativo, pivote final) ===== */
typedef struct { size_t lo, hi; } Frame;
static size_t part_lomuto(int *A, size_t lo, size_t hi, Metrics *mt){
    int pivot = A[hi];
    size_t i = lo;
    for(size_t j=lo; j<hi; ++j){
        mt->comps++;
        if(A[j] <= pivot){
            if(i!=j){ int t=A[i]; A[i]=A[j]; A[j]=t; mt->moves+=3; }
            i++;
        }
    }
    if(i!=hi){ int t=A[i]; A[i]=A[hi]; A[hi]=t; mt->moves+=3; }
    return i;
}
static void quicksort_count(int *A, size_t n, Metrics *mt){
    if(n==0) return;
    size_t cap=64, top=0;
    Frame *st=(Frame*)malloc(cap*sizeof(Frame));
    st[top++] = (Frame){0, n-1};
    while(top){
        Frame f = st[--top];
        if(f.lo>=f.hi) continue;
        size_t p = part_lomuto(A, f.lo, f.hi, mt);
        size_t ln = (p>0? p - f.lo : 0), rn = (f.hi>p? f.hi - p : 0);
        Frame a={f.lo, p? p-1:0}, b={p+1, f.hi};
        Frame big = (ln>rn)? a : b, small = (ln>rn)? b : a;
        if(top+2>cap){ cap*=2; st=(Frame*)realloc(st, cap*sizeof(Frame)); }
        st[top++] = big;
        st[top++] = small;
    }
    free(st);
}

/* ===== Entrada ===== */
static size_t load_ints_from_file(const char *path, int **out, size_t limit){
    FILE *f = fopen(path, "rb");
    if(!f) return 0;
    size_t cap = 1<<20, n=0;
    int *arr = (int*)malloc(cap*sizeof(int));
    if(!arr){ fprintf(stderr,"Sin memoria\n"); exit(1); }
    for(;;){
        int x; int r = fscanf(f, "%d", &x);
        if(r!=1) break;
        if(limit && n==limit) break;
        if(n==cap){
            cap*=2;
            int *tmp=(int*)realloc(arr, cap*sizeof(int));
            if(!tmp){ fprintf(stderr,"Sin memoria\n"); exit(1); }
            arr=tmp;
        }
        arr[n++]=x;
    }
    fclose(f);
    *out = arr;
    return n;
}

/* ===== Impresión/benchmark ===== */
static void print_header(void){
    printf("-----------------------------------------------------------------------------------------------\n");
    printf("        N    | CASO  | ALGORITMO |     COMPARACIONES |           MOVES |     TIEMPO\n");
    printf("-----------------------------------------------------------------------------------------------\n");
}
static void print_omitted(size_t n, const char *caso, const char *algo, const char *motivo){
    printf("%12zu | %-5s | %-8s | %15s | %15s | %s\n", n, caso, algo, "-", "-", motivo);
}

static void bench_case(const char *caso, const char *algo,
                       void (*sortfun_q)(int*, size_t, Metrics*),
                       void (*sortfun_m)(int*, size_t, Metrics*, int*),
                       int *base, size_t n, int *work, int *auxMerge)
{
    memcpy(work, base, n*sizeof(int));
    Metrics mt={0,0}; Timer tm; timer_start(&tm);
    if(sortfun_m) sortfun_m(work, n, &mt, auxMerge);
    else          sortfun_q(work, n, &mt);
    double ms = timer_ms(&tm);
    printf("%12zu | %-5s | %-8s | %15llu | %15llu | %10.3f ms\n",
           n, caso, algo, mt.comps, mt.moves, ms);
}

int main(int argc, char **argv){
    const char *path = (argc>=2)? argv[1] : "numeros.txt";
    size_t limit = (argc>=3)? (size_t)strtoull(argv[2], NULL, 10) : 0;

    int *all=NULL; size_t total = load_ints_from_file(path, &all, limit);
    if(total==0){
        fprintf(stderr,
            "No pude abrir/leer '%s'. Coloca tu archivo como 'numeros.txt' o ejecuta: ./sortbench <archivo> [limite]\n",
            path);
        return 1;
    }

    size_t Ns[] = {10ULL,100ULL,1000ULL,10000ULL,100000ULL,1000000ULL,10000000ULL};
    size_t kN = sizeof(Ns)/sizeof(Ns[0]);

    print_header();

    for(size_t i=0;i<kN;i++){
        size_t n = Ns[i];
        if(n > total) break;

        /* bases por N */
        int *baseAsc  =(int*)malloc(n*sizeof(int));
        int *baseDesc =(int*)malloc(n*sizeof(int));
        int *baseRand =(int*)malloc(n*sizeof(int));
        int *work     =(int*)malloc(n*sizeof(int));
        int *auxMerge =(int*)malloc(n*sizeof(int));
        if(!baseAsc||!baseDesc||!baseRand||!work||!auxMerge){ fprintf(stderr,"Sin memoria\n"); exit(1); }

        memcpy(baseAsc, all, n*sizeof(int));
        qsort(baseAsc, n, sizeof(int), cmp_int_asc);
        memcpy(baseDesc, baseAsc, n*sizeof(int)); reverse_ints(baseDesc, n);
        memcpy(baseRand, baseAsc, n*sizeof(int));
        shuffle_ints(baseRand, n, (unsigned)(1469598103U ^ (unsigned)n));

        /* tareas por flujo (si no hay OpenMP, se ejecutan en serie ignorando los pragmas) */
        #pragma omp parallel
        {
            #pragma omp single nowait
            {
                /* MEDIO */
                #pragma omp task shared(baseRand,work,auxMerge) firstprivate(n)
                bench_case("MEDIO","MERGE",NULL,mergesort_count, baseRand,n,work,auxMerge);
                #pragma omp task shared(baseRand,work) firstprivate(n)
                bench_case("MEDIO","QUICK",quicksort_count,NULL, baseRand,n,work,NULL);

                /* MEJOR */
                #pragma omp task shared(baseAsc,work,auxMerge) firstprivate(n)
                bench_case("MEJOR","MERGE",NULL,mergesort_count, baseAsc,n,work,auxMerge);

                if(n < SKIP_QSORT_BEST_AFTER){
                    #pragma omp task shared(baseAsc,work) firstprivate(n)
                    bench_case("MEJOR","QUICK",quicksort_count,NULL, baseAsc,n,work,NULL);
                }else{
                    print_omitted(n, "MEJOR", "QUICK", "omitido (O(n^2) en ordenado >= 1e6)");
                }

                /* PEOR */
                #pragma omp task shared(baseDesc,work,auxMerge) firstprivate(n)
                bench_case("PEOR","MERGE",NULL,mergesort_count, baseDesc,n,work,auxMerge);

                if(n < SKIP_WORST_AFTER){
                    #pragma omp task shared(baseDesc,work) firstprivate(n)
                    bench_case("PEOR","QUICK",quicksort_count,NULL, baseDesc,n,work,NULL);
                }else{
                    print_omitted(n, "PEOR", "QUICK", "omitido (coste cuadrático >= 1e6)");
                }
            }
        }

        printf("-----------------------------------------------------------------------------------------------\n");

        free(baseAsc); free(baseDesc); free(baseRand);
        free(work); free(auxMerge);
    }

    free(all);
    return 0;
}
