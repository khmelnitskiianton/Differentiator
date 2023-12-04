#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#include "colors.h"
#include "tree.h"
#include "tree_functions.h"
#include "log.h"
#include "log_tex.h"
#include "parsing.h"
#include "functions.h"
#include "myassert.h"

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        printf(GREEN "\n<<<FILE TO IN/OUT: %s>\n" RESET, argv[1]);
    }
    srand((unsigned int)time(NULL));//задали srand

    BinaryTree_t myTree = {};
    TreeCtor (&myTree);
    PrintLogStart ();
    PrintLogTree (&myTree);
    fprintf(stdout, GREEN "Download completed!\n\n" RESET);
    //==================================================================
    fprintf(stdout, GREEN "Download Data base...\n" RESET);
    UploadDataBase(&myTree, argv[1]);
    fprintf(stdout, GREEN "Downloading complete!\n\n" RESET);
    PrintLogTree (&myTree);
    PrintLogTexStart();
    WriteTexFormula(&myTree);
    //==================================================================
    TreeOptimize(&myTree);
    EnterVariables(&myTree);
    TreeCalculating(&myTree);
    TreeDifferentiate(&myTree);
    PrintLogTree (&myTree);
    //==================================================================
    PrintLogTexFinish();
    PrintLogFinish();
    TreeDtor(&myTree);
    printf(GREEN "\n<<<GOOD ENDING>>>\n\n" RESET);
    return 0;
}