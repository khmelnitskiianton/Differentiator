#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "colors.h"
#include "tree.h"
#include "calculation.h"
#include "tree_functions.h"
#include "parsing.h"
#include "functions.h"
#include "log.h"
#include "log_tex.h"
#include "myassert.h"
#include "verificator.h"
#include "DSL.h"

static double       RecEvaluate(Node_t* CurrentNode, BinaryTree* myTree);
static bool         Compare (double x, double y);
static void         RecFree (Node_t* CurrentNode);
static EnumOfErrors RecOptimizeNeutral(Node_t* CurrentNode, BinaryTree_t* myTree);
static bool         RecOptimizeConst(Node_t* CurrentNode, BinaryTree_t* myTree);
static EnumOfErrors DeleteNeutralNode(Node_t* NeutralNode, Node_t* BranchNode, BinaryTree_t* myTree);
static EnumOfErrors DeleteNeutralBranch(Node_t* CurrentNode, double NewValue);

EnumOfErrors EnterVariables(BinaryTree_t* myTree)
{
    if (*(myTree->Variables[0].Name) == 0) return ERR_OK;
    fprintf(stdout, YELLOW "\n\nYou should enter values of variables!\n\n" RESET);
    for (size_t i = 0; i < SIZE_OF_VARIABLES; i++)
    {
        if (*(myTree->Variables[i].Name) == 0) return ERR_OK;
        fprintf(stdout, BLUE "Inter variable \"%s\": " RESET, myTree->Variables[i].Name);
        scanf("%lf", &myTree->Variables[i].Number); //TODO: check for stupid
    }
    return ERR_OK;
}

EnumOfErrors TreeCalculating(BinaryTree_t* myTree)
{
    WriteTexText("После предварительных преобразований, слишком простых для разъяснения получаем: \\\\");
    WriteTexFormula(myTree);
    double result = RecEvaluate(myTree->Root, myTree);
    printf(CYAN "\nResult: %lf\n" RESET, result);
    WriteTexText("\nВ начале рассчитаем значение этой функции при заданных аргументах: \\\\\n");
    size_t i = 0;
    WriteTexText("\n\\begin{center}\n");
    while (myTree->Variables[i].Name[0])
    {
        WriteTexText(myTree->Variables[i].Name);
        WriteTexText(" = ");
        WriteTexNumber(myTree->Variables[i].Number);
        WriteTexText(",");
        i++;
    }
    WriteTexText("\n\\end{center}");
    WriteTexText("\nОчевидно, что оно будет равно: ");
    WriteTexNumber(result);
    WriteTexText(" \\\\\n");
    return ERR_OK;
}

static double RecEvaluate(Node_t* CurrentNode, BinaryTree* myTree)
{
    if (!CurrentNode) 
    {
        return NAN;
    }
    if (CurrentNode->Type == NUMBER)
    {
        return CurrentNode->Value.Number;
    }
    if (CurrentNode->Type == VARIABLE)
    {
        return myTree->Variables[CurrentNode->Value.Index].Number;
    }
    //TODO: optimization for unary operators
    double LeftNumber  = RecEvaluate(CurrentNode->Left, myTree);
    double RightNumber = RecEvaluate(CurrentNode->Right, myTree);

    if (CurrentNode->Type == OPERATOR)
    {
        return Operators[CurrentNode->Value.Index].Operation(LeftNumber, RightNumber); //TODO: DSL
    }

    MYASSERT(0, ERR_UNKNOWN_TYPE, return NAN);
}   

EnumOfErrors TreeOptimize(BinaryTree_t* myTree)
{
    myTree->ChangeOptimize=1;
    while (myTree->ChangeOptimize)
    {
        myTree->ChangeOptimize = 0;
        RecOptimizeConst  (myTree->Root, myTree);
        RecOptimizeNeutral(myTree->Root, myTree);
        PrintLogTree(myTree);
    }
    return ERR_OK;
}

//1. Свертка констант done 
/* 
    0*... ...*0 1*... ...*1
    0+... ...+0
    0-... ...-0
    .../1
    ...^0 0^... ...^1 1^...
*/

//TODO: If unary fnctions has right tree
static EnumOfErrors RecOptimizeNeutral(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    if (!CurrentNode) {return ERR_OK;}
    if (CurrentNode->Type == OPERATOR)
    {
        switch (Operators[CurrentNode->Value.Index].Name[0])
        {
            //TODO: DSL
            case '+': 
                if ((CurrentNode->Left->Type  == NUMBER) && (Compare(CurrentNode->Left->Value.Number, 0)))  {DeleteNeutralNode(CurrentNode->Left, CurrentNode->Right, myTree); myTree->ChangeOptimize=1; return ERR_OK;}
                if ((CurrentNode->Right->Type == NUMBER) && (Compare(CurrentNode->Right->Value.Number, 0))) {DeleteNeutralNode(CurrentNode->Right, CurrentNode->Left, myTree); myTree->ChangeOptimize=1; return ERR_OK;}            
            break;
            case '-': //-0 справа TODO: можно и для 0- менять на -1*
                if ((CurrentNode->Right->Type == NUMBER) && (Compare(CurrentNode->Right->Value.Number, 0))) {DeleteNeutralNode(CurrentNode->Right, CurrentNode->Left, myTree);  myTree->ChangeOptimize=1; return ERR_OK;}
            break;
            case '/':
                if ((CurrentNode->Right->Type == NUMBER) && (Compare(CurrentNode->Right->Value.Number, 1))) {DeleteNeutralNode(CurrentNode->Right, CurrentNode->Left, myTree); myTree->ChangeOptimize=1; return ERR_OK;}
            break;
            case '^':
                if ((CurrentNode->Right->Type == NUMBER) && (Compare(CurrentNode->Right->Value.Number, 1))) {DeleteNeutralNode(CurrentNode->Right, CurrentNode->Left, myTree); myTree->ChangeOptimize=1; return ERR_OK;}
                if ((CurrentNode->Left->Type  == NUMBER) && (Compare(CurrentNode->Left->Value.Number, 1)))  {DeleteNeutralBranch(CurrentNode, 1); myTree->ChangeOptimize=1; return ERR_OK;}
                if ((CurrentNode->Right->Type == NUMBER) && (Compare(CurrentNode->Right->Value.Number, 0))) {DeleteNeutralBranch(CurrentNode, 1); myTree->ChangeOptimize=1; return ERR_OK;}
                if ((CurrentNode->Left->Type  == NUMBER) && (Compare(CurrentNode->Left->Value.Number, 0)))  {DeleteNeutralBranch(CurrentNode, 0); myTree->ChangeOptimize=1; return ERR_OK;}
            break;
            case '*':
                if ((CurrentNode->Left->Type  == NUMBER) && (Compare(CurrentNode->Left->Value.Number, 1)))  {DeleteNeutralNode(CurrentNode->Left, CurrentNode->Right, myTree); myTree->ChangeOptimize=1; return ERR_OK;}
                if ((CurrentNode->Right->Type == NUMBER) && (Compare(CurrentNode->Right->Value.Number, 1))) {DeleteNeutralNode(CurrentNode->Right, CurrentNode->Left, myTree); myTree->ChangeOptimize=1; return ERR_OK;}
                if ((CurrentNode->Left->Type  == NUMBER) && (Compare(CurrentNode->Left->Value.Number, 0)))  {DeleteNeutralBranch(CurrentNode, 0); myTree->ChangeOptimize=1; return ERR_OK;}
                if ((CurrentNode->Right->Type == NUMBER) && (Compare(CurrentNode->Right->Value.Number, 0))) {DeleteNeutralBranch(CurrentNode, 0); myTree->ChangeOptimize=1; return ERR_OK;}
            break;

            default: break;
        }
    }
    RecOptimizeNeutral(CurrentNode->Left, myTree);
    RecOptimizeNeutral(CurrentNode->Right, myTree);
    return ERR_OK;
}

static EnumOfErrors DeleteNeutralNode(Node_t* NeutralNode, Node_t* BranchNode, BinaryTree_t* myTree)
{
    free(NeutralNode);
    Node_t* PreviousNode = BranchNode->Parent;
    //началась пересвязка
    if (!(PreviousNode->Parent)) //корень!
    {
        myTree->Root       = BranchNode;
        BranchNode->Parent = NULL;
        free(PreviousNode);
        return ERR_OK;
    }
    Node_t* NewPreviousNode = PreviousNode->Parent;
    if (NewPreviousNode->Left == PreviousNode)  NewPreviousNode->Left = BranchNode;
    else                                        NewPreviousNode->Right = BranchNode;
    BranchNode->Parent = NewPreviousNode;
    free(PreviousNode);
    return ERR_OK;
}

static EnumOfErrors DeleteNeutralBranch(Node_t* CurrentNode, double NewValue)
{
    //Почистили все что было потом
    RecFree(CurrentNode->Left);
    RecFree(CurrentNode->Right);
    //Замена типа узла на новое значение
    CurrentNode->Left  = NULL;
    CurrentNode->Right = NULL;
    CurrentNode->Type = NUMBER;
    CurrentNode->Value.Number = NewValue;
    return ERR_OK;
}

//2. Удаление нейтральных элементов
static bool RecOptimizeConst(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    if (!CurrentNode)                   return 1;
    if (CurrentNode->Type == NUMBER)    return 1;
    if (CurrentNode->Type == VARIABLE)  return 0;

    bool left  = RecOptimizeConst(CurrentNode->Left, myTree);
    bool right = RecOptimizeConst(CurrentNode->Right, myTree);

    if (left && right && (CurrentNode->Type == OPERATOR))
    {
        CurrentNode->Type         = NUMBER;
        if (Operators[CurrentNode->Value.Index].TypeOperator) CurrentNode->Value.Number = Operators[CurrentNode->Value.Index].Operation(CurrentNode->Left->Value.Number, CurrentNode->Right->Value.Number);
        else CurrentNode->Value.Number = Operators[CurrentNode->Value.Index].Operation(CurrentNode->Left->Value.Number, NAN);
        free(CurrentNode->Left);
        free(CurrentNode->Right);
        CurrentNode->Left  = NULL;
        CurrentNode->Right = NULL;
        myTree->ChangeOptimize=1;
        return 1;   
    }
    else return 0;
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

//===========================================================================================================
//===========================================================================================================
//Взятие производной

EnumOfErrors TreeDifferentiate(BinaryTree_t* myTree)
{
    fprintf(stdout, BLUE "\nStarting differentiate your function...\n" RESET);
    WriteTexText("\nТеперь возьмем эту производную, которую в уме берут в начальной советской школе: \\\\");
    Node_t* OldRoot = myTree->Root;
    myTree->Root = RecDiff(myTree->Root, myTree);
    RecFree(OldRoot);//очищаю старое
    fprintf(stdout, CYAN "\nDone!\n");
    WriteTexText("\nИтак если вы еще не уснули к этому моменту, то поздравляю, мы дошли до ответа: \\\\");
    TreeOptimize(myTree);
    WriteTexFormula(myTree);
    return ERR_OK;
}

Node_t* RecDiff(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    //WriteFormula(myTree);
    if (CurrentNode->Type == NUMBER) //Производная константы 0
    {
        WriteCringeStart();
        WriteTexText("\n\\begin{equation}\n");
        WriteTexText("\\left(");
        WriteTexNumber(CurrentNode->Value.Number);
        WriteTexText("\\right)^{\\prime}");
        WriteTexText("\n\\end{equation}");
        WriteCringeEnd();
        WriteTexText("\n\\begin{equation}\n");
        WriteTexText("0");
        WriteTexText("\n\\end{equation}");
        return NUM(0);
    }
    if (CurrentNode->Type == VARIABLE) //если просто узел с x то 1
    {
        WriteCringeStart();
        WriteTexText("\n\\begin{equation}\n");
        WriteTexText("\\left(");
        WriteTexText(myTree->Variables[CurrentNode->Value.Index].Name);
        WriteTexText("\\right)^{\\prime}");
        WriteTexText("\n\\end{equation}");
        WriteCringeEnd();
        WriteTexText("\n\\begin{equation}\n");
        WriteTexText("1");
        WriteTexText("\n\\end{equation}");
        return NUM(1);
    }
    if (CurrentNode->Type == OPERATOR)
    {
        return Operators[CurrentNode->Value.Index].DiffOperation(myTree, CurrentNode);
    }
    MYASSERT(0, ERR_REC_DIFF, return NULL)
}

Node_t* DiffCreateNode (EnumOfType NewType, NodeValue_t NewValue, Node_t* LeftNode, Node_t* RightNode)
{
    Node_t* NewNode = (Node_t*) calloc (1, sizeof (NewNode[0]));
    MYASSERT(NewNode, ERR_BAD_CALLOC, return NULL)
    InitNode(NewNode);
    NewNode->Left   = LeftNode;
    NewNode->Right  = RightNode;
    NewNode->Type   = NewType;

    if (LeftNode) LeftNode->Parent = NewNode;
    if (RightNode) RightNode->Parent = NewNode;

    if (NewNode->Type == NUMBER) 
    {
        NewNode->Value.Number = NewValue.Number;
    } 
    if ((NewNode->Type == OPERATOR)||(NewNode->Type == VARIABLE))
    {
        NewNode->Value.Index  = NewValue.Index;
    }
    return NewNode;
}

Node_t* RecDiffCreateNode (EnumOfType NewType, NodeValue_t NewValue, Node_t* LeftNode, Node_t* RightNode)
{
    Node_t* NewNode = (Node_t*) calloc (1, sizeof (Node_t));
    MYASSERT(NewNode, ERR_BAD_CALLOC, return NULL)
    InitNode(NewNode);
    if (LeftNode) 
    {
        NewNode->Left = RecDiffCreateNode(LeftNode->Type, LeftNode->Value, LeftNode->Left, LeftNode->Right);
        NewNode->Left->Parent = NewNode;
    }
    else NewNode->Left = NULL;
    if (RightNode) 
    {
        NewNode->Right = RecDiffCreateNode(RightNode->Type, RightNode->Value, RightNode->Left, RightNode->Right);
        NewNode->Right->Parent = NewNode;
    }
    else NewNode->Right = NULL;

    NewNode->Type = NewType;

    if (NewNode->Type == NUMBER) 
    {
        NewNode->Value.Number = NewValue.Number;
    } 
    if ((NewNode->Type == OPERATOR)||(NewNode->Type == VARIABLE))
    {
        NewNode->Value.Index  = NewValue.Index;
    }
    return NewNode;
}