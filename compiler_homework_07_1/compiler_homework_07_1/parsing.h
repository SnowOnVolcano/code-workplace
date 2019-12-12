#pragma once

#include "head.h"
#include "symbolTable.h"
#include "getsym.h"
#include "medi.h"

// �﷨����-�������
enum statement_types
{
	// conditionalStatement
	S_CONDITIONAL,
	// loopStatement
	S_LOOP,
	// statecolumn
	S_STATECOLUMN,
	// refunc_callStatement
	S_REFUNCCALL,
	// unrefunc_callStatement
	S_UNREFUNCCALL,
	// assignStatement
	S_ASSIGN,
	// readStatement
	S_READ,
	// printStatement
	S_PRINT,
	// nullStatement
	S_NULL,
	// returnStatement
	S_RETURN
};

SymbolTable_t* symbolTables[0x100];
SymbolTable_t* stringPool;
int stIndex;

int init_parsing();						// ��ʼ��

char* string();							// �ַ���
void program();							// ����
void con_info();						// ����˵��
void con_definition();					// ��������
int unsigned_integer();					// �޷�������
int integer();							// ����
void var_info();						// ����˵��
void var_definition();					// ��������
void declarator();						// ����ͷ��
void re_func_definition();				// �з��غ�������
void unre_func_definition();			// �޷��غ�������
void compoundStatement();				// �������
void paraList();						// ������
void mainFunction();					// ������
int expression(int* value, bool* certain, char* name);	// ���ʽ
int term(int* value, bool* certain, char* name);		// ��
int factor(int* value, bool* certain, char* name);		// ����
int statement();						// ���
int iden_statement();					// ��ʶ����ͷ�����
void assignStatement();					// ��ֵ���
void conditionalStatement();			// �������
void condition(int* value, bool* certain, char* name);	// ����
void loopStatement();					// ѭ�����
void stride();							// ����
void refunc_callStatement();			// �з���ֵ�����������
void unrefunc_callStatement();			// �޷���ֵ�����������
void valueparaList(List_t* paraList);	// ֵ������
void statecolumn();						// �����
void readStatement();					// �����
void printStatement();					// д���
void returnStatement();					// �������

