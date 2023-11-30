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
#include "myassert.h"
#include "verificator.h"

static void   clean_buffer (void);
static void   PrintInNode (Node_t* CurrentNode, FILE* filestream, BinaryTree_t* myTree);
static double RecEvaluate(Node_t* CurrentNode, BinaryTree* myTree);
static bool   Compare (double x, double y);
static void   RecFree (Node_t* CurrentNode);
static EnumOfErrors RecOptimizeNeutral(Node_t* CurrentNode, BinaryTree_t* myTree);
static bool         RecOptimizeConst(Node_t* CurrentNode, BinaryTree_t* myTree);
static EnumOfErrors DeleteNeutralNode(Node_t* NeutralNode, Node_t* BranchNode, BinaryTree_t* myTree);
static EnumOfErrors DeleteNeutralBranch(Node_t* CurrentNode, double NewValue);

EnumOfErrors TreeInOrder (BinaryTree_t* myTree, FILE* filestream)
{
    if (!filestream)
    {
        fprintf(stderr, MAGENTA "Forgot to write file where to write tree!\nTry to start with [./tree.exe <name_of_file>]\n\n" RESET);
        return ERR_BAD_FILESTREAM;
    }
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    Verify(myTree);
    PrintInNode(myTree->Root, filestream, myTree);
    fprintf(filestream, "\n");
    return ERR_OK;
}

static void PrintInNode (Node_t* CurrentNode, FILE* filestream, BinaryTree_t* myTree)
{
    if (!CurrentNode) {fprintf(filestream, "_"); return;}
    fprintf(filestream, "(");

    PrintInNode(CurrentNode->Left, filestream, myTree);

    //TODO: cool macros maybe
    if (CurrentNode->Type == INIT)                             
    {                                                              
        fprintf(filestream, SPECIFIER_INIT, CurrentNode->Value.Number); 
    }
    if (CurrentNode->Type == NUMBER)                                
    {                                                             
        fprintf(filestream, SPECIFIER_NUMBER, CurrentNode->Value.Number); 
    }
    if (CurrentNode->Type == OPERATOR)                            
    {                                                             
        fprintf(filestream, SPECIFIER_OPERATOR_STR, Operators[CurrentNode->Value.Index].Name); 
    }
    if (CurrentNode->Type == VARIABLE)
    {                                                             
        fprintf(filestream, SPECIFIER_VARIABLE_STR, myTree->Variables[CurrentNode->Value.Index].Name); 
    }

    PrintInNode(CurrentNode->Right, filestream, myTree);

    fprintf(filestream, ")");
}

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

static void clean_buffer (void)
{
    char buff[SIZE_OF_BUFFER] = {};
    fgets(buff, SIZE_OF_BUFFER, stdin);
}

EnumOfErrors TreeCalculating(BinaryTree_t* myTree)
{
    double result = RecEvaluate(myTree->Root, myTree);
    printf(CYAN "\nResult: %lf\n" RESET, result);
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
static EnumOfErrors RecOptimizeNeutral(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    if (!CurrentNode) {return ERR_OK;}
    if (CurrentNode->Type == OPERATOR)
    {
        switch (Operators[CurrentNode->Value.Index].Name[0])
        {
            //TODO: DSL
            case '+': //+0 слева справа
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
                if ((CurrentNode->Left->Type  == NUMBER) && (Compare(CurrentNode->Left->Value.Number, 1)))  {DeleteNeutralNode(CurrentNode->Left, CurrentNode->Right, myTree); myTree->ChangeOptimize=1; return ERR_OK;}
                if ((CurrentNode->Right->Type == NUMBER) && (Compare(CurrentNode->Right->Value.Number, 0))) {DeleteNeutralBranch(CurrentNode, 1); myTree->ChangeOptimize=1; return ERR_OK;}
                if ((CurrentNode->Left->Type  == NUMBER) && (Compare(CurrentNode->Left->Value.Number, 0)))  {DeleteNeutralBranch(CurrentNode, 0); myTree->ChangeOptimize=1; return ERR_OK;}
                if ((CurrentNode->Left->Type  == NUMBER) && (Compare(CurrentNode->Left->Value.Number, 0)))  {DeleteNeutralBranch(CurrentNode, 1); myTree->ChangeOptimize=1; return ERR_OK;}
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
        CurrentNode->Value.Number = Operators[CurrentNode->Value.Index].Operation(CurrentNode->Left->Value.Number, CurrentNode->Right->Value.Number);
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