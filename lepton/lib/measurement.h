#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "complex.h"

extern double* measurement_probabilities(
    complex *state_vector,
    unsigned char num_qubits,
    unsigned char *qubits_to_measure,
    unsigned char num_qubits_to_measure
);

extern unsigned int* measurement_counts(
    double* probabiblities,
    unsigned char num_qubits_measured,
    unsigned int shots
);

extern complex* measurement_postselection(
    unsigned int measured_state,
    complex *state_vector,
    unsigned char num_qubits,
    unsigned char *qubits_to_measure,
    unsigned char num_qubits_to_measure
);

#endif /* MEASUREMENT_H */