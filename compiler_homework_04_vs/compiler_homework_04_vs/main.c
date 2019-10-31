#include "head.h"
#include "parsing.h"

int main() {
	init_getsym();
	init_symtable();
	getsym();
	program();
	return 0;
}