#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

#include "colors.h"
#include "tree.h"
#include "log_tex.h"
#include "myassert.h"
#include "calculation.h"


static FILE*  FileLogTex = NULL;
const size_t SIZE_OF_BUFFER = 100;
static FILE* OpenFile (const char* file_open, const char* option);
static void  CloseFile (FILE* file_text);
static void  GeneratePdf (void);
static bool  Compare (double x, double y);
static size_t GetSizeOfNum(double Number);
static double WidthOfNode(Node_t* CurrentNode, BinaryTree_t* myTree);
static double Maxi(double x, double y);
static char InsertChange(Node_t* CurrentNode, BinaryTree_t* myTree);
static bool CheckForChangeLeft(Node_t* CurrentNode);
static bool CheckForChangeRight(Node_t* CurrentNode);

void _PrintLogTexStart (void)
{
    struct stat sb = {};
    if (stat("./" FOLDER_LOG_TEX, &sb) || !S_ISDIR(sb.st_mode)) 
    { 
        system("mkdir " FOLDER_LOG_TEX);
        system("touch " FILE_LOG_TEX);
    }

    FileLogTex = OpenFile (FILE_LOG_TEX, "w");

    fprintf(FileLogTex,
"\\documentclass[a4paper, 12pt]{article}\n"
"\\usepackage[utf8]{inputenc}\n"
"\\usepackage[russian,english]{babel}\n"
"\\usepackage[T2A]{fontenc}\n"
"\\usepackage[left=10mm, top=20mm, right=18mm, bottom=15mm, footskip=10mm]{geometry}\n"
"\\usepackage{indentfirst}\n"
"\\usepackage[yyyymmdd,hhmmss]{datetime}\n"
"\\usepackage{titlesec}\n"
"\\usepackage{graphicx}\n"
"\\graphicspath{{images/}}\n"
"\\DeclareGraphicsExtensions{.pdf,.png,.jpg}\n"
"\\usepackage{wrapfig}\n"
"\\usepackage{blindtext}\n"
"\\usepackage{cmap}\n"
"\\usepackage[x11names]{xcolor}\n"
"\\usepackage{amsmath} %% math packages\n"
"\\usepackage{amsfonts}\n"
"\\usepackage{amsmath}\n"
"\\usepackage{amssymb}\n"
"\\usepackage{amsthm}\n"
"\\usepackage{mathtools}\n"
"\\usepackage{uniquecounter}\n"
"\\usepackage{placeins}\n"
"\\usepackage[italicdiff]{physics}\n"
"\\usepackage{multirow}\n"
"\\usepackage{hyperref} %% links in document\n"
"\\usepackage{lipsum}\n"
"\\hypersetup{\n"
"    colorlinks=true,\n"
"    linkcolor=red,\n"
"    filecolor=magenta,\n"
"    urlcolor=blue,\n"
"    pdftitle={Links},\n"
"    pdfpagemode=FullScreen,\n"
"}\n"
"\\usepackage{import} %%inkspace images\n"
"\\usepackage{xifthen}\n"
"\\usepackage{pdfpages}\n"
"\\usepackage{transparent}\n"
"\\usepackage{caption}\n"
"\\usepackage{fancyhdr}\n"
"\\usepackage{type1cm}\n"
"\\usepackage{draftwatermark}\n"
"\\SetWatermarkAngle{45}\n"
"\\SetWatermarkLightness{0.85}\n"
"\\SetWatermarkFontSize{1cm}\n"
"\\SetWatermarkScale{2.3}\n"
"\\SetWatermarkColor[gray] {0.91} %% gray level\n"
"\\SetWatermarkText{\\textsl{Петрович спасибо за матан!}}\n\n"
"\\begin{document}\n"
"\\pagecolor{white}\n"
"\\definecolor{mycolor}{HTML}{671800}\n"
"\\pagestyle{fancy}\n"
"\\fancyhf{}\n"
"\\rhead{\\textit{Правильный матан}}\n"
"\\lhead{\\textit{Хмельницкий А.А}}\n"
"\\rfoot{}\n"
"\\thispagestyle{empty}\n"
"\\begin{center}\n"
"\\large{«Московский физкультурно-туристический институт»} \\\\  \n"
"\\large{Физтех-школа радитехники и компьютерных технологий } \\\\\n"
"\\vspace*{6cm}\n"
"\\LARGE{\\textbf{\\underline{Учебник по введению в математический анализ}}} \\\\ \n"
"\\vspace*{1cm}\n"
"\\LARGE{\\textbf{\\texttt{Правильная версия}}}\n"
"\\end{center}\n"
"\\vspace*{1cm}\n"
"\\begin{flushright}\n"
"\\large{\n"
"\\textbf{Выполнил:} \\\\ \\textit{Хмельницкий А. А., БО1-306} \\\\\n"
"\\textbf{Консультант:} \\\\ \\textit{Дединский И. Р. (aka ded32)}\n"
"}\n"
"\\end{flushright}\n"
"\\vspace*{10cm}\n"
"\\begin{center}\n"
"2023\n"
"\\end{center}\n"
"\\newpage\n"
"%%============================================================================\n"
"%%ТЕЛО\n"
"\\section{Производная}\n"
"Мы начинаем изучение матана с этой темы. Считая что вы сдали ЕГЭ в котором есть задача на вычисление производной, поэтому предполагается что вы прошли эту тему в школе и способны взять такую, которую мы сейчас возьмем в качестве простенького вводного примера:\\\\\n"
);
}

//Мы начинаем изучение матана с этой темы. Считая что вы сдали ЕГЭ в котором есть задача на вычисление производной, поэтому предполагается что вы прошли эту тему в школе и способны взять такую, которую мы сейчас возьмем в качестве простенького вводного примера:\\\\\n"

void _PrintLogTexFinish (void)
{
    fprintf(FileLogTex, "\\end{document}\n"); 
    CloseFile(FileLogTex);
    GeneratePdf();
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

static void GeneratePdf (void)
{
    system("pdflatex --output-directory=" FOLDER_LOG_TEX " " FILE_LOG_TEX " " FILE_LOG_OUT " >/dev/null");
}

void _WriteTexFormula (BinaryTree_t* myTree)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return)
    fprintf(FileLogTex, "\n\\begin{equation}\n");
    _WriteTexNode(myTree->Root, myTree);
    fprintf(FileLogTex, "\n\\end{equation}\n");
    fprintf(FileLogTex, "\n");
    return;
}

void _WriteTexNode (Node_t* CurrentNode, BinaryTree_t* myTree)
{
    if (!CurrentNode) {return;}

    if (CurrentNode->Type == NUMBER)                                
    {
        _WriteTexNumber(CurrentNode->Value.Number);
    }
    if (CurrentNode->Type == VARIABLE)
    {
        fprintf(FileLogTex, SPECIFIER_VARIABLE_STR, myTree->Variables[CurrentNode->Value.Index].Name); 
    }

    bool left_change = CheckForChangeLeft(CurrentNode); 
    bool right_change = CheckForChangeRight(CurrentNode);
    if (CurrentNode->Type == OPERATOR)
    {
        bool bracket_left = 1;
        bool bracket_right = 1;
        if (CurrentNode->Left->Type == OPERATOR) bracket_left = (Operators[CurrentNode->Left->Value.Index].TypeOperator);
        if ((CurrentNode->Left->Type == VARIABLE) || (CurrentNode->Left->Type == NUMBER)) bracket_left = 0;
        if (CurrentNode->Right)
        {
            if ((CurrentNode->Right->Type == VARIABLE) || (CurrentNode->Right->Type == NUMBER)) bracket_right = 0;
            if (CurrentNode->Right->Type == OPERATOR) bracket_right = (Operators[CurrentNode->Right->Value.Index].TypeOperator);
        }

        switch (CurrentNode->Value.Index)
        {
            case ADD: //+
                IF_BRACKET(bracket_left, "left(")
                if (left_change) 
                {
                    char NameChange = InsertChange(CurrentNode->Left, myTree);
                    fprintf(FileLogTex, " %c ", NameChange);
                }
                else WriteTexNode(CurrentNode->Left, myTree);
                IF_BRACKET(bracket_left, "right)")
                fprintf(FileLogTex, "+");
                IF_BRACKET(bracket_right, "left(")
                if (right_change)
                {
                    char NameChange = InsertChange(CurrentNode->Right, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(CurrentNode->Right, myTree);
                IF_BRACKET(bracket_right, "right)")
            break;
            case SUB: //-
                IF_BRACKET(bracket_left, "left(")
                if (left_change) 
                {
                    char NameChange = InsertChange(CurrentNode->Left, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(CurrentNode->Left, myTree);
                IF_BRACKET(bracket_left, "right)")
                fprintf(FileLogTex, "-");
                IF_BRACKET(bracket_right, "left(")
                if (right_change)
                {
                    char NameChange = InsertChange(CurrentNode->Right, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(CurrentNode->Right, myTree);
                IF_BRACKET(bracket_right, "right)")
            break;
            case MUL: //*
                IF_BRACKET(bracket_left, "left(")
                if (left_change) 
                {
                    char NameChange = InsertChange(CurrentNode->Left, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(CurrentNode->Left, myTree);
                IF_BRACKET(bracket_left, "right)")
                fprintf(FileLogTex, "\\cdot ");
                IF_BRACKET(bracket_right, "left(")
                if (right_change)
                {
                    char NameChange = InsertChange(CurrentNode->Right, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(CurrentNode->Right, myTree);
                IF_BRACKET(bracket_right, "right)")
            break;
            case DIV: ///
                fprintf(FileLogTex, "\\frac{");
                if (left_change) 
                {
                    char NameChange = InsertChange(CurrentNode->Left, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(CurrentNode->Left, myTree);
                fprintf(FileLogTex, "}{");
                if (right_change)
                {
                    char NameChange = InsertChange(CurrentNode->Right, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(CurrentNode->Right, myTree);
                fprintf(FileLogTex, "}");
            break;
            case POW: //^
                if (CurrentNode->Left->Type == NUMBER)
                {
                    if (Compare(CurrentNode->Left->Value.Number, 0.5))
                    {
                        fprintf(FileLogTex, "\\sqrt{");
                        if (left_change) 
                        {
                            char NameChange = InsertChange(CurrentNode->Left, myTree);
                            fprintf(FileLogTex, "%c", NameChange);
                        }
                        else WriteTexNode(CurrentNode->Left, myTree);
                        fprintf(FileLogTex, "}");     
                        return;   
                    }
                }
                IF_BRACKET(bracket_left, "left(")
                if (left_change) 
                {
                    char NameChange = InsertChange(CurrentNode->Left, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(CurrentNode->Left, myTree);
                IF_BRACKET(bracket_left, "right)")
                fprintf(FileLogTex, "^");
                fprintf(FileLogTex, "{");
                if (right_change)
                {
                    char NameChange = InsertChange(CurrentNode->Right, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(CurrentNode->Right, myTree);
                fprintf(FileLogTex, "}");
            break;
            case SIN: //sin
                fprintf(FileLogTex, "sin\\left(");
                WriteTexNode(CurrentNode->Left, myTree);
                fprintf(FileLogTex, "\\right)");
            break;
            case COS: //cos
                fprintf(FileLogTex, "cos\\left(");
                WriteTexNode(CurrentNode->Left, myTree);
                fprintf(FileLogTex, "\\right)");
            break;
            case TG: //tg
                fprintf(FileLogTex, "tg\\left(");
                WriteTexNode(CurrentNode->Left, myTree);
                fprintf(FileLogTex, "\\right)");
            break;
            case CTG: //ctg
                fprintf(FileLogTex, "ctg\\left(");
                WriteTexNode(CurrentNode->Left, myTree);
                fprintf(FileLogTex, "\\right)");
            break;
            case LN: //ln
                fprintf(FileLogTex, "ln\\left(");
                WriteTexNode(CurrentNode->Left, myTree);
                fprintf(FileLogTex, "\\right)");
            break;
            default: break;
        }
    }
    return;
}

static bool Compare (double x, double y)
{
    if (((isnan (x) == 1) && (isnan (y) == 1)) || (fabs (x - y) < EPSILONE))
        return 1;
    else
        return 0;
}

void _WriteTexText(const char* str)
{
    fprintf(FileLogTex, "%s", str);
} 

static double Maxi(double x, double y)
{
    if ((x > y) || Compare(x,y)) return x;
    else return y;
}

void _WriteTexNumber(double Number)
{
    if (Number < 0) 
    {
        if(fabs(Number - round(Number)) < EPSILONE)
        {
            fprintf(FileLogTex, "(%d)", int(Number));    
        }
        else fprintf(FileLogTex, "(" SPECIFIER_NUMBER ")", Number); 
    }
    else 
    {
        if(fabs(Number - round(Number)) < EPSILONE)
        {
            fprintf(FileLogTex, " %d ", int(Number));    
        }
        else fprintf(FileLogTex, SPECIFIER_NUMBER, Number); 
    }    
}

void _WriteCringeStart(void)
{
    size_t RandomIndex = (size_t) rand() % (SIZE_OF_CRINGE_START); //rand = [0, SIZE_OF_CRINGE)
    fprintf(FileLogTex, "\n%s \\\\ \n", CringeStart[RandomIndex]);
}

void _WriteCringeEnd(void)
{
    size_t RandomIndex = (size_t) rand() % (SIZE_OF_CRINGE_END); //rand = [0, SIZE_OF_CRINGE)
    fprintf(FileLogTex, "\n%s \\\\ \n", CringeEnd[RandomIndex]);
}

void _WriteTexCalculating(double result, BinaryTree_t* myTree)
{
    WriteTexText("После предварительных преобразований, слишком простых для разъяснения получаем: \\\\");
    WriteTexFormula(myTree);
    WriteTexText("\nВ начале рассчитаем значение этой функции при заданных аргументах: \\\\\n");
    size_t i = 0;
    WriteTexText("\n\\begin{center}\n");
    while (myTree->Variables[i].Name[0])
    {
        WriteTexText(myTree->Variables[i].Name);
        WriteTexText(" = ");
        WriteTexNumber(myTree->Variables[i].Number);
        WriteTexText(",");
        i++;
    }
    WriteTexText("\n\\end{center}");
    WriteTexText("\nОчевидно, что оно будет равно: ");
    WriteTexNumber(result);
    WriteTexText(" \\\\\n");
}

void _WriteTexDifferentiate(BinaryTree_t* myTree)
{
    WriteTexText("\nИтак если вы еще не уснули к этому моменту, то поздравляю, мы дошли до ответа: \\\\");
    WriteTexFormula(myTree);
    WriteTexText("\n");
}

void _GetTexSizeTree(BinaryTree_t* myTree)
{
    WidthOfNode(myTree->Root, myTree);
}

static double WidthOfNode(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    if (CurrentNode->Type == VARIABLE) 
    {
        CurrentNode->TexSize = (double) strlen(myTree->Variables[CurrentNode->Value.Index].Name);
        return CurrentNode->TexSize;
    }
    if (CurrentNode->Type == NUMBER) 
    {
        CurrentNode->TexSize = (double) GetSizeOfNum(CurrentNode->Value.Number);
        return CurrentNode->TexSize;
    }

    if (CurrentNode->Type == OPERATOR)
    {
        switch (CurrentNode->Value.Index)
        {
            case ADD: //+
            case SUB:
                CurrentNode->TexSize = WidthOfNode(CurrentNode->Left, myTree) + WidthOfNode(CurrentNode->Right, myTree) + 3;
                return CurrentNode->TexSize;
            break;
            case MUL:
                CurrentNode->TexSize = WidthOfNode(CurrentNode->Left, myTree) + WidthOfNode(CurrentNode->Right, myTree) + 5;
                return CurrentNode->TexSize;
            break;
            case DIV:
                CurrentNode->TexSize = Maxi(WidthOfNode(CurrentNode->Left, myTree), WidthOfNode(CurrentNode->Right, myTree));
                return CurrentNode->TexSize;
            break;
            case POW:
                CurrentNode->TexSize = WidthOfNode(CurrentNode->Left, myTree) + 3 + 0.5 * WidthOfNode(CurrentNode->Right, myTree);
                return CurrentNode->TexSize;
            break;
            case SIN:
            case COS:
            case CTG:
                CurrentNode->TexSize = WidthOfNode(CurrentNode->Left, myTree) + 5;
                return CurrentNode->TexSize;
            break;
            case TG:
            case LN:
                CurrentNode->TexSize = WidthOfNode(CurrentNode->Left, myTree) + 4;
                return CurrentNode->TexSize;
            break;
            default: break;
        }
    }
    return NAN;
}

static size_t GetSizeOfNum(double Number)
{
    char num_buffer[SIZE_OF_BUFFER] = {};
    if (Number < 0) 
    {
        if(fabs(Number - round(Number)) < EPSILONE)
        {
            snprintf(num_buffer, SIZE_OF_BUFFER,"(%d)", int(Number));    
        }
        else snprintf(num_buffer, SIZE_OF_BUFFER,"(" SPECIFIER_NUMBER ")", Number); 
    }
    else 
    {
        if(fabs(Number - round(Number)) < EPSILONE)
        {
            snprintf(num_buffer, SIZE_OF_BUFFER,"%d", int(Number));    
        }
        else snprintf(num_buffer, SIZE_OF_BUFFER, SPECIFIER_NUMBER, Number); 
    }    
    return strlen(num_buffer);
}

static bool CheckForChangeLeft(Node_t* CurrentNode)
{
    if ((CurrentNode->TexSize > SIZE_LIMIT) && (CurrentNode->Left->TexSize < SIZE_LIMIT))
    {
        return 1;
    }
    return 0;
}

static bool CheckForChangeRight(Node_t* CurrentNode)
{
    if (!CurrentNode->Right) return 0;
    if ((CurrentNode->TexSize > SIZE_LIMIT) && (CurrentNode->Right->TexSize < SIZE_LIMIT))
    {
        return 1;
    }
    return 0;
}

static char InsertChange(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    for (size_t i = 0; i < SIZE_OF_CHANGES; i++)
    {
        if (myTree->Changes[i].Name == 0)
        {
            myTree->Changes[i].Name       = 'A' + (char) i;
            myTree->Changes[i].ChangeNode = CurrentNode;
            return myTree->Changes[i].Name;
        }
    }
    return '\0';
    printf(RED "\nOVERFLOW ARRAY OF CHANGES\n" RESET);
}

void _WriteChanges(BinaryTree_t* myTree)
{
    if (myTree->Changes[0].Name == 0) return;

    fprintf(FileLogTex, "\nВ данной задаче для удобства мы ввели следующие замены: \\\\\n");

    for (size_t i = 0; i < SIZE_OF_CHANGES; i++)
    {
        if (myTree->Changes[i].Name == 0) return;
        fprintf(FileLogTex, "\n\\[ %c = ", myTree->Changes[i].Name);
        _WriteTexNode(myTree->Changes[i].ChangeNode, myTree);
        fprintf(FileLogTex, "\\] \n");
    }
}