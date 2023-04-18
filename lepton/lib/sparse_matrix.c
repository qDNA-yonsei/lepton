#include <stdio.h>
#include <stdlib.h>
#include "sparse_matrix.h"

/** @brief Print a sparse matrix.
 *
 *  @param A Sparse complex matrix.
 *  @param nnz Number of non-zero elements in spase matrix A.
 */
void print_sparse_matrix(sparse_element *A, unsigned int nnz)
{
    for (unsigned int i = 0; i < nnz; i++) {
        printf(
            "(%d, %d): %.2f + %.2fi\n",
            A[i].row, A[i].col, A[i].value.real, A[i].value.imag
        );
    }
}

/** @brief Generate a sparse identity matrix of size n.
 *
 *  @param n length of the identity diagonal.
 *  @return Sparse complex identity matrix of size n.
 */
sparse_element *generate_sparse_identity_matrix(unsigned int n)
{
    sparse_element *A = (sparse_element*)malloc(n * sizeof(sparse_element));

    for (unsigned int i = 0; i < n; i++) {
        A[i].row = i;
        A[i].col = i;
        A[i].value.real = 1.0;
        A[i].value.imag = 0.0;
    }

    return A;
}

/** @brief Calculate the Kronecker product for two sparse
 *         matrices A and B.
 *  @param A Sparse complex matrix.
 *  @param nnzA Number of non-zero elements in spase matrix A.
 *  @param B Sparse complex matrix.
 *  @param nnzB Number of non-zero elements in spase matrix B.
 *  @param nB Matrix B number of rows (or columns).
 *  @param nnzC Number of non-zero elements in the new spase matrix C.
 *
 *  @return Sparse complex matrix of size nnzC.
 */
sparse_element *sparse_kronecker_product(
    sparse_element *A,
    unsigned int nnzA,
    sparse_element *B,
    unsigned int nnzB,
    unsigned int nB,
    unsigned int *nnzC
)
{
    *nnzC = nnzA * nnzB;

    sparse_element* C = (sparse_element*)malloc((*nnzC) * sizeof(sparse_element));

    unsigned int i, j, k = 0;
    unsigned int rowA, colA;
    unsigned int rowB, colB;
    unsigned int rowC, colC;
    complex valueA, valueB, *valueC;
    for (i = 0; i < nnzA; i++) {
        rowA = A[i].row;
        colA = A[i].col;
        valueA = A[i].value;
        for (j = 0; j < nnzB; j++) {
            rowB = B[j].row;
            colB = B[j].col;

            valueB = B[j].value;
            rowC = rowA * nB + rowB;
            colC = colA * nB + colB;

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

/** @brief Multiply a column vector by a sparse matrix.
 *
 *  @param A Sparse complex matrix.
 *  @param nnz Number of non-zero elements in spase matrix A.
 *  @param x Complex vector.
 *  @param m Lenght of the vector x.
 *
 *  @return Complex vector of size m.
 */
complex *sparse_matrix_vector_multiplication(
    sparse_element *A,
    unsigned int nnz,
    complex *x,
    unsigned int m
)
{
    unsigned int i;
    complex* y = (complex*)malloc(m * sizeof(complex));
    for (i = 0; i < m; i++) {
        y[i].real = 0;
        y[i].imag = 0;
    }

    unsigned int row, col;
    complex value, *mult, *add;
    for (i = 0; i < nnz; i++) {
        row = A[i].row;
        col = A[i].col;
        value = A[i].value;

        mult = multiply_complex(value, x[col]);
        add = add_complex(y[row], *mult);
        y[row] = *add;
        free(mult);
        free(add);
    }

    return y;
}

/** @brief Sums two sparse matrices.
 *
 *  @param A Sparse complex matrix.
 *  @param nnzA Number of non-zero elements in spase matrix A.
 *  @param B Sparse complex matrix.
 *  @param nnzB Number of non-zero elements in spase matrix B.
 *  @param nnzC Number of non-zero elements in the new spase matrix C.
 *
 *  @return Sparse complex matrix of size nnzC.
 */
sparse_element *sparse_add(
    sparse_element* A,
    unsigned int nnzA,
    sparse_element* B,
    unsigned int nnzB,
    unsigned int *nnzC
)
{
    // allocate memory for the result matrix
    sparse_element* C = (sparse_element*)malloc((nnzA + nnzB) * sizeof(sparse_element));

    // initialize pointers for iterating through the three matrices
    unsigned int i = 0, j = 0, k = 0;
    complex *add;

    // loop through both matrices
    while (i < nnzA && j < nnzB) {
        // if the indices match, add the values and store in the result matrix
        if (A[i].row == B[j].row && A[i].col == B[j].col) {
            C[k].row = A[i].row;
            C[k].col = A[i].col;
            add = add_complex(A[i].value, B[j].value);
            C[k].value = *add;
            free(add);
            i++;
            j++;
            k++;
        }
        // if the row indices don't match, move the pointer of the matrix with the smaller row index
        else if (A[i].row < B[j].row) {
            C[k].row = A[i].row;
            C[k].col = A[i].col;
            C[k].value = A[i].value;
            i++;
            k++;
        }
        else {
            C[k].row = B[j].row;
            C[k].col = B[j].col;
            C[k].value = B[j].value;
            j++;
            k++;
        }
    }

    // add any remaining elements in A
    while (i < nnzA) {
        C[k].row = A[i].row;
        C[k].col = A[i].col;
        C[k].value = A[i].value;
        i++;
        k++;
    }

    // add any remaining elements in B
    while (j < nnzB) {
        C[k].row = B[j].row;
        C[k].col = B[j].col;
        C[k].value = B[j].value;
        j++;
        k++;
    }

    *nnzC = k;

    // Reallocate memory for result matrix C to minimize memory usage
    C = (sparse_element*)realloc(C, k * sizeof(sparse_element));

    return C;
}
