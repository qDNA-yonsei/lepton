#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "measurement.h"

double measure(
    complex *state_vector,
    unsigned char num_qubits,
    unsigned char *qubits_to_measure,
    unsigned char num_qubits_to_measure,
    unsigned int shots,
    char trace_out
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
            idx += ((i >> qubits_to_measure[j]) & 1) << j;
        }
        amp = state_vector[i];
        prob = pow(complex_abs(amp), 2);
        probs[idx] += prob;
    }

    shots = (shots < 1) ? 1 : shots;

    /* Randomly select a state based on the probabilities. */
    double* probs_ptr;
    double rand_val;
    double cum_prob;
    unsigned int measured_state = 0;
    for (j = 0; j < shots; j++) {
        rand_val = ((double)rand() / RAND_MAX);
        cum_prob = 0.0;
        probs_ptr = probs;
        for (i = 0; i < length_measure; i++) {
            cum_prob += *probs_ptr++;
            if (rand_val <= cum_prob) {
                measured_state += i;
                break;
            }
        }
    }
    double sampling_average = ((double)measured_state) / shots;
    measured_state = i; // last measured state.

    /*
    * Update the state vector based on the last measurement
    * (partial trace or tracing out).
    */
    if (trace_out) {
        for (i = 0; i < length_state; i++) {
            for (j = 0; j < num_qubits_to_measure; j++){
                if (
                    ((i >> qubits_to_measure[j]) & 1) !=
                    ((measured_state >> j) & 1)
                ) {
                    state_vector[i].real = 0.0;
                    state_vector[i].imag = 0.0;
                    break;
                }
            }
        }
        complex normalization_factor;
        normalization_factor.real = 0.0;
        normalization_factor.imag = 0.0;
        for (i = 0; i < length_state; i++) {
            normalization_factor.real += pow(complex_abs(state_vector[i]), 2);
        }
        normalization_factor.real = sqrt(normalization_factor.real);
        complex *div;
        for (i = 0; i < length_state; i++) {
            div = divide_complex(state_vector[i], normalization_factor);
            state_vector[i] = *div;
            free(div);
        }
    }

    /* Free allocated memory and return the measured state. */
    free(probs);
    return sampling_average;
}
