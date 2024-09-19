#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gate.h"

#ifdef DEBUG
#define debug(x) {printf("-> ");printf(x);printf("\n");}
#define debug2(x,y) {printf("-> ");printf(x,y);printf("\n");}
#else
#define debug(x)
#define debug2(x,y)
#endif

extern sparse_element *create_full_gate(
    sparse_element *gate,
    unsigned int nnz_gate,
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *controls,
    unsigned int num_controls,
    unsigned int *nnz_full_gate
);

sparse_element *id(
    unsigned int num_qubits,
    unsigned int *nnz
)
{
    *nnz = 1 << num_qubits;

    return generate_sparse_identity_matrix(*nnz);
}

sparse_element *x(
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *nnz
)
{
    sparse_element gate[2];

    gate[0].row = 0;
    gate[0].col = 1;
    gate[0].value.real = 1.0;
    gate[0].value.imag = 0.0;

    gate[1].row = 1;
    gate[1].col = 0;
    gate[1].value.real = 1.0;
    gate[1].value.imag = 0.0;

    return create_full_gate(gate, 2, num_qubits, target, NULL, 0, nnz);
}

sparse_element *y(
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *nnz
)
{
    sparse_element gate[2];

    gate[0].row = 0;
    gate[0].col = 1;
    gate[0].value.real = 0.0;
    gate[0].value.imag = -1.0;

    gate[1].row = 1;
    gate[1].col = 0;
    gate[1].value.real = 0.0;
    gate[1].value.imag = 1.0;

    return create_full_gate(gate, 2, num_qubits, target, NULL, 0, nnz);
}

sparse_element *z(
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *nnz
)
{
    sparse_element gate[2];

    gate[0].row = 0;
    gate[0].col = 0;
    gate[0].value.real = 1.0;
    gate[0].value.imag = 0.0;

    gate[1].row = 1;
    gate[1].col = 1;
    gate[1].value.real = -1.0;
    gate[1].value.imag = 0.0;

    return create_full_gate(gate, 2, num_qubits, target, NULL, 0, nnz);
}

sparse_element *h(
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *nnz
)
{
    debug("h: start")
    sparse_element gate[4];

    float z = 1/sqrt(2);

    gate[0].row = 0;
    gate[0].col = 0;
    gate[0].value.real = z;
    gate[0].value.imag = 0.0;

    gate[1].row = 0;
    gate[1].col = 1;
    gate[1].value.real = z;
    gate[1].value.imag = 0.0;

    gate[2].row = 1;
    gate[2].col = 0;
    gate[2].value.real = z;
    gate[2].value.imag = 0.0;

    gate[3].row = 1;
    gate[3].col = 1;
    gate[3].value.real = -z;
    gate[3].value.imag = 0.0;

    debug("h: end")
    return create_full_gate(gate, 4, num_qubits, target, NULL, 0, nnz);
}



sparse_element *rx(
    float theta,
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *nnz
)
{
    return u(theta, -M_PI_2, M_PI_2, num_qubits, target, nnz);
}

sparse_element *ry(
    float theta,
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *nnz
)
{
    return u(theta, 0, 0, num_qubits, target, nnz);
}

sparse_element *rz(
    float phi,
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *nnz
)
{
    complex *value;

    sparse_element gate[2];

    value = polar_to_standard(1.0, -phi / 2);
    gate[0].row = 0;
    gate[0].col = 0;
    gate[0].value = *value;
    free(value);

    value = polar_to_standard(1.0, phi / 2);
    gate[1].row = 1;
    gate[1].col = 1;
    gate[1].value = *value;
    free(value);

    return create_full_gate(gate, 2, num_qubits, target, NULL, 0, nnz);
}


sparse_element *p(
    float lambda,
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *nnz
)
{
    return u(0, 0, lambda, num_qubits, target, nnz);
}


sparse_element *u(
    float theta,
    float phi,
    float lambda,
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *nnz
)
{
    complex *value;

    sparse_element gate[4];
    gate[0].row = 0;
    gate[0].col = 0;
    gate[0].value.real = cos(theta / 2) ;
    gate[0].value.imag = 0.0;

    value = polar_to_standard(-sin(theta / 2), lambda);
    gate[1].row = 0;
    gate[1].col = 1;
    gate[1].value = *value;
    free(value);

    value = polar_to_standard(sin(theta / 2), phi);
    gate[2].row = 1;
    gate[2].col = 0;
    gate[2].value = *value;
    free(value);

    value = polar_to_standard(cos(theta / 2), phi + lambda);
    gate[3].row = 1;
    gate[3].col = 1;
    gate[3].value = *value;
    free(value);

    return create_full_gate(gate, 4, num_qubits, target, NULL, 0, nnz);
}


sparse_element *cx(
    unsigned int num_qubits,
    unsigned int target,
    unsigned int control,
    unsigned int *nnz
)
{
    sparse_element gate[2];

    gate[0].row = 0;
    gate[0].col = 1;
    gate[0].value.real = 1.0;
    gate[0].value.imag = 0.0;

    gate[1].row = 1;
    gate[1].col = 0;
    gate[1].value.real = 1.0;
    gate[1].value.imag = 0.0;

    unsigned int controls[1];
    controls[0] = control;

    return create_full_gate(gate, 2, num_qubits, target, controls, 1, nnz);
}

sparse_element *cy(
    unsigned int num_qubits,
    unsigned int target,
    unsigned int control,
    unsigned int *nnz
)
{
    sparse_element gate[2];

    gate[0].row = 0;
    gate[0].col = 1;
    gate[0].value.real = 0.0;
    gate[0].value.imag = -1.0;

    gate[1].row = 1;
    gate[1].col = 0;
    gate[1].value.real = 0.0;
    gate[1].value.imag = 1.0;

    unsigned int controls[1];
    controls[0] = control;

    return create_full_gate(gate, 2, num_qubits, target, controls, 1, nnz);
}

sparse_element *cz(
    unsigned int num_qubits,
    unsigned int target,
    unsigned int control,
    unsigned int *nnz
)
{
    sparse_element gate[2];

    gate[0].row = 0;
    gate[0].col = 0;
    gate[0].value.real = 1.0;
    gate[0].value.imag = 0.0;

    gate[1].row = 1;
    gate[1].col = 1;
    gate[1].value.real = -1.0;
    gate[1].value.imag = 0.0;

    unsigned int controls[1];
    controls[0] = control;

    return create_full_gate(gate, 2, num_qubits, target, controls, 1, nnz);
}

sparse_element *swap(
    unsigned int num_qubits,
    unsigned int target1,
    unsigned int target2,
    unsigned int *nnz
)
{
    /* https://algassert.com/post/1717 */
    unsigned int rows = 1 << num_qubits;
    unsigned int nnzCx;
    sparse_element *gate1 = cx(num_qubits, target1, target2, &nnzCx);
    sparse_element *gate2 = cx(num_qubits, target2, target1, &nnzCx);
    sparse_element *gate3 = sparse_multiplication(gate2, nnzCx, rows, gate1, nnzCx, rows, nnz);
    free(gate2);
    gate2 = sparse_multiplication(gate1, nnzCx, rows, gate3, *nnz, rows, nnz);
    free(gate1);
    free(gate3);

    return gate2;
}

sparse_element *cswap(
    unsigned int num_qubits,
    unsigned int target1,
    unsigned int target2,
    unsigned int control,
    unsigned int *nnz
)
{
    unsigned int rows = 1 << num_qubits;
    unsigned int nnzCx;
    sparse_element *gate1 = ccx(num_qubits, target1, control, target2, &nnzCx);
    sparse_element *gate2 = ccx(num_qubits, target2, control, target1, &nnzCx);
    sparse_element *gate3 = sparse_multiplication(gate2, nnzCx, rows, gate1, nnzCx, rows, nnz);
    free(gate2);
    gate2 = sparse_multiplication(gate1, nnzCx, rows, gate3, *nnz, rows, nnz);
    free(gate1);
    free(gate3);

    return gate2;
}

sparse_element *ccx(
    unsigned int num_qubits,
    unsigned int target,
    unsigned int control1,
    unsigned int control2,
    unsigned int *nnz
)
{
    sparse_element gate[2];

    gate[0].row = 0;
    gate[0].col = 1;
    gate[0].value.real = 1.0;
    gate[0].value.imag = 0.0;

    gate[1].row = 1;
    gate[1].col = 0;
    gate[1].value.real = 1.0;
    gate[1].value.imag = 0.0;

    unsigned int controls[2];
    controls[0] = control1;
    controls[1] = control2;

    return create_full_gate(gate, 2, num_qubits, target, controls, 2, nnz);
}

sparse_element *zero()
{
    sparse_element *gate = (sparse_element*)malloc(sizeof(sparse_element));

    gate[0].row = 0;
    gate[0].col = 0;
    gate[0].value.real = 1.0;
    gate[0].value.imag = 0.0;

    return gate;
}

sparse_element *one()
{
    sparse_element *gate = (sparse_element*)malloc(sizeof(sparse_element));

    gate[0].row = 1;
    gate[0].col = 1;
    gate[0].value.real = 1.0;
    gate[0].value.imag = 0.0;

    return gate;
}


sparse_element *recursive_gate(
    sparse_element *gate,
    unsigned int nnz_gate,
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *controls,
    unsigned int *control_state,
    unsigned int num_controls,
    unsigned int *nnz_full_gate,
    unsigned int current_qubit
)
{
    debug("recursive_gate: start")
    debug2("recursive_gate: current_qubits=%d", current_qubit)

    if (current_qubit >= num_qubits) {
        debug("recursive_gate: ends recursion")

        // Ends recursion.
        *nnz_full_gate = 1;
        return generate_sparse_identity_matrix(1);
    }

    sparse_element *next_gate;
    unsigned int nnz_next_gate;

    next_gate = recursive_gate(
        gate, nnz_gate, num_qubits, target, controls, control_state, num_controls, &nnz_next_gate, current_qubit + 1
    );

    debug("recursive_gate: continue")
    debug2("recursive_gate: current_qubits=%d", current_qubit)

    sparse_element *full_gate;

    if (current_qubit == target) {
        debug("recursive_gate: target")
        full_gate = sparse_kronecker_product(
            gate, nnz_gate, next_gate, nnz_next_gate, 1 << (num_qubits-1-current_qubit), nnz_full_gate
        );
    } else {
        unsigned int i;
        for (i = 0; i < num_controls; i++) {
            if (current_qubit == controls[i]) {
                sparse_element *projector;
                if (control_state[i] == 0) {
                    projector = zero();
                } else {
                    projector = one();
                }

                full_gate = sparse_kronecker_product(
                    projector, 1, next_gate, nnz_next_gate, 1 << (num_qubits-1-current_qubit), nnz_full_gate
                );

                free(projector);

                break;
            }
        }
        if (i >= num_controls) {
            debug("recursive_gate: id product")

            unsigned int nnz_id_gate;
            sparse_element *id_gate = id(1, &nnz_id_gate);

            debug2("recursive_gate: nnz_id_gate=%d", nnz_id_gate)
            debug2("recursive_gate: nnz_next_gate=%d", nnz_next_gate)

            full_gate = sparse_kronecker_product(
                id_gate, nnz_id_gate, next_gate, nnz_next_gate, 1 << (num_qubits-1-current_qubit), nnz_full_gate
            );

            debug("recursive_gate: free(id_gate)")

            free(id_gate);
        }
    }

    debug("recursive_gate: free(next_gate)")

    free(next_gate);

    debug("recursive_gate: end")

    return full_gate;
}

unsigned int *itob(unsigned int const size, unsigned int const *b)
{
    debug("itob: start")

    unsigned int *bits = (unsigned int*)malloc(size * sizeof(unsigned int));

    for (int j = size-1; j >= 0; j--) {
        bits[j] = (b[0] >> j) & 1;
    }

    debug("itob: end")

    return bits;
}

sparse_element *create_full_gate(
    sparse_element *gate,
    unsigned int nnz_gate,
    unsigned int num_qubits,
    unsigned int target,
    unsigned int *controls,
    unsigned int num_controls,
    unsigned int *nnz_full_gate
)
{
    sparse_element *full_gate;

    if (num_controls == 0) {
        debug("create_full_gate: num_controls=0")

        full_gate = recursive_gate(
            gate, nnz_gate, num_qubits, target, NULL, NULL, num_controls, nnz_full_gate, 0
        );
    }
    else {
        debug("create_full_gate: num_controls!=0")

        unsigned int num_partial_states = (1 << num_controls) - 1;
        unsigned int *ctrl_state = itob(num_controls, &num_partial_states);

        full_gate = recursive_gate(
            gate, nnz_gate, num_qubits, target, controls, ctrl_state, num_controls, nnz_full_gate, 0
        );

        free(ctrl_state);

        unsigned int nnz_partial_gate;
        unsigned int nnz_id_gate;
        sparse_element *partial_gate;
        sparse_element *temp_gate;
        sparse_element *id_gate = id(1, &nnz_id_gate);

        for (unsigned int i = 0; i < num_partial_states; i++) {
            ctrl_state = itob(num_controls, &i);
            partial_gate = recursive_gate(
                id_gate, 2, num_qubits, target, controls, ctrl_state, num_controls, &nnz_partial_gate, 0
            );
            free(ctrl_state);
            temp_gate = sparse_add(full_gate, *nnz_full_gate, partial_gate, nnz_partial_gate, nnz_full_gate);
            free(partial_gate);
            free(full_gate);
            full_gate = temp_gate;
        }

        free(id_gate);
    }

    return full_gate;
}
