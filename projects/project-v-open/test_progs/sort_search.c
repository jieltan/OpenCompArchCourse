//////////////////////////////////////////////////////
// grab some values, sort them, and then binary search
// helper functions ripped from geeksforgeeks
//  - Jielun Tan, 03/2019
//////////////////////////////////////////////////////
#ifndef DEBUG
extern void exit();
#endif
#include <stdlib.h>
#define N 99

//bubble sort
void bubble(int arr[], int size) {

	for(int i = 0; i < (size-1); i++)
		for(int j = i+1; j< size; j++)
			if(arr[j] < arr[i]){
				int temp = arr[j];
				arr[j] = arr[i];
				arr[i] = temp;
			}
}

// A recursive binary search function. It returns 
// location of x in given array arr[l..r] is present, 
// otherwise -1 
int binarySearch(int arr[], int l, int r, int x) { 
	if (r >= l) { 
		int mid = l + (r - l) / 2; 
  
		// If the element is present at the middle 
		// itself 
		if (arr[mid] == x) 
			return mid; 
  
		// If element is smaller than mid, then 
		// it can only be present in left subarray 
		if (arr[mid] > x) 
			return binarySearch(arr, l, mid - 1, x); 
  
		// Else the element can only be present 
		// in right subarray 
		return binarySearch(arr, mid + 1, r, x); 
	} 
  
	// We reach here when element is not 
	// present in array 
	return -1; 
} 
  
int main(void) { 
	
	int arr[N]; 
	for (int i = 0; i < N; ++i)
		arr[i] = rand() & (31);

	int x = 10;
	bubble(arr, N);
	int result = binarySearch(arr, 0, N-1, x);
	int another_result = binarySearch(arr, 0, N-1,420);
	int larry = binarySearch(arr, 0, N-1,33); //larry
	int goat = binarySearch(arr, 0, N-1,23); //mj or l3-6
	int brady = binarySearch(arr, 0, N-1,12);



	return 0; 
} 
