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
#else
    #define PrintLogTexTree(myTree)
    #define PrintLogTexStart()
    #define PrintLogTexFinish()
    #define WriteTexFormula(myTree)
    #define WriteTexNode(StartNode, myTree)
    #define WriteTexNumber(number)
	#define WriteCringeStart()
	#define WriteCringeEnd()
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
void  _WriteCringeStart  ();
void  _WriteCringeEnd    ();

#define FOLDER_LOG_TEX "tex"
#define FILE_LOG_TEX "tex/matan_book.tex"
#define FILE_LOG_OUT "tex/matan_book.pdf"
 
constexpr const char* CringeStart[] = 
{
    "Очевидно, что ",
	"Это элементарнийшее выражение ",  
	"Каждый советский эмбрион знает, что",
	"Вы проходили в школе, что ",
	"Надеюсь вы уже уснули, поэтому бла-бла-бла ",
	"Аналогично ",
	"Получаем, что",
	"Для любого $\\varepsilon$ > 0",
	"Отметим, что ",
	"Каждому кто закончил церковно-приходскую школу известно, что",
	"Вольфрам(спасибо что не молибден) говорит, что",
	"Я устал придумывать пасты поэтому ",
	"После округления ",
	"Аппроксимируя получаем, что ",
	"Косвенно получаем, что ",
	"Надо записать пока не забыл, надо забыть пока не записал, что",
	"Используя $9\\frac{3}{4}$ том Ландау-Лифшица получаем, что ",
};
constexpr const char* CringeEnd[] =
{
	"таким образом становится ",
	"переходит в ",
	"что-то странное, пусть станет ",
	"- табличная функция, равна ",
	"короче ",
	"лёгким движением руки превращается в элегантное ",
	"равно",
	"по 256 аксиоме Дединского будет",
	"может быть станет(хотя хз)",
	"непротиворечит следующему переходу ",
	"далее следует мысленный переход",
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
};

const int SIZE_OF_CRINGE_START = sizeof(CringeStart)/sizeof(CringeStart[0]); //размер массива оператора
const int SIZE_OF_CRINGE_END   = sizeof(CringeEnd)/sizeof(CringeEnd[0]); //размер массива оператора

#endif