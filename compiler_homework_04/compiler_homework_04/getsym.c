#include "getsym.h"

char mnemonices[][10] = {
	"IDENFR",  "INTCON",  "CHARCON", "STRCON",   "CONSTTK",  "INTTK",
	"CHARTK",  "VOIDTK",  "MAINTK",  "IFTK",     "ELSETK",   "DOTK",
	"WHILETK", "FORTK",   "SCANFTK", "PRINTFTK", "RETURNTK", "PLUS",
	"MINU",    "MULT",    "DIV",     "LSS",      "LEQ",      "GRE",
	"GEQ",     "EQL",     "NEQ",     "ASSIGN",   "SEMICN",   "COMMA",
	"LPARENT", "RPARENT", "LBRACK",  "RBRACK",   "LBRACE",   "RBRACE" };

void thisLine_init() { thisLineCount = 0; printf("%4d |  %s\n", lineNum, thisLine); memset(thisLine, 0, LINESIZE); }
void thisLine_plus() { if (isAhead == 0 && buffer != '\n') { thisLine[thisLineCount++] = buffer; } }
void thisLine_minu() { if (isAhead == 0) { thisLine[--thisLineCount] = '\0'; } }

void getNext() { buffer = fgetc(fpIn);	thisLine_plus(); }
void retract() { ungetc(buffer, fpIn);  thisLine_minu(); }
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
	}
	else if (!strcmp(token, "int")) {
		return INTTK;
	}
	else if (!strcmp(token, "char")) {
		return CHARTK;
	}
	else if (!strcmp(token, "void")) {
		return VOIDTK;
	}
	else if (!strcmp(token, "main")) {
		return MAINTK;
	}
	else if (!strcmp(token, "if")) {
		return IFTK;
	}
	else if (!strcmp(token, "else")) {
		return ELSETK;
	}
	else if (!strcmp(token, "do")) {
		return DOTK;
	}
	else if (!strcmp(token, "while")) {
		return WHILETK;
	}
	else if (!strcmp(token, "for")) {
		return FORTK;
	}
	else if (!strcmp(token, "scanf")) {
		return SCANFTK;
	}
	else if (!strcmp(token, "printf")) {
		return PRINTFTK;
	}
	else if (!strcmp(token, "return")) {
		return RETURNTK;
	}
	else {
		return IDENFR;
	}
}

void saveCurrentSym() {
	prebuffer = buffer;
	strcpy(pretoken, token);
	precount = count;
	presymbol = symbol;
	prefpInOffset = ftell(fpIn);
	isAhead = 1;
}

void restorePreviousSym() {
	buffer = prebuffer;
	strcpy(token, pretoken);
	count = precount;
	symbol = presymbol;
	fseek(fpIn, prefpInOffset, SEEK_SET);
	isAhead = 0;
}

int init_getsym() {
	isAhead = 0;
	isNewLine = 0;
	if ((fpIn = fopen("testfile.txt", "rb")) != NULL) {
		fpOut = fopen("output.txt", "w");
		return 0;
	}
	else {
		return _ERROR;
	}
}

int getsym() {
	clearToken();
	do {
		if (isAhead == 0 && isNewLine == 1) { lineNum++; isNewLine--; }
		getNext();
		if (isAhead == 0 && buffer == '\n') { isNewLine = 1; thisLine_init(); }
	} while (isSpace(buffer));
	
	if (buffer == EOF) {
		thisLine_init();
		return _NORMAL_EXIT;
	}
	
	if (isLetter(buffer)) {
		while (isAlnum(buffer)) {
			catToken();
			getNext();
		}
		retract();
		symbol = reserver();
	}
	else if (isDigit(buffer)) {
		while (isDigit(buffer)) {
			catToken();
			getNext();
		}
		retract();
		symbol = INTCON;
	}
	else if (isSquote(buffer)) {
		getNext();
		catToken();

		/*错误处理*/ /*非法符号或不符合词法*/
		if (!isChar(buffer)) { error(ERROR_A); }
		
		getNext();

		/*错误处理*/ /*非法符号或不符合词法*/
		if (!isSquote(buffer)) { error(ERROR_A); retract(); }
		
		symbol = CHARCON;
	}
	else if (isDquote(buffer)) {
		getNext();
		while (!isDquote(buffer) && buffer != '\n') {

			/*错误处理*/ /*非法符号或不符合词法*/
			if (!isStr(buffer)) { error(ERROR_A); }

			catToken();
			getNext();
		}

		/*错误处理*/ /*非法符号或不符合词法*/
		if (buffer == '\n') { 
			error(ERROR_A);
			retract();
			for (int i = count - 1; i >= 0 && token[i] != ')'; i--){
				ungetc(token[i], fpIn);
				thisLine_minu();
			}
			ungetc(')', fpIn);
		}

		symbol = STRCON;
	}
	else if (isPlus(buffer)) {
		symbol = PLUS;
	}
	else if (isMinus(buffer)) {
		symbol = MINU;
	}
	else if (isStar(buffer)) {
		symbol = MULT;
	}
	else if (isDivi(buffer)) {
		symbol = DIV;
	}
	else if (isLss(buffer)) {
		catToken();
		getNext();
		if (isEqu(buffer)) {
			catToken();
			symbol = LEQ;
		}
		else {
			retract();
			symbol = LSS;
		}
	}
	else if (isGre(buffer)) {
		catToken();
		getNext();
		if (isEqu(buffer)) {
			catToken();
			symbol = GEQ;
		}
		else {
			retract();
			symbol = GRE;
		}
	}
	else if (isEqu(buffer)) {
		catToken();
		getNext();
		if (isEqu(buffer)) {
			catToken();
			symbol = EQL;
		}
		else {
			retract();
			symbol = ASSIGN;
		}
	}
	else if (isNeq(buffer)) {
		catToken();
		getNext();
		if (isEqu(buffer)) {
			catToken();
			symbol = NEQ;
		}
		else {
			retract();
			symbol = _ERROR;
		}
	}
	else if (isSemi(buffer)) {
		symbol = SEMICN;
	}
	else if (isComma(buffer)) {
		symbol = COMMA;
	}
	else if (isLpar(buffer)) {
		symbol = LPARENT;
	}
	else if (isRpar(buffer)) {
		symbol = RPARENT;
	}
	else if (isLbrack(buffer)) {
		symbol = LBRACK;
	}
	else if (isRbrack(buffer)) {
		symbol = RBRACK;
	}
	else if (isLbrace(buffer)) {
		symbol = LBRACE;
	}
	else if (isRbrace(buffer)) {
		symbol = RBRACE;
	}
	else {
		symbol = _ERROR;
	}
	if (count == 0) {
		catToken();
	}
	return 0;
}

int print_sym() {
	fprintf(fpOut, "%s %s\n", mnemonices[symbol], token);
	// printf("%s %s\n", mnemonices[symbol], token);
	return _NORMAL_EXIT;
}
