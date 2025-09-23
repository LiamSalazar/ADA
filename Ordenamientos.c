/* 
Alarcón Ruiz Sergio Fernando
Salazar Martínez Liam Antonio
Vargas Nicolás Bianca Celeste 
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SIZE 10000        
#define AVG_TRIALS 5      
void copyArray(const int *src, int *dst, int n) {
    memcpy(dst, src, n * sizeof(int));
}

void generateBestCase(int *a, int n) {
    for (int i = 0; i < n; ++i) a[i] = i;              // ascendente
}

void generateWorstCase(int *a, int n) {
    for (int i = 0; i < n; ++i) a[i] = n - i;          // descendente
}

void generateAverageCase(int *a, int n) {
    for (int i = 0; i < n; ++i) a[i] = rand() % 100000; // aleatorio
}

void bubbleSort(int *arr, int n, long long *ops) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            (*ops)++; // comparación
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                (*ops) += 3; // intercambio
            }
        }
    }
}

void selectionSort(int *arr, int n, long long *ops) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            (*ops)++; // comparación
            if (arr[j] < arr[minIndex]) minIndex = j;
        }
        if (minIndex != i) {
            int tmp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = tmp;
            (*ops) += 3; // intercambio
        }
    }
}

void insertionSort(int *arr, int n, long long *ops) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        (*ops)++; 
        while (j >= 0 && arr[j] > key) {
            (*ops)++; // comparación verdadera (arr[j] > key)
            arr[j + 1] = arr[j];
            j--;
            (*ops)++; // movimiento
        }
        arr[j + 1] = key;
        (*ops)++; // inserción final
    }
}

typedef void (*sort_fn)(int*, int, long long*);

void medir_un_caso(const char *nombre_algo,
                    const char *nombre_caso,
                    const int *base,
                    sort_fn ordena,
                    long long *ops_out,
                    double *time_out) {
    int *arr = (int*)malloc(SIZE * sizeof(int));
    copyArray(base, arr, SIZE);

    long long ops = 0;
    clock_t t0 = clock();
    ordena(arr, SIZE, &ops);
    clock_t t1 = clock();
    free(arr);

    *ops_out = ops;
    *time_out = (double)(t1 - t0) / CLOCKS_PER_SEC;
    (void)nombre_algo; (void)nombre_caso; 
}

void medir_promedio(const char *nombre_algo,
                    const int *plantilla,   
                    sort_fn ordena,
                    long long *ops_avg_out,
                    double *time_avg_out) {
    long long ops_sum = 0;
    double   time_sum = 0.0;
    (void)plantilla; // no se usa

    for (int k = 0; k < AVG_TRIALS; ++k) {
        int *arr = (int*)malloc(SIZE * sizeof(int));
        generateAverageCase(arr, SIZE);

        long long ops = 0;
        clock_t t0 = clock();
        ordena(arr, SIZE, &ops);
        clock_t t1 = clock();

        ops_sum  += ops;
        time_sum += (double)(t1 - t0) / CLOCKS_PER_SEC;
        free(arr);
    }
    *ops_avg_out  = ops_sum / AVG_TRIALS;
    *time_avg_out = time_sum / AVG_TRIALS;
    (void)nombre_algo;
}

void imprimir_encabezado() {
    printf("\nRESULTADOS (n = %d)\n", SIZE);
    printf("Algoritmo           | Caso       | Operaciones         | Tiempo (s)\n");
    printf("--------------------+------------+----------------------+-----------\n");
}

void imprimir_renglon(const char *alg, const char *caso, long long ops, double t) {
    printf("%-20s | %-10s | %20lld | %9.6f\n", alg, caso, ops, t);
}

int main(void) {
    srand((unsigned)time(NULL));

    int *best = (int*)malloc(SIZE * sizeof(int));
    int *worst = (int*)malloc(SIZE * sizeof(int));
    generateBestCase(best, SIZE);
    generateWorstCase(worst, SIZE);

    imprimir_encabezado();

    long long ops; double tiempo;

    medir_un_caso("Burbuja", "Mejor", best, bubbleSort, &ops, &tiempo);
    imprimir_renglon("Burbuja", "Mejor", ops, tiempo);

    medir_promedio("Burbuja", NULL, bubbleSort, &ops, &tiempo);
    imprimir_renglon("Burbuja", "Promedio", ops, tiempo);

    medir_un_caso("Burbuja", "Peor", worst, bubbleSort, &ops, &tiempo);
    imprimir_renglon("Burbuja", "Peor", ops, tiempo);

    medir_un_caso("Insercion", "Mejor", best, insertionSort, &ops, &tiempo);
    imprimir_renglon("Insercion", "Mejor", ops, tiempo);

    medir_promedio("Insercion", NULL, insertionSort, &ops, &tiempo);
    imprimir_renglon("Insercion", "Promedio", ops, tiempo);

    medir_un_caso("Insercion", "Peor", worst, insertionSort, &ops, &tiempo);
    imprimir_renglon("Insercion", "Peor", ops, tiempo);

    medir_un_caso("Seleccion", "Mejor", best, selectionSort, &ops, &tiempo);
    imprimir_renglon("Seleccion", "Mejor", ops, tiempo);

    medir_promedio("Seleccion", NULL, selectionSort, &ops, &tiempo);
    imprimir_renglon("Seleccion", "Promedio", ops, tiempo);

    medir_un_caso("Seleccion", "Peor", worst, selectionSort, &ops, &tiempo);
    imprimir_renglon("Seleccion", "Peor", ops, tiempo);

    free(best);
    free(worst);

    printf("\nNotas:\n");
    printf("- 'Promedio' promedia %d corridas aleatorias distintas.\n", AVG_TRIALS);
    printf("- El conteo de operaciones replica el criterio de tu código (comparaciones + 3 por intercambio en Burbuja/Seleccion e inserciones/movimientos en Insercion).\n");
    return 0;
}
