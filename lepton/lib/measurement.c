#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "measurement.h"

int measure(complex *state_vector, int num_qubits, int *qubits_to_measure, int num_qubits_to_measure)
{
    int length_measure = 1 << num_qubits_to_measure;
    int length_state = 1 << num_qubits;

    /* Calculate the probability of measuring each state: |amp|^2 . */
    double *probs = (double*)malloc(sizeof(double) * length_measure);
    for (int i = 0; i < length_measure; i++) {
        probs[i] = 0.0;
    }

    for (int i = 0; i < length_state; i++) {
        int idx = 0;
        for (int j = 0; j < num_qubits_to_measure; j++) {
            idx += ((i >> qubits_to_measure[j]) & 1) << j;
        }
        complex amp;
        amp = state_vector[i];
        double prob = pow(complex_abs(amp), 2);
        probs[idx] += prob;
    }

    /* Randomly select a state based on the probabilities. */
    //srand((unsigned int)time(NULL));
    srand((unsigned int)clock());
    rand();
    double rand_val = ((double)rand() / RAND_MAX);
    double cum_prob = 0.0;
    int measured_state = -1;
    for (int i = 0; i < length_measure; i++) {
        cum_prob += probs[i];
        if (rand_val <= cum_prob) {
            measured_state = i;
            break;
        }
    }

    /*
     * Update the state vector based on the measurement
     * (partial trace or tracing out).
     */
    if (measured_state >= 0) {
        for (int i = 0; i < length_state; i++) {
            for (int j = 0; j < num_qubits_to_measure; j++){
                if (
                    ((i >> qubits_to_measure[j]) & 1) !=
                    ((measured_state >> j) & 1)
                ) {
                    state_vector[i].real = 0.0;
                    state_vector[i].imag = 0.0;
                }
            }
        }
        complex normalization_factor;
        normalization_factor.real = 0.0;
        normalization_factor.imag = 0.0;
        for (int i = 0; i < length_state; i++) {
            normalization_factor.real += pow(complex_abs(state_vector[i]), 2);
        }
        normalization_factor.real = sqrt(normalization_factor.real);
        complex *div;
        for (int i = 0; i < length_state; i++) {
            div = divide_complex(state_vector[i], normalization_factor);
            state_vector[i] = *div;
            free(div);
        }
    }

    /* Free allocated memory and return the measured state. */
    free(probs);
    return measured_state;
}
