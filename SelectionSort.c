#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000

void generateRandomArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }
}

void selectionSort(int arr[], int size, long long *ops) {
    for (int i = 0; i < size - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < size; j++) {
            (*ops)++; // Comparación
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
            (*ops) += 3; // Intercambio
        }
    }
}

int main() {
    int arr[SIZE];
    long long opsSelection = 0;
    clock_t start, end;
    double timeSelection;
    
    srand(time(NULL));
    generateRandomArray(arr, SIZE);

    start = clock();
    selectionSort(arr, SIZE, &opsSelection);
    end = clock();
    timeSelection = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Selection Sort - Operaciones: %lld, Tiempo: %f segundos\n", opsSelection, timeSelection);
    
    return 0;
}
