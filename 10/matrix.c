#include <stdio.h>
#include <stdlib.h>
// по первому значению(кол - во строк) генерируем кол - во массивов с данными и так же построчно их считываем

int malloc_memory(int *a, int *n);
    int n = 5;
    int *arr = malloc(n * sizeof(int)); // память под 5 int
int calloc_memory(int *a, int *n);
    int n = 5;
    int *arr = calloc(n, sizeof(int)); // память под 5 int, заполнена нулями
int realloc_memory(int *a, int *n);
    int n = 3;
    int *arr = malloc(n * sizeof(int));

    if (arr == NULL) return 1;

    for (int i = 0; i < n; i++) arr[i] = i + 1;

    // Увеличиваем массив до 5 элементов
    n = 5;
    arr = realloc(arr, n * sizeof(int));
    if (arr == NULL) return 1;
int main() {
    int *data = malloc(sizeof(int));
    int *n;
    if (input(data, &n) == 0) {
        return 0;
    }

}


int input(int *a, int *n, int *a) {

    if (scanf("%d", a) != 1) {
        printf("n/a");
    }

    if (*n <= 0) {
        printf("n/a\n");
    }
    if()
    for (int i = 0; i < *n; i++) {
        if (scanf("%d", a[i]) != 1) {
            printf("n/a");
            return 0;
        }
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (c != ' ') {
            printf("n/a");
            return 0;
        }
    }
    return 1;
}
