/*------------------------------------
	Здравствуй, человек!
	Чтобы получить ключ 
	поработай с комментариями.
-------------------------------------*/

#include <stdio.h>
#define NMAX 10

void input (int *buffer, int *length);
void output(int *a, int n, int arrayLength);
int sum_numbers(int *buffer, int length);
int find_numbers(int* buffer, int length, int number, int* numbers, int arrayLength);
int elementsCount(int *buffer, int length);

/*------------------------------------
	Функция получает массив данных 
	через stdin.
	Выдает в stdout особую сумму
	и сформированный массив 
	специальных элементов
	(выбранных с помощью найденной суммы):
	это и будет частью ключа
-------------------------------------*/
int main()
{
    int n, data[NMAX];
    input(data, &n);

    int data2[elementsCount(data, n)];

    find_numbers(data, n, sum_numbers(data, n), data2, elementsCount(data, n));
    output(data2, sum_numbers(data, n), elementsCount(data, n));

}

/*------------------------------------
	Функция должна находить
	сумму четных элементов 
	с 0-й позиции.
-------------------------------------*/
int sum_numbers(int *buffer, int length)
{
	int sum = 0;
	
	for (int i = 0; i < length; i++)
	{
		if (buffer[i] % 2 == 0)
		{
			sum = sum + buffer[i];
		}
	}
	return sum;
}

/*------------------------------------
	Функция должна находить
	все элементы, на которые нацело
	делится переданное число и
	записывает их в выходной массив.
-------------------------------------*/
int find_numbers(int* buffer, int length, int number, int* numbers, int arrayLength)
{
    int counter = 0;
    for (int i = 0; i < length; i++)
	{
		if (number % buffer[i] == 0)
		{
			numbers[arrayLength + counter] = buffer[i];
            counter++;
		}
	}
    return 0;
}

void input(int *a, int *n) {
	if (scanf("%d", n) != 1) {
        printf("n/a\n");
    }
    if (*n <= 0 || *n > 10 || *n == 1) {
        printf("n/a\n");
    }

    for (int *p = a + 1; p - a < *n; p++) {
        if (scanf("%d", p) != 1) {
            printf("n/a\n");
        }
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (c != ' ') {
            printf("n/a\n");
        }
    }
}

void output(int *a, int n, int arrayLength) {
    printf("%d\n", n);
    for (int i = 0; i < arrayLength; i++) {
        printf("%d ", *(a + i));
    }
}

int elementsCount(int *buffer, int length)
{
	int count = 0;
	
	for (int i = 0; i < length; i++)
	{
		if (buffer[i] % 2 == 0)
		{
			count += 1;
		}
	}
	return count;
}