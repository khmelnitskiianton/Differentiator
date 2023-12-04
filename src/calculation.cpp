#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tree.h"
#include "calculation.h"
#include "myassert.h"
#include "functions.h"
#include "DSL.h"
#include "log_tex.h"

static bool Compare (double x, double y);
static void RecFree (Node_t* CurrentNode);

//Binary Operations
double _add(const double a, const double b)
{
    return a + b;
}

Node_t* _dif_add(BinaryTree_t* myTree, Node_t* CurrentNode)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("+");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");

    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("+\\left(");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    return OPR(ADD, DIF(L), DIF(R));
}

double _sub(const double a, const double b)
{
    return a - b;
}

Node_t* _dif_sub(BinaryTree_t* myTree, Node_t* CurrentNode)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("-");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");

    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("-\\left(");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    return OPR(SUB, DIF(L), DIF(R));
}

double _mul(const double a, const double b)
{
    return a * b;
}

Node_t* _dif_mul(BinaryTree_t* myTree, Node_t* CurrentNode)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)");
    WriteTexText("\\cdot");
    WriteTexText("\\left(");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)");
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");

    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}\\cdot ");
    WriteTexNode(R, myTree);
    WriteTexText("+\\left(");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}\\cdot ");
    WriteTexNode(L, myTree);
    WriteTexText("\n\\end{equation}");
    return OPR(ADD, OPR(MUL, DIF(L), CPY(R)), OPR(MUL, CPY(L), DIF(R)));
}

double _div(const double a, const double b)
{
    bool check_div_zero = Compare(b,0);
    USER_ERROR(!check_div_zero, ERR_DIV_ZERO, exit(0))
    return a / b;
}

Node_t* _dif_div(BinaryTree_t* myTree, Node_t* CurrentNode)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(\\frac{");
    WriteTexNode(L, myTree);
    WriteTexText("}{");
    WriteTexNode(R, myTree);
    WriteTexText("}\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");

    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\frac{\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("-\\left(");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}}{");
    WriteTexNode(R, myTree);
    WriteTexText("^{\\prime}");
    WriteTexText("\n\\end{equation}");
    return OPR(DIV, OPR(SUB, OPR(MUL, DIF(L), CPY(R)), OPR(MUL, CPY(L), DIF(R))), OPR(POW, CPY(R), NUM(2)));
}

double _pow(const double a, const double b)
{
    bool check_pow_nan = Compare(b,0);
    USER_ERROR(!check_pow_nan, ERR_POW_NAN, exit(0))
    //TODO: a<0 && b != double
    return pow(a, b);
}

Node_t* _dif_pow(BinaryTree_t* myTree, Node_t* CurrentNode)
{
    if (R->Type == NUMBER) 
    {
        WriteCringeStart();
        WriteTexText("\n\\begin{equation}\n");
        WriteTexText("\\left(\\left(");
        WriteTexNode(L, myTree);
        WriteTexText("\\right)^{");
        WriteTexNumber(R->Value.Number);
        WriteTexText("}\\right)^{\\prime}");
        WriteTexText("\n\\end{equation}");

        WriteCringeEnd();
        WriteTexText("\n\\begin{equation}\n");
        WriteTexNumber(R->Value.Number);
        WriteTexText("\\cdot\\left(");
        WriteTexNode(L, myTree);
        WriteTexText("\\right)^{");
        WriteTexNumber(R->Value.Number-1);
        WriteTexText("}\\cdot\\left(");
        WriteTexNode(L, myTree);                                                                                                                            
        WriteTexText("\\right)^{\\prime}");
        WriteTexText("\n\\end{equation}");
        return OPR(MUL, NUM(R->Value.Number), OPR(MUL, OPR(POW, CPY(L), DiffCreateNode(R->Type, {.Number = R->Value.Number - 1}, (R->Left), (R->Right))), DIF(L)));
    }
    else if (L->Type == NUMBER) 
    {
        WriteCringeStart();
        WriteTexText("\n\\begin{equation}\n");
        WriteTexText("\\left(\\left(");
        WriteTexNumber(L->Value.Number);
        WriteTexText("\\right)^{");
        WriteTexNode(R, myTree);
        WriteTexText("}\\right)^{\\prime}");
        WriteTexText("\n\\end{equation}");

        WriteCringeEnd();
        WriteTexText("\n\\begin{equation}\n");
        WriteTexNumber(L->Value.Number);
        WriteTexText("^{");
        WriteTexNode(R, myTree);
        WriteTexText("}\\cdot\\left(");
        WriteTexNode(R, myTree);
        WriteTexText("\\right)^{\\prime}\\cdot ln\\left(");
        WriteTexNumber(L->Value.Number);
        WriteTexText("\\right)");
        WriteTexText("\n\\end{equation}");
        return OPR(MUL, OPR(MUL, OPR(POW, CPY(L), CPY(R)), DIF(R)), OPR(LN, CPY(L), NULL));
    }
    else 
    {
        WriteCringeStart();
        WriteTexText("\n\\begin{equation}\n");
        WriteTexText("\\left(\\left(");
        WriteTexNode(L, myTree);
        WriteTexText("\\right)^{");
        WriteTexNode(R, myTree);
        WriteTexText("}\\right)^{\\prime}");
        WriteTexText("\n\\end{equation}");

        WriteCringeEnd();
        WriteTexText("\n\\begin{equation}\n");
        WriteTexText("\\left(e^{");
        WriteTexNode(R, myTree);
        WriteTexText("\\cdot ln\\left(");
        WriteTexNode(L, myTree);
        WriteTexText("\\right)}\\right)^{\\prime} = ");
        WriteTexText("e^{");
        WriteTexNode(R, myTree);
        WriteTexText("\\cdot ln\\left(");
        WriteTexNode(L, myTree);
        WriteTexText("\\right)} \\cdot \\left(");
        WriteTexNode(R, myTree);
        WriteTexText("\\cdot ln\\left(");
        WriteTexNode(L, myTree);
        WriteTexText("\\right)\\right)^{\\prime}");
        WriteTexText("\n\\end{equation}");
        Node_t* NewNode = OPR(MUL, CPY(R), OPR(LN, CPY(L), NULL));
        Node_t* DiffNode = DIF(NewNode);
        RecFree(NewNode);
        return OPR(MUL, OPR(POW, CPY(L), CPY(R)), DiffNode);
    }
    return NULL;
}

//Unary Operations
double _sin(const double a, const double b)
{
    UNUSED(b);
    return sin(a);
}

Node_t* _dif_sin(BinaryTree_t* myTree, Node_t* CurrentNode)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(cos\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("sin\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\cdot\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    return OPR(MUL, OPR(COS, CPY(L), NULL), DIF(L));
}

double _cos(const double a, const double b)
{
    UNUSED(b);
    return cos(a);
}

Node_t* _dif_cos(BinaryTree_t* myTree, Node_t* CurrentNode)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(cos\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("-sin\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\cdot\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    return OPR(MUL, NUM(-1), OPR(MUL, OPR(SIN, CPY(L), NULL), DIF(L)));
}

double _tg(const double a, const double b)
{
    UNUSED(b);
    bool check_div_zero = Compare(cos(a),0);
    USER_ERROR(!check_div_zero, ERR_DIV_ZERO, exit(0))
    return sin(a)/cos(a);
}

Node_t* _dif_tg(BinaryTree_t* myTree, Node_t* CurrentNode)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(tg\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\frac{1}{cos\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{2}}\\cdot\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    return OPR(MUL, OPR(DIV, NUM(1), OPR(POW, OPR(COS, CPY(L), NULL), NUM(2))), DIF(L));
}

double _ctg(const double a, const double b)
{
    UNUSED(b);
    bool check_div_zero = Compare(sin(a),0);
    USER_ERROR(!check_div_zero, ERR_DIV_ZERO, exit(0))
    return cos(a)/sin(a);
}

Node_t* _dif_ctg(BinaryTree_t* myTree, Node_t* CurrentNode)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(ctg\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("-\\frac{1}{sin\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\left)^{2}}\\cdot\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    return OPR(MUL, OPR(DIV, NUM(-1), OPR(MUL, OPR(SIN, CPY(L), NULL), OPR(SIN, CPY(L), NULL))), DIF(L));
}

double _ln(const double a, const double b)
{
    UNUSED(b);
    USER_ERROR(a > 0, ERR_LOG_MINUS, exit(0))
    return log(a);
}

Node_t* _dif_ln(BinaryTree_t* myTree, Node_t* CurrentNode)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(ln\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\frac{1}{");
    WriteTexNode(L, myTree);
    WriteTexText("}\\cdot\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    return OPR(MUL, OPR(DIV, NUM(1), CPY(L)), DIF(L));
}

static bool Compare (double x, double y)
{
    if (((isnan (x) == 1) && (isnan (y) == 1)) || (fabs (x - y) < EPSILONE))
        return 1;
    else
        return 0;
}

static void RecFree (Node_t* CurrentNode)
{
    MYASSERT(CurrentNode, ERR_BAD_POINTER_NODE, return)

    if (CurrentNode->Left)
    {
        RecFree (CurrentNode->Left);
    }
    if (CurrentNode->Right)
    {
        RecFree (CurrentNode->Right);
    }
    free(CurrentNode);
}