#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define X_MIN 0.0
#define X_MAX (4.0 * M_PI)
#define Y_MIN -1.0
#define Y_MAX 1.0
#define MAX_EXPR_LEN 256
#define STACK_CAPACITY 256
#define MAX_STACK_SIZE 256



typedef struct {
  char data[MAX_STACK_SIZE][50];
  int top;
} OperatorStack;

typedef struct {
  double data[STACK_CAPACITY];
  int top;
} Stack;


void init_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]);
void draw_graph(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], const char *rpn_expr);
void print_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]);

int infix_to_rpn(const char *input, char *output);
int calculate(const char *rpn_expr, double x_value, double *result);


static int my_isspace(int c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}
static int my_isdigit(int c) { return (c >= '0' && c <= '9'); }
static int my_isalpha(int c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }


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


void init_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]) {
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    for (int j = 0; j < SCREEN_WIDTH; j++) {
      screen[i][j] = '.';
    }
    screen[i][SCREEN_WIDTH] = '\0';
  }
}

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

void print_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]) {
  for (int i = SCREEN_HEIGHT-1; i >= 0; i--) {
    printf("%s\n", screen[i]);
  }
}


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
