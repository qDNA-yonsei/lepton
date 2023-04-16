/*
 * https://openqasm.com/index.html
 * https://arxiv.org/abs/1707.03429
 */
#ifndef GATE_H
#define GATE_H

#include "sparse_matrix.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923 
#endif

#ifndef M_PI_4
#define M_PI_4 0.785398163397448309616 
#endif

sparse_element *id(unsigned char num_qubits, unsigned int *nnz);

sparse_element *x(unsigned char num_qubits, unsigned char target, unsigned int *nnz);
sparse_element *y(unsigned char num_qubits, unsigned char target, unsigned int *nnz);
sparse_element *z(unsigned char num_qubits, unsigned char target, unsigned int *nnz);
sparse_element *h(unsigned char num_qubits, unsigned char target, unsigned int *nnz);

sparse_element *rx(double theta, unsigned char num_qubits, unsigned char target, unsigned int *nnz);
sparse_element *ry(double theta, unsigned char num_qubits, unsigned char target, unsigned int *nnz);
sparse_element *rz(double phi, unsigned char num_qubits, unsigned char target, unsigned int *nnz);
sparse_element *p(double lambda, unsigned char num_qubits, unsigned char target, unsigned int *nnz);

sparse_element *u(double theta, double phi, double lambda, unsigned char num_qubits, unsigned char target, unsigned int *nnz);

sparse_element *cx(unsigned char num_qubits, unsigned char target, unsigned char control, unsigned int *nnz);
sparse_element *cz(unsigned char num_qubits, unsigned char target, unsigned char control, unsigned int *nnz);

sparse_element *ccx(unsigned char num_qubits, unsigned char target, unsigned char control1, unsigned char control2, unsigned int *nnz);

#endif /* GATE_H */