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
extern int branch;
extern int level;
extern int temp_count;
extern int temps[64];
extern bool in_switch;

// 初始化中间代码生成程序
int init_medi();

void init_temp();

char* new_temp();

// 新建一个Label
char* new_label(SymbolTable_t* func_item, char* info);

// 结束退出
void exit_medi();
// 函数声明
void declare_func_medi(SymbolItem_t* func_item);

// 函数参数声明
void declare_para_medi(int type, char* name);

// 变量声明
void declare_var_medi(SymbolItem_t* var_item);

// 函数调用
void invoke_func_medi(char* name);

// 返回语句
void return_medi_c(char* v);
void return_medi_i(int v);
void return_medi_f(SymbolItem_t* func_item);

// 跳转标志符
void label_medi(char* label);

// cond must not be zero
void cal_medi_iccc(int op, char* result, char* a1, char* a2);
void cal_medi_icci(int op, char* result, char* a1, int a2);
void cal_medi_icic(int op, char* result, int a1, char* a2);

// 赋值语句
void assign_medi_cc(char* n1, char* n2);
void assign_medi_ci(char* name, int value);

// 传入参数
void push_medi_c(char* name);
void push_medi_i(int name);

// 接受返回值
void return_get_medi(char* name);

// 等于 0 时跳转
void branch_zero_medi(char* name, char* label);

// 相等时跳转 
void branch_equal_medi(char* name, int value, char* label);

// 无条件跳转
void jump_medi(char* label);

// 无条件跳转并链接
void jump_link_medi(char* label);

// 数组取值
void array_get_medi_ccc(char* array_name, char* offset, char* result);
void array_get_medi_cic(char* array_name, int offset, char* result);

// 数组赋值
void array_set_medi_ccc(char* array_name, char* offset, char* value);
void array_set_medi_cic(char* array_name, int offset, char* value);
void array_set_medi_cci(char* array_name, char* offset, int value);
void array_set_medi_cii(char* array_name, int offset, int value);


// vector<char*> str_set;
// 输出语句
void printf_medi_ic(int type, char* v);
void printf_medi_ii(int type, int v);

// 输入语句
void scanf_medi(int type, char* v);

void medi(char* line);
