#include <stdio.h>
#define NMAX 10

int input(int *a);
void sort(int *a);
void output(int *a);

int main() {
    int data[NMAX];
    if (input(data) == 0) {
        return 0;
    }
    sort(data);
    output(data);
    
}

int input(int *a) {

    for (int *p = a + 1; p - a < NMAX; p++) {
        if (scanf("%d", p) != 1) {
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

void sort(int *a){
    for(int i = 0; i < NMAX; i++){
        for(int j = i + 1; j < NMAX; j++){
            if(*(a + j) < *(a + i)) {
                int var = *(a + i);
                *(a + i) = *(a + j);
                *(a + j) = var;
            }
        }
    }
}


void output(int *a){
    for(int i = 0; i < NMAX; i++){
        printf("%d ", *(a + i));
    }
}