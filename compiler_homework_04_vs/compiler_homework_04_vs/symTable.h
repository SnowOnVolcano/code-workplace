#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include "head.h"
#include "uthash-master/uthash.h"

// 符号表
enum typesOfItemsInSymbolTable {
	FUNC_HAS_RETURN_INT,
	FUNC_HAS_RETURN_CHAR,
	FUNC_NON_RETURN,
	VARIABLE_INT,
	VARIABLE_CHAR,
	CONST_INT,
	CONST_CHAR,
	ARRAY_INT,
	ARRAY_CHAR
};

struct SymbolTable
{
	char key[64];
	int type;
	struct list* valueList;
	UT_hash_handle hh;
};

extern struct SymbolTable* symtabler;

extern void init_symtable();

extern struct SymbolTable* find_sym(char* key);

// 获取标识符的类型
extern int get_symType(char* key);

// 获取标识符的值列表
extern struct list* get_symValueList(char* key);

// 添加新的标识符
extern int add_sym(char* key, int type);

// 添加指定标识符的类型
extern void change_symType(char* key, int newType);

// 删除标识符表
extern void delete_sym(char* key);

#endif // !_SYMTABLE_H_