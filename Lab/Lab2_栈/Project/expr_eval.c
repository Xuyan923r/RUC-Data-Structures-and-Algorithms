#include <stdio.h>
#include <ctype.h>

#define MAXSIZE 100

char optr[MAXSIZE]; 
int top_optr = -1;

int opnd[MAXSIZE];   
int top_opnd = -1;

void push_optr(char c) { optr[++top_optr] = c; }
char pop_optr() { return optr[top_optr--]; }
char peek_optr() { return optr[top_optr]; }

void push_opnd(int x) { opnd[++top_opnd] = x; }
int pop_opnd() { return opnd[top_opnd--]; }

void show_state(const char* token, const char* action) {
    printf("当前输入: %s\n", token);
    printf("主要操作: %s\n", action);

    printf("操作符栈: ");
    if (top_optr == -1) printf("空");
    else for (int i = 0; i <= top_optr; i++) printf("%c ", optr[i]);
    printf("\n");

    printf("操作数栈: ");
    if (top_opnd == -1) printf("空");
    else for (int i = 0; i <= top_opnd; i++) printf("%d ", opnd[i]);
    printf("\n\n");
}


char compare(char op1, char op2) {
    char table[7][7] = {
      /*        +    -    *    /    (    )    =   */
      /* + */  {'>', '>', '<', '<', '<', '>', '>'},
      /* - */  {'>', '>', '<', '<', '<', '>', '>'},
      /* * */  {'>', '>', '>', '>', '<', '>', '>'},
      /* / */  {'>', '>', '>', '>', '<', '>', '>'},
      /* ( */  {'<', '<', '<', '<', '<', '=', ' '},
      /* ) */  {'>', '>', '>', '>', ' ', '>', '>'},
      /* = */  {'<', '<', '<', '<', '<', ' ', '='}
    };

    int idx1, idx2;

    switch (op1) {
        case '+': idx1 = 0; break;
        case '-': idx1 = 1; break;
        case '*': idx1 = 2; break;
        case '/': idx1 = 3; break;
        case '(': idx1 = 4; break;
        case ')': idx1 = 5; break;
        case '=': idx1 = 6; break;
    }

    switch (op2) {
        case '+': idx2 = 0; break;
        case '-': idx2 = 1; break;
        case '*': idx2 = 2; break;
        case '/': idx2 = 3; break;
        case '(': idx2 = 4; break;
        case ')': idx2 = 5; break;
        case '=': idx2 = 6; break;
    }

    return table[idx1][idx2];
}


int apply(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;   // 默认整数除法
    }
    return 0;
}


int main() {
    char expr[256];
    printf("请输入以=结尾的表达式: ");
    scanf("%s", expr);

    push_optr('='); 
    int i = 0;
    char ch = expr[i];

    while (ch != '\0') {
        if (isdigit(ch)) {
            int num = 0;
            while (isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            push_opnd(num);

            char buf[32];
            sprintf(buf, "%d", num);
            show_state(buf, "读入数字并压入操作数栈");

            ch = expr[i];
        }
        else {
            char top = peek_optr();
            char t = compare(top, ch);

            if (t == '<') {
                push_optr(ch);
                char buf[2] = { ch, '\0' };
                show_state(buf, "移进（压入运算符栈）");
                ch = expr[++i];
            }
            else if (t == '=') {
                pop_optr();
                char buf[2] = { ch, '\0' };
                show_state(buf, "括号匹配/或=匹配，弹栈");
                ch = expr[++i];
            }
            else if (t == '>') {
                char op = pop_optr();
                int b = pop_opnd();
                int a = pop_opnd();
                int res = apply(a, b, op);
                push_opnd(res);

                char buf[2] = { op, '\0' };
                show_state(buf, "规约（弹栈计算一次）");
            }
        }
    }

    printf("\n最终结果: %d\n", pop_opnd());
    return 0;
}
