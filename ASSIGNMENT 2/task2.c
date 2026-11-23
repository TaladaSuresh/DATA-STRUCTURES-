#include <stdio.h>

int main(void) {
    int arr[] = {1, 3, 5, 7, 9, 11, 13};
    int n = sizeof(arr) / sizeof(arr[0]);
    int key = 6, low = 0, high = n - 1, found = -1, step = 1;

    printf("Array: [1, 3, 5, 7, 9, 11, 13]\nSearching for key: %d\n", key);
    printf("Binary search trace (indices are zero-based):\n");
    while (low <= high) {
        int mid = low + (high - low) / 2;
        printf("Step %d: low=%d, high=%d, mid=%d, arr[mid]=%d\n", step++, low, high, mid, arr[mid]);
        if (arr[mid] == key) {
            found = mid;
            break;
        } else if (arr[mid] < key) {
            low = mid + 1;
            printf("  %d < %d, search right half.\n", arr[mid], key);
        } else {
            high = mid - 1;
            printf("  %d > %d, search left half.\n", arr[mid], key);
        }
    }
    if (found == -1)
        printf("Result: %d not found in the array.\n", key);
    else
        printf("Result: %d found at index %d.\n", key, found);

    printf("Why: the array is sorted, but 6 is absent. Each comparison removes half the remaining range; eventually low > high.\n");
    return 0;
}
