#include "errorHandler.h"

FILE* fpError;

int init_errorHandler() {
	fpError = fopen("error.txt", "w");
	return 0;
}

int error(int etype) {
	puts("-----------");
	if (etype == ERROR_K) {
		printf("%d %c\n", lineNum - isMinuLine, etype + 'a');
		fprintf(fpError, "%d %c\n", lineNum, etype + 'a');
	}
	else if (etype >= 0) {
		printf("%d %c\n", lineNum, etype + 'a');
		fprintf(fpError, "%d %c\n", lineNum, etype + 'a');
	}
	else if (etype == -1) {
		printf("%d _ERROR\n", lineNum);
		fprintf(fpError, "%d _ERROR\n", lineNum);
	}
	else if (etype == -2) {
		printf("%d _ERROR_GET\n", lineNum);
		fprintf(fpError, "%d _ERROR_GET\n", lineNum);
	}
	
	/*switch (etype)
	{
	case _ERROR_GET:
		break;
	case _ERROR:
		break;
	case ERROR_A:// 非法符号或不符合词法
		break;
	case ERROR_B:// 名字重定义
		break;
	case ERROR_C:// 未定义的名字
		break;
	case ERROR_D:// 函数参数个数不匹配
		break;
	case ERROR_E:// 函数参数类型不匹配
		break;
	case ERROR_F:// 条件判断中出现不合法的类型
		break;
	case ERROR_G:// 无返回值的函数存在不匹配的return语句
		break;
	case ERROR_H:// 有返回值的函数缺少return语句或存在不匹配的return语句
		break;
	case ERROR_I:// 数组元素的下标只能是整型表达式
		break;
	case ERROR_J:// 不能改变常量的值
		break;
	case ERROR_K:// 应为分号
		break;
	case ERROR_L:// 应为右小括号
		break;
	case ERROR_M:// 应为右中括号
		break;
	case ERROR_N:// do-while语句中缺少‘while’
		break;
	case ERROR_O:// 常量定义中‘=’后面只能是整型或字符型常量
		break;
	default:
		break;
	}*/

	puts("-----------");
	return 0;
}

