#include <stdio.h>
#define NMAX 10

int input(int *a, int *b);
void swap(int *a, int *b);
void output(int *a);
int medianOfThree(int arr[], int low, int high);
int partition(int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void heapify(int arr[], int n, int i);
void heapSort(int arr[], int n);

int main() {
    int data[NMAX], data2[NMAX];
    if (input(data, data2) == 0) {
        return 0;
    }

    quickSort(data, 0, NMAX - 1);
    output(data);
    heapSort(data2, NMAX);
    output(data2);
    
}

int input(int *a, int *b) {

    for (int i = 0; i < NMAX; i++) {
        if (scanf("%d", a + i) != 1) {
            printf("n/a\n");
            return 0;
        }

        *(b + i) = *(a + i);
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (c != ' ') {
            printf("n/a\n");
            return 0;
        }
    }
    return 1;
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int medianOfThree(int arr[], int low, int high) {
    int mid = (low + high) / 2;

    if (arr[low] > arr[mid])
        swap(&arr[low], &arr[mid]);
    if (arr[low] > arr[high])
        swap(&arr[low], &arr[high]);
    if (arr[mid] > arr[high])
        swap(&arr[mid], &arr[high]);

    swap(&arr[mid], &arr[high]);
    return arr[high];
}

int partition(int arr[], int low, int high) {
    int pivot = medianOfThree(arr, low, high);
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


void output(int arr[]) {
    for (int i = 0; i < NMAX; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}
