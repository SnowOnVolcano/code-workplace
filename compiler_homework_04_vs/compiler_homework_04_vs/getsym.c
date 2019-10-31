#include "getsym.h"

char mnemonices[][10] = {
	"IDENFR",  "INTCON",  "CHARCON", "STRCON",   "CONSTTK",  "INTTK",
	"CHARTK",  "VOIDTK",  "MAINTK",  "IFTK",     "ELSETK",   "DOTK",
	"WHILETK", "FORTK",   "SCANFTK", "PRINTFTK", "RETURNTK", "PLUS",
	"MINU",    "MULT",    "DIV",     "LSS",      "LEQ",      "GRE",
	"GEQ",     "EQL",     "NEQ",     "ASSIGN",   "SEMICN",   "COMMA",
	"LPARENT", "RPARENT", "LBRACK",  "RBRACK",   "LBRACE",   "RBRACE" };

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
	ahead_flag = 1;
}

void restorePreviousSym() {
	buffer = prebuffer;
	strcpy(token, pretoken);
	count = precount;
	symbol = presymbol;
	fseek(fpIn, prefpInOffset, SEEK_SET);
	ahead_flag = 0;
}

int init_getsym() {
	if ((fpIn = fopen("testfile.txt", "r")) != NULL) {
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
		getNext();
		if (buffer == '\n') {
			line_num++;
		}
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
		if (!isChar(buffer)) {
			error(ERROR_A);
		}
		catToken();
		getNext();
		if (!isSquote(buffer)) {
			error(ERROR_A);
		}
		symbol = CHARCON;
	}
	else if (isDquote(buffer)) {
		getNext();
		while (!isDquote(buffer)) {
			if (!isStr(buffer)) {
				error(ERROR_A);
			}
			catToken();
			getNext();
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
		error(ERROR_A);
	}
	if (count == 0) {
		catToken();
	}
	return 0;
}

int getsym_print() {
	int r = getsym();
	print_sym();
	return r;
}


bool print_sym() {
	fprintf(fpOut, "%s %s\n", mnemonices[symbol], token);
	// printf("%s %s\n", mnemonices[symbol], token);
	return true;
}

//void output() {
//	while (!getsym()) {
//		if (symbol == _ERROR) {
//			error(_ERROR);
//		}
//		else {
//			fprintf(fpOut, "%s %s\n", mnemonices[symbol], token);
//		}
//	}
//	fclose(fpIn);
//	fclose(fpOut);
//}

//int main() {
//	init_getsym();
//	output();
//}