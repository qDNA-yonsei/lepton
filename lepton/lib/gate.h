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

sparse_element *id(unsigned int num_qubits, unsigned int *nnz);

sparse_element *x(unsigned int num_qubits, unsigned int target, unsigned int *nnz);
sparse_element *y(unsigned int num_qubits, unsigned int target, unsigned int *nnz);
sparse_element *z(unsigned int num_qubits, unsigned int target, unsigned int *nnz);
sparse_element *h(unsigned int num_qubits, unsigned int target, unsigned int *nnz);

sparse_element *rx(float theta, unsigned int num_qubits, unsigned int target, unsigned int *nnz);
sparse_element *ry(float theta, unsigned int num_qubits, unsigned int target, unsigned int *nnz);
sparse_element *rz(float phi, unsigned int num_qubits, unsigned int target, unsigned int *nnz);
sparse_element *p(float lambda, unsigned int num_qubits, unsigned int target, unsigned int *nnz);

sparse_element *u(float theta, float phi, float lambda, unsigned int num_qubits, unsigned int target, unsigned int *nnz);

sparse_element *cx(unsigned int num_qubits, unsigned int target, unsigned int control, unsigned int *nnz);
sparse_element *cy(unsigned int num_qubits, unsigned int target, unsigned int control, unsigned int *nnz);
sparse_element *cz(unsigned int num_qubits, unsigned int target, unsigned int control, unsigned int *nnz);
sparse_element *swap(unsigned int num_qubits, unsigned int target1, unsigned int target2, unsigned int *nnz);
sparse_element *cswap(unsigned int num_qubits, unsigned int target1, unsigned int target2, unsigned int control, unsigned int *nnz);

sparse_element *ccx(unsigned int num_qubits, unsigned int target, unsigned int control1, unsigned int control2, unsigned int *nnz);

#endif /* GATE_H */