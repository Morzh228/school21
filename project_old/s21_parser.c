#include "s21_parser.h"
#include <string.h>
#include "s21_stack.h"

static int my_isspace(int c);
static int my_isdigit(int c);
static int my_isalpha(int c);
static void op_push(OperatorStack *s, const char *item);
static char *op_pop(OperatorStack *s);
static char *op_peek(OperatorStack *s);
static int is_function(const char *token);
static int get_precedence(const char *op);
static void process_digit(const char **p, char *output);
static int process_alpha(const char **p, char *output, OperatorStack *stack);
static int process_operator(char op_char, int last_op, OperatorStack *stack,
                            char *output);
static int process_rbracket(OperatorStack *stack, char *output);

static int my_isspace(int c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
          c == '\f');
}
static int my_isdigit(int c) { return (c >= '0' && c <= '9'); }
static int my_isalpha(int c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static void op_push(OperatorStack *s, const char *item) {
  if (s->top < MAX_STACK_SIZE - 1) strcpy(s->data[++s->top], item);
}
static char *op_pop(OperatorStack *s) {
  return (s->top >= 0) ? s->data[s->top--] : NULL;
}
static char *op_peek(OperatorStack *s) {
  return (s->top >= 0) ? s->data[s->top] : NULL;
}

static int is_function(const char *token) {
  const char *FUNCS[] = {"sin", "cos", "tan", "ctg", "sqrt", "ln"};
  int result = 0;
  for (int i = 0; i < 6 && !result; i++) {
    if (strcmp(token, FUNCS[i]) == 0) result = 1;
  }
  return result;
}

static int get_precedence(const char *op) {
  int priority = 0;
  if (op && op[0]) {
    char c = op[0];
    if (c == '+' || c == '-') priority = 1;
    if (c == '*' || c == '/') priority = 2;
    if (c == '~') priority = 3;
  }
  return priority;
}

static void process_digit(const char **p, char *output) {
  char num_buf[50];
  int i = 0;
  while (my_isdigit(**p) || **p == '.') {
    num_buf[i++] = *(*p)++;
  }
  (*p)--;
  num_buf[i] = '\0';
  strcat(output, num_buf);
  strcat(output, " ");
}

static int process_alpha(const char **p, char *output, OperatorStack *stack) {
  char ident_buf[50];
  int i = 0;
  int status = 1;
  while (my_isalpha(**p)) {
    ident_buf[i++] = *(*p)++;
  }
  (*p)--;
  ident_buf[i] = '\0';

  if (strcmp(ident_buf, "x") == 0) {
    strcat(output, "x ");
  } else if (is_function(ident_buf)) {
    op_push(stack, ident_buf);
  } else {
    status = 0;
  }
  return status;
}

static int process_operator(char op_char, int last_op, OperatorStack *stack,
                            char *output) {
  int status = 1;
  char op_buf[2] = {op_char, '\0'};
  if ((op_char == '-' || op_char == '+') && last_op) {
    op_buf[0] = (op_char == '-') ? '~' : '#';
  }
  if (op_buf[0] != '#') {
    int prec = get_precedence(op_buf);
    if (prec > 0) {
      while (op_peek(stack) && strcmp(op_peek(stack), "(") != 0 &&
             (get_precedence(op_peek(stack)) > prec ||
              (get_precedence(op_peek(stack)) == prec))) {
        strcat(output, op_pop(stack));
        strcat(output, " ");
      }
      op_push(stack, op_buf);
    } else {
      status = 0;
    }
  }
  return status;
}

static int process_rbracket(OperatorStack *stack, char *output) {
  int status = 1;
  while (op_peek(stack) && strcmp(op_peek(stack), "(") != 0) {
    strcat(output, op_pop(stack));
    strcat(output, " ");
  }
  if (!op_peek(stack)) {
    status = 0;
  } else {
    op_pop(stack);
    if (op_peek(stack) && is_function(op_peek(stack))) {
      strcat(output, op_pop(stack));
      strcat(output, " ");
    }
  }
  return status;
}

int infix_to_rpn(const char *input, char *output) {
  OperatorStack stack = {.top = -1};
  output[0] = '\0';
  int status = 1;
  int last_was_operator_or_lbracket = 1;

  for (const char *p = input; *p && status; p++) {
    if (my_isspace(*p)) continue;

    if (my_isdigit(*p) || (*p == '.' && my_isdigit(p[1]))) {
      process_digit(&p, output);
      last_was_operator_or_lbracket = 0;
    } else if (my_isalpha(*p)) {
      status = process_alpha(&p, output, &stack);
      last_was_operator_or_lbracket = 0;
    } else if (*p == '(') {
      op_push(&stack, "(");
      last_was_operator_or_lbracket = 1;
    } else if (*p == ')') {
      status = process_rbracket(&stack, output);
      last_was_operator_or_lbracket = 0;
    } else {
      status = process_operator(*p, last_was_operator_or_lbracket, &stack, output);
      last_was_operator_or_lbracket = 1;
    }
  }

  while (op_peek(&stack) && status) {
    if (strcmp(op_peek(&stack), "(") == 0) status = 0;
    else {
      strcat(output, op_pop(&stack));
      strcat(output, " ");
    }
  }
  if (strlen(output) == 0 && status) status = 0;
  return status;
}