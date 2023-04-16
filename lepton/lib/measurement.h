#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "complex.h"

extern int measure(complex *state_vector, unsigned char num_qubits, unsigned char *qubits_to_measure, unsigned char num_qubits_to_measure, char trace_out);

#endif /* MEASUREMENT_H */