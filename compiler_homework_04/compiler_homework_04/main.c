#include "head.h"
#include "parsing.h"

int initial() {
	init_getsym();
	init_parsing();
	return 0;
}

int main() {
	initial();
	getsym();
	program();
	return 0;
}