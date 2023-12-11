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
#include "DSL.h"

static const char* const CringeStart[] = 
{
    "Очевидно, что ",
	"Это элементарнийшее выражение ",  
	"Каждый советский эмбрион знает, что",
	"Вы проходили в школе, что ",
	"Надеюсь вы уже уснули, поэтому бла-бла-бла ",
	"Аналогично выходит, что ",
	"Получаем, что",
	"Для любого $\\varepsilon$ > 0",
	"Отметим, что ",
	"Легко представить, что",
	"Каждому кто закончил церковно-приходскую школу известно, что",
	"Вольфрам(спасибо что не молибден) говорит, что",
	"Я устал придумывать пасты поэтому ",
	"После округления ",
	"Аппроксимируя получаем, что ",
	"Косвенно получаем, что ",
	"А Петрович знает, что ",
	"Надо записать пока не забыл, надо забыть пока не записал, что",
	"Используя $9\\frac{3}{4}$ том Ландау-Лифшица получаем, что ",
	"Переходя в полярную-биномиальную-гиперболическую СК",
	"Полторашка мурлычет, поэтому"
};
static const char* const CringeEnd[] =
{
	"становится ",
	"simple переходит в ",
	"к примеру пусть становится ",
	"- табличная функция, которая равна ",
	"если записать короче, получаем",
	"лёгким движением руки превращается в элегантное ",
	"трудновато держать в уме, поэтому равно",
	"по 256 аксиоме Дединского будет",
	"может быть станет(хотя хз)",
	"непротиворечит следующему переходу к ",
	"далее следует мысленный переход к ",
	"по 1024 методу Султанова преобразуется в ",
	"тривиально решается так",
	"после округления вниз преобразуется к",
	"после округления вверх обращается в",
	"примем на веру, что это становится ",
	"в комплесном решении будет",
	"объяснение следующего перехода остается вам в качестве д/з ",
	"при решении тривиальным способом станет ",
	"по 64 теореме Савватеева равно ",
	"остсюда прямо следует ",
	"Python бы преобразовал это в ",
	"легко сводится с помощью Excel к ",
	"в других обозначениях",
	"после замены базиса получается",
	"трах тибидох ",
};

const int SIZE_OF_CRINGE_START = sizeof(CringeStart)/sizeof(CringeStart[0]); //размер массива оператора
const int SIZE_OF_CRINGE_END   = sizeof(CringeEnd)/sizeof(CringeEnd[0]); //размер массива оператора

//статические глобалки, гораздо удобнее в рамках логов, иначе везде +1 парам
static FILE*  FileLogTex     = NULL;
static FILE*  FileLogPython  = NULL;
static FILE*  FileLogDots    = NULL;
static const  size_t  SIZE_OF_BUFFER = 100;

//статик функции
static FILE*  OpenFile           (const char* file_open, const char* option);
static void   CloseFile          (FILE* file_text);
static void   GeneratePdf        (void);
static bool   Compare            (double x, double y);
static size_t GetSizeOfNum       (double Number);
static double WidthOfNode        (Node_t* CurrentNode, BinaryTree_t* myTree);
static double Maxi               (double x, double y);
static char   InsertChange       (Node_t* CurrentNode, BinaryTree_t* myTree);
static bool   CheckForChangeLeft (Node_t* CurrentNode);
static bool   CheckForChangeRight(Node_t* CurrentNode);
static double RecEvaluate(Node_t* CurrentNode, BinaryTree* myTree, double x);
void GenerateGraphic(void);
void GeneratePyDots(BinaryTree_t* myTree);


void _PrintLogTexStart (void)
{
    struct stat sb = {};
    if (stat("./" FOLDER_LOG_TEX, &sb) || !S_ISDIR(sb.st_mode)) 
    { 
        system("mkdir " FOLDER_LOG_TEX);
        system("touch " FILE_LOG_TEX);
        system("touch " FOLDER_LOG_TEX "/" FILE_LOG_PYTHON);
        system("touch " FOLDER_LOG_TEX "/" FILE_LOG_DOTS);
    }

    FileLogTex     = OpenFile (FILE_LOG_TEX, "w");

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
    "\\captionsetup[figure]{name=Рисунок}\n"
    "\\captionsetup[table]{name=Таблица}\n"
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
    "\\textbf{Выполнил:} \\\\ \\textit{\\href{https://www.youtube.com/watch?v=kk3_5AHEZxE}{Хмельницкий А. А., БО1-306(aka khmelnitskiianton)}} \\\\\n"
    "\\textbf{Консультант:} \\\\ \\textit{\\href{https://www.youtube.com/watch?v=cdX8r3ZSzN4}{Дединский И. Р. (aka ded32)}}\n"
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
    system("pdflatex --output-directory=" FOLDER_LOG_TEX " " FILE_LOG_TEX " " FILE_LOG_OUT ">/dev/null");//>/dev/null
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
    if (!C) {return;}

    if (C->Type == NUMBER)                                
    {
        _WriteTexNumber(C->Value.Number);
    }
    if (C->Type == VARIABLE)
    {
        fprintf(FileLogTex, SPECIFIER_VARIABLE_STR, myTree->Variables[C->Value.Index].Name); 
    }

    bool left_change = CheckForChangeLeft(C); 
    bool right_change = CheckForChangeRight(C);
    if (C->Type == OPERATOR)
    {
        bool bracket_left  = 0;
        bool bracket_right = 0;
        if (L->Type == OPERATOR) 
        {
            if ((C->Value.Index == MUL)&&((L->Value.Index == ADD)||(L->Value.Index == SUB))) 
            {
                bracket_left = 1;
            }
            if (C->Value.Index == POW)
            {
                bracket_left = 1;
            }
        }
        if (R && (R->Type == OPERATOR))
        {
            if ((C->Value.Index == MUL)&&((R->Value.Index == ADD)||(R->Value.Index == SUB))) bracket_right = 1;
        }

        switch (C->Value.Index)
        {
            case ADD: //+
                IF_BRACKET(bracket_left, "left(")
                if (left_change) 
                {
                    char NameChange = InsertChange(L, myTree);
                    fprintf(FileLogTex, " %c ", NameChange);
                }
                else WriteTexNode(L, myTree);
                IF_BRACKET(bracket_left, "right)")
                fprintf(FileLogTex, "+");
                IF_BRACKET(bracket_right, "left(")
                if (right_change)
                {
                    char NameChange = InsertChange(R, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(R, myTree);
                IF_BRACKET(bracket_right, "right)")
            break;
            case SUB: //-
                IF_BRACKET(bracket_left, "left(")
                if (left_change) 
                {
                    char NameChange = InsertChange(L, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(L, myTree);
                IF_BRACKET(bracket_left, "right)")
                fprintf(FileLogTex, "-");
                IF_BRACKET(bracket_right, "left(")
                if (right_change)
                {
                    char NameChange = InsertChange(R, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(R, myTree);
                IF_BRACKET(bracket_right, "right)")
            break;
            case MUL: //*
                IF_BRACKET(bracket_left, "left(")
                if (left_change) 
                {
                    char NameChange = InsertChange(L, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(L, myTree);
                IF_BRACKET(bracket_left, "right)")
                fprintf(FileLogTex, "\\cdot ");
                IF_BRACKET(bracket_right, "left(")
                if (right_change)
                {
                    char NameChange = InsertChange(R, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(R, myTree);
                IF_BRACKET(bracket_right, "right)")
            break;
            case DIV: ///
                fprintf(FileLogTex, "\\frac{");
                if (left_change) 
                {
                    char NameChange = InsertChange(L, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(L, myTree);
                fprintf(FileLogTex, "}{");
                if (right_change)
                {
                    char NameChange = InsertChange(R, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(R, myTree);
                fprintf(FileLogTex, "}");
            break;
            case POW: //^
                if (L->Type == NUMBER)
                {
                    if (Compare(L->Value.Number, 0.5))
                    {
                        fprintf(FileLogTex, "\\sqrt{");
                        if (left_change) 
                        {
                            char NameChange = InsertChange(L, myTree);
                            fprintf(FileLogTex, "%c", NameChange);
                        }
                        else WriteTexNode(L, myTree);
                        fprintf(FileLogTex, "}");     
                        return;   
                    }
                }
                IF_BRACKET(bracket_left, "left(")
                if (left_change) 
                {
                    char NameChange = InsertChange(L, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(L, myTree);
                IF_BRACKET(bracket_left, "right)")
                fprintf(FileLogTex, "^");
                fprintf(FileLogTex, "{");
                if (right_change)
                {
                    char NameChange = InsertChange(R, myTree);
                    fprintf(FileLogTex, "%c", NameChange);
                }
                else WriteTexNode(R, myTree);
                fprintf(FileLogTex, "}");
            break;
            case SIN: //sin
                fprintf(FileLogTex, "sin\\left(");
                WriteTexNode(L, myTree);
                fprintf(FileLogTex, "\\right)");
            break;
            case COS: //cos
                fprintf(FileLogTex, "cos\\left(");
                WriteTexNode(L, myTree);
                fprintf(FileLogTex, "\\right)");
            break;
            case TG: //tg
                fprintf(FileLogTex, "tg\\left(");
                WriteTexNode(L, myTree);
                fprintf(FileLogTex, "\\right)");
            break;
            case CTG: //ctg
                fprintf(FileLogTex, "ctg\\left(");
                WriteTexNode(L, myTree);
                fprintf(FileLogTex, "\\right)");
            break;
            case LN: //ln
                fprintf(FileLogTex, "ln\\left(");
                WriteTexNode(L, myTree);
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
    WriteTexText(" \\\\\n\n");

    GeneratePyDots(myTree);
    GenerateGraphic();
    WriteTexText("\n\n");
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
    if (C->Type == VARIABLE) 
    {
        C->TexSize = (double) strlen(myTree->Variables[C->Value.Index].Name);
        return C->TexSize;
    }
    if (C->Type == NUMBER) 
    {
        C->TexSize = (double) GetSizeOfNum(C->Value.Number);
        return C->TexSize;
    }

    if (C->Type == OPERATOR)
    {
        switch (C->Value.Index)
        {
            case ADD: //+
            case SUB:
                C->TexSize = WidthOfNode(L, myTree) + WidthOfNode(R, myTree) + 3;
                return C->TexSize;
            break;
            case MUL:
                C->TexSize = WidthOfNode(L, myTree) + WidthOfNode(R, myTree) + 5;
                return C->TexSize;
            break;
            case DIV:
                C->TexSize = Maxi(WidthOfNode(L, myTree), WidthOfNode(R, myTree));
                return C->TexSize;
            break;
            case POW:
                C->TexSize = WidthOfNode(L, myTree) + 3 + 0.5 * WidthOfNode(R, myTree);
                return C->TexSize;
            break;
            case SIN:
            case COS:
            case CTG:
                C->TexSize = WidthOfNode(L, myTree) + 5;
                return C->TexSize;
            break;
            case TG:
            case LN:
                C->TexSize = WidthOfNode(L, myTree) + 4;
                return C->TexSize;
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
    if ((C->TexSize > SIZE_LIMIT) && (L->TexSize < SIZE_LIMIT))
    {
        return 1;
    }
    return 0;
}

static bool CheckForChangeRight(Node_t* CurrentNode)
{
    if (!R) return 0;
    if ((C->TexSize > SIZE_LIMIT) && (R->TexSize < SIZE_LIMIT))
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
            myTree->Changes[i].ChangeNode = C;
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

//========================================================================================================
//Python graphics

static double RecEvaluate(Node_t* CurrentNode, BinaryTree* myTree, double x)
{
    if (!C) 
    {
        return NAN;
    }
    if (C->Type == NUMBER)
    {
        return CurrentNode->Value.Number;
    }
    if (C->Type == VARIABLE)
    {
        return x;
    }
    double LeftNumber  = RecEvaluate(C->Left, myTree, x);
    double RightNumber = RecEvaluate(C->Right, myTree, x);
    if (C->Type == OPERATOR)
    {
        return Operators[C->Value.Index].Operation(LeftNumber, RightNumber);
    }
    MYASSERT(0, ERR_UNKNOWN_TYPE, return NAN);
}  

void GeneratePyDots(BinaryTree_t* myTree)
{
    FileLogDots = OpenFile (FOLDER_LOG_TEX "/" FILE_LOG_DOTS, "w");
    double current_x = NAN;
    double current_y = NAN;
    for (int i = LEFT_EDGE; i < RIGHT_EDGE; i++)
    {           
        current_x = ((double) i) / 5;          //[LEFT_EDGE/10; RIGHT_EDGE/10]
        current_y = RecEvaluate(myTree->Root, myTree, current_x);
        if (Compare(current_y, 0)) break;
        if (isnan(current_y)) continue;
        fprintf(FileLogDots, "%.2lf\t%.4lf\n", current_x, RecEvaluate(myTree->Root, myTree, current_x)); //x y in .txt
    }
    CloseFile(FileLogDots);
}

void GenerateGraphic(void)
{
    FileLogPython = OpenFile (FOLDER_LOG_TEX "/" FILE_LOG_PYTHON, "w");
    fprintf(FileLogPython, 
        "import matplotlib.pyplot as plt\n"
        "plt.figure(figsize=(11,11))\n"
        "plt.title('График функции f(x)')\n"
        "plt.ylabel('f(x)')\n"
        "plt.xlabel('x')\n\n"
        "x = []\n"
        "y = []\n"
        "file = open('"FOLDER_LOG_TEX "/" FILE_LOG_DOTS "', 'r')\n"
        "while (line := file.readline()):\n"
        "\ts = line.split()\n"
        "\tif (len(s) != 0):\n"
        "\t\tx.append(float(s[0]))\n"
        "\t\ty.append(float(s[1]))\n\n"
        "plt.plot(x, y, color = 'g')\n"
        "plt.grid(visible = True, which='major', axis='both', alpha=1)\n"
        "plt.grid(visible = True, which='minor', axis='both', alpha=1)\n"
        "plt.savefig('" FOLDER_LOG_TEX "/" FILE_LOG_GRAPHIC "')\n"
        "file.close()\n"
    );
    CloseFile(FileLogPython);

    system("python3 " FOLDER_LOG_TEX "/" FILE_LOG_PYTHON);
    WriteTexText("Для большей наглядности(очевидности) построим график этой элементарщины:\\\\\n");
    WriteTexText("\\begin{figure}[!h]\n"
                 "\\centering\n"
                 "\\includegraphics[scale=0.6]{" FILE_LOG_GRAPHIC "}\n"
                 "\\caption{\\href{https://youtu.be/dQw4w9WgXcQ?si=uVgA9WMINchWAf3T}{График функции $f(x)$}}\n"
                 "\\label{fig:graphic}\n"
                 "\\end{figure}\n");
}

//========================================================================================================
//========================================================================================================
//Functions of printing latex formuls while differentiate

void tex_dif_num(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    WriteCringeStart();							
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNumber(CurrentNode->Value.Number);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("0");
    WriteTexText("\n\\end{equation}");
}

void tex_dif_var(Node_t* CurrentNode, BinaryTree_t* myTree)
{
	WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexText(myTree->Variables[CurrentNode->Value.Index].Name);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("1");
    WriteTexText("\n\\end{equation}");
}

void tex_dif_add(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("+");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("+\\left(");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
}

void tex_dif_sub(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("-");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("-\\left(");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
}

void tex_dif_mul(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    if ((L->Type == OPERATOR)&&((L->Value.Index == ADD)||(L->Value.Index == SUB)))
    {
        WriteTexText("\\left(");
        WriteTexNode(L, myTree);
        WriteTexText("\\right)");
    }
    else WriteTexNode(L, myTree);
    WriteTexText("\\cdot ");
    if ((R->Type == OPERATOR)&&((R->Value.Index == ADD)||(R->Value.Index == SUB)))
    {
        WriteTexText("\\left(");
        WriteTexNode(R, myTree);
        WriteTexText("\\right)");
    }
    else WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}\\cdot ");
    if ((R->Type == OPERATOR)&&((R->Value.Index == ADD)||(R->Value.Index == SUB)))
    {
        WriteTexText("\\left(");
        WriteTexNode(R, myTree);
        WriteTexText("\\right)");
    }
    else WriteTexNode(R, myTree);
    WriteTexText("+\\left(");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}\\cdot ");
    if ((L->Type == OPERATOR)&&((L->Value.Index == ADD)||(L->Value.Index == SUB)))
    {
        WriteTexText("\\left(");
        WriteTexNode(L, myTree);
        WriteTexText("\\right)");
    }
    else WriteTexNode(L, myTree);
    WriteTexText("\n\\end{equation}");
}

void tex_dif_div(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    WriteCringeStart();		
    WriteTexText("\n\\begin{equation}\n");		
    WriteTexText("\\left(\\frac{");		
    WriteTexNode(L, myTree);		
    WriteTexText("}{");		
    WriteTexNode(R, myTree);		
    WriteTexText("}\\right)^{\\prime}");		
    WriteTexText("\n\\end{equation}");		
    WriteCringeEnd();		
    WriteTexText("\n\\begin{equation}\n");		
    WriteTexText("\\frac{\\left(");		
    WriteTexNode(L, myTree);		
    WriteTexText("\\right)^{\\prime}\\cdot ");
    if ((R->Type == OPERATOR)&&((R->Value.Index == ADD)||(R->Value.Index == SUB)))
    {
        WriteTexText("\\left(");
        WriteTexNode(R, myTree);
        WriteTexText("\\right)");
    }
    else WriteTexNode(R, myTree);		
    WriteTexText("-\\left(");		
    WriteTexNode(R, myTree);		
    WriteTexText("\\right)^{\\prime}\\cdot ");		
    if ((L->Type == OPERATOR)&&((L->Value.Index == ADD)||(L->Value.Index == SUB)))
    {
        WriteTexText("\\left(");
        WriteTexNode(L, myTree);
        WriteTexText("\\right)");
    }
    else WriteTexNode(L, myTree);
    WriteTexText("}{\\left(");			
	WriteTexNode(R, myTree);
	WriteTexText("\\right)^{2}}");		
    WriteTexText("\n\\end{equation}");
}

void tex_dif_pow_var_num(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNode(L, myTree);
    WriteTexText(" ^{");
    WriteTexNumber(R->Value.Number);
    WriteTexText("}\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexNumber(R->Value.Number);
    WriteTexText("\\cdot ");
    WriteTexNode(L, myTree);
    WriteTexText("^{");
    WriteTexNumber(R->Value.Number-1);
    WriteTexText("}\\cdot\\left(");
    WriteTexNode(L, myTree);	
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
}

void tex_dif_pow_num_var(Node_t* CurrentNode, BinaryTree_t* myTree)
{
	WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(");
    WriteTexNumber(L->Value.Number);
    WriteTexText(" ^{");
    WriteTexNode(R, myTree);
    WriteTexText("}\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexNumber(L->Value.Number);
    WriteTexText("^{");
    WriteTexNode(R, myTree);
    WriteTexText("}\\cdot\\left(");
    WriteTexNode(R, myTree);
    WriteTexText("\\right)^{\\prime}\\cdot ln\\left(");
    WriteTexNumber(L->Value.Number);
    WriteTexText("\\right)");
    WriteTexText("\n\\end{equation}");
}

void tex_dif_pow_xx(Node_t* CurrentNode, BinaryTree_t* myTree)
{
	WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{");
    WriteTexNode(R, myTree);
    WriteTexText("}\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(e^{");
    WriteTexNode(R, myTree);
    WriteTexText("\\cdot ln\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)}\\right)^{\\prime} = ");
    WriteTexText("e^{");
    WriteTexNode(R, myTree);
    WriteTexText("\\cdot ln\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)} \\cdot \\left(");
    WriteTexNode(R, myTree);
    WriteTexText("\\cdot ln\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");	
}

void tex_dif_sin(Node_t* CurrentNode, BinaryTree_t* myTree)
{
	WriteCringeStart();			
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(cos\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("sin\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\cdot\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");	
}

void tex_dif_cos(Node_t* CurrentNode, BinaryTree_t* myTree)
{
	WriteCringeStart();				
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(cos\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("-sin\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\cdot\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
}

void tex_dif_tg(Node_t* CurrentNode, BinaryTree_t* myTree)
{
	WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(tg\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\frac{1}{cos\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{2}}\\cdot\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
}

void tex_dif_ctg(Node_t* CurrentNode, BinaryTree_t* myTree)
{
    WriteCringeStart();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("\\left(ctg\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
    WriteCringeEnd();
    WriteTexText("\n\\begin{equation}\n");
    WriteTexText("-\\frac{1}{sin\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\left)^{2}}\\cdot\\left(");
    WriteTexNode(L, myTree);
    WriteTexText("\\right)^{\\prime}");
    WriteTexText("\n\\end{equation}");
}

void tex_dif_ln(Node_t* CurrentNode, BinaryTree_t* myTree)
{
	WriteCringeStart();
	WriteTexText("\n\\begin{equation}\n");
	WriteTexText("\\left(ln\\left(");
	WriteTexNode(L, myTree);
	WriteTexText("\\right)\\right)^{\\prime}");
	WriteTexText("\n\\end{equation}");
	WriteCringeEnd();
	WriteTexText("\n\\begin{equation}\n");
	WriteTexText("\\frac{1}{");
	WriteTexNode(L, myTree);
	WriteTexText("}\\cdot\\left(");
	WriteTexNode(L, myTree);
	WriteTexText("\\right)^{\\prime}");
	WriteTexText("\n\\end{equation}");
}