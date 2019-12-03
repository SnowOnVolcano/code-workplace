#include "head.h"
#include "parsing.h"
#include "analyseMedi.h"

int initial() {
	init_getsym();
	init_medi();
	init_parsing();
	init_errorHandler();
	lineNum = 1;
	return 0;
}

int main() {
	initial();
	getsym();
	program();
	fclose(fMediOut);
	init_analyseMedi();
	analyseMedi();
	fclose(fmedi);
	fclose(fmips);
	return 0;
}