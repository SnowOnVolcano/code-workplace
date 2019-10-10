#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char buffer;           // current character
char token[TOKENSIZE]; // current word
int count;             // current index of token
int symbol;            // type of current word
FILE *fpIn;            // pointer to file input
FILE *fpOut;           // pointer to file output

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
char *mnemonices[] = {
    "IDENFR",  "INTCON",  "CHARCON", "STRCON",   "CONSTTK",  "INTTK",
    "CHARTK",  "VOIDTK",  "MAINTK",  "IFTK",     "ELSETK",   "DOTK",
    "WHILETK", "FORTK",   "SCANFTK", "PRINTFTK", "RETURNTK", "PLUS",
    "MINU",    "MULT",    "DIV",     "LSS",      "LEQ",      "GRE",
    "GEQ",     "EQL",     "NEQ",     "ASSIGN",   "SEMICN",   "COMMA",
    "LPARENT", "RPARENT", "LBRACK",  "RBRACK",   "LBRACE",   "RBRACE"};
void getNext() { buffer = fgetc(fpIn); }
void retract() { ungetc(buffer, fpIn); }
void clearToken() {
  memset(token, 0, TOKENSIZE);
  buffer = '\0';
  count = 0;
  symbol = 0;
}
void catToken() { token[count++] = buffer; }
int reserver() {
  if (!strcmp(token, "const")) {
    return CONSTTK;
  } else if (!strcmp(token, "int")) {
    return INTTK;
  } else if (!strcmp(token, "char")) {
    return CHARTK;
  } else if (!strcmp(token, "void")) {
    return VOIDTK;
  } else if (!strcmp(token, "main")) {
    return MAINTK;
  } else if (!strcmp(token, "if")) {
    return IFTK;
  } else if (!strcmp(token, "else")) {
    return ELSETK;
  } else if (!strcmp(token, "do")) {
    return DOTK;
  } else if (!strcmp(token, "while")) {
    return WHILETK;
  } else if (!strcmp(token, "for")) {
    return FORTK;
  } else if (!strcmp(token, "scanf")) {
    return SCANFTK;
  } else if (!strcmp(token, "printf")) {
    return PRINTFTK;
  } else if (!strcmp(token, "return")) {
    return RETURNTK;
  } else {
    return IDENFR;
  }
}
void error() {}

int init_getsym() {
  if ((fpIn = fopen("testfile.txt", "r")) != NULL) {
    fpOut = fopen("output.txt", "w");
    return 0;
  } else {
    return _ERROR;
  }
}

int getsym() {
  clearToken();
  do {
    getNext();
  } while (isSpace(buffer));
  if (buffer == EOF) {
    return _NORMAL_EXIT;
  }
  if (isLetter(buffer)) {
    while (isAlnum(buffer)) {
      catToken();
      getNext();
    }
    retract();
    symbol = reserver();
  } else if (isDigit(buffer)) {
    while (isDigit(buffer)) {
      catToken();
      getNext();
    }
    retract();
    symbol = INTCON;
  } else if (isSquote(buffer)) {
    getNext();
    while (!isSquote(buffer)) {
      catToken();
      getNext();
    }
    if (count == 1) {
      symbol = CHARCON;
    } else {
      symbol = _ERROR;
    }
  } else if (isDquote(buffer)) {
    getNext();
    while (!isDquote(buffer)) {
      catToken();
      getNext();
    }
    symbol = STRCON;
  } else if (isPlus(buffer)) {
    symbol = PLUS;
  } else if (isMinus(buffer)) {
    symbol = MINU;
  } else if (isStar(buffer)) {
    symbol = MULT;
  } else if (isDivi(buffer)) {
    symbol = DIV;
  } else if (isLss(buffer)) {
    catToken();
    getNext();
    if (isEqu(buffer)) {
      catToken();
      symbol = LEQ;
    } else {
      retract();
      symbol = LSS;
    }
  } else if (isGre(buffer)) {
    catToken();
    getNext();
    if (isEqu(buffer)) {
      catToken();
      symbol = GEQ;
    } else {
      retract();
      symbol = GRE;
    }
  } else if (isEqu(buffer)) {
    catToken();
    getNext();
    if (isEqu(buffer)) {
      catToken();
      symbol = EQL;
    } else {
      retract();
      symbol = ASSIGN;
    }
  } else if (isNeq(buffer)) {
    catToken();
    getNext();
    if (isEqu(buffer)) {
      catToken();
      symbol = NEQ;
    } else {
      retract();
      symbol = _ERROR;
    }
  } else if (isSemi(buffer)) {
    symbol = SEMICN;
  } else if (isComma(buffer)) {
    symbol = COMMA;
  } else if (isLpar(buffer)) {
    symbol = LPARENT;
  } else if (isRpar(buffer)) {
    symbol = RPARENT;
  } else if (isLbrack(buffer)) {
    symbol = LBRACK;
  } else if (isRbrack(buffer)) {
    symbol = RBRACK;
  } else if (isLbrace(buffer)) {
    symbol = LBRACE;
  } else if (isRbrace(buffer)) {
    symbol = RBRACE;
  } else {
    symbol = _ERROR;
  }
  if (count == 0) {
    catToken();
  }
  return 0;
}

void output() {
  while (!getsym()) {
    if (symbol == _ERROR) {
      error();
    } else {
      fprintf(fpOut, "%s %s\n", mnemonices[symbol], token);
    }
  }
  fclose(fpIn);
  fclose(fpOut);
}

int main() {
  init_getsym();
  output();
}