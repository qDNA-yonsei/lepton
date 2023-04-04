#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include "complex.h"

// Struct for a sparse matrix element
typedef struct {
    int row;
    int col;
    complex value;
} sparse_element;


extern void print_sparse_matrix(sparse_element *A, int nnz);

extern sparse_element *generate_sparse_identity_matrix(int n);
extern sparse_element *sparse_kronecker_product(sparse_element *A, int nnzA, sparse_element *B, int nnzB, int nB, int *nnzC);
extern complex *sparse_matrix_vector_multiplication(sparse_element *A, int nnz, complex *x, int m);

#endif /* SPARSE_MATRIX_H */