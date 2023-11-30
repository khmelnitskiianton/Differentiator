#ifndef CALCULATION_H
#define CALCULATION_H

double _add (const double a, const double b);
double _sub (const double a, const double b);
double _mul (const double a, const double b);
double _div (const double a, const double b);
double _pow (const double a, const double b);
double _sin (const double a, const double b);
double _cos (const double a, const double b);
double _tg  (const double a, const double b);
double _ctg (const double a, const double b);
double _sqrt(const double a, const double b);

typedef struct Oper {
    const   char* Name;
    double  (*Operation) (const double, const double);
    bool    TypeOperator; //0 - Binary, 1 - Unary
} Oper_t;

const Oper_t  Operators[] = {
    {"+",    _add, 0},
    {"-",    _sub, 0},
    {"*",    _mul, 0},
    {"/",    _div, 0},
    {"^",    _pow, 0},
    {"sin",  _sin, 1},
    {"cos",  _cos, 1},
    {"tg",   _tg,  1},
    {"ctg",  _ctg, 1},
    {"sqrt", _sqrt,1},
};

const size_t SIZE_OF_OPERATORS = sizeof(Operators)/sizeof(Operators[0]); //размер массива оператора

#endif