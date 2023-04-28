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
    sparse_element* A_ptr = A;
    sparse_element* A_ptr_max = A + nnz;
    while (A_ptr < A_ptr_max) {
        printf(
            "(%d, %d): %f + %fi\n",
            A_ptr->row, A_ptr->col, A_ptr->value.real, A_ptr->value.imag
        );
        A_ptr++;
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

    unsigned int i = 0;
    sparse_element* A_ptr = A;
    sparse_element* A_ptr_max = A + n;
    while (A_ptr < A_ptr_max) {
        A_ptr->row = i;
        A_ptr->col = i;
        A_ptr->value.real = 1.0;
        A_ptr->value.imag = 0.0;
        A_ptr++;
        i++;
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
    
    complex *valueC;
    sparse_element* A_ptr = A;
    sparse_element* A_ptr_max = A + nnzA;
    sparse_element* B_ptr;
    sparse_element* B_ptr_max = B + nnzB;
    sparse_element* C_ptr = C;
    while (A_ptr < A_ptr_max) {
        B_ptr = B;
        while (B_ptr < B_ptr_max) {
            valueC = multiply_complex(A_ptr->value, B_ptr->value);
            C_ptr->row = A_ptr->row * nB + B_ptr->row;
            C_ptr->col = A_ptr->col * nB + B_ptr->col;
            C_ptr->value = *valueC;
            free(valueC);
            B_ptr++;
            C_ptr++;
        }
        A_ptr++;
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
    
    complex* y_ptr = y;
    complex* y_ptr_max = y + m;
    while (y_ptr < y_ptr_max) {
        y_ptr->real = 0;
        y_ptr->imag = 0;
        y_ptr++;
    }

    unsigned int row, col;
    complex value, *mult, *add;
    sparse_element* A_ptr = A;
    sparse_element* A_ptr_max = A + nnz;
    while (A_ptr < A_ptr_max) {
        row = A_ptr->row;
        col = A_ptr->col;
        value = A_ptr->value;
        A_ptr++;

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
    unsigned int i = 0, j = 0;
    complex* add;
    sparse_element* C_ptr = C;
    // loop through both matrices
    while (i < nnzA && j < nnzB) {
        // if the indices match, add the values and store in the result matrix
        if (A[i].row == B[j].row && A[i].col == B[j].col) {
            C_ptr->row = A[i].row;
            C_ptr->col = A[i].col;
            add = add_complex(A[i].value, B[j].value);
            C_ptr->value = *add;
            free(add);
            i++;
            j++;
        }
        // if the row indices don't match, move the pointer of the matrix with the smaller row index
        else if (A[i].row < B[j].row) {
            *C_ptr = A[i];
            i++;
        }
        else {
            *C_ptr = B[j];
            j++;
        }
        C_ptr++;
    }

    // add any remaining elements in A
    while (i < nnzA) {
        *C_ptr = A[i];
        i++;
        C_ptr++;
    }

    // add any remaining elements in B
    while (j < nnzB) {
        *C_ptr = B[j];
        j++;
        C_ptr++;
    }

    *nnzC = C_ptr - C;

    // Reallocate memory for result matrix C to minimize memory usage
    C = (sparse_element*)realloc(C, (*nnzC) * sizeof(sparse_element));

    return C;
}

/** @brief Multiply two sparse matrices.
 *
 *  @param A Sparse complex matrix.
 *  @param nnzA Number of non-zero elements in spase matrix A.
 *  @param rowsA Number of rows of matrix A.
 *  @param B Sparse complex matrix.
 *  @param nnzB Number of non-zero elements in spase matrix B.
 *  @param colsB Number of cols of matrix B.
 *  @param nnzC Number of non-zero elements in the new spase matrix C.
 *
 *  @return Sparse complex matrix of size nnzC.
 */
sparse_element* sparse_multiplication(
    sparse_element* A,
    unsigned int nnzA,
    unsigned int rowsA,
    sparse_element* B,
    unsigned int nnzB,
    unsigned int colsB,
    unsigned int *nnzC
)
{
    // Create the result matrix as a dynamic array
    sparse_element* C = (sparse_element*) malloc((nnzA * nnzB) * sizeof(sparse_element));

    unsigned int i, j;
    complex sum, *mult;
    sparse_element* A_ptr;
    sparse_element* A_ptr_max = A + nnzA;
    sparse_element* B_ptr;
    sparse_element* B_ptr_max = B + nnzB;
    sparse_element* C_ptr = C;
    // Loop over each row of A
    for (i = 0; i < rowsA; i++) {
        // Loop over each column of B
        for (j = 0; j < colsB; j++) {
            sum.real = 0.0;
            sum.imag = 0.0;

            A_ptr = A;
            // Loop over each non-zero element of row i of A and column j of B
            while (A_ptr < A_ptr_max) {
                if (A_ptr->row == i) {
                    B_ptr = B;
                    // Search for the matching element in B
                    while (B_ptr < B_ptr_max) {
                        if (B_ptr->col == j && A_ptr->col == B_ptr->row) {
                            // Multiply the two matching elements
                            mult = multiply_complex(A_ptr->value, B_ptr->value);
                            sum.real += mult->real;
                            sum.imag += mult->imag;
                            free(mult);
                        }
                        B_ptr++;
                    }
                }
                A_ptr++;
            }

            // If the sum is not zero, add the new element to C
            if (sum.real != 0.0 || sum.imag != 0.0) {
                C_ptr->row = i;
                C_ptr->col = j;
                C_ptr->value = sum;
                C_ptr++;
            }
        }
    }

    *nnzC = C_ptr - C;

    // Reallocate the C array to the correct size and return
    C = (sparse_element*)realloc(C, (*nnzC) * sizeof(sparse_element));

    return C;
}