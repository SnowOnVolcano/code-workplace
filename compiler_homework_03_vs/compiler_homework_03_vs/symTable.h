#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include "head.h"
#include "uthash-master/uthash.h"

// ·ûºÅ±í
enum itemsInSymbolTable {
	ST_HAS_RETURN_FUNC,
	ST_NON_RETURN_FUNC,
	ST_VARIABLE_IDEN,
	ST_CONST_IDEN
};

struct SymbolTable
{
	char key[64];
	int value;
	UT_hash_handle hh;
};

extern struct SymbolTable* symtabler;

extern void init_symtable();

extern struct SymbolTable* find_sym(char* key);

extern int get_symType(char* key);

extern void add_sym(char* key, int value_buf);

extern void delete_sym(char* key);

#endif // !_SYMTABLE_H_