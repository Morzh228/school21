#ifndef S21_CALCULATOR_H
#define S21_CALCULATOR_H

// Основная функция: вычисляет значение выражения, заданного RPN-строкой.
// Возвращает 1 при успехе, 0 - при ошибке.
int calculate(const char *rpn_expr, double x_value, double *result);

#endif  // S21_CALCULATOR_H