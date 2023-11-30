#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "tree.h"
#include "log.h"
#include "myassert.h"
#include "calculation.h"

static size_t NumImage  = 0;
static FILE*  FileLog   = NULL;
static FILE*  FileGraph = NULL;
static size_t NullNodes = 0;

static FILE* GenerateImage (void);
static void  GenerateGraph (BinaryTree_t* myTree);
static void  WriteNode     (Node_t* CurrentNode, BinaryTree_t* myTree);
static void  WriteNullNode (const char* Place, Node_t* CurrentNode);
static void  WriteHead     (BinaryTree_t* myTree);
static void  WriteTree     (BinaryTree_t* myTree);
static FILE* OpenFile      (const char* file_open, const char* option);
static void  CloseFile     (FILE* file_text);

void _PrintLogStart (void)
{
    struct stat sb = {};
    if (stat("./" FOLDER_LOG, &sb) || !S_ISDIR(sb.st_mode)) 
    { 
        system("mkdir " FOLDER_LOG);
        system("touch " FILE_GRAPH);
        system("touch " FILE_LOG);
    }

    FileLog = OpenFile (FILE_LOG, "w");

    fprintf(FileLog,    "<!DOCTYPE html>\n"
                        "<html lang=\"eng\">\n"
                        "<head>\n"
                        "<meta charset = \"utf-8\"/>\n"
                        "<title>LOGS</title>\n"
                        "<style>\n"
                        "body {\n"
                        "\tbackground-color: " FILL_BACK_COLOR ";\n"
                        "}\n"
                        "</style>\n"
                        "</head>\n"
                        "<body>\n"
                        "<pre>\n"
                        "<header>\n"
                        "<h1> Logs for Binary Tree </h1>\n"
                        "</header>\n"
                        "<main>\n"
                        "<pre>\n"); 
}

void _PrintLogFinish (void)
{
    fprintf(FileLog,    "</pre>\n"
                        "</main>\n"
                        "</body>\n"
                        "</html>\n"); 
    CloseFile (FileLog);
}

void _PrintLogTree (BinaryTree_t* myTree, const char* file,  const char* function, const size_t line)
{
    GenerateGraph (myTree);
    FILE* pPipe = GenerateImage ();

    fprintf(FileLog,    "\n<p>\n"
                        "<b><big> ### In file: %s,\tIn function: %s,\tIn line: %lu ### </big></b>\n", file, function, line);

    size_t counter_strings = 0;
    while (counter_strings < 6)
    {
        if (fgetc(pPipe) == '\n')
        {
            counter_strings++;
        }
    }
    int symbol = fgetc(pPipe);

    while (symbol != EOF)
    {
        fputc(symbol, FileLog);
        symbol = fgetc(pPipe);
    }

    //"<img src=\"images/%lu." TYPE_OF_IMAGE "\" alt=\"Printing binary tree №%lu\" >\n"
                        
    fprintf(FileLog,    "</p>\n"
                        "<hr size = \"4\" color = \"#000000\">\n\n");
    (NumImage)++;
}

static void GenerateGraph (BinaryTree_t* myTree)
{
    FileGraph = OpenFile(FILE_GRAPH, "w");

    fprintf (FileGraph,     "digraph G{\n"
                            "rankdir = TB;\n"
                            "bgcolor = \"" FILL_BACK_COLOR "\";\n"
                            "node[color = \"" COLOR_FRAME "\", fontsize = 10];\n"
                            "edge[color = \"" COLOR_FRAME "\", fontsize = 15];\n"

                            "subgraph cluster_list {\n"
                            "\tnode[style = filled, fontsize = 10];\n"
                            "\tedge[color = \"" COLOR_FRAME "\", fontsize = 15];\n"
                            "\tstyle = \"dashed\";\n"
                            "\tcolor = \"" COLOR_STIPPLE "\";\n");

    WriteTree(myTree);

    fprintf (FileGraph,     "}\n");

    WriteHead(myTree);

    fprintf (FileGraph,     "}\n");

    CloseFile (FileGraph);
} 

static FILE* GenerateImage (void)
{
    FILE* pPipe = popen ("dot " FILE_GRAPH  " -T " TYPE_OF_IMAGE, "r");
    MYASSERT(pPipe, ERR_BAD_OPEN_FILE, )
    return pPipe;
}

static FILE* OpenFile (const char* file_open, const char* option)
{
    FILE *FileOpen = fopen (file_open, option);

    MYASSERT(FileOpen, OPEN_FILE, return 0);

    return FileOpen;
}

static void CloseFile (FILE* file_text)
{
	MYASSERT(file_text, BAD_POINTER_PASSED_IN_FUNC, assert(0));
    int result = fclose(file_text);
	MYASSERT(!result, CLOSE_FILE, assert(0));
}

static void WriteTree (BinaryTree_t* myTree)
{
    if (!(myTree->Root)) 
    {
        return;
    }
    else
    {
        WriteNode (myTree->Root, myTree);
        NullNodes = 0;
    }

}

static void WriteNode (Node_t* CurrentNode, BinaryTree_t* myTree)
{
    //TODO: cool macros maybe 
    if (CurrentNode->Type == INIT) 
    {                               
        fprintf (FileGraph, "\tnode%p [shape = Mrecord, style = filled, fillcolor = \"" COLOR_INIT "\", color = \"" COLOR_FRAME "\", label = \"{ PARENT: %p | PTR: %p | TYPE: %d | DATA: " SPECIFIER_INIT  " | { <f0> LEFT: %p | <f1> RIGHT: %p }}\"];\n", 
                CurrentNode, CurrentNode->Parent, CurrentNode, INIT, CurrentNode->Value.Number, CurrentNode->Left, CurrentNode->Right);                                                                                                                                              
    }
    if (CurrentNode->Type == NUMBER) 
    {                               
        fprintf (FileGraph, "\tnode%p [shape = Mrecord, style = filled, fillcolor = \"" COLOR_NUMBER "\", color = \"" COLOR_FRAME "\", label = \"{ PARENT: %p | PTR: %p | TYPE: %d | DATA: " SPECIFIER_NUMBER  " | { <f0> LEFT: %p | <f1> RIGHT: %p }}\"];\n", 
                CurrentNode, CurrentNode->Parent, CurrentNode, NUMBER, CurrentNode->Value.Number, CurrentNode->Left, CurrentNode->Right);                                                                                                                                              
    }
    if (CurrentNode->Type == OPERATOR) 
    {                               
        fprintf (FileGraph, "\tnode%p [shape = Mrecord, style = filled, fillcolor = \"" COLOR_OPERATOR "\", color = \"" COLOR_FRAME "\", label = \"{ PARENT: %p | PTR: %p | TYPE: %d | DATA: " SPECIFIER_OPERATOR_STR  " | { <f0> LEFT: %p | <f1> RIGHT: %p }}\"];\n", 
                CurrentNode, CurrentNode->Parent, CurrentNode, OPERATOR, Operators[CurrentNode->Value.Index].Name, CurrentNode->Left, CurrentNode->Right);                                                                                                                                              
    }
    if (CurrentNode->Type == VARIABLE) 
    {                               
        fprintf (FileGraph, "\tnode%p [shape = Mrecord, style = filled, fillcolor = \"" COLOR_VARIABLE "\", color = \"" COLOR_EDGE_GRAPH "\", label = \"{ PARENT: %p | PTR: %p | TYPE: %d | DATA: " SPECIFIER_VARIABLE_STR " | { <f0> LEFT: %p | <f1> RIGHT: %p }}\"];\n", 
                CurrentNode, CurrentNode->Parent, CurrentNode, VARIABLE, myTree->Variables[CurrentNode->Value.Index].Name, CurrentNode->Left, CurrentNode->Right);                                                                                                                                              
    }

    if (CurrentNode->Left)
    {
        WriteNode (CurrentNode->Left, myTree);
        fprintf (FileGraph, "\tnode%p: <f0> -> node%p [color = \"" COLOR_EDGE_GRAPH "\"]\n"
                , CurrentNode, CurrentNode->Left);
    }
    else
    {
        NullNodes++;
        //WriteNullNode ("<f0>", CurrentNode);
    }
    if (CurrentNode->Right)
    {
        WriteNode (CurrentNode->Right, myTree);
        fprintf (FileGraph, "\tnode%p: <f1> -> node%p [color = \"" COLOR_EDGE_GRAPH "\"]\n", CurrentNode, CurrentNode->Right);
    }
    else
    {
        NullNodes++;
        //WriteNullNode ("<f1>", CurrentNode);
    }
}

static void WriteNullNode (const char* Place, Node_t* CurrentNode)
{
    fprintf (FileGraph, "\tnull%lu [shape = ellipse, style = filled, fillcolor = \"" FILL_BACK_GRAPH_NULL "\", color = \"" COLOR_FRAME "\", label = \"Неизвестно кто\"];\n"
                        "\tnode%p: %s -> null%lu", NullNodes, CurrentNode, Place, NullNodes);
    if (!strcmp(Place, "<f0>")) fprintf (FileGraph, "[color = \"" COLOR_EDGE_GRAPH "\", label = \"нет\"];\n");
    if (!strcmp(Place, "<f1>")) fprintf (FileGraph, "[color = \"" COLOR_EDGE_GRAPH "\", label = \"да\"];\n");
}

static void WriteHead(BinaryTree_t* myTree)
{
    fprintf (FileGraph, "All[shape = Mrecord, label = \" HEADER | <f0> Root: %p \", style = \"filled\", fillcolor = \"" FILL_BACK_GRAPH "\"];\n", myTree->Root);
    if (myTree->Root)
    {
        fprintf (FileGraph, "All:<f0> -> node%p [color = \"" COLOR_EDGE_HEAD "\"];\n", myTree->Root);                                           
    }
}
