/////////////////////////////////////////////
// one forward layer of a fully connected NN
// basically just x @ W + b
//  - Jielun Tan, 03/2019
////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#ifndef DEBUG
extern void exit();
#endif
#define N 4

int n = N;

void kernel(int  vec_a[n],int  vec_b[n], int result[n], int b, int *out) { 
	int i = 0, j = 0, k = 0;  
	int dot = 0;
	//multiply and acc
	for (i = 0; i < n; ++i) {
		result[i] = vec_a[i] * vec_b[i];
		dot += result[i];
	}
	*out = dot + b;
}



int main() {
	int x[n], W[n], inner[n];
	int b;
	int out;
	//initializing weights
	for (int i = 0; i < n; ++i) {
		x[i] = rand() & ((1 << 16) - 1);
		W[i] = rand() & ((1 << 16) - 1);
		inner[i] = 0;
	}
	b = rand();
	//the actual layer
	kernel(x,W,inner,b, &out);
	
	return 0;

}
