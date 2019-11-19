#include "symbolTable.h"

SymbolTable_t* newSymbolTable(char* name) {
	SymbolTable_t* newone;
	newone = (SymbolTable_t*)malloc(sizeof(SymbolTable_t));
	strcpy(newone->name, name);
	newone->count = 0;
	newone->returnType = 0;
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

int getSymbolIndex(SymbolTable_t* symboltable, char* key) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		return s->index;
	}
}

int getSymbolOffset(SymbolTable_t* symboltable, char* key) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		return s->offset;
	}
}

char* getSymbolString(SymbolTable_t* symboltable, char* key) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return NULL;
	}
	else {
		return s->string;
	}
}

int getSymbolValue(SymbolTable_t* symboltable, char* key) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		return s->value;
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
		s->paraList = NULL;
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

int addSymbolIndex(SymbolTable_t* symboltable, char* key, int index) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		s->index = index;
	}
	return 0;
}

int addSymbolOffset(SymbolTable_t* symboltable, char* key, int offset) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		s->offset = offset;
	}
	return 0;
}

int addSymbolString(SymbolTable_t* symboltable, char* key, char* string) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		strcpy(s->string, string);
	}
	return 0;
}

int addSymbolValue(SymbolTable_t* symboltable, char* key, int value) {
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		s->value = value;
	}
	return 0;
}

int addSymbolParaListItem(SymbolTable_t* symboltable, char* key, int data){
	int r;
	SymbolItem_t* s = getSymbol(symboltable, key);
	if (s == NULL) {
		return -1;
	}
	else {
		if (s->paraList == NULL) {
			s->paraList = newList();
		}
		r = appendListTail(s->paraList, data);
		return r;
	}
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

