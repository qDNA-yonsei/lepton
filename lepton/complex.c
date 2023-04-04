#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

// Print a complex number in the standard format: (a.real) + (a.imag)i
void print_complex(complex a)
{
    printf("%.3f+%.3fi", a.real, a.imag);
}

// Print a complex number in the polar format: (abs)exp((arg)i)
void print_complex_polar(complex a)
{
    double magnitude = complex_abs(a);
    double angle = complex_arg(a);

    printf("%.3f*exp(%.3fi)", magnitude, angle);
}

/* Arithmetic operations */

// Add two complex numbers: a + b
complex add_complex(complex a, complex b)
{
    complex c;
    c.real = a.real + b.real;
    c.imag = a.imag + b.imag;
    return c;
}

// Subtract two complex numbers: a - b
complex subtract_complex(complex a, complex b)
{
    complex c;
    c.real = a.real - b.real;
    c.imag = a.imag - b.imag;
    return c;
}

// Multiply two complex numbers: a * b
complex multiply_complex(complex a, complex b)
{
    complex c;
    c.real = a.real * b.real - a.imag * b.imag;
    c.imag = a.real * b.imag + a.imag * b.real;
    return c;
}

// Divide two complex numbers: a / b
complex divide_complex(complex a, complex b)
{
    complex c;
    double d = b.real * b.real + b.imag * b.imag;
    c.real = (a.real * b.real + a.imag * b.imag) / d;
    c.imag = (a.imag * b.real - a.real * b.imag) / d;
    return c;
}

/* Basic operations*/

// Absolute value (or modulus or magnitude) of a complex number 
double complex_abs(complex a)
{
    double b;
    b = sqrt(a.real * a.real + a.imag * a.imag);
    return b;
}

// Argument (or phase) of a complex number
double complex_arg(complex a)
{
    double b;
    b = atan2(a.imag, a.real);
    return b;
}

// Complex conjugate
complex complex_conjugate(complex a)
{
    complex b;
    b.real = a.real;
    b.imag = -a.imag;
    return b;
}