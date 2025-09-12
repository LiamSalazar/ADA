#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000

void generateRandomArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }
}

void copyArray(int source[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
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

void insertionSort(int arr[], int size, long long *ops) {
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        (*ops)++; 
        while (j >= 0 && arr[j] > key) {
            (*ops)++; 
            arr[j + 1] = arr[j];
            j--;
            (*ops)++; 
        }
        arr[j + 1] = key;
        (*ops)++; 
    }
}

int main() {
    int originalArray[SIZE], arr1[SIZE], arr2[SIZE], arr3[SIZE];
    long long opsBubble = 0, opsSelection = 0, opsInsertion = 0;
    clock_t start, end;
    double timeBubble, timeSelection, timeInsertion;
    
    srand(time(NULL));
    generateRandomArray(originalArray, SIZE);

    copyArray(originalArray, arr1, SIZE);
    start = clock();
    bubbleSort(arr1, SIZE, &opsBubble);
    end = clock();
    timeBubble = ((double)(end - start)) / CLOCKS_PER_SEC;

    copyArray(originalArray, arr2, SIZE);
    start = clock();
    selectionSort(arr2, SIZE, &opsSelection);
    end = clock();
    timeSelection = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    copyArray(originalArray, arr3, SIZE);
    start = clock();
    insertionSort(arr3, SIZE, &opsInsertion);
    end = clock();
    timeInsertion = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Resultados del ordenamiento:\n");
    printf("Bubble Sort - Operaciones: %lld, Tiempo: %f segundos\n", opsBubble, timeBubble);
    printf("Selection Sort - Operaciones: %lld, Tiempo: %f segundos\n", opsSelection, timeSelection);
    printf("Insertion Sort - Operaciones: %lld, Tiempo: %f segundos\n", opsInsertion, timeInsertion);
    
    return 0;
}