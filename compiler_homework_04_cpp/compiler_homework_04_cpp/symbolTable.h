#pragma once

#include "linkList.h"
#include "uthash-master/uthash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TOKENSIZE 0x80

// 符号表
enum itemsInSymbolItem {
	ST_HAS_RETURN_FUNC,
	ST_NON_RETURN_FUNC,
	ST_VARIABLE_IDEN,
	ST_CONST_IDEN
};

typedef struct symbolItem
{
	char key[TOKENSIZE];
	int type;
	List_t* paraList;
	UT_hash_handle hh;
} SymbolItem_t;

typedef struct symbolTable
{
	char name[TOKENSIZE];
	struct symbolItem* head;
} SymbolTable_t;


// 新建一个符号表
extern SymbolTable_t* newSymbolTable(char* name);

// 获得相应的符号表表项
extern SymbolItem_t* getSymbol(SymbolTable_t* symboltable, char* key);

// 获得相应的符号表的表项的类型
extern int getSymbolType(SymbolTable_t* symboltable, char* key);

// 获得相应的符号表表项的参数表
extern List_t* getSymbolParaList(SymbolTable_t* symboltable, char* key);

// 向符号表添加一个表项
extern int addSymbol(SymbolTable_t* symboltable, char* key);

// 向符号表的指定表项添加 类型
extern int addSymbolType(SymbolTable_t* symboltable, char* key, int type);

// 向符号表的指定表项添加 参数表
extern int addSymbolParaList(SymbolTable_t* symboltable, char* key, List_t* paraList);

// 从符号表删除一个表项
extern int deleteSymbol(SymbolTable_t* symboltable, char* key);
