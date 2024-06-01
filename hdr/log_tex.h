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
#define FILE_LOG_TEX "tex/math_book.tex"
#define FILE_LOG_OUT "tex/math_book.pdf"
#define FILE_LOG_GRAPHIC "graphic.png"
#define FILE_LOG_DOTS "dots.txt"
#define FILE_LOG_PYTHON "drawer.py"

const double SIZE_LIMIT     = 70;
const int RIGHT_EDGE = 100;
const int LEFT_EDGE = -RIGHT_EDGE;

//функции для вывода теха
void tex_dif_num			(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_var			(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_add			(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_sub			(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_mul			(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_div			(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_pow_num_var	(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_pow_var_num	(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_pow_xx			(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_sin			(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_cos			(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_tg				(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_ctg			(Node_t* CurrentNode, BinaryTree_t* myTree);
void tex_dif_ln				(Node_t* CurrentNode, BinaryTree_t* myTree);

#ifdef DEBUG_LOG_TEX
	#define TEX_DIF_NUM(CurrentNode, myTree) \
			tex_dif_num(CurrentNode, myTree);
	#define TEX_DIF_VAR(CurrentNode, myTree) \
			tex_dif_var(CurrentNode, myTree);
	#define TEX_DIF_ADD(CurrentNode, myTree) \
			tex_dif_add(CurrentNode, myTree);
	#define TEX_DIF_SUB(CurrentNode, myTree) \
			tex_dif_sub(CurrentNode, myTree);
	#define TEX_DIF_MUL(CurrentNode, myTree) \
			tex_dif_mul(CurrentNode, myTree);
	#define TEX_DIF_DIV(CurrentNode, myTree) \
			tex_dif_div(CurrentNode, myTree);
	#define TEX_DIF_POW_VAR_NUM(CurrentNode, myTree) \
			tex_dif_pow_var_num(CurrentNode, myTree);	
	#define TEX_DIF_POW_NUM_VAR(CurrentNode, myTree) \
			tex_dif_pow_num_var(CurrentNode, myTree);
	#define TEX_DIF_POW_XX(CurrentNode, myTree) \
			tex_dif_pow_xx(CurrentNode, myTree);
	#define TEX_DIF_SIN(CurrentNode, myTree) \
			tex_dif_sin(CurrentNode, myTree);
	#define TEX_DIF_COS(CurrentNode, myTree) \
			tex_dif_cos(CurrentNode, myTree);
	#define TEX_DIF_TG(CurrentNode, myTree) \
			tex_dif_tg(CurrentNode, myTree);
	#define TEX_DIF_CTG(CurrentNode, myTree) \
			tex_dif_ctg(CurrentNode, myTree);
	#define TEX_DIF_LN(CurrentNode, myTree) \
			tex_dif_ln(CurrentNode, myTree);
#else
	#define TEX_DIF_NUM(CurrentNode, myTree) 
	#define TEX_DIF_VAR(CurrentNode, myTree)
	#define TEX_DIF_ADD(CurrentNode, myTree)
	#define TEX_DIF_SUB(CurrentNode, myTree)
	#define TEX_DIF_MUL(CurrentNode, myTree)
	#define TEX_DIF_DIV(CurrentNode, myTree)
	#define TEX_DIF_POW_NUM_VAR(CurrentNode, myTree)
	#define TEX_DIF_POW_VAR_NUM(CurrentNode, myTree)
	#define TEX_DIF_POW_XX(CurrentNode, myTree)
	#define TEX_DIF_SIN(CurrentNode, myTree)
	#define TEX_DIF_COS(CurrentNode, myTree)
	#define TEX_DIF_TG(CurrentNode, myTree)
	#define TEX_DIF_CTG(CurrentNode, myTree)
	#define TEX_DIF_LN(CurrentNode, myTree)
#endif

#endif