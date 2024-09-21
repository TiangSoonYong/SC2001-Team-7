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

// Admin functions
int* generateArr(int size, int max);
int* copyArr(int* ptr, int s, int e);
void printArr(int* ptr, int size);
void checkArr(int* ptr, int size);

// Global
long long keyComparison;

int main(){
    int size, max, choice, threshold;
    int* ptr;
    int* tempPtr;  // corrected this to be a pointer
    clock_t t;

    while(1){
        printf("Enter size of elements:");
        scanf("%d", &size);
        printf("Enter largest number (x):");
        scanf("%d", &max);

        ptr = generateArr(size, max);

        printf("1: Insertion Sort\n2: Merge Sort\n3: Hybrid Sort\nPlease enter choice:");
        scanf("%d", &choice);

        while(choice != 4){
            if(choice == 3){
                printf("Enter threshold:");
                scanf("%d", &threshold);
            }

            tempPtr = copyArr(ptr, 0, size-1);  // Allocate temporary array for sorting
            keyComparison = 0LL;
            t = clock();

            switch(choice){
                case 1:
                    printf("\ninsertionSort()\n");
                    insertionSort(tempPtr, 0, size-1);
                    break;
                case 2:
                    printf("\nmergeSort()\n");
                    mergeSort(tempPtr, 0, size-1, 0);
                    break;
                case 3:
                    printf("\nhybridSort()\n");
                    hybridSort(tempPtr, threshold, 0, size-1, 0);
                    break;
            }

            t = clock() - t;

            printf("Array Size: %d, Largest Number: %d", size, max);
            if(choice == 3) printf(", Threshold: %d", threshold);
            printf("\n");
            printf("Number of key comparisons: %lld\nTime Elapsed: %f seconds\n\n", keyComparison, ((double)t)/CLOCKS_PER_SEC);

            printf("1: Insertion Sort\n2: Merge Sort\n3: Hybrid Sort\n4: Reset Array\n5: Exit\nPlease enter choice:");
            scanf("%d", &choice);

            free(tempPtr);  // Free the dynamically allocated memory for tempPtr
            if(choice == 5) break;
        }

        free(ptr);  // Free memory allocated for the original array
        if(choice == 5) break;
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

    // Fixed off-by-one errors
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
        for (int j = i; j > s; j--){
            if (compare(ptr[j], ptr[j-1]) < 0)
                swap(ptr, j, j-1);
            else
                break;
        }
    }
}

void swap(int* ptr, int index1, int index2){
    int temp = ptr[index1];
    ptr[index1] = ptr[index2];
    ptr[index2] = temp;
}

int compare(int value1, int value2){
    keyComparison++;
    if(value1 > value2) return 1;
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

void printArr(int* ptr, int size){
    for (int i = 0; i < size; ++i) {
        printf("%d, ", ptr[i]);
    }
    printf("\n");
}

void checkArr(int* ptr, int size){
    for (int i = 1; i < size; ++i) {
        if(ptr[i] < ptr[i-1]){
            printf("Unsorted\n");
            return;
        }
    }
    printf("Sorted\n");
}

int* copyArr(int* ptr, int s, int e){
    int *temp = (int*)malloc((e - s + 1) * sizeof(int));
    for (int i = 0; i <= e - s; i++) {
        temp[i] = ptr[s + i];
    }
    return temp;
}
