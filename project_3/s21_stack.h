#ifndef S21_STACK_DEF_H
#define S21_STACK_DEF_H

#define MAX_STACK_SIZE 256

typedef struct {
  char data[MAX_STACK_SIZE][50];
  int top;
} OperatorStack;

typedef struct {
  double data[MAX_STACK_SIZE];
  int top;
} NumberStack;

#endif  