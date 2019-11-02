#pragma once

#include "linkList.h"
#include "uthash-master/uthash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TOKENSIZE 0x80

// ���ű�
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


// �½�һ�����ű�
extern SymbolTable_t* newSymbolTable(char* name);

// �����Ӧ�ķ��ű����
extern SymbolItem_t* getSymbol(SymbolTable_t* symboltable, char* key);

// �����Ӧ�ķ��ű�ı��������
extern int getSymbolType(SymbolTable_t* symboltable, char* key);

// �����Ӧ�ķ��ű����Ĳ�����
extern List_t* getSymbolParaList(SymbolTable_t* symboltable, char* key);

// ����ű����һ������
extern int addSymbol(SymbolTable_t* symboltable, char* key);

// ����ű��ָ��������� ����
extern int addSymbolType(SymbolTable_t* symboltable, char* key, int type);

// ����ű��ָ��������� ������
extern int addSymbolParaList(SymbolTable_t* symboltable, char* key, List_t* paraList);

// �ӷ��ű�ɾ��һ������
extern int deleteSymbol(SymbolTable_t* symboltable, char* key);
