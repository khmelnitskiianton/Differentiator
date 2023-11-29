#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "colors.h"
#include "tree.h"
#include "tree_functions.h"
#include "log.h"
#include "functions.h"
#include "myassert.h"

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        printf(GREEN "\n<<<Файл для сохранения базы данных: %s>\n" RESET, argv[1]);
    }

    BinaryTree_t myTree = {};
    TreeCtor (&myTree);
    PrintLogStart ();
    PrintLogTree (&myTree);
    fprintf(stdout, GREEN "Загрузка завершена!\n\n" RESET);
    //==================================================================
    fprintf(stdout, GREEN "Загрузка базы данных...\n" RESET);
    UploadDataBase(&myTree, argv[1]);
    fprintf(stdout, GREEN "Загрузка базы данных завершена!\n\n" RESET);
    PrintLogTree (&myTree);
    //==================================================================
    TreeInOrder(&myTree, stdout);
    EnterVariables(&myTree);
    //==================================================================
    PrintLogFinish();
    TreeDtor (&myTree);
    printf(GREEN "\n<<<GOOD ENDING>>>\n\n" RESET);
    return 0;
}