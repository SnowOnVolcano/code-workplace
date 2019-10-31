#include "symTable.h"

struct SymbolTable* symtabler = NULL;

void init_symtable() {
	symtabler = NULL;
}

struct SymbolTable* find_sym(char* key)
{
	struct SymbolTable* s;
	HASH_FIND_STR(symtabler, key, s);
	if (s == NULL) {
		error(ERROR_C);
	}
	return s;
}

int get_symType(char* key) {
	struct SymbolTable* s = find_sym(key);
	return s->type;
}

struct list* get_symValueList(char* key) {
	struct SymbolTable* s = find_sym(key);
	return s->valueList;
}

void add_sym(char* key, int type) {
	struct SymbolTable* s;
	HASH_FIND_STR(symtabler, key, s);
	if (s == NULL) {
		s = (struct SymbolTable*)malloc(sizeof(struct SymbolTable));
		strcpy(s->key, key);
		HASH_ADD_STR(symtabler, key, s);
		s->type = type;
	}
	else {
		error(ERROR_B);
	}
}

void change_symType(char* key, int newType) {
	struct SymbolTable* s = find_sym(key);
	s->type = newType;
}

void add_symValueList(char* key, struct list* valueList) {
	struct SymbolTable* s = find_sym(key);
	s->valueList = valueList;
}

void delete_sym(char* key) {
	struct SymbolTable* s = NULL;
	HASH_FIND_STR(symtabler, key, s);
	if (s == NULL) {
		HASH_DEL(symtabler, s);
		free(s);
	}
}

