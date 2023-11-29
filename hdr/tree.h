#ifndef TREE_H
#define TREE_H

#include "element.h"

struct Node {
    NodeValue_t  Value;
    EnumOfType   Type;
    struct Node* Right;
    struct Node* Left;
    struct Node* Parent;
};

typedef struct Node Node_t;

typedef struct BinaryTree {
    Node_t* Root;
    size_t  Size;
    Var_t   Variables[SIZE_OF_VARIABLES];
} BinaryTree_t;

#endif