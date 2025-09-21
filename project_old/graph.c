#include <math.h>
#include <stdio.h>
#include <string.h>
#include "s21_calculator.h"
#include "s21_parser.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define X_MIN 0.0
#define X_MAX (4.0 * M_PI)
#define Y_MIN -1.0
#define Y_MAX 1.0
#define MAX_EXPR_LEN 256

// Прототипы функций, используемых в main
void init_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]);
void draw_graph(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], const char *rpn_expr);
void print_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]);

// Точка входа в программу
int main() {
  char input_expr[MAX_EXPR_LEN] = {0};

  if (fgets(input_expr, sizeof(input_expr), stdin) == NULL) {
    printf("n/a\n");
    return 1;
  }
  input_expr[strcspn(input_expr, "\n")] = 0;

  char rpn_expr[MAX_EXPR_LEN * 2] = {0};

  if (infix_to_rpn(input_expr, rpn_expr)) {
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1];
    init_screen(screen);
    draw_graph(screen, rpn_expr);
    print_screen(screen);
  } else {
    printf("n/a\n");
  }

  return 0;
}


// Инициализирует экран символами '.'
void init_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]) {
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    for (int j = 0; j < SCREEN_WIDTH; j++) {
      screen[i][j] = '.';
    }
    screen[i][SCREEN_WIDTH] = '\0';
  }
}

// Заполняет экран точками графика
void draw_graph(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], const char *rpn_expr) {
  double x_step = (X_MAX - X_MIN) / (SCREEN_WIDTH - 1);

  for (int col = 0; col < SCREEN_WIDTH; col++) {
    double x = X_MIN + col * x_step;
    double y;

    if (calculate(rpn_expr, x, &y)) {
      if (y >= Y_MIN && y <= Y_MAX) {
        int row = round(12.0 - y * 12.0);
        if (row < 0) row = 0;
        if (row >= SCREEN_HEIGHT) row = SCREEN_HEIGHT - 1;
        screen[row][col] = '*';
      }
    }
  }
}

// Выводит содержимое экрана в терминал
void print_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]) {
  for (int i = SCREEN_HEIGHT-1; i >= 0; i--) {
    printf("%s\n", screen[i]);
  }
}