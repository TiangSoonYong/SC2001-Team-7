#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Hybrid Mergesort with Insertion Sort for better efficiency */
void hybridSort(int* ptr, int threshold, int s, int e, int inplace);
void mergeFast(int* ptr, int s, int e);
void merge(int* ptr, int s, int e);
void mergeSort(int* ptr, int s, int e, int inplace);
void insertionSort (int* ptr, int s, int e);
void swap(int* ptr, int index1, int index2);
int compare(int value1, int value2);

// Original Merge Sort functions
void mergeOriginal(int* ptr, int s, int mid, int e);
void mergeSortOriginal(int* ptr, int s, int e);

// Admin functions
int* generateArr(int size, int max);
int* copyArr(int* ptr, int s, int e);

// Global variable for key comparisons
long long keyComparison;

int main() {
    int size, max, choice, threshold;
    int* ptr;
    int* tempPtr;  // Temporary array for sorting
    clock_t t;

    while(1) {
        printf("Enter size of elements: ");
        scanf("%d", &size);
        printf("Enter largest number (x): ");
        scanf("%d", &max);

        ptr = generateArr(size, max);

        printf("\nSorting Options:\n");
        printf("1: Insertion Sort\n");
        printf("2: Merge Sort (Original)\n");
        printf("3: Merge Sort (Hybrid)\n");
        printf("4: Compare both\n");
        printf("5: Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        while (choice != 5) {
            if (choice == 3 || choice == 4) {
                printf("Enter threshold for hybrid sort: ");
                scanf("%d", &threshold);
            }

            if (choice == 4) {
                // Compare both original and hybrid sort
                printf("\nComparing Original Merge Sort and Hybrid Merge Sort...\n");

                // Original Merge Sort
                tempPtr = copyArr(ptr, 0, size - 1);
                keyComparison = 0LL;
                t = clock();
                mergeSortOriginal(tempPtr, 0, size - 1);
                t = clock() - t;
                printf("Original Merge Sort:\n");
                printf("Number of key comparisons: %lld\nTime Elapsed: %f seconds\n\n", keyComparison, ((double)t) / CLOCKS_PER_SEC);
                free(tempPtr);

                // Hybrid Merge Sort
                tempPtr = copyArr(ptr, 0, size - 1);
                keyComparison = 0LL;
                t = clock();
                hybridSort(tempPtr, threshold, 0, size - 1, 0);
                t = clock() - t;
                printf("Hybrid Merge Sort:\n");
                printf("Number of key comparisons: %lld\nTime Elapsed: %f seconds\n\n", keyComparison, ((double)t) / CLOCKS_PER_SEC);
                free(tempPtr);
            } else {
                // Single sort (Insertion, Merge, or Hybrid)
                tempPtr = copyArr(ptr, 0, size - 1);  // Allocate temporary array for sorting
                keyComparison = 0LL;
                t = clock();

                switch (choice) {
                    case 1:
                        printf("\ninsertionSort()\n");
                        insertionSort(tempPtr, 0, size - 1);
                        break;
                    case 2:
                        printf("\nmergeSortOriginal()\n");
                        mergeSortOriginal(tempPtr, 0, size - 1);
                        break;
                    case 3:
                        printf("\nhybridSort()\n");
                        hybridSort(tempPtr, threshold, 0, size - 1, 0);
                        break;
                }

                t = clock() - t;

                printf("Array Size: %d, Largest Number: %d", size, max);
                if (choice == 3 || choice == 4) printf(", Threshold: %d", threshold);
                printf("\n");
                printf("Number of key comparisons: %lld\nTime Elapsed: %f seconds\n\n", keyComparison, ((double)t) / CLOCKS_PER_SEC);

                free(tempPtr);  // Free the dynamically allocated memory for tempPtr
            }

            printf("\nSorting Options:\n");
            printf("1: Insertion Sort\n");
            printf("2: Merge Sort (Original)\n");
            printf("3: Merge Sort (Hybrid)\n");
            printf("4: Compare both\n");
            printf("5: Reset Array\n");
            printf("6: Exit\n");
            printf("Please enter your choice: ");
            scanf("%d", &choice);

            if (choice == 6) break;
        }

        free(ptr);  // Free memory allocated for the original array
        if (choice == 6) break;
    }

    return 0;
}

/* Hybrid sort using merge sort and insertion sort */
void hybridSort(int* ptr, int threshold, int s, int e, int inplace){
    if(e - s <= 0) return;
    else if(e - s + 1 <= threshold)  // Fix condition to account for subarray size
        insertionSort(ptr, s, e);
    else{
        int mid = (s + e) / 2;
        hybridSort(ptr, threshold, s, mid, inplace);
        hybridSort(ptr, threshold, mid + 1, e, inplace);
        if(inplace)
            merge(ptr, s, e);
        else
            mergeFast(ptr, s, e);
    }
}

void mergeSort(int* ptr, int s, int e, int inplace){
    if (e - s <= 0) return;
    int mid = (s + e) / 2;
    mergeSort(ptr, s, mid, inplace);
    mergeSort(ptr, mid + 1, e, inplace);
    if(inplace)
        merge(ptr, s, e);
    else
        mergeFast(ptr, s, e);
}

void merge(int* ptr, int s, int e){
    if (e - s <= 0) return;
    int mid = (s + e) / 2;
    int a = s, b = mid + 1, i, tmp;

    while (a <= mid && b <= e) {
        int cmp = compare(ptr[a], ptr[b]);
        if (cmp > 0) {  // ptr[a] > ptr[b]
            tmp = ptr[b++];
            for (i = ++mid; i > a; i--)
                ptr[i] = ptr[i-1];
            ptr[a++] = tmp;
        } else if (cmp < 0)  // ptr[a] < ptr[b]
            a++;
        else {  // ptr[a] == ptr[b]
            tmp = ptr[b++];
            a++;
            for (i = ++mid; i > a; i--)
                ptr[i] = ptr[i-1];
            ptr[a++] = tmp;
        }
    }
}

void mergeFast(int* ptr, int s, int e){
    if (e - s <= 0) return;
    int mid = (s + e) / 2;
    int* left = copyArr(ptr, s, mid);
    int* right = copyArr(ptr, mid + 1, e);
    int l = 0, r = 0, i = s;

    while (l < mid - s + 1 && r < e - mid) {
        int cmp = compare(left[l], right[r]);
        if (cmp >= 0) ptr[i++] = right[r++];
        if (cmp <= 0) ptr[i++] = left[l++];
    }

    while (l < mid - s + 1) ptr[i++] = left[l++];
    while (r < e - mid) ptr[i++] = right[r++];

    free(left);
    free(right);
}

void insertionSort (int* ptr, int s, int e){
    for (int i = s + 1; i <= e; i++){
        int current = ptr[i];
        int j = i - 1;
        while (j >= s && compare(ptr[j], current) > 0) {
            ptr[j + 1] = ptr[j];  // Shift elements to the right
            j--;
        }
        ptr[j + 1] = current;  // Place current element in the correct position
    }
}

void swap(int* ptr, int index1, int index2){
    int temp = ptr[index1];
    ptr[index1] = ptr[index2];
    ptr[index2] = temp;
}

int compare(int value1, int value2){
    keyComparison++;  // Always count comparisons
    if (value1 > value2) return 1;
    else if (value1 < value2) return -1;
    else return 0;
}

/* Function to generate an array with random elements */
int* generateArr(int size, int max){
    int *ptr = (int*)malloc(size * sizeof(int));

    if (ptr == NULL) {
        printf("Memory not allocated.\n");
        return NULL;
    }

    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        ptr[i] = rand() % (max + 1);
    }

    return ptr;
}

int* copyArr(int* ptr, int s, int e){
    int size = e - s + 1;
    int *temp = (int*)malloc(size * sizeof(int));  // Allocate memory for the copy
    if (temp == NULL) {
        printf("Memory not allocated.\n");
        return NULL;
    }
    // Copy elements from original array
    for (int i = 0; i < size; i++) {
        temp[i] = ptr[s + i];
    }
    return temp;
}

/* Original Merge Sort Implementation */
void mergeOriginal(int* ptr, int s, int mid, int e){
    int n1 = mid - s + 1;  // Size of the left subarray
    int n2 = e - mid;      // Size of the right subarray

    // Create temporary arrays for the left and right subarrays
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++) L[i] = ptr[s + i];
    for (int i = 0; i < n2; i++) R[i] = ptr[mid + 1 + i];

    // Merge the temporary arrays back into the original array
    int i = 0;  // Initial index of left subarray
    int j = 0;  // Initial index of right subarray
    int k = s;  // Initial index of the merged subarray

    while (i < n1 && j < n2) {
        keyComparison++;  // Count comparison
        if (L[i] <= R[j]) {
            ptr[k] = L[i];
            i++;
        } else {
            ptr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        ptr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        ptr[k] = R[j];
        j++;
        k++;
    }

    // Free the temporary arrays
    free(L);
    free(R);
}

void mergeSortOriginal(int* ptr, int s, int e){
    if (s < e) {
        int mid = s + (e - s) / 2;

        // Sort the left and right halves
        mergeSortOriginal(ptr, s, mid);
        mergeSortOriginal(ptr, mid + 1, e);

        // Merge the sorted halves
        mergeOriginal(ptr, s, mid, e);
    }
}
