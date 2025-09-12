#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000

void generateRandomArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }
}

void bubbleSort(int arr[], int size, long long *ops) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            (*ops)++; // Comparación
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                (*ops) += 3; // Intercambio
            }
        }
    }
}

int main() {
    int arr[SIZE];
    long long opsBubble = 0;
    clock_t start, end;
    double timeBubble;
    
    srand(time(NULL));
    generateRandomArray(arr, SIZE);

    start = clock();
    bubbleSort(arr, SIZE, &opsBubble);
    end = clock();
    timeBubble = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Bubble Sort - Operaciones: %lld, Tiempo: %f segundos\n", opsBubble, timeBubble);
    
    return 0;
}
