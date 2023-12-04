#ifndef FUNC_H
#define FUNC_H

#include "errors_enum.h"

const size_t SIZE_OF_BUFFER = 100;
const int DONE = 1;
const int ERROR = 0;

//TODO: DSL

EnumOfErrors TreeCalculating   (BinaryTree_t* myTree);
EnumOfErrors EnterVariables    (BinaryTree_t* myTree);
EnumOfErrors TreeOptimize      (BinaryTree_t* myTree);
EnumOfErrors TreeDifferentiate (BinaryTree_t* myTree);

Node_t*      DiffCreateNode (EnumOfType NewType, NodeValue_t NewValue, Node_t* LeftNode, Node_t* RightNode);
Node_t*      RecDiffCreateNode (EnumOfType NewType, NodeValue_t NewValue, Node_t* LeftNode, Node_t* RightNode);
Node_t*      RecDiff(Node_t* CurrentNode, BinaryTree_t* myTree);

#endif