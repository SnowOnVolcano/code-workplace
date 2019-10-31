#ifndef _HEAD_H_
#define _HEAD_H_

typedef int bool;

#include "linklist.h"

#define TOKENSIZE 0x80
#define _NORMAL_EXIT 1
#define _ERROR -1
#define _ERROR_STATEMENT -2
#define true 1
#define false 0

// 程序行号
extern int line_num; 
// 错误类型
enum error_types
{
	// 非法符号或不符合词法
	ERROR_A,
	// 名字重定义
	ERROR_B,
	// 未定义的名字
	ERROR_C,
	// 函数参数个数不匹配
	ERROR_D,
	// 函数参数类型不匹配
	ERROR_E,
	// 条件判断中出现不合法的类型
	ERROR_F,
	// 无返回值的函数存在不匹配的return语句
	ERROR_G,
	// 有返回值的函数缺少return语句或存在不匹配的return语句
	ERROR_H,
	// 数组元素的下标只能是整型表达式
	ERROR_I,
	// 不能改变常量的值
	ERROR_J,
	// 应为分号
	ERROR_K,
	// 应为右小括号
	ERROR_L,
	// 应为右中括号
	ERROR_M,
	// do-while语句中缺少‘while’
	ERROR_N,
	// 常量定义中‘=’后面只能是整型或字符型常量
	ERROR_O
};
// 错误处理函数
extern void error(int etype);

#endif // !_HEAD_H_
