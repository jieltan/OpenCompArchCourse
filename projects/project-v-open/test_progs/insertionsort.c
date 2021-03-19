//basic insertion sort
#include <stdlib.h>
#ifndef DEBUG
extern void exit();
#endif
#define N 99
void insertion(int arr[], int size){
	int min_idx, min;
	for(int i = 0; i < (size-1); i++){
		min_idx = i;
		min = arr[i];
		for(int j = i+1; j< size; j++){
			if(arr[j] < min){
				min_idx = j;
				min = arr[j];
			}
		}
		int temp = arr[i];
		arr[i] = min;
		arr[min_idx] = temp;
	}
}

int main(){
	//numbers 0 - 22 (inclusive)
	int arr[N];
	for (int i = 0; i < N; ++i) 
		arr[i] = rand() % 1023;

	insertion(arr, N);

	return 0;
}

