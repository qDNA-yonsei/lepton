#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "complex.h"

extern int measure(complex *state_vector, int num_qubits, int *qubits_to_measure, int num_qubits_to_measure);

#endif /* MEASUREMENT_H */