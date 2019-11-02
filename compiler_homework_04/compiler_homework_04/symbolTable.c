#include "symbolTable.h"

//SymbolItem_t* symboltable->head = NULL;

SymbolTable_t* newSymbolTable(char* name) {
	SymbolTable_t* newone;
	newone = (SymbolTable_t*)malloc(sizeof(SymbolTable_t));
	strcpy(newone->name, name);
	newone->count = 0;
	newone->head = NULL;
	return newone;
}

SymbolItem_t* getSymbol(SymbolTable_t* symboltable, char* key)
{
	SymbolItem_t* s;
	HASH_FIND_STR(symboltable->head, key, s);
	return s;
}

int getSymbolType(SymbolTable_t* symboltable, char* key) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		return s->type;
	}
}

List_t* getSymbolParaList(SymbolTable_t* symboltable, char* key) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return NULL;
	}
	else {
		return s->paraList;
	}
}

int addSymbol(SymbolTable_t* symboltable, char* key, int type) {
	SymbolItem_t* s;
	HASH_FIND_STR(symboltable->head, key, s);
	if (s != NULL) {
		return -1;
	}
	else {
		s = (SymbolItem_t*)malloc(sizeof(SymbolItem_t));
		strcpy(s->key, key);
		s->type = type;
		HASH_ADD_STR(symboltable->head, key, s);
		if (symboltable->count == 0) {
			symboltable->head = s;
		}
		symboltable->count++;
	}
	return 0;
}

int addSymbolType(SymbolTable_t* symboltable, char* key, int type) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		s->type = type;
	}
	return 0;
}

int addSymbolParaList(SymbolTable_t* symboltable, char* key, List_t* paraList) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		s->paraList = paraList;
	}
	return 0;
}

int deleteSymbol(SymbolTable_t* symboltable, char* key) {
	SymbolItem_t* s = NULL;
	HASH_FIND_STR(symboltable->head, key, s);
	if (s == NULL) {
		return -1;
	}
	else {
		HASH_DEL(symboltable->head, s);
		free(s);
	}
	return 0;
}

