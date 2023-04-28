#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

// Print a complex number in the standard format: (a.real) + (a.imag)i
void print_complex(complex a)
{
    printf("%f+%fi", a.real, a.imag);
}

// Print a complex number in the polar format: (abs)exp((arg)i)
void print_complex_polar(complex a)
{
    double magnitude = complex_abs(a);
    double angle = complex_arg(a);

    printf("%f*exp(%fi)", magnitude, angle);
}

// Convert polar information into a standard complex number
complex *polar_to_standard(double mag, double arg) {
    complex *a = (complex*)malloc(sizeof(complex));
    a->real = mag * cos(arg);
    a->imag = mag * sin(arg);
    return a;
}

/* Arithmetic operations */

// Add two complex numbers: a + b
complex *add_complex(complex a, complex b)
{
    complex *c = (complex*)malloc(sizeof(complex));
    c->real = a.real + b.real;
    c->imag = a.imag + b.imag;
    return c;
}

// Subtract two complex numbers: a - b
complex *subtract_complex(complex a, complex b)
{
    complex *c = (complex*)malloc(sizeof(complex));
    c->real = a.real - b.real;
    c->imag = a.imag - b.imag;
    return c;
}

// Multiply two complex numbers: a * b
complex *multiply_complex(complex a, complex b)
{
    complex *c = (complex*)malloc(sizeof(complex));
    c->real = a.real * b.real - a.imag * b.imag;
    c->imag = a.real * b.imag + a.imag * b.real;
    return c;
}

// Divide two complex numbers: a / b
complex *divide_complex(complex a, complex b)
{
    complex *c = (complex*)malloc(sizeof(complex));
    double d = b.real * b.real + b.imag * b.imag;
    c->real = (a.real * b.real + a.imag * b.imag) / d;
    c->imag = (a.imag * b.real - a.real * b.imag) / d;
    return c;
}

/* Basic operations*/

// Absolute value (or modulus or magnitude) of a complex number
double complex_abs(complex a)
{
    return sqrt(a.real * a.real + a.imag * a.imag);
}

// Argument (or phase) of a complex number
double complex_arg(complex a)
{
    return atan2(a.imag, a.real);
}

// Complex conjugate
complex *complex_conjugate(complex a)
{
    complex *b = (complex*)malloc(sizeof(complex));
    b->real = a.real;
    b->imag = -a.imag;
    return b;
}
