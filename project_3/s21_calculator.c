#include "s21_calculator.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_CAPACITY 256 

typedef struct {
  double data[STACK_CAPACITY];
  int top;
} Stack;

static void stack_init(Stack *s) { s->top = 0; }

static int stack_push(Stack *s, double val) {
  return (s->top < STACK_CAPACITY) ? (s->data[s->top++] = val, 1) : 0;
}

static int stack_pop(Stack *s, double *val) {
  return (s->top > 0) ? (*val = s->data[--s->top], 1) : 0;
}

static int is_number(const char *str) {
  int result = 1;
  size_t i = 0;
  if (str[0] == '-' && str[1] != '\0') i = 1;
  while (str[i] != '\0' && result) {
    if ((str[i] < '0' || str[i] > '9') && str[i] != '.') result = 0;
    i++;
  }
  return result;
}

static void eval_operator(Stack *s, const char *op, int *ok) {
  double a = 0.0, b = 0.0, r = 0.0;
  if (*ok && stack_pop(s, &b) && stack_pop(s, &a)) {
    if (strcmp(op, "+") == 0) r = a + b;
    else if (strcmp(op, "-") == 0) r = a - b;
    else if (strcmp(op, "*") == 0) r = a * b;
    else if (strcmp(op, "/") == 0 && b != 0.0) r = a / b;
    else *ok = 0;
    if (*ok) *ok = stack_push(s, r);
  } else {
    *ok = 0;
  }
}

static void eval_function(Stack *s, const char *f, int *ok) {
  double x = 0.0, r = 0.0;
  if (*ok && stack_pop(s, &x)) {
    if (strcmp(f, "~") == 0) r = -x;
    else if (strcmp(f, "sin") == 0) r = sin(x);
    else if (strcmp(f, "cos") == 0) r = cos(x);
    else if (strcmp(f, "tan") == 0) r = tan(x);
    else if (strcmp(f, "ctg") == 0 && fabs(sin(x)) > 1e-12) r = cos(x) / sin(x);
    else if (strcmp(f, "sqrt") == 0 && x >= 0.0) r = sqrt(x);
    else if (strcmp(f, "ln") == 0 && x > 0.0) r = log(x);
    else *ok = 0;
    if (*ok) *ok = stack_push(s, r);
  } else {
    *ok = 0;
  }
}

int calculate(const char *rpn_expr, double x_value, double *result) {
  Stack s;
  int ok = 1;
  stack_init(&s);

  char mutable_expr[strlen(rpn_expr) + 1];
  strcpy(mutable_expr, rpn_expr);

  char *token = strtok(mutable_expr, " ");
  while (token != NULL && ok) {
    if (strcmp(token, "x") == 0) {
      ok = stack_push(&s, x_value);
    } else if (is_number(token)) {
      ok = stack_push(&s, atof(token));
    } else if (strlen(token) == 1 && strchr("+-*/", token[0]) != NULL) {
      eval_operator(&s, token, &ok);
    } else {
      eval_function(&s, token, &ok);
    }
    token = strtok(NULL, " ");
  }
  if (ok && s.top == 1) {
    ok = stack_pop(&s, result);
  } else {
    ok = 0;
  }
  return ok;
}