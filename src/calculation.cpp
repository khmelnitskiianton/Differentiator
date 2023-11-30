#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "calculation.h"
#include "myassert.h"

static bool Compare (double x, double y);
static bool check_double (double z);
const double EPSILONE = 1e-5; 

//Binary Operations
double _add(const double a, const double b)
{
    return a + b;
}
double _sub(const double a, const double b)
{
    return a - b;
}
double _mul(const double a, const double b)
{
    return a * b;
}
double _div(const double a, const double b)
{
    bool check_div_zero = Compare(b,0);
    USER_ERROR(!check_div_zero, ERR_DIV_ZERO, exit(0))
    return a / b;
}
double _pow(const double a, const double b)
{
    bool check_pow_nan = Compare(b,0);
    USER_ERROR(!check_pow_nan, ERR_POW_NAN, exit(0))
    //TODO: a<0 and b not int!
    return pow(a, b);
}
//Unary Operations
double _sin(const double a, const double b)
{
    return sin(a);
}
double _cos(const double a, const double b)
{
    return cos(a);
}
double _tg(const double a, const double b)
{
    bool check_div_zero = Compare(cos(a),0);
    USER_ERROR(!check_div_zero, ERR_DIV_ZERO, exit(0))
    return sin(a)/cos(a);
}
double _ctg(const double a, const double b)
{
    bool check_div_zero = Compare(sin(a),0);
    USER_ERROR(!check_div_zero, ERR_DIV_ZERO, exit(0))
    return cos(a)/sin(a);
}
double _sqrt(const double a, const double b)
{
    bool check_sqrt_minus = (a < 0);
    USER_ERROR(!check_sqrt_minus, ERR_SQRT_MINUS, exit(0))
    return sqrt(a);
}

static bool Compare (double x, double y)
{
    if (((isnan (x) == 1) && (isnan (y) == 1)) || (fabs (x - y) < EPSILONE))
        return 1;
    else
        return 0;
}
static bool check_double (double z)
{
    return (isnan (z) || isfinite (z));
}