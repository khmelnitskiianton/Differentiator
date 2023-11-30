#ifndef FUNC_H
#define FUNC_H

#include "errors_enum.h"

const size_t SIZE_OF_BUFFER = 100;
const int DONE = 1;
const int ERROR = 0;
const double EPSILONE = 10e-4;

//TODO: DSL

//база
//Node_t* RecSearch (const Elem_t* Value, Node_t* CurrentNode);
//---------------------------------------------

EnumOfErrors TreeInOrder     (BinaryTree_t* myTree, FILE* filestream);
EnumOfErrors TreeCalculating (BinaryTree_t* myTree);
EnumOfErrors EnterVariables  (BinaryTree_t* myTree);
EnumOfErrors TreeOptimize    (BinaryTree_t* myTree);

#endif