#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUBITS 10
#define MAX_GATE_NAME_LEN 10
#define CIRCUIT_WIDTH 20
#define CIRCUIT_DEPTH 100

// Parse a QASM file and store the circuit in a 2D array of characters
void parse_qasm(const char* filename, char circuit[CIRCUIT_WIDTH][CIRCUIT_DEPTH])
{
    // Initialize circuit array to all spaces
    memset(circuit, ' ', CIRCUIT_WIDTH * CIRCUIT_DEPTH);

    // Open QASM file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Loop over each line in the file
    int qubit, qubit_target, num_qubits = MAX_QUBITS;
    int qubit_control1, qubit_control2;
    char line[100];
    char instruction[MAX_GATE_NAME_LEN];
    int qubit_offsets[MAX_QUBITS];
    for (int i = 0; i < MAX_QUBITS; i++) {
        qubit_offsets[i] = 0;
    }

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%s ", instruction);
        
        if (strcmp(instruction, "OPENQASM") == 0) {
            /* code */
        }
        else if (strcmp(instruction, "include") == 0) {
            /* code */
        }
        else if (strcmp(instruction, "qreg") == 0) {
            sscanf(line, "%s q[%d]", instruction, &num_qubits);
        }
        else if (strcmp(instruction, "creg") == 0) {
            /* code */
        }
        else if (strcmp(instruction, "barrier") == 0) {
            int max_offset = 0;
            for (int qubit = 0; qubit < num_qubits; qubit++) {
                if (qubit_offsets[qubit] > max_offset) {
                    max_offset = qubit_offsets[qubit];
                }
            }
            for (int qubit = 0; qubit < num_qubits; qubit++) {
                int row = qubit * 2;
                for (int col = qubit_offsets[qubit]; col <= max_offset; col++) {
                    circuit[row][col] = '-';
                }
                circuit[row][max_offset + 1] = '|';
                if (qubit > 0) {
                    circuit[row - 1][max_offset + 1] = '|';
                }
                qubit_offsets[qubit] = max_offset + 2;
            }
        }
        else if (strcmp(instruction, "cx") == 0 || strcmp(instruction, "cz") == 0) {
            // Two-qubit instruction
            sscanf(line, "%s q[%d], q[%d];", instruction, &qubit, &qubit_target);

            char target_symbol = 'x';
            if (strcmp(instruction, "cz") == 0) {
                target_symbol = 'o';
            }

            int row_control = qubit * 2;
            int col_control = qubit_offsets[qubit];
            int row_target = qubit_target * 2;
            int col_target = qubit_offsets[qubit_target];

            int col = (col_control > col_target) ? col_control + 1 : col_target + 1;

            circuit[row_control][col] = 'o';
            circuit[row_target][col] = target_symbol;

            int qubit_max = (qubit < qubit_target) ? qubit_target : qubit;
            int qubit_min = (qubit < qubit_target) ? qubit : qubit_target;
            for (int q = qubit_min; q <= qubit_max; q++) {
                int row = q * 2;

                for (int i = qubit_offsets[q]; i < col; i++) {
                    circuit[row][i] = '-';
                }
                
                qubit_offsets[q] = col + 1;
            }

            circuit[qubit_min * 2 + 1][col] = '|';
            for (int q = qubit_min + 1; q < qubit_max; q++) {
                int row = q * 2;
                
                circuit[row][col] = '|';
                circuit[row + 1][col] = '|';
            }
            
        }
        else if (strcmp(instruction, "ccx") == 0) {
            // Three-qubit instruction
            sscanf(line, "%s q[%d], q[%d], q[%d]", instruction, &qubit_control1, &qubit_control2, &qubit_target);

            char target_symbol = 'x';
            
            int row_control1 = qubit_control1 * 2;
            int col_control1 = qubit_offsets[qubit_control1];
            int row_control2 = qubit_control2 * 2;
            int col_control2 = qubit_offsets[qubit_control2];
            int row_target = qubit_target * 2;
            int col_target = qubit_offsets[qubit_target];

            int col = (col_control1 > col_target) ? col_control1: col_target;
            col = (col_control2 > col) ? col_control2: col;
            col += 1;

            circuit[row_control1][col] = 'o';
            circuit[row_control2][col] = 'o';
            circuit[row_target][col] = target_symbol;

            int qubit_max = (qubit_control1 < qubit_target) ? qubit_target : qubit_control1;
            qubit_max = (qubit_control2 < qubit_max) ? qubit_max : qubit_control2;
            int qubit_min = (qubit_control1 < qubit_target) ? qubit_control1 : qubit_target;
            qubit_min = (qubit_control2 < qubit_min) ? qubit_control2 : qubit_min;
            for (int q = qubit_min; q <= qubit_max; q++) {
                int row = q * 2;

                for (int i = qubit_offsets[q]; i < col; i++) {
                    circuit[row][i] = '-';
                }
                
                qubit_offsets[q] = col + 1;
            }

            circuit[qubit_min * 2 + 1][col] = '|';
            for (int q = qubit_min + 1; q < qubit_max; q++) {
                int row = q * 2;
                
                circuit[row][col] = '|';
                circuit[row + 1][col] = '|';
            }
            
        }
        else {
            // Single-qubit instruction
            sscanf(line, "%s q[%d]", instruction, &qubit);

            if (strcmp(instruction, "measure") == 0) {
                instruction[0] = 'M';
                instruction[1] = '\0';
            }

            int row = qubit * 2;
            int col = qubit_offsets[qubit];

            circuit[row][col] = '-';
            col += 1;

            int i;
            for (i = 0; i < strlen(instruction); i++) {
                circuit[row][col+i] = instruction[i];
            }
            qubit_offsets[qubit] = col + i;
        }

    }
    
    // Complete wires
    int max_offset = 0;
    for (int qubit = 0; qubit < MAX_QUBITS; qubit++) {
        if (qubit_offsets[qubit] > max_offset) {
            max_offset = qubit_offsets[qubit];
        }
    }
    for (int qubit = 0; qubit < MAX_QUBITS; qubit++) {
        int row = qubit * 2;
        if (qubit_offsets[qubit] > 0) {
            for (int i = qubit_offsets[qubit]; i <= max_offset; i++) {
                circuit[row][i] = '-';
            }
            circuit[row][max_offset + 1] = '\0';
            circuit[row + 1][max_offset + 1] = '\0';
        }
        else {
            circuit[row][0] = '\0';
            circuit[row + 1][0] = '\0';
        }
    }

    // Close file
    fclose(file);
}

// Print the circuit array to stdout
void print_circuit(char circuit[CIRCUIT_WIDTH][CIRCUIT_DEPTH])
{
    for (int i = 0; i < CIRCUIT_WIDTH; i++) {
        if (circuit[i][0] != '\0') {
            printf(circuit[i]);
            printf("\n");
        }
    }
}

int main(int argc, char** argv)
{
    // Check for correct number of command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [QASM file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse QASM file and print circuit
    char circuit[CIRCUIT_WIDTH][CIRCUIT_DEPTH];
    parse_qasm(argv[1], circuit);
    print_circuit(circuit);

    return 0;
}