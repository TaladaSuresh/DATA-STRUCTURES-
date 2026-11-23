#include <stdio.h>
#define MAX 5

typedef struct {
    int items[MAX];
    int top;
} Stack;

void initialize(Stack *s) { s->top = -1; }
int isEmpty(const Stack *s) { return s->top == -1; }
int isFull(const Stack *s) { return s->top == MAX - 1; }
void push(Stack *s, int value) {
    if (isFull(s)) printf("Stack is full\n");
    else s->items[++s->top] = value;
}
int pop(Stack *s) {
    if (isEmpty(s)) { printf("Stack is empty\n"); return -1; }
    return s->items[s->top--];
}
void display(const Stack *s) {
    printf("Stack contents (top to bottom): ");
    for (int i = s->top; i >= 0; i--) printf("%d ", s->items[i]);
    printf("\n");
}
int main(void) {
    Stack s;
    initialize(&s);
    push(&s, 10); push(&s, 20); push(&s, 30);
    display(&s);
    printf("Popped element: %d\n", pop(&s));
    display(&s);
    return 0;
}
