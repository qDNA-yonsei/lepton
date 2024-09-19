/*
 * https://en.wikibooks.org/wiki/C_Programming/complex.h
 * https://en.wikipedia.org/wiki/C_mathematical_functions#complex.h
 */
#ifndef COMPLEX_H
#define COMPLEX_H

// Struct for a complex number
typedef struct {
    float real;
    float imag;
} complex;

extern void print_complex(complex a);
extern void print_complex_polar(complex a);
extern complex *polar_to_standard(float mag, float arg);

/* Arithmetic operations */
extern complex *add_complex(complex a, complex b);
extern complex *subtract_complex(complex a, complex b);
extern complex *multiply_complex(complex a, complex b);
extern complex *divide_complex(complex a, complex b);

/* Basic operations*/
extern float complex_abs(complex a);
extern float complex_arg(complex a);
extern complex *complex_conjugate(complex a);

#endif /* COMPLEX_H */