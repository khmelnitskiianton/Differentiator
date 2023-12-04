#ifndef CALCULATION_H
#define CALCULATION_H

#define UNUSED(x) (void)(x)

double _add (const double a, const double b);
double _sub (const double a, const double b);
double _mul (const double a, const double b);
double _div (const double a, const double b);
double _pow (const double a, const double b);
double _sin (const double a, const double b);
double _cos (const double a, const double b);
double _tg  (const double a, const double b);
double _ctg (const double a, const double b);
double _ln  (const double a, const double b); 

Node_t* _dif_add(BinaryTree_t* myTree, Node_t* CurrentNode);
Node_t* _dif_sub(BinaryTree_t* myTree, Node_t* CurrentNode);
Node_t* _dif_mul(BinaryTree_t* myTree, Node_t* CurrentNode);
Node_t* _dif_div(BinaryTree_t* myTree, Node_t* CurrentNode);
Node_t* _dif_pow(BinaryTree_t* myTree, Node_t* CurrentNode);
Node_t* _dif_sin(BinaryTree_t* myTree, Node_t* CurrentNode);
Node_t* _dif_cos(BinaryTree_t* myTree, Node_t* CurrentNode);
Node_t* _dif_tg (BinaryTree_t* myTree, Node_t* CurrentNode);
Node_t* _dif_ctg(BinaryTree_t* myTree, Node_t* CurrentNode);
Node_t* _dif_ln (BinaryTree_t* myTree, Node_t* CurrentNode);

typedef struct Oper {
    const char* Name;
    double      (*Operation) (const double, const double);
    bool        TypeOperator; //0 - Binary, 1 - Unary TODO: use it
    Node_t*     (*DiffOperation)(BinaryTree_t* myTree, Node_t* CurrentNode);
} Oper_t;

constexpr Oper_t  Operators[] = {
    {"+",    _add, 1, _dif_add},
    {"-",    _sub, 1, _dif_sub},
    {"*",    _mul, 1, _dif_mul},
    {"/",    _div, 1, _dif_div},
    {"^",    _pow, 1, _dif_pow},
    {"sin",  _sin, 0, _dif_sin},
    {"cos",  _cos, 0, _dif_cos},
    {"tg",   _tg,  0, _dif_tg},
    {"ctg",  _ctg, 0, _dif_ctg},
    {"ln",   _ln,  0, _dif_ln},
};

enum EnumOfFunctions {
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    SIN,
    COS,
    TG,
    CTG,
    LN,
};

const size_t SIZE_OF_OPERATORS = sizeof(Operators)/sizeof(Operators[0]); //размер массива оператора

#endif