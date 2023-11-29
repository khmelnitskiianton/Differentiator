#ifndef FUNC_H
#define FUNC_H

#include "errors_enum.h"

const size_t SIZE_OF_BUFFER = 100;
const int DONE = 1;
const int ERROR = 0;
const int NOT_IN_OPER = -1;

//TODO: DSL

//база
size_t  FileSize   (FILE *file_text);
Node_t* RecSearch (const Elem_t* Value, Node_t* CurrentNode);
//---------------------------------------------

//Сохранение базы данных
EnumOfErrors TreeInOrder   (BinaryTree_t* myTree, FILE* filestream);
void PrintInNode (Node_t* CurrentNode, FILE* filestream, BinaryTree_t* myTree);
//-------------------------------------------------------------------

//Чтение базы данных
EnumOfErrors UploadDataBase (BinaryTree_t* myTree, const char* file_database);
EnumOfErrors RecScanData(size_t* position, const char* text_buffer, Node_t** ResNode, Node_t* CurrentNode, BinaryTree_t* myTree);
size_t SkipSpaces(size_t current_position, const char* text_buffer);
int CharInOperators(const char ch);
int StrInOperators(const char* str);
EnumOfErrors ProcessObject (char* object_buffer, Node_t* NewNode, BinaryTree_t* myTree);
int InsertVariable (char* object_buffer, BinaryTree_t* myTree);
void CleanCharBuffer(char* buffer, const size_t buffer_size);
//--------------------------------------------------------------------------------------

//Ввод переменных
EnumOfErrors EnterVariables(BinaryTree_t* myTree);
void clean_buffer (void);

#endif