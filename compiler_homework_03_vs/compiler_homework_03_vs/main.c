#include "head.h"
#include "parsing.h"

int main() {
	//init_getsym();
	//init_symtable();
	//getsym();
	//program();
	add_sym("111", 1);
	add_sym("222", 2);
	get_symType("111");
	get_symType("222");
	return 0;
}