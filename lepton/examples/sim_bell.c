#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../lib/sparse_matrix.h"
#include "../lib/gate.h"
#include "../lib/measurement.h"

#pragma printf = "%d %f"

void print_state_vector(complex *state_vector, int num_qubits)
{
    unsigned int length = 1 << num_qubits;
    //printf("_standard format_\n");
    for (unsigned int i = 0; i < length; i++) {
        print_complex(state_vector[i]);
        printf("\n");
    }
    // printf("\n_polar format_\n");
    // for (int i = 0; i < length; i++) {
    //     print_complex_polar(state_vector[i]);
    //     printf("\n");
    // }
}

int main(int argc, char *argv[], char *envv[])
{
    /*
     * Simulates a Bell state: (1/sqrt(2))*(|00>-|11>)
     *
     * The current `ad hoc` code creates all matrix operations before the
     * simulation, but to reduce memory usage, it is best to create matrix
     * operations as required and release them afterwards, as an ideal
     * approach.
    */

    complex state_vector[4];   // Two qubit state vector
    sparse_element *cnot_gate; // CNOT gate
    sparse_element *id_gate;   // Identity gate
    sparse_element *h_gate;    // Hadamard gate
    sparse_element *x_gate;    // X gate

    // Initialize the state vector.
    state_vector[0].real = 1.0;
    state_vector[0].imag = 0.0;
    for (unsigned int i = 1; i < 4; i++) {
        state_vector[i].real = 0.0;
        state_vector[i].imag = 0.0;
    }

    unsigned int nnzCx;
    unsigned int nnzId;
    unsigned int nnzH;
    unsigned int nnzX;

    // Initialize the CNOT gate.
    cnot_gate = cx(2, 1, 0, &nnzCx);

    // Initialize the Identity gat1
    id_gate = id(2, &nnzId);

    // Initialize the Hadamard gate.
    h_gate = h(2, 0, &nnzH);
    
    // Initialize the X gate.
    x_gate = x(2, 0, &nnzX);

    /* Perform the simulation. */
    complex *current_state, *current_state2;
    current_state = sparse_matrix_vector_multiplication(x_gate, nnzX, state_vector, 4);
    current_state2 = sparse_matrix_vector_multiplication(h_gate, nnzH, current_state, 4);
    free(current_state);
    current_state = sparse_matrix_vector_multiplication(cnot_gate, nnzCx, current_state2, 4);
    
    free(current_state2);
    free(cnot_gate);
    free(id_gate);
    free(h_gate);
    free(x_gate);

    /* Print final state vector. */
    printf("State vector before the measurement:\n");
    print_state_vector(current_state, 2);
    printf("\n");

    /* Perform one measurement on two qubits. */
    unsigned char *qubits_to_measure = (unsigned char*)malloc(2 * sizeof(unsigned char));
    qubits_to_measure[0] = 0;
    qubits_to_measure[1] = 1;

    srand((unsigned int)clock());
    int outcome = measure(current_state, 2, qubits_to_measure, 2, 1);

    /* Print state vector after the measurement. */
    printf("State vector after the measurement:\n");
    print_state_vector(current_state, 2);
    printf("\n");

    printf("Measurement outcome: %d\n", outcome);

    return 0;
}