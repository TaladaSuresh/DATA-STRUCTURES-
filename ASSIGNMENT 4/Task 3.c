/* TASK 3 - EMERGENCY DEPARTMENT MAX-HEAP PRIORITY QUEUE */
#include <stdio.h>

#define MAX 32

typedef struct {
    int severity;
    int patient;
    int arrival;
} Case;

Case heap[MAX];
int size = 0;
int next_arrival = 0;

void swap(Case *a, Case *b) {
    Case temp = *a;
    *a = *b;
    *b = temp;
}

int higher_priority(Case a, Case b) {
    if (a.severity != b.severity) return a.severity > b.severity;
    return a.arrival < b.arrival;
}

void push(int severity, int patient) {
    if (size == MAX) return;

    heap[size] = (Case){severity, patient, next_arrival++};
    int i = size++;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!higher_priority(heap[i], heap[parent])) break;
        swap(&heap[i], &heap[parent]);
        i = parent;
    }
}

Case pop(void) {
    Case empty = {0, -1, 0};
    if (size == 0) return empty;

    Case top = heap[0];
    heap[0] = heap[--size];
    int i = 0;

    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int best = i;

        if (left < size && higher_priority(heap[left], heap[best]))
            best = left;
        if (right < size && higher_priority(heap[right], heap[best]))
            best = right;
        if (best == i) break;

        swap(&heap[i], &heap[best]);
        i = best;
    }
    return top;
}

int main(void) {
    int severity[] = {3, 9, 5, 7, 1, 8};

    for (int p = 0; p < 6; ++p)
        push(severity[p], 101 + p);

    while (size) {
        Case c = pop();
        printf("P%d(%d) ", c.patient, c.severity);
    }
    printf("\n");
    return 0;
}
