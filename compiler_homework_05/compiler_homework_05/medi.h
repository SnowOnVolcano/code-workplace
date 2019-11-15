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

FILE* fMediOut;		// 中间代码输出文件指针

int init_medi();	// 初始化中间代码生成程序

void init_temp();
char* new_temp();
char* new_label(SymbolItem_t* func_item, char* info);

void exit_medi();

void declare_func_medi(SymbolItem_t* func_item);

void invoke_func_medi(char* name);

void return_medi(char* v);
void return_medi(int v);
void return_medi(SymbolItem_t* func_item);

// void declare_global_var_medi(vector<VarItem*> vars);

void declare_para_medi(int type, char* name);

// void declare_var_medi(VarItem* var_item);

void label_medi(char* label);

void cal_medi(int op, char* result, char* a1, char* a2);
void cal_medi(int op, char* result, char* a1, int a2);
void cal_medi(int op, char* result, int a1, char* a2);

void assign_medi(char* n1, char* n2);
void assign_medi(char* name, int value);

void push_medi(char* name);
void push_medi(int name);

void return_get_medi(char* name);

void branch_zero_medi(char* name, char* label);

void branch_equal_medi(char* name, int value, char* label);

void jump_medi(char* label);

void jump_link_medi(char* label);

void array_get_medi(char* array_name, char* offset, char* result);
void array_get_medi(char* array_name, int offset, char* result);

void array_set_medi(char* array_name, char* offset, char* value);
void array_set_medi(char* array_name, int offset, char* value);
void array_set_medi(char* array_name, char* offset, int value);
void array_set_medi(char* array_name, int offset, int value);

void printf_medi(int type, char* v);
void printf_medi(int type, int v);
void printf_medi(int type, char* str);

void scanf_medi(int type, char* v);

void medi(char* line);