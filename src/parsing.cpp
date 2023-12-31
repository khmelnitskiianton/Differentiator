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

static FILE*        OpenFile        (const char* file_open, const char* option);
static void         CloseFile       (FILE* file_text);
static size_t       FileSize        (FILE *file_text);
static EnumOfErrors RecScanData     (size_t* position, const char* text_buffer, Node_t** ResNode, Node_t* CurrentNode, BinaryTree_t* myTree);
static size_t       SkipSpaces      (size_t current_position, const char* text_buffer);
static int          CharInOperators (const char ch);
static int          StrInOperators  (const char* str);
static EnumOfErrors ProcessObject   (char* object_buffer, Node_t* NewNode, BinaryTree_t* myTree);
static int          InsertVariable  (char* object_buffer, BinaryTree_t* myTree);
static void         CleanCharBuffer (char* buffer, const size_t buffer_size);

EnumOfErrors UploadDataBase (BinaryTree_t* myTree, const char* file_database)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    MYASSERT(file_database, ERR_WHAT_FILE_OF_DATA, return ERR_WHAT_FILE_OF_DATA)

    FILE* FileRead = OpenFile (file_database, "r");
    USER_ERROR(FileRead, ERR_NO_FILE_TO_OPEN, exit(0))

    size_t size_text = FileSize (FileRead);

    char* text_buffer = NULL;
    text_buffer = (char*) calloc (size_text + 1, sizeof (char));
    MYASSERT(text_buffer, ERR_BAD_CALLOC, return ERR_BAD_CALLOC)
    size_t result_size = fread (text_buffer, 1, size_text, FileRead);

    MYASSERT(result_size == size_text, ERR_BAD_FREAD, return ERR_BAD_FREAD);

    if (*text_buffer == '\0') 
    {
        fprintf(stdout, YELLOW "Data base is empty!\n" RESET);
        return ERR_OK;
    }

	*(text_buffer + size_text) = '\0';
    CloseFile (FileRead);

    //printf("\n%s\n", text_buffer);

    size_t position = 0;
    RecScanData(&position, text_buffer, NULL, myTree->Root, myTree);

    free(text_buffer);

    return ERR_OK;   
}

static FILE* OpenFile (const char* file_open, const char* option)
{
    FILE *FileOpen = fopen (file_open, option);

    MYASSERT(FileOpen, OPEN_FILE, return NULL);

    return FileOpen;
}

static void CloseFile (FILE* file_text)
{
	MYASSERT(file_text, BAD_POINTER_PASSED_IN_FUNC, assert(0));
    int result = fclose(file_text);
	MYASSERT(!result, CLOSE_FILE, assert(0));
}

static size_t FileSize (FILE *file_text)
{
    MYASSERT(file_text, ERR_BAD_POINTER_FILE, return 0)
	struct stat st;
    int fd = fileno (file_text); 
    fstat (fd, &st);
    size_t size_text = (size_t) st.st_size;
	return size_text;
}

static EnumOfErrors RecScanData(size_t* position, const char* text_buffer, Node_t** ResNode, Node_t* CurrentNode, BinaryTree_t* myTree)
{
    //printf("Найден0: %c %d\n", *(text_buffer + *position), *position);
    *position = SkipSpaces (*position, text_buffer); //пропускаем пробелы
    if (!strncmp((text_buffer + *position), "_", 1))
    {
        //дальше поддерево - пустота
        *position = *position + 1;
        *position = SkipSpaces (*position, text_buffer);
        return ERR_OK;
    }
    //нашли открывающуюся скобочку
    if (*(text_buffer + *position) == '(')
    {
        //создание в начале
        char object_buffer[SIZE_OF_BUFFER] = {};
        Node_t* NewNode = CreateNode(myTree);

        (*position)++;
        *position = SkipSpaces (*position, text_buffer); //пропускаем пробелы

        EnumOfErrors result = ERR_OK; 

        //1. Считываем ЛЕВО
        result = RecScanData(position, text_buffer, &NewNode->Left, NewNode, myTree);
        MYASSERT(result == ERR_OK, ERR_BAD_REC_SCAN, return ERR_BAD_REC_SCAN);
        *position = SkipSpaces (*position, text_buffer);

        //2. Пытаемся считать значение
        
        //PrintLogTree (myTree);
        //printf("Найден1: %c %d\n", *(text_buffer + *position), *position);
        size_t shift = 0;

        while (isalnum(*(text_buffer + *position)) || (CharInOperators(*(text_buffer + *position)) != -1) || (*(text_buffer + *position) == '.')) 
        {
            //printf("Найден2: %c %d\n", *(text_buffer + *position), *position);
            MYASSERT(shift < SIZE_OF_BUFFER, ERR_OVERFLOW_BUFF, return ERR_OVERFLOW_BUFF)
            *(object_buffer + shift) = *(text_buffer + *position);
            shift++;
            (*position)++;
        }
        //printf("Найден3: %c %d\n", *(text_buffer + *position), *position);
        result = ProcessObject(object_buffer, NewNode, myTree); //обработка значения прочитанного
        MYASSERT(result == ERR_OK, ERR_PROCESS_OBJECT, return ERR_PROCESS_OBJECT);

        CleanCharBuffer(object_buffer, SIZE_OF_BUFFER);
        *position = SkipSpaces (*position, text_buffer);

        //Пересвязка
        NewNode->Parent = CurrentNode;//подвязка от предыдущего
        if (CurrentNode)//проверка на корень
        {
            *ResNode = NewNode; //подвязка прошлого к новому
        }
        else
        {
            myTree->Root = NewNode;
        }

        //3. Считываем ПРАВО
        result = RecScanData(position, text_buffer, &NewNode->Right, NewNode, myTree);
        MYASSERT(result == ERR_OK, ERR_BAD_REC_SCAN, return ERR_BAD_REC_SCAN);
        *position = SkipSpaces (*position, text_buffer);
        //printf("Найден4: %c %d\n", *(text_buffer + *position), *position);
        //4. Закрытие
        if (*(text_buffer + *position) == ')')
        {
            (*position)++;
            return ERR_OK;
        }
        else
        {
            USER_ERROR(0, ERR_NO_CLOSE_BRACKET_NODE, exit(0))
        }
    }
    else
    {
        USER_ERROR(0, ERR_NO_OPEN_BRACKET_NODE, exit(0))
    }
}

static size_t SkipSpaces(size_t current_position, const char* text_buffer)
{
    size_t end_position = current_position;
    MYASSERT(text_buffer, ERR_BAD_POINTER_DATA, return 0)
    while (isspace(*(text_buffer + end_position)) && (*(text_buffer + end_position) != '\0')) { end_position++; }
    return end_position;
}

static int CharInOperators(const char ch)
{
    for (size_t i = 0; i < SIZE_OF_OPERATORS; i++)
    {
        if (Operators[i].Name[0] == ch) return (int) i;
    }
    return NOT_IN_OPER;
}

static int StrInOperators(const char* str)
{
    for (size_t i = 0; i < SIZE_OF_OPERATORS; i++)
    {
        if (!strcmp(Operators[i].Name, str)) return (int) i;
    }
    return NOT_IN_OPER;
}

static EnumOfErrors ProcessObject (char* object_buffer, Node_t* NewNode, BinaryTree_t* myTree)
{
    //в буффере лежит наш аргумент: sin, aboba, 123.00, /*+-
    double new_value = NAN;
    if (sscanf(object_buffer, SPECIFIER_SCANF, &new_value) == 1)  //если цифра 100% число
    {
        NewNode->Value.Number = new_value;
        NewNode->Type         = NUMBER;
        return ERR_OK;
    }
    int index_operator = -1;
    if ((index_operator = StrInOperators(object_buffer)) != -1)   //если символ оператора то 100% оператор
    {
        NewNode->Value.Index  = index_operator; //вернул индекс в массиве операторов!
        NewNode->Type         = OPERATOR;
        return ERR_OK;
    }
    int index_variable = -1;   //если иное то переменная
    index_variable = InsertVariable (object_buffer, myTree);  //позиции в буфере
    NewNode->Value.Index      = index_variable;
    NewNode->Type             = VARIABLE;

    MYASSERT(index_variable != -1, ERR_PROCESS_OBJECT, return ERR_PROCESS_OBJECT)
    return ERR_OK;
}

static int InsertVariable (char* object_buffer, BinaryTree_t* myTree)
{
    for (size_t i = 0; i < SIZE_OF_VARIABLES; i++)
    {
        if (!strncmp(myTree->Variables[i].Name, object_buffer, SIZE_OF_VAR))
        {
            return (int) i;
        }
        if (!(*(myTree->Variables[i]).Name)) //ищем свободное место в массиве переменных
        {
            strncpy(myTree->Variables[i].Name, object_buffer, SIZE_OF_VAR);
            return (int) i;
        }
    }
    return -1;
}

static void CleanCharBuffer(char* buffer, const size_t buffer_size)
{
    for (size_t i = 0; i < buffer_size; i++)
    {
        *(buffer + i) = 0;
    }
}