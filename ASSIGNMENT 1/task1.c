#include <stdio.h>

long long factorial_iterative(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) result *= i;
    return result;
}

long long factorial_recursive(int n) {
    if (n <= 1) return 1;
    return n * factorial_recursive(n - 1);
}

int main(void) {
    int n = 5;
    printf("Factorial demonstration (n = %d)\n", n);
    printf("Iterative factorial: %lld\n", factorial_iterative(n));
    printf("Recursive factorial: %lld\n", factorial_recursive(n));
    printf("\nIterative: uses a loop; generally less call overhead.\n");
    printf("Recursive: calls itself; uses stack space for each call.\n");
    return 0;
}
