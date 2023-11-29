#ifndef CALCULATION_H
#define CALCULATION_H

double _Add (const double a, const double b);
double _Sub (const double a, const double b);
double _Mul (const double a, const double b);
double _Div (const double a, const double b);
double _Pow (const double a, const double b);
double _Sin (const double a, const double b);
double _Cos (const double a, const double b);
double _Tg  (const double a, const double b);
double _Ctg (const double a, const double b);
double _Sqrt(const double a, const double b);

typedef struct Oper {
    const char* Name;
    double (*operation) (const double, const double);
} Oper_t;

const Oper_t  Operators[] = {
    {"+",    _Add},
    {"-",    _Sub},
    {"*",    _Mul},
    {"/",    _Div},
    {"^",    _Pow},
    {"sin",  _Sin},
    {"cos",  _Cos},
    {"tg",   _Tg},
    {"ctg",  _Ctg},
    {"sqrt", _Sqrt},
};

const size_t SIZE_OF_OPERATORS = sizeof(Operators)/sizeof(Operators[0]); //размер массива оператора

#endif