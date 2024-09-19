/* Drafter version 0.0.1
   ASCII art of circuit diagrams

   https://github.com/qDNA-yonsei/lepton
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define lower_case(c) ((c) | 32)

#define MAX_GATE_NAME_LEN 32

#ifdef __Z88DK
    #define MAX_QASM_LINE_LEN 128
    #define MAX_QUBITS 16
    #define CIRCUIT_WIDTH 32 // Must be: MAX_QUBITS*2
    #define CIRCUIT_DEPTH 128
#else
    #define MAX_QASM_LINE_LEN 1024
    #define MAX_QUBITS 32
    #define CIRCUIT_WIDTH 64
    #define CIRCUIT_DEPTH 1024
#endif

const char *presentation =
    "DRAFTER version 0.0.4\n"
    "ASCII art of circuit diagrams\n"
    "github.com/qDNA-yonsei/lepton, 2023\n"
    "\n";

const char *usage =
    "Usage: drafter [<qasm>] [/s] [/h]\n"
    "\n"
    "<qasm>: QASM file name.\n"
    "/s: Silent mode.\n"
    "    The presentation is not printed.\n"
    "/h: This help.\n";

const char* invalid_parameter = "Invalid parameter(s)\n";
const char* invalid_num_qubits = "Num. of qubits %d exceeded the max. %d\n";

// Parse a QASM file and store the circuit in a 2D array of characters
void parse_qasm(const char* filename, char* circuit)
{
    // Initialize circuit array to all spaces
    memset(circuit, ' ', CIRCUIT_WIDTH * CIRCUIT_DEPTH);

    // Open QASM file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    unsigned int qubit_target;
    unsigned int qubit_control1, qubit_control2;
    unsigned int num_qubits = MAX_QUBITS;
    char line[MAX_QASM_LINE_LEN];
    char instruction[MAX_GATE_NAME_LEN];

    unsigned char qubit_offsets[MAX_QUBITS];
    memset(qubit_offsets, 0, MAX_QUBITS);

    // Loop over each line in the file
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) > 6) {
            sscanf(line, "%s ", instruction);

            if (strcmp(instruction, "OPENQASM") == 0) {
                /* code */
            }
            else if (strcmp(instruction, "include") == 0) {
                /* code */
            }
            else if (strcmp(instruction, "qreg") == 0) {
                sscanf(line, "%s q[%d]", instruction, &num_qubits);

                if (num_qubits > MAX_QUBITS) {
                    printf(invalid_num_qubits, num_qubits, MAX_QUBITS);
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
            }
            else if (strcmp(instruction, "creg") == 0) {
                /* code */
            }
            else if (strcmp(instruction, "barrier") == 0) {
                unsigned char max_offset = 0;
                for (unsigned int qubit = 0; qubit < num_qubits; qubit++) {
                    if (qubit_offsets[qubit] > max_offset) {
                        max_offset = qubit_offsets[qubit];
                    }
                }
                unsigned int row;
                for (unsigned int qubit = 0; qubit < num_qubits; qubit++) {
                    row = qubit * 2 * CIRCUIT_DEPTH;
                    for (unsigned char col = qubit_offsets[qubit]; col <= max_offset; col++) {
                        circuit[row + col] = '-';
                    }
                    circuit[row + (max_offset + 1)] = '|';
                    if (qubit > 0) {
                        circuit[(row - CIRCUIT_DEPTH) + (max_offset + 1)] = '|';
                    }
                    qubit_offsets[qubit] = max_offset + 2;
                }
            }
            else if (
                strcmp(instruction, "cx") == 0 ||
                strcmp(instruction, "cy") == 0 ||
                strcmp(instruction, "cz") == 0 ||
                strcmp(instruction, "swap") == 0
            ) {
                // Two-qubit instruction
                sscanf(line, "%s q[%d], q[%d]", instruction, &qubit_control1, &qubit_target);

                char target_symbol = 'x';
                char control_symbol = 'o';
                if (strcmp(instruction, "cz") == 0) {
                    target_symbol = 'o';
                }
                else if (strcmp(instruction, "cy") == 0) {
                    target_symbol = 'y';
                }
                else if (strcmp(instruction, "swap") == 0) {
                    target_symbol = 'X';
                    control_symbol = 'X';
                }

                unsigned char row_control = qubit_control1 * 2;
                unsigned char col_control = qubit_offsets[qubit_control1];
                unsigned char row_target = qubit_target * 2;
                unsigned char col_target = qubit_offsets[qubit_target];

                unsigned char col = (col_control > col_target) ? col_control + 1 : col_target + 1;

                circuit[row_control*CIRCUIT_DEPTH + col] = control_symbol;
                circuit[row_target*CIRCUIT_DEPTH + col] = target_symbol;

                unsigned char qubit_max = (qubit_control1 < qubit_target) ? qubit_target : qubit_control1;
                unsigned char qubit_min = (qubit_control1 < qubit_target) ? qubit_control1 : qubit_target;

                unsigned int row;
                for (unsigned char q = qubit_min; q <= qubit_max; q++) {
                    row = q * 2 * CIRCUIT_DEPTH;

                    for (unsigned char i = qubit_offsets[q]; i < col; i++) {
                        circuit[row + i] = '-';
                    }

                    qubit_offsets[q] = col + 1;
                }

                circuit[(qubit_min * 2 + 1)*CIRCUIT_DEPTH + col] = '|';
                for (unsigned char q = qubit_min + 1; q < qubit_max; q++) {
                    row = q * 2 * CIRCUIT_DEPTH;

                    circuit[row + col] = '|';
                    circuit[(row + CIRCUIT_DEPTH) + col] = '|';
                }

            }
            else if (
                strcmp(instruction, "ccx") == 0 ||
                strcmp(instruction, "cswap") == 0
            ) {
                // Three-qubit instruction
                sscanf(
                    line, "%*s q[%d], q[%d], q[%d]", &qubit_control1, &qubit_control2, &qubit_target
                );

                char target_symbol = 'x';
                char control1_symbol = 'o';
                char control2_symbol = 'o';
                if (strcmp(instruction, "cswap") == 0) {
                    target_symbol = 'X';
                    control2_symbol = 'X';
                }

                unsigned char row_control1 = qubit_control1 * 2;
                unsigned char col_control1 = qubit_offsets[qubit_control1];
                unsigned char row_control2 = qubit_control2 * 2;
                unsigned char col_control2 = qubit_offsets[qubit_control2];
                unsigned char row_target = qubit_target * 2;
                unsigned char col_target = qubit_offsets[qubit_target];

                unsigned char col = (col_control1 > col_target) ? col_control1: col_target;
                col = (col_control2 > col) ? col_control2: col;
                col += 1;

                unsigned char qubit_max = (qubit_control1 < qubit_target) ? qubit_target : qubit_control1;
                qubit_max = (qubit_control2 < qubit_max) ? qubit_max : qubit_control2;
                unsigned char qubit_min = (qubit_control1 < qubit_target) ? qubit_control1 : qubit_target;
                qubit_min = (qubit_control2 < qubit_min) ? qubit_control2 : qubit_min;

                unsigned int row;
                for (unsigned char q = qubit_min; q <= qubit_max; q++) {
                    row = q * 2 * CIRCUIT_DEPTH;

                    for (unsigned char i = qubit_offsets[q]; i < col; i++) {
                        circuit[row + i] = '-';
                    }

                    qubit_offsets[q] = col + 1;
                }

                circuit[(qubit_min * 2 + 1)*CIRCUIT_DEPTH + col] = '|';
                for (unsigned char q = qubit_min + 1; q < qubit_max; q++) {
                    row = q * 2 * CIRCUIT_DEPTH;

                    circuit[row + col] = '|';
                    circuit[(row + CIRCUIT_DEPTH) + col] = '|';
                }

                circuit[row_control1*CIRCUIT_DEPTH + col] = control1_symbol;
                circuit[row_control2*CIRCUIT_DEPTH + col] = control2_symbol;
                circuit[row_target*CIRCUIT_DEPTH + col] = target_symbol;

            }
            else {
                // Single-qubit instruction
                sscanf(line, "%s q[%d]", instruction, &qubit_target);

                if (strcmp(instruction, "measure") == 0) {
                    instruction[0] = 'M';
                    instruction[1] = '\0';
                }

                unsigned int row = qubit_target * 2 * CIRCUIT_DEPTH;
                unsigned char col = qubit_offsets[qubit_target];

                circuit[row + col] = '-';
                col += 1;

                unsigned char i;
                for (i = 0; i < strlen(instruction); i++) {
                    circuit[row + (col+i)] = instruction[i];
                }
                qubit_offsets[qubit_target] = col + i;
            }
        }

    }

    // Complete wires
    unsigned char max_offset = 0;
    for (unsigned char qubit = 0; qubit < MAX_QUBITS; qubit++) {
        if (qubit_offsets[qubit] > max_offset) {
            max_offset = qubit_offsets[qubit];
        }
    }
    unsigned int row;
    for (unsigned char qubit = 0; qubit < MAX_QUBITS; qubit++) {
        row = qubit * 2 * CIRCUIT_DEPTH;
        if (qubit_offsets[qubit] > 0) {
            for (unsigned char i = qubit_offsets[qubit]; i <= max_offset; i++) {
                circuit[row + i] = '-';
            }
            circuit[row + (max_offset + 1)] = '\0';
            circuit[(row + CIRCUIT_DEPTH) + (max_offset + 1)] = '\0';
        }
        else {
            circuit[row] = '\0';
            circuit[row + CIRCUIT_DEPTH] = '\0';
        }
    }

    // Close file
    fclose(file);
}

// Print the circuit array to stdout
void print_circuit(char* circuit)
{
    for (unsigned char i = 0; i < CIRCUIT_WIDTH; i++) {
        if (circuit[i * CIRCUIT_DEPTH] != '\0') {
            printf(circuit+(i * CIRCUIT_DEPTH));
            printf("\n");
        }
    }
}

int main(int argc, char** argv)
{
    if(argc == 1) {
        printf(presentation);
        printf(usage);
        exit(EXIT_FAILURE);
    }

    char present = 1;
    char param_char;
    const char* filename;
    for (argv++; --argc; argv++) {
        if(**argv == '/') {
            param_char = lower_case(*(*argv+1));
            if(param_char == 's') {
                present = 0;
            }
            else if(param_char == 'h') {
                printf("%s", presentation);
                printf("%s", usage);
                return 0;
            }
            else {
                printf("%s", invalid_parameter);
                exit(EXIT_FAILURE);
            }
        }
        else {
            filename = *argv;
        }
    }

    if (present) {
        printf(presentation);
    }

    // Parse QASM file and print circuit
    char* circuit = (char*)malloc(CIRCUIT_WIDTH * CIRCUIT_DEPTH);

    parse_qasm(filename, circuit);
    print_circuit(circuit);

    free(circuit);

    return 0;
}
