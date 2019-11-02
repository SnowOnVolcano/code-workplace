#include "head.h"
#include "parsing.h"

int initial() {
	init_getsym();
	init_parsing();
	lineNum = 0;
	return 0;
}

int main() {
	initial();
	getsym();
	program();
	return 0;
}