#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    char nombre[20];
    int inicio;
    int fin;
} solicitud;

int comparar_por_fin(const void *a, const void *b) {
    const solicitud *sa = (const solicitud *)a;
    const solicitud *sb = (const solicitud *)b;
    return sa->fin - sb->fin;
}

void copiar_solicitudes(solicitud destino[], solicitud origen[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        destino[i] = origen[i];
    }
}

int seleccionar_asesorias(solicitud solicitudes[], int n, long long *operaciones) {
    int i;
    int seleccionadas = 0;
    int indice_ultima = -1;
    *operaciones = 0;

    if (n == 0) {
        return 0;
    }

    qsort(solicitudes, n, sizeof(solicitud), comparar_por_fin);

    indice_ultima = 0;
    seleccionadas++;
    *operaciones += 1;

    for (i = 1; i < n; i++) {
        *operaciones += 1;
        if (solicitudes[i].inicio >= solicitudes[indice_ultima].fin) {
            indice_ultima = i;
            seleccionadas++;
            *operaciones += 2;
        }
    }

    return seleccionadas;
}

void ejecutar_caso(const char *nombre_caso, solicitud solicitudes[], int n) {
    solicitud copia[20];
    long long operaciones = 0;
    int seleccionadas = 0;
    clock_t tiempo_inicio, tiempo_fin;
    double tiempo_segundos;

    copiar_solicitudes(copia, solicitudes, n);

    tiempo_inicio = clock();
    seleccionadas = seleccionar_asesorias(copia, n, &operaciones);
    tiempo_fin = clock();

    tiempo_segundos = (double)(tiempo_fin - tiempo_inicio) / CLOCKS_PER_SEC;

    printf("| %-12s | %-17d | %-15lld | %-10.8f |\n",
           nombre_caso, seleccionadas, operaciones, tiempo_segundos);
}

void resolver_caso_propuesto(solicitud solicitudes[], int n) {
    solicitud copia[20];
    int indices_seleccionados[20];
    int cantidad_seleccionados = 0;
    int indice_ultima;
    int i;

    copiar_solicitudes(copia, solicitudes, n);
    qsort(copia, n, sizeof(solicitud), comparar_por_fin);

    indice_ultima = 0;
    indices_seleccionados[cantidad_seleccionados++] = 0;

    for (i = 1; i < n; i++) {
        if (copia[i].inicio >= copia[indice_ultima].fin) {
            indice_ultima = i;
            indices_seleccionados[cantidad_seleccionados++] = i;
        }
    }

    printf("\nRESOLUCION DEL CASO PROPUESTO\n");
    printf("Numero maximo de estudiantes atendidos: %d\n", cantidad_seleccionados);
    printf("Asesorias seleccionadas:\n");
    for (i = 0; i < cantidad_seleccionados; i++) {
        int idx = indices_seleccionados[i];
        printf("  %s: %04d - %04d\n", copia[idx].nombre, copia[idx].inicio, copia[idx].fin);
    }
}

int main() {
    int n_propuesto = 7;
    solicitud caso_propuesto[7] = {
        {"Juventino",  800,  900},
        {"Hortensia",  900, 1030},
        {"Salvador",   830,  930},
        {"Carla",     1000, 1100},
        {"Melany",    1100, 1200},
        {"Mariela",    930, 1030},
        {"Jonny",     1200, 1300}
    };

    int n_mejor = 7;
    solicitud caso_mejor[7] = {
        {"A",  800, 1300},
        {"B",  805, 1310},
        {"C",  810, 1320},
        {"D",  815, 1330},
        {"E",  820, 1340},
        {"F",  825, 1350},
        {"G",  830, 1400}
    };

    int n_medio = 7;
    solicitud caso_medio[7] = {
        {"A",  800,  930},
        {"B",  900, 1000},
        {"C",  950, 1100},
        {"D", 1030, 1200},
        {"E", 1130, 1230},
        {"F", 1215, 1300},
        {"G", 1245, 1400}
    };

    int n_peor = 7;
    solicitud caso_peor[7] = {
        {"A",  800,  900},
        {"B",  900, 1000},
        {"C", 1000, 1100},
        {"D", 1100, 1200},
        {"E", 1200, 1300},
        {"F", 1300, 1400},
        {"G", 1400, 1500}
    };

    printf("TABLA DE RESULTADOS DEL ALGORITMO VORAZ DE ASESORIAS\n\n");
    printf("Formato de tiempos: segundos (s)\n\n");
    printf("-------------------------------------------------------------------------\n");
    printf("| Caso         | Estudiantes sel.   | Operaciones      | Tiempo (s) |\n");
    printf("-------------------------------------------------------------------------\n");

    ejecutar_caso("Propuesto", caso_propuesto, n_propuesto);
    ejecutar_caso("Mejor",     caso_mejor,     n_mejor);
    ejecutar_caso("Medio",     caso_medio,     n_medio);
    ejecutar_caso("Peor",      caso_peor,      n_peor);

    printf("-------------------------------------------------------------------------\n");

    resolver_caso_propuesto(caso_propuesto, n_propuesto);

    return 0;
}
