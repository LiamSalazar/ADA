#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000

void generateRandomArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
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
    int arr[SIZE];
    long long opsInsertion = 0;
    clock_t start, end;
    double timeInsertion;
    
    srand(time(NULL));
    generateRandomArray(arr, SIZE);

    start = clock();
    insertionSort(arr, SIZE, &opsInsertion);
    end = clock();
    timeInsertion = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Insertion Sort - Operaciones: %lld, Tiempo: %f segundos\n", opsInsertion, timeInsertion);
    
    return 0;
}
