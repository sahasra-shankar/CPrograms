#include "mathlib.h"

#include <math.h>
#include <stdio.h>

#define EPSILON 1e-14

double Abs(double x) //replaces fabs
{
    if (x < 0) {
        x = x * -1;
    }
    return x;
}

//referred to Eugene's code a bit
double Sin(double x) //test in range [-2pi, 2pi], step = 0.1
{
    double sum = x;
    double num = x;
    double den = 1.0;
    double term = num / den;
    for (double k = 1.0; Abs(term) > EPSILON; k += 2) {
        if (x < -2 * M_PI || x > 2 * M_PI) {
            x = fmod(x, M_PI);
        }
        num *= -x * x;
        den *= (k + 1) * (k + 2);
        term = num / den;
        sum += term;
    }
    return sum;
}

//referred to Eugene's code a bit
double Cos(double x) //test in range [-2pi, 2pi], steps = 0.1
{
    double sum = 1.0;
    double num = 1.0;
    double den = 1.0;
    double term = num / den;
    for (double k = 0.0; Abs(term) > EPSILON; k += 2) {
        if (x < -2 * M_PI || x > 2 * M_PI) {
            x = fmod(x, 2 * M_PI);
        }
        num *= -x * x;
        den *= (k + 1) * (k + 2);
        term = num / den;
        sum += term;
    }
    return sum;
}

double Tan(double x) //test in range [-pi/3, pi/3], steps = 0.1
{
    return Sin(x) / Cos(x);
}

//referred to Prof. Long's code
double Exp(double x) //test in range [1, 10), steps = 0.1
{
    double term = 1.0;
    double sum = term;
    double k = 1.0;
    while (Abs(term) > EPSILON) {
        term = x / k * term;
        sum += term;
        k += 1.0;
    }
    return sum;
}

//referred to Prof. Long's code
double Log(double x) //test in range [1, 10), steps = 0.1
{
    double sum = 1.0;
    double k = Exp(sum);
    while (Abs(k - x) > EPSILON) {
        sum += (x - k) / k;
        k = Exp(sum);
    }
    return sum;
}
