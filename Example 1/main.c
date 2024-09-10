#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
In Mergesort, when the sizes of subarrays are small, the overhead of many recursive
calls makes the algorithm inefficient. Therefore, in real use, we often combine
Mergesort with Insertion Sort to come up with a hybrid sorting algorithm for better
efficiency. The idea is to set a small integer S as a threshold for the size of subarrays.
Once the size of a subarray in a recursive call of Mergesort is less than or equal to S,
the algorithm will switch to Insertion Sort, which is efficient for small-sized input.
*/

//Function prototype for sorting
void hybridSort(int* ptr, int threshold, int s, int e);

void mergeSort(int* ptr, int s, int e);
void merge(int* ptr, int n, int m);

void insertionSort (int* ptr, int s, int e);
void swap(int* ptr, int index1, int index2);

int compare(int value1, int value2);

//Admin functions
int* generateArr(int size, int max);
void printArr(int* ptr, int size);
int* copyArr(int* ptr, int size);

//Global
long long keyComparison;

int main(){
    // address of the block created hold by this pointer
    int size, max, threshold;
    clock_t t;

    // Size of the array
    printf("Enter size of elements:");
    scanf("%d", &size);
    printf("Enter largest number(x):");
    scanf("%d", &max);
    printf("Enter threshold:");
    scanf("%d", &threshold);

    int *ptr = generateArr(size, max);
    //printf("\nOriginal:");
    //printArr(ptr, size);

    printf("\ninsertionSort()\n");
    int* tempIS = copyArr(ptr, size);
    keyComparison = 0LL;
    t = clock();
    insertionSort(tempIS, 0, size);
    t = clock() - t;
    printf("Number of key comparison: %lld\nTime Elapsed:%f seconds\n", keyComparison, ((double)t)/CLOCKS_PER_SEC);
    //printArr(tempIS, size);

    printf("insertionSort() 2\n");
    keyComparison = 0LL;
    t = clock();
    insertionSort(tempIS, 0, size);
    t = clock() - t;
    printf("Number of key comparison: %lld\nTime Elapsed:%f seconds\n", keyComparison, ((double)t)/CLOCKS_PER_SEC);
    //printArr(tempIS, size);


    printf("\nmergeSort()\n");
    int* tempMS = copyArr(ptr, size);
    keyComparison = 0LL;
    t = clock();
    mergeSort(tempMS, 0, size-1);
    t = clock() - t;
    printf("Number of key comparison: %lld\nTime Elapsed:%f seconds\n", keyComparison, ((double)t)/CLOCKS_PER_SEC);
    //printArr(tempMS, size);

    printf("mergeSort() 2\n");
    keyComparison = 0LL;
    t = clock();
    mergeSort(tempMS, 0, size-1);
    t = clock() - t;
    printf("Number of key comparison: %lld\nTime Elapsed:%f seconds\n", keyComparison, ((double)t)/CLOCKS_PER_SEC);
    //printArr(tempMS, size);

    printf("\nhybridSort()\n");
    int* tempHB = copyArr(ptr,size);
    keyComparison = 0LL;
    t = clock();
    hybridSort(tempHB, threshold, 0, size-1);
    t = clock() - t;
    printf("Number of key comparison: %lld\nTime Elapsed:%f seconds\n", keyComparison, ((double)t)/CLOCKS_PER_SEC);
    //printArr(tempHB, size);

    printf("hybridSort() 2\n");
    keyComparison = 0LL;
    t = clock();
    hybridSort(tempHB, threshold, 0, size-1);
    t = clock() - t;
    printf("Number of key comparison: %lld\nTime Elapsed:%f seconds\n", keyComparison, ((double)t)/CLOCKS_PER_SEC);
    //printArr(tempHB, size);

    free(ptr);
    return 0;
}

/*
(a) Algorithm implementation: Implement the above hybrid algorithm.
*/
void hybridSort(int* ptr, int threshold, int s, int e){
    if(e-s<= threshold) insertionSort(ptr, s, e);
    else{
        int mid = (s+e)/2;
        hybridSort(ptr, threshold, s, mid);
        hybridSort(ptr, threshold, mid+1, e);
        merge(ptr, s, e);
    }
}

void mergeSort(int* ptr, int s, int e){// s=start, e=end
    if (e-s <= 0) return;
    int mid = (s+e)/2;
    mergeSort(ptr, s, mid);
    mergeSort(ptr, mid+1, e);
    merge(ptr, s, e);
}


void merge(int* ptr, int s, int e){
    if (e-s <= 0) return;
    int mid = (s+e)/2;
    int a = s, b = mid+1, i, tmp;
    while (a <= mid && b <= e) {
        int cmp = compare(ptr[a], ptr[b]);
        if (cmp > 0) { //ptr[a] > ptr[b]
            tmp = ptr[b++];
            for (i = ++mid; i > a; i--)
                ptr[i] = ptr[i-1];
            ptr[a++] = tmp;
        }
        else if (cmp < 0) //ptr[a] < ptr[b]
            a++;
        else {   //ptr[a] == ptr[b]
            if (a == mid && b == e)
                break;
            tmp = ptr[b++];
            a++;
            for (i = ++mid; i > a; i--)
                ptr[i] = ptr[i-1];
            ptr[a++] = tmp;
        }
    } // end of while loop;
} // end of merge

void insertionSort (int* ptr, int s, int e){
    for (int i=s+1; i < e; i++){
        for (int j=i; j > s; j--){
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
/*
(b) Generate input data: Generate arrays of increasing sizes, in a range from
1,000 to 10 million. For each of the sizes, generate a random dataset of integers
in the range of [1, …, x], where x is the largest number you allow for your
datasets.
*/
//https://www.geeksforgeeks.org/dynamic-array-in-c/
int* generateArr(int size, int max)
{
    //  Memory allocates dynamically using malloc()
    int *ptr = (int*)malloc(size * sizeof(int));

    // Checking for memory allocation
    if (ptr == NULL) {
        printf("Memory not allocated.\n");
    }
    else {

        // Memory allocated
        printf("Memory successfully allocated using "
               "malloc.\n");

        srand(time(NULL));
        // Get the elements of the array
        for (int i = 0; i < size; ++i) {
            ptr[i] = rand()%(max+1);
        }
    }
    return ptr;
}

void printArr(int* ptr, int size){
    // Print the elements of the array
    printf("The elements of the array are:\n ");
    for (int i = 0; i < size; ++i) {
        printf("%d, ", ptr[i]);
    }
    printf("\n\n");
}

int* copyArr(int* ptr, int size){
    int *temp = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i){
        temp[i] = ptr[i];
    }
    return temp;
}
