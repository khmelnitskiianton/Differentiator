#ifndef TREE_H
#define TREE_H

#include "element.h"

struct Node {
    NodeValue_t  Value;
    EnumOfType   Type;
    double       TexSize; //for latex
    struct Node* Right;
    struct Node* Left;
    struct Node* Parent;
};

typedef struct Node Node_t;

typedef struct Change {
    char    Name;
    Node_t* ChangeNode;
} Change_t;

typedef struct BinaryTree {
    Node_t*  Root;
    size_t   Size;
    Var_t    Variables[SIZE_OF_VARIABLES];
    bool     ChangeOptimize;
    Change_t Changes[SIZE_OF_CHANGES];
} BinaryTree_t;

#endif