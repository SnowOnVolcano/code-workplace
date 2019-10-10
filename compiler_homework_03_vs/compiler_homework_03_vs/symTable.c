#include "symTable.h"

struct SymbolTable* symtabler = NULL;

void init_symtable(){
	symtabler = NULL;
}

struct SymbolTable* find_sym(char* key)
{
	struct SymbolTable* s;
	HASH_FIND_STR(symtabler, key, s);
	return s;
}

int get_symType(char* key) {
	struct SymbolTable* s = find_sym(key);
	if (s == NULL) {
		error();
		return _ERROR;
	}
	else {
		return s->value;
	}
}

void add_sym(char* key, int value_buf) {
	struct SymbolTable* s;
	HASH_FIND_STR(symtabler, key, s);
	if (s == NULL) {
		s = (struct SymbolTable*)malloc(sizeof(struct SymbolTable));
		strcpy(s->key, key);
		HASH_ADD_STR(symtabler, key, s);
	}
	s->value = value_buf;
}

void delete_sym(char* key) {
	struct SymbolTable* s = NULL;
	HASH_FIND_STR(symtabler, key, s);
	if (s == NULL) {
		HASH_DEL(symtabler, s);
		free(s);
	}
}

