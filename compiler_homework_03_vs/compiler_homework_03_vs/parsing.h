#ifndef _PARSING_H_
#define _PARSING_H_
#include "head.h"
#include "symTable.h"
#include "getsym.h"

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

void program();					// ����
void con_info();				// ����˵��
void con_definition();			// ��������
void unsigned_integer();		// �޷�������
void integer();					// ����
void declarator();				// ����ͷ��
void var_info();				// ����˵��
void var_definition();			// ��������
void re_func_definition();		// �з��غ�������
void unre_func_definition();	// �޷��غ�������
void compoundStatement();		// �������
void paraList();				// ������
void mainFunction();			// ������
void expression();				// ���ʽ
void term();					// ��
void factor();					// ����
int  statement();				// ���
int iden_statement();			// ��ʶ����ͷ�����
void assignStatement();			// ��ֵ���
void conditionalStatement();    // �������
void condition();				// ����
void loopStatement();			// ѭ�����
void stride();					// ����
void refunc_callStatement();	// �з���ֵ�����������
void unrefunc_callStatement();	// �޷���ֵ�����������
void valueparaList();			// ֵ������
void statecolumn();				// �����
void readStatement();			// �����
void printStatement();			// д���
void returnStatement();			// �������


#endif