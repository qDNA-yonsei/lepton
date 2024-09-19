#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "complex.h"

extern float* measurement_probabilities(
    complex *state_vector,
    unsigned int num_qubits,
    unsigned int *qubits_to_measure,
    unsigned int num_qubits_to_measure
);

extern unsigned int* measurement_counts(
    float* probabiblities,
    unsigned int num_qubits_measured,
    unsigned int shots
);

extern complex* measurement_postselection(
    unsigned int measured_state,
    complex *state_vector,
    unsigned int num_qubits,
    unsigned int *qubits_to_measure,
    unsigned int num_qubits_to_measure
);

#endif /* MEASUREMENT_H */