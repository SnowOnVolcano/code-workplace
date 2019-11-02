#include "head.h"
#include "parsing.h"

int initial() {
	init_getsym();
	stIndex = -1;
	return 0;
}

int main() {
	initial();
	getsym();
	program();
	int a = 0;
	return 0;
}