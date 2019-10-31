#ifndef _GETSYM_H_
#define _GETSYM_H_
#include "head.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


#define isSpace(x) isspace(x)
#define isDigit(x) isdigit(x)
#define isLetter(x) (x == '_' || isalpha(x))
#define isAlnum(x) (x == '_' || isalnum(x))
#define isSquote(x) (x == '\'')
#define isDquote(x) (x == '\"')
#define isPlus(x) (x == '+')
#define isMinus(x) (x == '-')
#define isStar(x) (x == '*')
#define isDivi(x) (x == '/')
#define isLss(x) (x == '<')
#define isGre(x) (x == '>')
#define isEqu(x) (x == '=')
#define isNeq(x) (x == '!')
#define isSemi(x) (x == ';')
#define isComma(x) (x == ',')
#define isLpar(x) (x == '(')
#define isRpar(x) (x == ')')
#define isLbrack(x) (x == '[')
#define isRbrack(x) (x == ']')
#define isLbrace(x) (x == '{')
#define isRbrace(x) (x == '}')
#define isStr(x) (x==32||x==33||(x>=35&&x<=126))
#define isChar(x) (isLetter(x)||x=='+'||x=='-'||x=='*'||x=='/')

// 词法分析-序号对应表
enum mnemonic {
	IDENFR,
	INTCON,
	CHARCON,
	STRCON,
	CONSTTK,
	INTTK,
	CHARTK,
	VOIDTK,
	MAINTK,
	IFTK,
	ELSETK,
	DOTK,
	WHILETK,
	FORTK,
	SCANFTK,
	PRINTFTK,
	RETURNTK,
	PLUS,
	MINU,
	MULT,
	DIV,
	LSS,
	LEQ,
	GRE,
	GEQ,
	EQL,
	NEQ,
	ASSIGN,
	SEMICN,
	COMMA,
	LPARENT,
	RPARENT,
	LBRACK,
	RBRACK,
	LBRACE,
	RBRACE
};

// 词法分析-名称对应表
extern char mnemonices[][10];

char buffer;			// current character
char token[TOKENSIZE];	// current word
int count;				// current index of token
int symbol;				// type of current word
char prebuffer;			// previous character
char pretoken[TOKENSIZE];	// previous word
int precount;				// previous index of token
int presymbol;				// type of previous word
long int prefpInOffset;	// previous offset of fpIn(FILE*)
bool ahead_flag;		// 是否预读：1-是，0-不是
FILE* fpIn;				// pointer to current location of the file "testfile.txt"
FILE* fpOut;			// pointer to current location of the file "output.txt"

extern void getNext();
extern void retract();
extern void clearToken();
extern void catToken();
extern int reserver();
extern void saveCurrentSym();
extern void restorePreviousSym();
extern int init_getsym();
extern int getsym_print();
extern int getsym();
extern bool print_sym();


#endif