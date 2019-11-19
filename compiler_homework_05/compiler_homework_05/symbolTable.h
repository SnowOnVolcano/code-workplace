#pragma once

#include "linkList.h"
#include "uthash/uthash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define KEYSIZE 0x80

// 符号表
enum itemsInSymbolItem {
	FUNC_NON_RETURN,
	FUNC_HAS_RETURN_INT,
	FUNC_HAS_RETURN_CHAR,
	VARIABLE_INT,
	VARIABLE_CHAR,
	CONST_INT,
	CONST_CHAR,
	ARRAY_INT,
	ARRAY_CHAR,
	STRING
};

typedef struct symbolItem
{
	char key[KEYSIZE];
	char string[0x400];
	int type;
	int index;
	int offset;
	int value;
	List_t* paraList;
	UT_hash_handle hh;
} SymbolItem_t;

typedef struct symbolTable
{
	char name[KEYSIZE];
	int count;
	int returnType;
	struct symbolItem* head;
} SymbolTable_t;


// 新建一个符号表
extern SymbolTable_t* newSymbolTable(char* name);

// 获得相应的符号表表项
extern SymbolItem_t* getSymbol(SymbolTable_t* symboltable, char* key);

// 获得相应的符号表的表项的类型
extern int getSymbolType(SymbolTable_t* symboltable, char* key);

// 获得相应的符号表的表项(数组)的大小
extern int getSymbolIndex(SymbolTable_t* symboltable, char* key);

// 获得相应的符号表的表项 存地址的偏移
extern int getSymbolOffset(SymbolTable_t* symboltable, char* key);

// 获得相应的符号表的表项 字符串
extern char* getSymbolString(SymbolTable_t* symboltable, char* key);

// 获得相应的符号表的表项 值
extern int getSymbolValue(SymbolTable_t* symboltable, char* key);

// 获得相应的符号表表项的参数表
extern List_t* getSymbolParaList(SymbolTable_t* symboltable, char* key);

// 向符号表添加一个表项
extern int addSymbol(SymbolTable_t* symboltable, char* key, int type);

// 向符号表的指定表项添加 类型
extern int addSymbolType(SymbolTable_t* symboltable, char* key, int type);

// 向符号表的指定表项添加 数组的大小
extern int addSymbolIndex(SymbolTable_t* symboltable, char* key, int index);

// 向符号表的指定表项添加 存地址的偏移
extern int addSymbolOffset(SymbolTable_t* symboltable, char* key, int offset);

// 向符号表的指定表项添加 字符串
extern int addSymbolString(SymbolTable_t* symboltable, char* key, char* string);

// 向符号表的指定表项添加 值
extern int addSymbolValue(SymbolTable_t* symboltable, char* key, int value);

// 向符号表的指定表项添加 参数表项
extern int addSymbolParaListItem(SymbolTable_t* symboltable, char* key, int data);

// 从符号表删除一个表项
extern int deleteSymbol(SymbolTable_t* symboltable, char* key);
