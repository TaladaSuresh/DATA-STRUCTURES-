/* TASK 4 - STABLE MERGE SORT REPAIR FOR SORTING FORENSICS */
#include <stdio.h>
#include <stdlib.h>

static long comparisons = 0;

void merge(int a[], int temp[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        ++comparisons;
        if (a[i] <= a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
    }
    while (i <= mid) temp[k++] = a[i++];
    while (j <= right) temp[k++] = a[j++];

    for (i = left; i <= right; ++i) a[i] = temp[i];
}

void merge_sort(int a[], int temp[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort(a, temp, left, mid);
    merge_sort(a, temp, mid + 1, right);
    merge(a, temp, left, mid, right);
}

int main(void) {
    int marks[] = {7, 2, 9, 4, 3, 8, 5};
    int n = (int)(sizeof(marks) / sizeof(marks[0]));
    int *temp = malloc((size_t)n * sizeof(*temp));
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    merge_sort(marks, temp, 0, n - 1);

    for (int i = 0; i < n; ++i) printf("%d ", marks[i]);
    printf("\nComparisons: %ld\n", comparisons);

    free(temp);
    return 0;
}
