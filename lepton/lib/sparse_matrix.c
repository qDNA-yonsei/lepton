#include <stdio.h>
#include <stdlib.h>
#include "sparse_matrix.h"

// Print a sparse matrix
void print_sparse_matrix(sparse_element *A, int nnz)
{
    for (int i = 0; i < nnz; i++) {
        printf(
            "(%d, %d): %.2f + %.2fi\n",
            A[i].row, A[i].col, A[i].value.real, A[i].value.imag
        );
    }
}

// Generate a sparse identity matrix of size n.
sparse_element *generate_sparse_identity_matrix(int n)
{
    sparse_element *A = (sparse_element*)malloc(n * sizeof(sparse_element));

    for (int i = 0; i < n; i++) {
        A[i].row = i;
        A[i].col = i;
        A[i].value.real = 1.0;
        A[i].value.imag = 0.0;
    }

    return A;
}

/**
 * sparse_kronecker_product() - Calculate the Kronecker product for two sparse
 *                              matrices A and B.
 * @A: Sparse complex matrix.
 * @nnzA: Number of non-zero elements in spase matrix A.
 * @B: Sparse complex matrix.
 * @nnzB: Number of non-zero elements in spase matrix B.
 * @nB: Matrix B number of rows (or columns).
 * @nnzC: Number of non-zero elements in the new spase matrix C.
 * 
 * Return: Sparse complex matrix of size nnzC.
 */
sparse_element *sparse_kronecker_product(sparse_element *A, int nnzA, sparse_element *B, int nnzB, int nB, int *nnzC)
{
    *nnzC = nnzA * nnzB;

    sparse_element* C = (sparse_element*)malloc((*nnzC) * sizeof(sparse_element));

    int k = 0;
    complex valueA, valueB, *valueC;
    for (int i = 0; i < nnzA; i++) {
        int rowA = A[i].row;
        int colA = A[i].col;
        valueA = A[i].value;
        for (int j = 0; j < nnzB; j++) {
            int rowB = B[j].row;
            int colB = B[j].col;

            valueB = B[j].value;
            int rowC = rowA * nB + rowB;
            int colC = colA * nB + colB;

            valueC = multiply_complex(valueA, valueB);
            C[k].row = rowC;
            C[k].col = colC;
            C[k].value = *valueC;
            free(valueC);
            k++;
        }
    }

    return C;
}

/**
 * sparse_matrix_vector_multiplication() - Multiply a column vector by a sparse
 *                                         matrix.
 * @A: Sparse complex matrix.
 * @nnz: Number of non-zero elements in spase matrix A.
 * @x: Complex vector.
 * @m: Lenght of the vector x.
 *
 * Return: Complex vector of size m.
 */
complex *sparse_matrix_vector_multiplication(sparse_element *A, int nnz, complex *x, int m)
{
    complex* y = (complex*)malloc(m * sizeof(complex));
    for (int i = 0; i < m; i++) {
        y[i].real = 0;
        y[i].imag = 0;
    }

    complex value, *mult, *add;
    for (int i = 0; i < nnz; i++) {
        int row = A[i].row;
        int col = A[i].col;
        value = A[i].value;

        mult = multiply_complex(value, x[col]);
        add = add_complex(y[row], *mult);
        y[row] = *add;
        free(mult);
        free(add);
    }

    return y;
}
