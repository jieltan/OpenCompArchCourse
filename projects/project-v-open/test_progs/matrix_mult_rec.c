///////////////////////////////////////////////////
// algorithm taken from geeksforgeeks.org
// performs a recursive matrix multiplication
// on 2 3x3 matrices
// kindly contributed by group 5 Winter 2019
//   - Jielun Tan, 12/2019
//////////////////////////////////////////////////
#include <stdio.h> 
#ifndef DEBUG
extern void exit();
#endif

// NOTE: matrix bounds must be 100 because variable matrices
// would not compile on our version of the RISCV GNU compiler
void multiplyMatrixRec(int row1, int col1, int A[3][100], 
                       int row2, int col2, int B[3][100], 
                       int C[3][100]) 
{ 
    // Note that below variables are static 
    // i and j are used to know current cell of 
    // result matrix C[][]. k is used to know 
    // current column number of A[][] and row 
    // number of B[][] to be multiplied 
    static int i = 0, j = 0, k = 0; 
  
    // If all rows traversed. 
    if (i >= row1) 
        return; 
  
    // If i < row1 
    if (j < col2) 
    { 
      if (k < col1) 
      { 
         C[i][j] += A[i][k] * B[k][j]; 
         k++; 
  
         multiplyMatrixRec(row1, col1, A, row2, col2, 
                                               B, C); 
      } 
  
      k = 0; 
      j++; 
      multiplyMatrixRec(row1, col1, A, row2, col2, B, C); 
    } 
  
    j = 0; 
    i++; 
    multiplyMatrixRec(row1, col1, A, row2, col2, B, C); 
} 
  
// Function to multiply two matrices A[][] and B[][] 
void multiplyMatrix(int row1, int col1, int A[3][100], 
                    int row2, int col2, int B[3][100]) 
{ 
    if (row2 != col1) 
    { 
        return; 
    } 
  
    int C[100][100] = {0}; 
  
    multiplyMatrixRec(row1, col1, A, row2, col2, B, C); 
  
    // Print the result 
    for (int i = 0; i < row1; i++) 
    { 
        for (int j = 0; j < col2; j++) {
            int q = 0;
            q++;
        }
    } 
} 
  
// Driven Program 
int main() 
{ 
    int A[3][100] = { {1, 2, 3}, 
                    {4, 5, 6}, 
                    {7, 8, 9}}; 
  
    int B[3][100] = { {1, 2, 3}, 
                    {4, 5, 6}, 
                    {7, 8, 9} }; 
  
    int row1 = 3, col1 = 3, row2 = 3, col2 = 3; 
    multiplyMatrix(row1, col1, A, row2, col2, B); 
  
    return 0; 
} 
