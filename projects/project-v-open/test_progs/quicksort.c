// ripped from geeksforgeeks

/* C implementation QuickSort */
#include <stdio.h> 
#include <stdlib.h>

#ifndef DEBUG
extern void exit();
#endif

#define N 99 

// A utility function to swap two elements 
void swap(int* a, int* b) { 
	int t = *a; 
	*a = *b; 
	*b = t; 
} 
  
/* This function takes last element as pivot, places 
   the pivot element at its correct position in sorted 
	array, and places all smaller (smaller than pivot) 
   to left of pivot and all greater elements to right 
   of pivot */
int partition (int arr[], int low, int high) { 
	int pivot = arr[high];	// pivot 
	int i = (low - 1);  // Index of smaller element 

	for (int j = low; j <= high- 1; j++) { 
		// If current element is smaller than or 
		// equal to pivot 
		if (arr[j] <= pivot) { 
			i++;	// increment index of smaller element 
			swap(&arr[i], &arr[j]); 
		} 
	} 
	swap(&arr[i + 1], &arr[high]); 
	return (i + 1); 
} 
  
/* The main function that implements QuickSort 
 arr[] --> Array to be sorted, 
  low  --> Starting index, 
  high  --> Ending index */
void quickSort(int arr[], int low, int high) { 
	if (low < high) { 
		/* pi is partitioning index, arr[p] is now 
		   at right place */
		int pi = partition(arr, low, high); 
  
		// Separately sort elements before 
		// partition and after partition 
		quickSort(arr, low, pi - 1); 
		quickSort(arr, pi + 1, high); 
	} 
} 
#ifdef DEBUG
/* Function to print an array */
void printArray(int arr[], int size) { 
	int i; 
	for (i=0; i < size; i++) {
	   printf("%d ", arr[i]); 
	}
	printf("n"); 
} 
#endif
  
// Driver program to test above functions 
int main() { 
	int arr[N];
	for (int i = 0; i < N; ++i)
		arr[i] = rand() & 1023;
	quickSort(arr, 0, N-1); 
#ifdef DEBUG
	printf("Sorted array: n"); 
	printArray(arr, N); 
#endif

	return 0; 
} 
