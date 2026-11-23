#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

int main(void) {
    char infix[] = "A+B*C-D";
    char stack[MAX], postfix[MAX];
    int top = -1, j = 0;

    printf("Infix expression: %s\n", infix);
    printf("Conversion trace (operator stack, top at right):\n");

    for (int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];
        if (isalnum((unsigned char)ch)) {
            postfix[j++] = ch;
            printf("Read %c -> append operand; postfix: %.*s\n", ch, j, postfix);
        } else {
            while (top >= 0 && precedence(stack[top]) >= precedence(ch)) {
                postfix[j++] = stack[top--];
            }
            stack[++top] = ch;
            printf("Read %c -> pop higher/equal precedence, then push; stack: ", ch);
            for (int k = 0; k <= top; k++) printf("%c ", stack[k]);
            printf("| postfix: %.*s\n", j, postfix);
        }
    }
    while (top >= 0) postfix[j++] = stack[top--];
    postfix[j] = '\0';

    printf("Final postfix expression: %s\n", postfix);
    printf("Precedence: * is evaluated before + and -. + and - are left-associative.\n");
    printf("Therefore B*C is emitted first, then A is combined with it, then D is subtracted.\n");
    return 0;
}
