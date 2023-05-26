#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "measurement.h"

double* measurement_probabilities(
    complex *state_vector,
    unsigned char num_qubits,
    unsigned char *qubits_to_measure,
    unsigned char num_qubits_to_measure
)
{
    unsigned int i, j;
    unsigned int length_measure = 1 << num_qubits_to_measure;
    unsigned int length_state = 1 << num_qubits;

    /* Calculate the probability of measuring each state: |amp|^2 . */
    double* probs = (double*)malloc(sizeof(double) * length_measure);
    for (i = 0; i < length_measure; i++) {
        probs[i] = 0.0;
    }

    complex amp;
    double prob;
    unsigned int idx;
    for (i = 0; i < length_state; i++) {
        idx = 0;
        for (j = 0; j < num_qubits_to_measure; j++) {
            // Big-endian (bit zero - LSb - is on the left - index 0 - of the binary string).
            // On the circuit, qubit zero is the less significant one.
            idx += ((i >> (num_qubits-qubits_to_measure[j]-1)) & 1) << (num_qubits_to_measure-j-1);
        }
        amp = state_vector[i];
        prob = pow(complex_abs(amp), 2);
        probs[idx] += prob;
    }

    return probs;
}

unsigned int* measurement_counts(
    double* probabilities,
    unsigned char num_qubits_measured,
    unsigned int shots
)
{
    unsigned int i, j;
    unsigned int length_measure = 1 << num_qubits_measured;

    unsigned int* counts = (unsigned int*)malloc(sizeof(unsigned int) * length_measure);
    for (i = 0; i < length_measure; i++) {
        counts[i] = 0;
    }

    /* Randomly select a state based on the probabilities. */
    double* probs_ptr;
    double rand_val;
    double cum_prob;
    for (j = 0; j < shots; j++) {
        rand_val = ((double)rand() / RAND_MAX);
        cum_prob = 0.0;
        probs_ptr = probabilities;
        for (i = 0; i < length_measure; i++) {
            cum_prob += *probs_ptr++;
            if (rand_val <= cum_prob) {
                counts[i] += 1;
                break;
            }
        }
    }

    return counts;
}

complex* measurement_postselection(
    unsigned int measured_state,
    complex *state_vector,
    unsigned char num_qubits,
    unsigned char *qubits_to_measure,
    unsigned char num_qubits_to_measure
)
{
    unsigned int i, j;
    unsigned int length_measure = 1 << num_qubits_to_measure;
    unsigned int length_state = 1 << num_qubits;

    complex* updated_vector = (complex*)malloc(sizeof(complex) * length_measure);
    for (i = 0; i < length_measure; i++) {
        updated_vector[i] = state_vector[i];
    }

    /*
    * Update the state vector based on `measured_state`.
    */
    for (i = 0; i < length_state; i++) {
        for (j = 0; j < num_qubits_to_measure; j++){
            if (
                ((i >> (num_qubits-qubits_to_measure[j]-1)) & 1) !=
                ((measured_state >> (num_qubits_to_measure-j-1)) & 1)
            ) {
                updated_vector[i].real = 0.0;
                updated_vector[i].imag = 0.0;
                break;
            }
        }
    }
    complex normalization_factor;
    normalization_factor.real = 0.0;
    normalization_factor.imag = 0.0;
    for (i = 0; i < length_state; i++) {
        normalization_factor.real += pow(complex_abs(updated_vector[i]), 2);
    }
    normalization_factor.real = sqrt(normalization_factor.real);
    complex *div;
    for (i = 0; i < length_state; i++) {
        div = divide_complex(updated_vector[i], normalization_factor);
        updated_vector[i] = *div;
        free(div);
    }

    return updated_vector;
}
