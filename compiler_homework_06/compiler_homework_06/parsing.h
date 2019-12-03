#pragma once

#include "head.h"
#include "symbolTable.h"
#include "getsym.h"
#include "medi.h"

// 语法分析-语句类型
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

int init_parsing();						// 初始化

char* string();							// 字符串
void program();							// 程序
void con_info();						// 常量说明
void con_definition();					// 常量定义
int unsigned_integer();					// 无符号整数
int integer();							// 整数
void var_info();						// 变量说明
void var_definition();					// 变量定义
void declarator();						// 声明头部
void re_func_definition();				// 有返回函数定义
void unre_func_definition();			// 无返回函数定义
void compoundStatement();				// 复合语句
void paraList();						// 参数表
void mainFunction();					// 主函数
int expression(int* value, bool* certain, char* name);	// 表达式
int term(int* value, bool* certain, char* name);		// 项
int factor(int* value, bool* certain, char* name);		// 因子
int statement();						// 语句
int iden_statement();					// 标识符开头的语句
void assignStatement();					// 赋值语句
void conditionalStatement();			// 条件语句
void condition(int* value, bool* certain, char* name);	// 条件
void loopStatement();					// 循环语句
void stride();							// 步长
void refunc_callStatement();			// 有返回值函数调用语句
void unrefunc_callStatement();			// 无返回值函数调用语句
void valueparaList(List_t* paraList);	// 值参数表
void statecolumn();						// 语句列
void readStatement();					// 读语句
void printStatement();					// 写语句
void returnStatement();					// 返回语句

