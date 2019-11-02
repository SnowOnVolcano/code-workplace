#include "head.h"
#include "parsing.h"

int main() {
	//init_getsym();
	//init_symtable();
	//getsym();
	//program();

	struct list* l = list_new();
	listnode_add(l, "aaa");
	listnode_add(l, "bbb");
	listnode_add(l, "ccc");
	listnode_add(l, "ddd");

	return 0;
}