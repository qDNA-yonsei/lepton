/* State Vector simulator version 0.0.1
   Quantum circuit statevector simulator

   https://github.com/qDNA-yonsei/lepton
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/sparse_matrix.h"
#include "../lib/gate.h"
#include "../lib/measurement.h"

#ifdef __Z88DK
#pragma printf = "%d %s %f"
#pragma scanf = "%d %s %f"
#endif

#define lower_case(c) ((c) | 32)

#define MAX_INSTRUCTION_LEN 32

#ifdef __Z88DK
#define MAX_QUBITS 8
#define MAX_QASM_LINE_LEN 128
#else
#define MAX_QUBITS 32
#define MAX_QASM_LINE_LEN 1024
#endif

#ifdef DEBUG
#define debug(x) {printf("-> ");printf(x);printf("\n");}
#define debug2(x,y) {printf("-> ");printf(x,y);printf("\n");}
#else
#define debug(x)
#define debug2(x,y)
#endif

const char *presentation =
    "State Vector simulator version 0.0.4\n"
    "Quantum circuit statevector simulator\n"
    "github.com/qDNA-yonsei/lepton, 2023\n"
    "\n";

const char *usage =
    "Usage: statevec [<qasm>] [/v<print>] [/m<shots>] [/s] [/h]\n"
    "\n"
    "<qasm>: QASM file name.\n"
    "/v: Enable or disable printing the state vector (0|1).\n"
    "    If <print>=0 (default), the state vector is not printed.\n"
    "/m: Set the number of mesurement shots (0-8192)\n"
    "    If <shots>=0 (default), the measurement counts are not printed.\n"
    "/s: Silent mode.\n"
    "    The presentation is not printed.\n"
    "/h: This help.\n";

const char* invalid_parameter = "Invalid parameter(s)\n";
const char* invalid_num_qubits = "Num. of qubits %d exceeded the max. %d\n";

void print_bits(int const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int j;

    for (j = size-1; j >= 0; j--) {
        byte = (b[0] >> j) & 1;
        printf("%d", byte);
    }
}

void print_state_vector(complex *state_vector, unsigned char num_qubits)
{
    unsigned int length = 1 << num_qubits;
    for (unsigned int i = 0; i < length; i++) {
        print_bits(num_qubits, &i);
        printf(" : ");
        print_complex(state_vector[i]);
        printf("\n");
    }
}

void print_probabilities_vector(double *probs_vector, unsigned int num_qubits)
{
    unsigned int length = 1 << num_qubits;
    for (unsigned int i = 0; i < length; i++) {
        print_bits(num_qubits, &i);
        printf(" : ");
        printf("%f", probs_vector[i]);
        printf("\n");
    }
}

void print_counts_vector(unsigned int *counts_vector, unsigned int num_qubits)
{
    unsigned int length = 1 << num_qubits;
    for (unsigned int i = 0; i < length; i++) {
        print_bits(num_qubits, &i);
        printf(" : ");
        printf("%d", counts_vector[i]);
        printf("\n");
    }
}

void replace_char(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
}

void remove_char(char* str, char find) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != find);
    }
    *pw = '\0';
}

// Parses a QASM file and builds the corresponding quantum gates.
complex *parse_qasm(
    const char* filename,
    unsigned int *num_qubits,
    unsigned int *qubits_to_measure,
    unsigned int *num_qubits_to_measure
)
{
    debug("parse_qasm: start")

    unsigned int N; // Number of amplitudes.
    complex *state_vector;
    unsigned int qubits_to_measure_index = 0;

    // Initialize number of qubits
    *num_qubits = MAX_QUBITS;

    debug("parse_qasm: initial variables")

    // Open QASM file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    debug("parse_qasm: file open")

    // Loop over each line in the file
    unsigned int qubit_target;
    unsigned int qubit_control1, qubit_control2;

    char line[MAX_QASM_LINE_LEN];
    char instruction[MAX_INSTRUCTION_LEN];
    char instruction2[MAX_INSTRUCTION_LEN];
    char instruction3[MAX_INSTRUCTION_LEN];

    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) > 6) {

            sscanf(line, "%s ", instruction);
            replace_char(instruction, '(', ' ');
            sscanf(instruction, "%s", instruction);

            debug2("parse_qasm: instruction = %s", instruction)

            if (strcmp(instruction, "OPENQASM") == 0) {
                /* code */
            }
            else if (strcmp(instruction, "include") == 0) {
                /* code */
            }
            else if (strcmp(instruction, "measure") == 0) {
                debug("parse_qasm: measure")

                sscanf(line, "%*s q[%d]", &qubit_target);

                debug2("parse_qasm: qubit_target = %d", qubit_target)

                qubits_to_measure[qubits_to_measure_index++] = qubit_target;
            }
            else if (strcmp(instruction, "qreg") == 0) {
                debug("parse_qasm: qreg")

                sscanf(line, "%*s q[%d]", num_qubits);

                debug2("parse_qasm: num_qubits = %d", *num_qubits)

                if (*num_qubits > MAX_QUBITS) {
                    printf(invalid_num_qubits, *num_qubits, MAX_QUBITS);
                    fclose(file);
                    exit(EXIT_FAILURE);
                }

                N = 1 << *num_qubits; // Number of amplitudes.

                state_vector = (complex*)malloc(sizeof(complex) * N);

                // Initialize the state vector |0>.
                state_vector[0].real = 1.0;
                state_vector[0].imag = 0.0;
                for (unsigned int i = 1; i < N; i++) {
                    state_vector[i].real = 0.0;
                    state_vector[i].imag = 0.0;
                }

                // Initialize qubits to measure
                memset(qubits_to_measure, 0, *num_qubits); // Max. num_qubits.

                debug("parse_qasm: end qreg")
            }
            else if (strcmp(instruction, "creg") == 0) {
                /* code */
            }
            else if (strcmp(instruction, "barrier") == 0) {
                /* code */
            }
            else if (
                strcmp(instruction, "rx") == 0 ||
                strcmp(instruction, "ry") == 0 ||
                strcmp(instruction, "rz") == 0 ||
                strcmp(instruction, "p") == 0
            ) {
                debug("parse_qasm: parameterized single-qubit")

                // Single-qubit parameterized instruction
                sscanf(line, "%s q[%d]", instruction2, &qubit_target);
                remove_char(instruction2, ' ');
                remove_char(instruction2, ')');
                replace_char(instruction2, '(', ' ');
                sscanf(instruction2, "%*s %s", instruction2);

                debug2("parse_qasm: instruction = %s", instruction)
                debug2("parse_qasm: instruction2 = %s", instruction2)
                debug2("parse_qasm: qubit_target = %d", qubit_target)

                double parameter_value;
                double parameter_value2;

                char *slash;
                slash = strchr(instruction2, '/');
                if (slash != NULL) {
                    replace_char(instruction2, '/', ' ');
                    sscanf(instruction2, "%s%*c%s", instruction2, instruction3);

                    debug2("parse_qasm: instruction2 = %s", instruction2)
                    debug2("parse_qasm: instruction3 = %s", instruction3)
                }

                if (strcmp(instruction2, "pi") == 0) {
                    parameter_value = M_PI;
                }
                else if (strcmp(instruction2, "-pi") == 0) {
                    parameter_value = -M_PI;
                }
                else {
                    #ifdef __Z88DK
                    sscanf(instruction2, "%f", &parameter_value);
                    #else
                    parameter_value = strtod(instruction2, NULL);
                    #endif
                }

                if (slash != NULL) {
                    #ifdef __Z88DK
                    sscanf(instruction3, "%f", &parameter_value2);
                    #else
                    parameter_value2 = strtod(instruction3, NULL);
                    #endif
                    parameter_value = parameter_value / parameter_value2;
                }

                debug2("parse_qasm: parameter_value = %f", parameter_value)
                debug2("parse_qasm: parameter_value2 = %f", parameter_value2)

                unsigned int nnz;
                sparse_element *gate;

                if (strcmp(instruction, "rx") == 0) {
                    gate = rx(parameter_value, *num_qubits, qubit_target, &nnz);
                }
                else if (strcmp(instruction, "ry") == 0) {
                    gate = ry(parameter_value, *num_qubits, qubit_target, &nnz);
                }
                else if (strcmp(instruction, "rz") == 0) {
                    gate = rz(parameter_value, *num_qubits, qubit_target, &nnz);
                }
                else if (strcmp(instruction, "p") == 0) {
                    gate = p(parameter_value, *num_qubits, qubit_target, &nnz);
                }

                complex *temp_state;
                temp_state = sparse_matrix_vector_multiplication(gate, nnz, state_vector, N);
                free(gate);
                free(state_vector);
                state_vector = temp_state;
            }
            else if (
                strcmp(instruction, "cx") == 0 ||
                strcmp(instruction, "cy") == 0 ||
                strcmp(instruction, "cz") == 0 ||
                strcmp(instruction, "swap") == 0
            ) {
                debug("parse_qasm: two-qubit")
                debug2("parse_qasm: num_qubits = %d", *num_qubits)

                // Two-qubit instruction
                sscanf(line, "%*s q[%d], q[%d]", &qubit_control1, &qubit_target);

                debug2("parse_qasm: qubit_target = %d", qubit_target)
                debug2("parse_qasm: qubit_control1 = %d", qubit_control1)

                unsigned int nnz;
                sparse_element *gate;

                if (strcmp(instruction, "cx") == 0) {
                    debug("parse_qasm: cx")
                    gate = cx(*num_qubits, qubit_target, qubit_control1, &nnz);
                }
                else if (strcmp(instruction, "cy") == 0) {
                    gate = cy(*num_qubits, qubit_target, qubit_control1, &nnz);
                }
                else if (strcmp(instruction, "cz") == 0) {
                    gate = cz(*num_qubits, qubit_target, qubit_control1, &nnz);
                }
                else if (strcmp(instruction, "swap") == 0) {
                    debug("parse_qasm: swap")
                    gate = swap(*num_qubits, qubit_target, qubit_control1, &nnz);
                }

                complex *temp_state;
                temp_state = sparse_matrix_vector_multiplication(gate, nnz, state_vector, N);
                free(gate);
                free(state_vector);
                state_vector = temp_state;

                debug("parse_qasm: end two-qubit")
            }
            else if (
                strcmp(instruction, "ccx") == 0 ||
                strcmp(instruction, "cswap") == 0
            ) {
                debug("parse_qasm: three-qubit")

                // Three-qubit instruction
                sscanf(
                    line, "%*s q[%d], q[%d], q[%d]", &qubit_control1, &qubit_control2, &qubit_target
                );

                debug2("parse_qasm: qubit_target = %d", qubit_target)
                debug2("parse_qasm: qubit_control1 = %d", qubit_control1)
                debug2("parse_qasm: qubit_control2 = %d", qubit_control2)

                unsigned int nnz;
                sparse_element *gate;

                if (strcmp(instruction, "ccx") == 0) {
                    debug("parse_qasm: ccx")
                    gate = ccx(*num_qubits, qubit_target, qubit_control1, qubit_control2, &nnz);
                }
                else if (strcmp(instruction, "cswap") == 0) {
                    debug("parse_qasm: cswap")
                    gate = cswap(*num_qubits, qubit_control2, qubit_target, qubit_control1, &nnz);
                }

                complex *temp_state;
                temp_state = sparse_matrix_vector_multiplication(gate, nnz, state_vector, N);
                free(gate);
                free(state_vector);
                state_vector = temp_state;
            }
            else {
                debug("parse_qasm: single-qubit")
                debug2("parse_qasm: num_qubits = %d", *num_qubits)

                // Single-qubit instruction
                sscanf(line, "%*s q[%d]", &qubit_target);

                debug2("parse_qasm: qubit_target = %d", qubit_target)

                unsigned int nnz;
                sparse_element *gate;

                if (strcmp(instruction, "x") == 0) {
                    debug("parse_qasm: x")
                    gate = x(*num_qubits, qubit_target, &nnz);
                }
                else if (strcmp(instruction, "y") == 0) {
                    debug("parse_qasm: y")
                    gate = y(*num_qubits, qubit_target, &nnz);
                }
                else if (strcmp(instruction, "z") == 0) {
                    debug("parse_qasm: z")
                    gate = z(*num_qubits, qubit_target, &nnz);
                }
                else if (strcmp(instruction, "h") == 0) {
                    debug("parse_qasm: h")
                    gate = h(*num_qubits, qubit_target, &nnz);
                }
                else {
                    debug("parse_qasm: id")
                    gate = id(*num_qubits, &nnz);
                }

                complex *temp_state;
                temp_state = sparse_matrix_vector_multiplication(gate, nnz, state_vector, N);
                free(gate);
                free(state_vector);
                state_vector = temp_state;

                debug("parse_qasm: end single-qubit")
            }
        }
    }

    // Close file
    fclose(file);

    *num_qubits_to_measure = qubits_to_measure_index;

    debug("parse_qasm: end")

    return state_vector;
}

int main(int argc, char** argv)
{
    if(argc == 1) {
        printf(presentation);
        printf(usage);
        exit(EXIT_FAILURE);
    }

    char verbose = 1;
    char param_char;
    char show_state_vector = 0;
    unsigned int shots = 0;
    const char* filename;
    for (argv++; --argc; argv++) {
        if(**argv == '/') {
            param_char = lower_case(*(*argv+1));
            if(param_char == 's') {
                verbose = 0;
            }
            else if(param_char == 'h') {
                printf("%s", presentation);
                printf("%s", usage);
                return 0;
            }
            else if(param_char == 'v') {
                show_state_vector = atoi(*argv+2);
            }
            else if(param_char == 'm') {
                shots = atoi(*argv+2);
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

    if (verbose) {
        printf(presentation);
    }

    complex *state_vector;
    unsigned int num_qubits;
    unsigned int num_qubits_to_measure;
    unsigned int *qubits_to_measure = (unsigned int*)malloc(MAX_QUBITS * sizeof(unsigned int*));

    debug("main: before parse_qasm")
    state_vector = parse_qasm(filename, &num_qubits, qubits_to_measure, &num_qubits_to_measure);
    debug("main: after parse_qasm")

    /* Print final state vector. */
    if (show_state_vector) {
        if (verbose) {
            printf("State vector:\n");
        }
        debug2("main: num_qubits = %d", num_qubits)
        print_state_vector(state_vector, num_qubits);
        printf("\n");
    }

    /* Print ideal measurement probabilities. */
    double* probs = measurement_probabilities(
            state_vector,
            num_qubits,
            qubits_to_measure,
            num_qubits_to_measure
        );

    if (verbose) {
        printf("Measurement probabilities:\n");
    }
    print_probabilities_vector(probs, num_qubits_to_measure);

    /* Perform the shots. */
    if (shots > 0) {
        debug2("main: shots : shots = %d", shots)
        debug2("main: shots : num_qubits_to_measure = %d", num_qubits_to_measure)

        /* Print measurement counts. */
        srand((unsigned int)clock());
        unsigned int* counts = measurement_counts(
            probs,
            num_qubits_to_measure,
            shots
        );

        printf("\n");
        if (verbose) {
            printf("Measurement counts:\n");
        }
        print_counts_vector(counts, num_qubits_to_measure);
    }

    return 0;
}
