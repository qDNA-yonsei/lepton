#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../lib/sparse_matrix.h"
#include "../lib/measurement.h"

#pragma printf = "%d %s %c %f"

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

    complex state_vector[4]; // Two qubit state vector
    sparse_element cnot[4];  // CNOT gate
    sparse_element id[2];    // Identity gate
    sparse_element *h;       // Hadamard gate
    sparse_element *x;       // X gate

    // Initialize the state vector.
    state_vector[0].real = 1.0;
    state_vector[0].imag = 0.0;
    state_vector[1].real = 0.0;
    state_vector[1].imag = 0.0;
    state_vector[2].real = 0.0;
    state_vector[2].imag = 0.0;
    state_vector[3].real = 0.0;
    state_vector[3].imag = 0.0;

    // Initialize the CNOT gate.
    cnot[0].row = 0;
    cnot[0].col = 0;
    cnot[0].value.real = 1.0;
    cnot[0].value.imag = 0.0;
    cnot[1].row = 1;
    cnot[1].col = 1;
    cnot[1].value.real = 1.0;
    cnot[1].value.imag = 0.0;
    cnot[2].row = 2;
    cnot[2].col = 3;
    cnot[2].value.real = 1.0;
    cnot[2].value.imag = 0.0;
    cnot[3].row = 3;
    cnot[3].col = 2;
    cnot[3].value.real = 1.0;
    cnot[3].value.imag = 0.0;

    // Initialize the Identity gate.
    id[0].row = 0;
    id[0].col = 0;
    id[0].value.real = 1;
    id[0].value.imag = 0;
    id[1].row = 1;
    id[1].col = 1;
    id[1].value.real = 1;
    id[1].value.imag = 0;

    // Initialize the Hadamard gate.
    sparse_element h_1[4]; // single-qubit Hadamard.
    double r = 1/sqrt(2);
    h_1[0].row = 0;
    h_1[0].col = 0;
    h_1[0].value.real = r;
    h_1[0].value.imag = 0;
    h_1[1].row = 0;
    h_1[1].col = 1;
    h_1[1].value.real = r;
    h_1[1].value.imag = 0;
    h_1[2].row = 1;
    h_1[2].col = 0;
    h_1[2].value.real = r;
    h_1[2].value.imag = 0;
    h_1[3].row = 1;
    h_1[3].col = 1;
    h_1[3].value.real = -r;
    h_1[3].value.imag = 0;

    unsigned int nnzH;
    h = sparse_kronecker_product(h_1, 4, id, 2, 2, &nnzH); // two-qubit Hadamard.

    // Initialize the X gate.
    sparse_element x_1[4]; // single-qubit X.
    x_1[0].row = 0;
    x_1[0].col = 1;
    x_1[0].value.real = 1;
    x_1[0].value.imag = 0;
    x_1[1].row = 1;
    x_1[1].col = 0;
    x_1[1].value.real = 1;
    x_1[1].value.imag = 0;

    unsigned int nnzX;
    x = sparse_kronecker_product(x_1, 2, id, 2, 2, &nnzX); // two-qubit X. |00>-|11>
    //x = sparse_kronecker_product(id, 2, x_1, 2, 2, &nnzX); // two-qubit X. |01>+|10>

    /* Perform the simulation. */
    complex *current_state, *current_state2;
    current_state = sparse_matrix_vector_multiplication(x, nnzX, state_vector, 4);
    current_state2 = sparse_matrix_vector_multiplication(h, nnzH, current_state, 4);
    free(current_state);
    current_state = sparse_matrix_vector_multiplication(cnot, 4, current_state2, 4);
    free(current_state2);

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