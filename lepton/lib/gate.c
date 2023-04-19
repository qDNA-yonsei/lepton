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
    unsigned char num_qubits,
    unsigned char target,
    unsigned char *controls,
    unsigned char num_controls,
    unsigned int *nnz_full_gate,
    unsigned char current_qubit
);

sparse_element *id(
    unsigned char num_qubits,
    unsigned int *nnz
) {
    debug("id: start")

    *nnz = 1 << num_qubits;

    debug2("id: num_qubits = %d", num_qubits)
    debug2("id: *nnz = %d", *nnz)

    return generate_sparse_identity_matrix(*nnz);
}

sparse_element *x(
    unsigned char num_qubits,
    unsigned char target,
    unsigned int *nnz
) {
    sparse_element gate[2];

    gate[0].row = 0;
    gate[0].col = 1;
    gate[0].value.real = 1.0;
    gate[0].value.imag = 0.0;

    gate[1].row = 1;
    gate[1].col = 0;
    gate[1].value.real = 1.0;
    gate[1].value.imag = 0.0;

    return create_full_gate(gate, 2, num_qubits, target, NULL, 0, nnz, 0);
}

sparse_element *y(
    unsigned char num_qubits,
    unsigned char target,
    unsigned int *nnz
) {
    sparse_element gate[2];

    gate[0].row = 0;
    gate[0].col = 1;
    gate[0].value.real = 0.0;
    gate[0].value.imag = -1.0;

    gate[1].row = 1;
    gate[1].col = 0;
    gate[1].value.real = 0.0;
    gate[1].value.imag = 1.0;

    return create_full_gate(gate, 2, num_qubits, target, NULL, 0, nnz, 0);
}

sparse_element *z(
    unsigned char num_qubits,
    unsigned char target,
    unsigned int *nnz
) {
    sparse_element gate[2];

    gate[0].row = 0;
    gate[0].col = 0;
    gate[0].value.real = 1.0;
    gate[0].value.imag = 0.0;

    gate[1].row = 1;
    gate[1].col = 1;
    gate[1].value.real = -1.0;
    gate[1].value.imag = 0.0;

    return create_full_gate(gate, 2, num_qubits, target, NULL, 0, nnz, 0);
}

sparse_element *h(
    unsigned char num_qubits,
    unsigned char target,
    unsigned int *nnz
) {
    debug("h: start")

    sparse_element gate[4];

    double z = 1/sqrt(2);

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

    return create_full_gate(gate, 4, num_qubits, target, NULL, 0, nnz, 0);
}



sparse_element *rx(
    double theta,
    unsigned char num_qubits,
    unsigned char target,
    unsigned int *nnz
) {
    return u(theta, -M_PI_2, M_PI_2, num_qubits, target, nnz);
}

sparse_element *ry(
    double theta,
    unsigned char num_qubits,
    unsigned char target,
    unsigned int *nnz
) {
    return u(theta, 0, 0, num_qubits, target, nnz);
}

sparse_element *rz(
    double phi,
    unsigned char num_qubits,
    unsigned char target,
    unsigned int *nnz
) {
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

    return create_full_gate(gate, 2, num_qubits, target, NULL, 0, nnz, 0);
}


sparse_element *p(
    double lambda,
    unsigned char num_qubits,
    unsigned char target,
    unsigned int *nnz
)
{
    return u(0, 0, lambda, num_qubits, target, nnz);
}


sparse_element *u(
    double theta,
    double phi,
    double lambda,
    unsigned char num_qubits,
    unsigned char target,
    unsigned int *nnz
) {
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

    return create_full_gate(gate, 4, num_qubits, target, NULL, 0, nnz, 0);
}


sparse_element *cx(
    unsigned char num_qubits,
    unsigned char target,
    unsigned char control,
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

    unsigned char controls[1];
    controls[0] = control;

    return create_full_gate(gate, 2, num_qubits, target, controls, 1, nnz, 0);
}

sparse_element *cy(
    unsigned char num_qubits,
    unsigned char target,
    unsigned char control,
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

    unsigned char controls[1];
    controls[0] = control;

    return create_full_gate(gate, 2, num_qubits, target, controls, 1, nnz, 0);
}

sparse_element *cz(
    unsigned char num_qubits,
    unsigned char target,
    unsigned char control,
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

    unsigned char controls[1];
    controls[0] = control;

    return create_full_gate(gate, 2, num_qubits, target, controls, 1, nnz, 0);
}

sparse_element *ccx(
    unsigned char num_qubits,
    unsigned char target,
    unsigned char control1,
    unsigned char control2,
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

    unsigned char controls[2];
    controls[0] = control1;
    controls[1] = control2;

    return create_full_gate(gate, 2, num_qubits, target, controls, 2, nnz, 0);
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



sparse_element *create_full_gate(
    sparse_element *gate,
    unsigned int nnz_gate,
    unsigned char num_qubits,
    unsigned char target,
    unsigned char *controls,
    unsigned char num_controls,
    unsigned int *nnz_full_gate,
    unsigned char current_qubit
)
{
    debug("create_full_gate: start")

    if (current_qubit >= num_qubits) {
        *nnz_full_gate = 1;
        debug("create_full_gate: end recursion")
        return generate_sparse_identity_matrix(1);
    }

    sparse_element *full_gate;
    sparse_element *next_gate;
    unsigned int nnz_next_gate;

    next_gate = create_full_gate(
        gate, nnz_gate, num_qubits, target, controls, num_controls, &nnz_next_gate, current_qubit + 1
    );

    if (current_qubit == target) {
        debug("create_full_gate: target")
        full_gate = sparse_kronecker_product(
            gate, nnz_gate, next_gate, nnz_next_gate, 1 << (num_qubits-1-current_qubit), nnz_full_gate
        );
    } else {
        debug("create_full_gate: target else")
        unsigned char i;
        unsigned int nnz_id_gate;
        sparse_element *id_gate = id(1, &nnz_id_gate);
        for (i = 0; i < num_controls; i++) {
            if (current_qubit == controls[i]) {
                debug("create_full_gate: control")

                sparse_element *partial_gate_1;
                sparse_element *partial_gate_2;
                unsigned int nnz_partial_gate_1;
                unsigned int nnz_partial_gate_2;

                sparse_element *one_matrix = one();
                partial_gate_1 = sparse_kronecker_product(
                    one_matrix, 1, next_gate, nnz_next_gate, 1 << (num_qubits-1-current_qubit), &nnz_partial_gate_1
                );
                free(next_gate);
                free(one_matrix);

                sparse_element *zero_matrix = zero();
                next_gate = create_full_gate(
                    id_gate, 2, num_qubits, target, NULL, 0, &nnz_next_gate, current_qubit + 1
                );
                partial_gate_2 = sparse_kronecker_product(
                    zero_matrix, 1, next_gate, nnz_next_gate, 1 << (num_qubits-1-current_qubit), &nnz_partial_gate_2
                );
                full_gate = sparse_add(
                    partial_gate_1, nnz_partial_gate_1, partial_gate_2, nnz_partial_gate_2, nnz_full_gate
                );
                free(partial_gate_1);
                free(partial_gate_2);
                free(zero_matrix);

                break;
            }
        }
        if (i >= num_controls) {
            debug("create_full_gate: id")
            full_gate = sparse_kronecker_product(
                id_gate, 2, next_gate, nnz_next_gate, 1 << (num_qubits-1-current_qubit), nnz_full_gate
            );
        }
    }

    free(next_gate);

    debug("create_full_gate: end")

    return full_gate;
}
