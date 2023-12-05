#ifndef ELEMENT_H
#define ELEMENT_H

#include <math.h>
#include <limits.h>

typedef double Elem_t;

#define SPECIFIER_SCANF "%lf"
#define SPECIFIER_INIT " %.3lf "
#define SPECIFIER_NUMBER " %.3lf "
#define SPECIFIER_OPERATOR_STR " %s "
#define SPECIFIER_VARIABLE_STR " %s "
#define SPECIFIER_VARIABLE_NUM " %.3lf "

const Elem_t POISON_ELEMENT = NAN;
const double EPSILONE = 1e-5; 

enum EnumOfType {
    INIT     = 0,
    NUMBER   = 1,
    OPERATOR = 2,
    VARIABLE = 3,
    FUNCTION = 4,
};

const size_t SIZE_OF_VARIABLES = 30;   //макс. кол-во переменных
const size_t SIZE_OF_VAR = 100;        //макс. размер одной переменной, можно сделать динамикой
const size_t SIZE_OF_CHANGES = 25;   //макс. кол-во переменных

typedef union NodeValue {
    Elem_t Number;
    int    Index;
} NodeValue_t;

typedef struct Var {
    char   Name[SIZE_OF_VAR];
    Elem_t Number;
} Var_t;

#endif