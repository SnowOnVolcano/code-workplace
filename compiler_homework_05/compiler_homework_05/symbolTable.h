#pragma once

#include "linkList.h"
#include "uthash/uthash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define KEYSIZE 0x80

// ���ű�
enum itemsInSymbolItem {
	FUNC_NON_RETURN,
	FUNC_HAS_RETURN_INT,
	FUNC_HAS_RETURN_CHAR,
	VARIABLE_INT,
	VARIABLE_CHAR,
	CONST_INT,
	CONST_CHAR,
	ARRAY_INT,
	ARRAY_CHAR
};

typedef struct symbolItem
{
	char key[KEYSIZE];
	int type;
	int index;
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


// �½�һ�����ű�
extern SymbolTable_t* newSymbolTable(char* name);

// �����Ӧ�ķ��ű����
extern SymbolItem_t* getSymbol(SymbolTable_t* symboltable, char* key);

// �����Ӧ�ķ��ű�ı��������
extern int getSymbolType(SymbolTable_t* symboltable, char* key);

// �����Ӧ�ķ��ű�ı���(����)�Ĵ�С
extern int getSymbolIndex(SymbolTable_t* symboltable, char* key);

// �����Ӧ�ķ��ű����Ĳ�����
extern List_t* getSymbolParaList(SymbolTable_t* symboltable, char* key);

// ����ű����һ������
extern int addSymbol(SymbolTable_t* symboltable, char* key, int type);

// ����ű��ָ��������� ����
extern int addSymbolType(SymbolTable_t* symboltable, char* key, int type);

// ����ű��ָ��������� ����Ĵ�С
extern int addSymbolIndex(SymbolTable_t* symboltable, char* key, int Index);

// ����ű��ָ��������� ��������
extern int addSymbolParaListItem(SymbolTable_t* symboltable, char* key, int data);

// �ӷ��ű�ɾ��һ������
extern int deleteSymbol(SymbolTable_t* symboltable, char* key);
