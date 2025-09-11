#include <stdio.h>
#include <stdlib.h>

int input(int **a, int *n);
void sort(int *a, int n);
void output(int *a, int n);

int main() {
    int *data, n;
    if (input(&data, &n) == 0) {
        return 0;
    }
    sort(data, n);
    output(data, n);
    free(data);
}

void sort(int *a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (*(a + j) < *(a + i)) {
                int var = *(a + i);
                *(a + i) = *(a + j);
                *(a + j) = var;
            }
        }
    }
}

void output(int *a, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", *(a + i));
    }
}

int input(int **a, int *n) {

    if (scanf("%d", n) != 1) {
        printf("n/a\n");
    }

    if (*n <= 0) {
        printf("n/a\n");
    }

    *a = (int *)malloc(*n * sizeof(int));;

    for (int i = 0; i < *n; i++) {
        if (scanf("%d", *a + i) != 1) {
            printf("n/a\n");
            return 0;
        }
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
