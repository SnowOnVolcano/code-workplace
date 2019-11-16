#pragma once
#include "symbolTable.h"
#include "getsym.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRSIZE 0x100 
#define true 1
#define false 0
typedef int bool;

FILE* fMediOut;		// �м��������ļ�ָ��
extern int branch;
extern int level;
extern int temp_count;
extern int temps[64];
extern bool in_switch;

// ��ʼ���м�������ɳ���
int init_medi();

void init_temp();

char* new_temp();

// �½�һ��Label
char* new_label(SymbolTable_t* func_item, char* info);

// �����˳�
void exit_medi();
// ��������
void declare_func_medi(SymbolItem_t* func_item);

// ������������
void declare_para_medi(int type, char* name);

// ��������
void declare_var_medi(SymbolItem_t* var_item);

// ��������
void invoke_func_medi(char* name);

// �������
void return_medi_c(char* v);
void return_medi_i(int v);
void return_medi_f(SymbolItem_t* func_item);

// ��ת��־��
void label_medi(char* label);

// cond must not be zero
void cal_medi_iccc(int op, char* result, char* a1, char* a2);
void cal_medi_icci(int op, char* result, char* a1, int a2);
void cal_medi_icic(int op, char* result, int a1, char* a2);

// ��ֵ���
void assign_medi_cc(char* n1, char* n2);
void assign_medi_ci(char* name, int value);

// �������
void push_medi_c(char* name);
void push_medi_i(int name);

// ���ܷ���ֵ
void return_get_medi(char* name);

// ���� 0 ʱ��ת
void branch_zero_medi(char* name, char* label);

// ���ʱ��ת 
void branch_equal_medi(char* name, int value, char* label);

// ��������ת
void jump_medi(char* label);

// ��������ת������
void jump_link_medi(char* label);

// ����ȡֵ
void array_get_medi_ccc(char* array_name, char* offset, char* result);
void array_get_medi_cic(char* array_name, int offset, char* result);

// ���鸳ֵ
void array_set_medi_ccc(char* array_name, char* offset, char* value);
void array_set_medi_cic(char* array_name, int offset, char* value);
void array_set_medi_cci(char* array_name, char* offset, int value);
void array_set_medi_cii(char* array_name, int offset, int value);


// vector<char*> str_set;
// ������
void printf_medi_ic(int type, char* v);
void printf_medi_ii(int type, int v);

// �������
void scanf_medi(int type, char* v);

void medi(char* line);
