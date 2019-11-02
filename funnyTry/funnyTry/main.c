#include "symbolTable.h"

int main() {
	SymbolTable_t* one = newSymbolTable("one");
	SymbolTable_t* two = newSymbolTable("two");
	addSymbol(one, "111");
	addSymbol(one, "222");
	addSymbolType(one, "111", 1);
	addSymbolType(one, "222", 2);

	addSymbol(two, "111");
	addSymbol(two, "222");
	addSymbolType(two, "111", 1);
	addSymbolType(two, "222", 2);
	
	int a = getSymbolType(one, "111");
	int b = getSymbolType(one, "222");
}