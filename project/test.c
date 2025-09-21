#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// ------------------- Прототипы -------------------
void init_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]);
void draw_graph(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], const char *rpn_expr);
void print_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]);

int infix_to_rpn(const char *input, char *output);
int calculate(const char *rpn_expr, double x_value, double *result);

// ------------------- Вспомогательные -------------------
static int my_isspace(int c) { return (c==' '||c=='\t'||c=='\n'||c=='\r'||c=='\v'||c=='\f'); }
static int my_isdigit(int c) { return (c>='0'&&c<='9'); }
static int my_isalpha(int c) { return ((c>='a'&&c<='z')||(c>='A'&&c<='Z')); }

static void op_push(OperatorStack *s, const char *item) {
    if (s->top < MAX_STACK_SIZE-1) strcpy(s->data[++s->top], item);
}
static char* op_pop(OperatorStack *s) { return (s->top>=0)? s->data[s->top--]: NULL; }
static char* op_peek(OperatorStack *s) { return (s->top>=0)? s->data[s->top]: NULL; }

static int is_function(const char *token) {
    const char *FUNCS[] = {"sin","cos","tan","ctg","sqrt","ln"};
    for(int i=0;i<6;i++) if(strcmp(token,FUNCS[i])==0) return 1;
    return 0;
}
static int get_precedence(const char *op) {
    if(!op||!op[0]) return 0;
    switch(op[0]){
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        case '~': return 4; // унарный минус
        default: return 0;
    }
}
static int is_right_associative(const char *op){ return op && op[0]=='^'; }

// ------------------- Парсер -------------------
static void process_digit(const char **p,char *output){
    char buf[50]; int i=0;
    while(my_isdigit(**p)||**p=='.') buf[i++]=*(*p)++;
    (*p)--; buf[i]='\0';
    strcat(output,buf); strcat(output," ");
}
static int process_alpha(const char **p,char *output,OperatorStack *stack){
    char buf[50]; int i=0; while(my_isalpha(**p)) buf[i++]=*(*p)++; (*p)--; buf[i]='\0';
    if(strcmp(buf,"x")==0) { strcat(output,"x "); return 1; }
    else if(is_function(buf)) { op_push(stack,buf); return 1; }
    return 0;
}
static int process_operator(char op_char,int last_op,OperatorStack *stack,char *output){
    int status=1;
    char op_buf[2]={op_char,'\0'};
    if((op_char=='-'||op_char=='+')&&last_op) op_buf[0]=(op_char=='-')?'~':'#';
    if(op_buf[0]!='#'){
        int prec=get_precedence(op_buf);
        if(prec>0){
            while(op_peek(stack)&&strcmp(op_peek(stack),"(")!=0&&
                 (get_precedence(op_peek(stack))>prec||
                 (get_precedence(op_peek(stack))==prec&&!is_right_associative(op_buf)))){
                strcat(output,op_pop(stack)); strcat(output," ");
            }
            op_push(stack,op_buf);
        }else status=0;
    }
    return status;
}
static int process_rbracket(OperatorStack *stack,char *output){
    int status=1;
    while(op_peek(stack)&&strcmp(op_peek(stack),"(")!=0){ strcat(output,op_pop(stack)); strcat(output," "); }
    if(!op_peek(stack)) status=0;
    else{
        op_pop(stack);
        if(op_peek(stack)&&is_function(op_peek(stack))) { strcat(output,op_pop(stack)); strcat(output," "); }
    }
    return status;
}

int infix_to_rpn(const char *input,char *output){
    OperatorStack stack={.top=-1};
    output[0]='\0'; int status=1;
    int last_was_op=1;
    for(const char *p=input;*p&&status;p++){
        if(my_isspace(*p)) continue;
        if(my_isdigit(*p)||(*p=='.'&&my_isdigit(p[1]))){ process_digit(&p,output); last_was_op=0; }
        else if(my_isalpha(*p)){ status=process_alpha(&p,output,&stack); last_was_op=0; }
        else if(*p=='('){ op_push(&stack,"("); last_was_op=1; }
        else if(*p==')'){ status=process_rbracket(&stack,output); last_was_op=0; }
        else{ status=process_operator(*p,last_was_op,&stack,output); last_was_op=1; }
    }
    while(op_peek(&stack)&&status){
        if(strcmp(op_peek(&stack),"(")==0) status=0;
        else{ strcat(output,op_pop(&stack)); strcat(output," "); }
    }
    return status;
}

// ------------------- Калькулятор -------------------
static void stack_init(Stack *s){ s->top=0; }
static int stack_push(Stack *s,double val){ return (s->top<STACK_CAPACITY)?(s->data[s->top++]=val,1):0; }
static int stack_pop(Stack *s,double *val){ return (s->top>0)?(*val=s->data[--s->top],1):0; }

static int is_number(const char *str){
    int i=0; if(str[0]=='-'&&str[1]!='\0') i=1;
    while(str[i]!='\0'){ if((str[i]<'0'||str[i]>'9')&&str[i]!='.') return 0; i++; }
    return 1;
}

static void eval_operator(Stack *s,const char *op,int *ok){
    double a,b,r; a=b=r=0.0;
    if(*ok && stack_pop(s,&b) && stack_pop(s,&a)){
        if(strcmp(op,"+")==0) r=a+b;
        else if(strcmp(op,"-")==0) r=a-b;
        else if(strcmp(op,"*")==0) r=a*b;
        else if(strcmp(op,"/")==0&&b!=0.0) r=a/b;
        else if(strcmp(op,"^")==0) r=pow(a,b);
        else *ok=0;
        if(*ok) *ok=stack_push(s,r);
    }else *ok=0;
}
static void eval_function(Stack *s,const char *f,int *ok){
    double x,r; x=r=0.0;
    if(*ok && stack_pop(s,&x)){
        if(strcmp(f,"~")==0) r=-x;
        else if(strcmp(f,"#")==0) r=x;
        else if(strcmp(f,"sin")==0) r=sin(x);
        else if(strcmp(f,"cos")==0) r=cos(x);
        else if(strcmp(f,"tan")==0) r=tan(x);
        else if(strcmp(f,"ctg")==0 && fabs(sin(x))>1e-12) r=cos(x)/sin(x);
        else if(strcmp(f,"sqrt")==0 && x>=0.0) r=sqrt(x);
        else if(strcmp(f,"ln")==0 && x>0.0) r=log(x);
        else *ok=0;
        if(*ok) *ok=stack_push(s,r);
    }else *ok=0;
}

int calculate(const char *rpn_expr,double x_value,double *result){
    Stack s; stack_init(&s); int ok=1;
    char mutable[strlen(rpn_expr)+1]; strcpy(mutable,rpn_expr);
    char *token=strtok(mutable," ");
    while(token && ok){
        if(strcmp(token,"x")==0) ok=stack_push(&s,x_value);
        else if(is_number(token)) ok=stack_push(&s,atof(token));
        else if(strlen(token)==1 && strchr("+-*/^",token[0])) eval_operator(&s,token,&ok);
        else eval_function(&s,token,&ok);
        token=strtok(NULL," ");
    }
    if(ok && s.top==1) ok=stack_pop(&s,result); else ok=0;
    return ok;
}

// ------------------- График -------------------
void init_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH+1]){
    for(int i=0;i<SCREEN_HEIGHT;i++){
        for(int j=0;j<SCREEN_WIDTH;j++) screen[i][j]='.';
        screen[i][SCREEN_WIDTH]='\0';
    }
}

void draw_graph(char screen[SCREEN_HEIGHT][SCREEN_WIDTH+1],const char *rpn_expr){
    double step=(X_MAX-X_MIN)/(SCREEN_WIDTH-1);
    for(int col=0;col<SCREEN_WIDTH;col++){
        double x=X_MIN+col*step,y;
        if(calculate(rpn_expr,x,&y)){
            if(y>=Y_MIN && y<=Y_MAX){
                int row=(int)round((y-Y_MIN)/(Y_MAX-Y_MIN)*(SCREEN_HEIGHT-1));
                if(row<0) row=0; if(row>=SCREEN_HEIGHT) row=SCREEN_HEIGHT-1;
                screen[row][col]='*';
            }
        }
    }
}

void print_screen(char screen[SCREEN_HEIGHT][SCREEN_WIDTH+1]){
    for(int i=0;i < SCREEN_HEIGHT;i++) printf("%s\n",screen[i]);
}

// ------------------- main -------------------
int main(){
    char input[MAX_EXPR_LEN]={0};
    if(fgets(input,sizeof(input),stdin)==NULL){ printf("n/a\n"); return 1; }
    input[strcspn(input,"\n")]=0;
    char rpn[MAX_EXPR_LEN*2]={0};
    if(infix_to_rpn(input,rpn)){
        char screen[SCREEN_HEIGHT][SCREEN_WIDTH+1];
        init_screen(screen);
        draw_graph(screen,rpn);
        print_screen(screen);
    }else printf("n/a\n");
    return 0;
}
