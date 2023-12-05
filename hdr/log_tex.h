#ifndef LOG_TEX_H
#define LOG_TEX_H

#define DEBUG_LOG_TEX 1

#ifdef DEBUG_LOG_TEX
    #define PrintLogTexStart() \
            _PrintLogTexStart()
    #define PrintLogTexFinish() \
            _PrintLogTexFinish()
    #define WriteTexFormula(myTree) \
            _WriteTexFormula(myTree) 
    #define WriteTexText(str) \
            _WriteTexText(str)
    #define WriteTexNode(StartNode, myTree) \
            _WriteTexNode(StartNode, myTree)
    #define WriteTexNumber(number) \
            _WriteTexNumber(number)
	#define WriteCringeStart() \
			_WriteCringeStart() 
	#define WriteCringeEnd() \
			_WriteCringeEnd()
	#define WriteTexCalculating(result, myTree) \
			_WriteTexCalculating(result, myTree)
	#define WriteTexDifferentiate(myTree) \
			_WriteTexDifferentiate(myTree)
	#define GetTexSizeTree(myTree)\
			_GetTexSizeTree(myTree)
	#define WriteChanges(myTree)\
			_WriteChanges(myTree)
	#define ON_TEX(...) __VA_ARGS__
#else
    #define PrintLogTexTree(myTree)
    #define PrintLogTexStart()
    #define PrintLogTexFinish()
    #define WriteTexFormula(myTree)
    #define WriteTexNode(StartNode, myTree)
    #define WriteTexNumber(number)
	#define WriteCringeStart()
	#define WriteCringeEnd()
	#define WriteTexCalculating(result, myTree)
	#define WriteTexDifferentiate(myTree)
	#define GetTexSizeTree(myTree)
	#define WriteChanges(myTree)
	#define ON_TEX(...)
#endif

#define IF_BRACKET(bool_var, side) \
        do                         \
        {                           \
            if (bool_var) fprintf(FileLogTex, "\\" side);  \
        } while (0);   

void  _PrintLogTexStart  (void);
void  _PrintLogTexFinish (void);
void  _WriteTexFormula   (BinaryTree_t* myTree);
void  _WriteTexText      (const char* str);
void  _WriteTexNode      (Node_t* CurrentNode, BinaryTree_t* myTree);
void  _WriteTexNumber    (double Number);
void  _WriteCringeStart  (void);
void  _WriteCringeEnd    (void);
void  _WriteTexCalculating (double result, BinaryTree_t* myTree);
void  _WriteTexDifferentiate (BinaryTree_t* myTree);
void  _GetTexSizeTree(BinaryTree_t* myTree);
void  _WriteChanges(BinaryTree_t* myTree);

#define FOLDER_LOG_TEX "tex"
#define FILE_LOG_TEX "tex/matan_book.tex"
#define FILE_LOG_OUT "tex/matan_book.pdf"

const double SIZE_LIMIT = 70;
 
constexpr const char* CringeStart[] = 
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
};
constexpr const char* CringeEnd[] =
{
	"таким образом становится ",
	"simple переходит в ",
	"что-то странное, пусть становится ",
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

#define TEX_DIF_NUM(CurrentNode, myTree) \
		WriteCringeStart();															\
        WriteTexText("\n\\begin{equation}\n");								\
        WriteTexText("\\left(");								\
        WriteTexNumber(CurrentNode->Value.Number);								\
        WriteTexText("\\right)^{\\prime}");								\
        WriteTexText("\n\\end{equation}");								\
        WriteCringeEnd();								\
        WriteTexText("\n\\begin{equation}\n");								\
        WriteTexText("0");								\
        WriteTexText("\n\\end{equation}");		

#define TEX_DIF_VAR(CurrentNode, myTree) \
		WriteCringeStart();									\
        WriteTexText("\n\\begin{equation}\n");									\
        WriteTexText("\\left(");									\
        WriteTexText(myTree->Variables[CurrentNode->Value.Index].Name);									\
        WriteTexText("\\right)^{\\prime}");									\
        WriteTexText("\n\\end{equation}");									\
        WriteCringeEnd();									\
        WriteTexText("\n\\begin{equation}\n");									\
        WriteTexText("1");									\
        WriteTexText("\n\\end{equation}");

#define TEX_DIF_ADD(CurrentNode, myTree) \
		WriteCringeStart();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\left(");							\
    	WriteTexNode(CurrentNode->Left, myTree);							\
    	WriteTexText("+");							\
    	WriteTexNode(CurrentNode->Right, myTree);							\
    	WriteTexText("\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");							\
    	WriteCringeEnd();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\left(");							\
    	WriteTexNode(CurrentNode->Left, myTree);							\
    	WriteTexText("\\right)^{\\prime}");							\
    	WriteTexText("+\\left(");							\
    	WriteTexNode(CurrentNode->Right, myTree);							\
    	WriteTexText("\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");		

#define TEX_DIF_SUB(CurrentNode, myTree) \
	    WriteCringeStart();								\
    	WriteTexText("\n\\begin{equation}\n");								\
    	WriteTexText("\\left(");								\
    	WriteTexNode(L, myTree);								\
    	WriteTexText("-");								\
    	WriteTexNode(R, myTree);								\
    	WriteTexText("\\right)^{\\prime}");								\
    	WriteTexText("\n\\end{equation}");								\
    	WriteCringeEnd();								\
    	WriteTexText("\n\\begin{equation}\n");								\
    	WriteTexText("\\left(");								\
    	WriteTexNode(L, myTree);								\
    	WriteTexText("\\right)^{\\prime}");								\
    	WriteTexText("-\\left(");								\
    	WriteTexNode(R, myTree);								\
    	WriteTexText("\\right)^{\\prime}");								\
    	WriteTexText("\n\\end{equation}");

#define TEX_DIF_MUL(CurrentNode, myTree) \
    	WriteCringeStart();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\left(");							\
    	WriteTexText("\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)");							\
    	WriteTexText("\\cdot");							\
    	WriteTexText("\\left(");							\
    	WriteTexNode(R, myTree);							\
    	WriteTexText("\\right)");							\
    	WriteTexText("\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");							\
    	WriteCringeEnd();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)^{\\prime}\\cdot ");							\
    	WriteTexNode(R, myTree);							\
    	WriteTexText("+\\left(");							\
    	WriteTexNode(R, myTree);							\
    	WriteTexText("\\right)^{\\prime}\\cdot ");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\n\\end{equation}");

#define TEX_DIF_DIV(CurrentNode, myTree) \
    	WriteCringeStart();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\left(\\frac{");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("}{");							\
    	WriteTexNode(R, myTree);							\
    	WriteTexText("}\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");							\
    	WriteCringeEnd();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\frac{\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)^{\\prime}");							\
    	WriteTexText("-\\left(");							\
    	WriteTexNode(R, myTree);							\
    	WriteTexText("\\right)^{\\prime}}{");							\
    	WriteTexNode(R, myTree);							\
    	WriteTexText("^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");

#define TEX_DIF_POW_VAR_NUM(CurrentNode, myTree) \
		WriteCringeStart();							\
        WriteTexText("\n\\begin{equation}\n");							\
        WriteTexText("\\left(\\left(");							\
        WriteTexNode(L, myTree);							\
        WriteTexText("\\right)^{");							\
        WriteTexNumber(R->Value.Number);							\
        WriteTexText("}\\right)^{\\prime}");							\
        WriteTexText("\n\\end{equation}");							\
        WriteCringeEnd();							\
        WriteTexText("\n\\begin{equation}\n");							\
        WriteTexNumber(R->Value.Number);							\
        WriteTexText("\\cdot\\left(");							\
        WriteTexNode(L, myTree);							\
        WriteTexText("\\right)^{");							\
        WriteTexNumber(R->Value.Number-1);							\
        WriteTexText("}\\cdot\\left(");							\
        WriteTexNode(L, myTree);								\
        WriteTexText("\\right)^{\\prime}");							\
        WriteTexText("\n\\end{equation}");							

#define TEX_DIF_POW_NUM_VAR(CurrentNode, myTree) \
		WriteCringeStart();								\
        WriteTexText("\n\\begin{equation}\n");								\
        WriteTexText("\\left(\\left(");								\
        WriteTexNumber(L->Value.Number);								\
        WriteTexText("\\right)^{");								\
        WriteTexNode(R, myTree);								\
        WriteTexText("}\\right)^{\\prime}");								\
        WriteTexText("\n\\end{equation}");								\
        WriteCringeEnd();								\
        WriteTexText("\n\\begin{equation}\n");								\
        WriteTexNumber(L->Value.Number);								\
        WriteTexText("^{");								\
        WriteTexNode(R, myTree);								\
        WriteTexText("}\\cdot\\left(");								\
        WriteTexNode(R, myTree);								\
        WriteTexText("\\right)^{\\prime}\\cdot ln\\left(");								\
        WriteTexNumber(L->Value.Number);								\
        WriteTexText("\\right)");								\
        WriteTexText("\n\\end{equation}");								

#define TEX_DIF_POW_XX(CurrentNode, myTree) \
		WriteCringeStart();							\
        WriteTexText("\n\\begin{equation}\n");							\
        WriteTexText("\\left(\\left(");							\
        WriteTexNode(L, myTree);							\
        WriteTexText("\\right)^{");							\
        WriteTexNode(R, myTree);							\
        WriteTexText("}\\right)^{\\prime}");							\
        WriteTexText("\n\\end{equation}");							\
        WriteCringeEnd();							\
        WriteTexText("\n\\begin{equation}\n");							\
        WriteTexText("\\left(e^{");							\
        WriteTexNode(R, myTree);							\
        WriteTexText("\\cdot ln\\left(");							\
        WriteTexNode(L, myTree);							\
        WriteTexText("\\right)}\\right)^{\\prime} = ");							\
        WriteTexText("e^{");							\
        WriteTexNode(R, myTree);							\
        WriteTexText("\\cdot ln\\left(");							\
        WriteTexNode(L, myTree);							\
        WriteTexText("\\right)} \\cdot \\left(");							\
        WriteTexNode(R, myTree);							\
        WriteTexText("\\cdot ln\\left(");							\
        WriteTexNode(L, myTree);							\
        WriteTexText("\\right)\\right)^{\\prime}");							\
        WriteTexText("\n\\end{equation}");							

#define TEX_DIF_SIN(CurrentNode, myTree) \
		WriteCringeStart();										\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\left(cos\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");							\
    	WriteCringeEnd();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("sin\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)\\cdot\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");		

#define TEX_DIF_COS(CurrentNode, myTree) \
		WriteCringeStart();											\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\left(cos\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");							\
    	WriteCringeEnd();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("-sin\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)\\cdot\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");

#define TEX_DIF_TG(CurrentNode, myTree) \
		WriteCringeStart();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\left(tg\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");							\
    	WriteCringeEnd();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\frac{1}{cos\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)^{2}}\\cdot\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");

#define TEX_DIF_CTG(CurrentNode, myTree) \
    	WriteCringeStart();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\left(ctg\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");							\
    	WriteCringeEnd();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("-\\frac{1}{sin\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\left)^{2}}\\cdot\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");

#define TEX_DIF_LN(CurrentNode, myTree) \
		WriteCringeStart();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\left(ln\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");							\
    	WriteCringeEnd();							\
    	WriteTexText("\n\\begin{equation}\n");							\
    	WriteTexText("\\frac{1}{");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("}\\cdot\\left(");							\
    	WriteTexNode(L, myTree);							\
    	WriteTexText("\\right)^{\\prime}");							\
    	WriteTexText("\n\\end{equation}");

#endif