#include <stdio.h>

void printArray(int arr[], int start, int end) {
    for (int i = start; i <= end; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void merge(int arr[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = 0;
    int temp[100]; 

    printf("\nClubbing: Left part: ");
    printArray(arr, left, mid);
    printf("          Right part: ");
    printArray(arr, mid + 1, right);

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= right)
        temp[k++] = arr[j++];

    for (i = 0; i < k; i++)
        arr[left + i] = temp[i];

    printf("After merging: ");
    printArray(arr, left, right);
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        printf("\nDividing: ");
        printArray(arr, left, right);
        printf("Left half: ");
        printArray(arr, left, mid);
        printf("Right half: ");
        printArray(arr, mid + 1, right);

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

int main() {
    int n;
    int arr[100]; 

    printf("Enter number of elements (at least 20): ");
    scanf("%d", &n);

    if (n < 20) {
        printf("Please enter at least 20 elements.\n");
        return 0;
    }

    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    printf("\nOriginal array:\n");
    printArray(arr, 0, n - 1);

    mergeSort(arr, 0, n - 1);

    printf("\nSorted array:\n");
    printArray(arr, 0, n - 1);

    return 0;
}
