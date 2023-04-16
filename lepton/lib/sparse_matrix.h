#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include "complex.h"

// Struct for a sparse matrix element
typedef struct {
    unsigned int row;
    unsigned int col;
    complex value;
} sparse_element;

extern void print_sparse_matrix(sparse_element *A, unsigned int nnz);

extern sparse_element *generate_sparse_identity_matrix(unsigned int n);
extern sparse_element *sparse_kronecker_product(sparse_element *A, unsigned int nnzA, sparse_element *B, unsigned int nnzB, unsigned int nB, unsigned int *nnzC);
extern complex *sparse_matrix_vector_multiplication(sparse_element *A, unsigned int nnz, complex *x, unsigned int m);
extern sparse_element *sparse_add(sparse_element* A, unsigned int nnzA, sparse_element* B, unsigned int nnzB, unsigned int *nnzC);

#endif /* SPARSE_MATRIX_H */