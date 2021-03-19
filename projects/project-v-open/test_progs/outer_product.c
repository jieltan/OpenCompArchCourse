////////////////////////////////////////////////
// outer product boyz
// the real workload that everyone optimizes for
// - Jielun Tan, 03/2019
////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#ifndef DEBUG
extern void exit();
#endif
#define N 16
int n = N;
void kernel(int  mat_a[n][n],int  mat_b[n][n], int partial[n][n][n], int result[n][n]) { 
    int i = 0, j = 0, k = 0;  
    //multiply
    for (i = 0; i < n; ++i) {
    	for (j = 0; j < n; ++j) {
    		for (k = 0; k < n; ++k) {
    			partial[i][j][k] = mat_a[j][i] * mat_b[i][k];
#ifdef DEBUG
				printf("partial is %i, i is %i, j is %i, k is %i\n", partial[i][j][k], i, j, k);
#endif
    		}
    	}
    }
    //acc
    for (i = 0; i < n; ++i) {
    	for (j = 0; j < n; ++j) {
    		for (k = 0; k < n; ++k) {
    			result[i][j] += partial[k][i][j];
    		}
    	}
    }
}



int main() {
	int i = 0, j = 0, k = 0;
	int mat_a[n][n], mat_b[n][n], result[n][n], partial[n][n][n];
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			mat_a[i][j] = rand() & ((1 << 16) - 1);
			mat_b[i][j] = rand() & ((1 << 16) - 1);
			result[i][j] = 0;
		}
	}

    kernel(mat_a,mat_b,partial,result);
	return 0;
}
