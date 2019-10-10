#include <stdio.h>

#define TOKENSIZE 0x40
#define _NORMAL_EXIT 1
#define _ERROR -1

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

extern char buffer;           // current character
extern char token[TOKENSIZE]; // current word
extern int count;             // current index of token
extern int symbol;            // type of current word
extern FILE *fpIn;            // pointer to file input
extern FILE *fpOut;           // pointer to file output

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
char mnemonices[][10] = {
    "IDENFR",  "INTCON",  "CHARCON", "STRCON",   "CONSTTK",  "INTTK",
    "CHARTK",  "VOIDTK",  "MAINTK",  "IFTK",     "ELSETK",   "DOTK",
    "WHILETK", "FORTK",   "SCANFTK", "PRINTFTK", "RETURNTK", "PLUS",
    "MINU",    "MULT",    "DIV",     "LSS",      "LEQ",      "GRE",
    "GEQ",     "EQL",     "NEQ",     "ASSIGN",   "SEMICN",   "COMMA",
    "LPARENT", "RPARENT", "LBRACK",  "RBRACK",   "LBRACE",   "RBRACE"};
extern void getNext();
extern void retract();
extern void clearToken();
extern void catToken();
extern int reserver();
extern void error();
extern int init_getsym();
extern int getsym();
extern void output();
