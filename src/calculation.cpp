#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "calculation.h"

//Binary Operations
double _Add(const double a, const double b)
{
    return a + b;
}
double _Sub(const double a, const double b)
{
    return a - b;
}
double _Mul(const double a, const double b)
{
    return a * b;
}
double _Div(const double a, const double b)
{
    return a / b;
}
double _Pow(const double a, const double b)
{
    return pow(a, b);
}
//Unary Operations
double _Sin(const double a, const double b)
{
    
    return sin(a);
}
double _Cos(const double a, const double b)
{
    return cos(a);
}
double _Tg(const double a, const double b)
{
    return sin(a)/cos(a);
}
double _Ctg(const double a, const double b)
{
    return cos(a)/sin(a);
}
double _Sqrt(const double a, const double b)
{
    return sqrt(a);
}