#include "analyseMedi.h"

// 初始化中间代码分析器
int init_analyseMedi() {
	if ((fmedi = fopen("mips_mediate_code.txt", "rb")) < 0) {
		printf("Cannot open the file!\n");
		exit(0);
	}
	fmips = fopen("mips.txt", "w");
	global_vars = newSymbolTable("");
	local_vars = newSymbolTable("");
	gpOffset = fpOffset = spOffset = 0;
	return 0;
}

bool isInteger(char str[]) {
	return (str[0] == '-' || str[0] == '+' || isdigit(str[0]));
}
void loadReg(char* reg_name, char* var_name, int index) {
	if (isInteger(var_name)) {
		fprintf(fmips, "li %s, %s\n", reg_name, var_name);
	}
	else {
		if (getSymbolType(local_vars, var_name) < 0) {
			fprintf(fmips, "lw %s, %d($gp)\n", reg_name, index * 4 + getSymbolOffset(global_vars, var_name));
		}
		else {
			fprintf(fmips, "lw %s, %d($sp)\n", reg_name, index * 4 + getSymbolOffset(local_vars, var_name));
		}
	}
}
void saveReg(char* reg_name, char* var_name, int index) {
	if (getSymbolType(local_vars, var_name) < 0) {
		if (getSymbolType(global_vars, var_name) < 0) {
			addSymbol(local_vars, var_name, VARIABLE_INT);
			addSymbolOffset(local_vars, var_name, spOffset);
			spOffset += 4;
			fprintf(fmips, "sw %s, %d($sp)\n", reg_name, index * 4 + getSymbolOffset(local_vars, var_name));
		}
		else {
			fprintf(fmips, "sw %s, %d($gp)\n", reg_name, index * 4 + getSymbolOffset(global_vars, var_name));
		}
	}
	else {
		fprintf(fmips, "sw %s, %d($sp)\n", reg_name, index * 4 + getSymbolOffset(local_vars, var_name));
	}
}

// 输出标头
int beforeAnalyse() {
	fprintf(fmips, ".data\n");
	SymbolItem_t* sLabel;
	for (sLabel = stringPool->head; sLabel != NULL; sLabel = (SymbolItem_t*)sLabel->hh.next) {
		fprintf(fmips, "%s: .asciiz \"%s\"\n", sLabel->key, sLabel->string);
	}
	fprintf(fmips, ".align 2\n");
	fprintf(fmips, "_global: .space %d\n", gpOffset);
	fprintf(fmips, ".align 2\n");
	fprintf(fmips, "_stack: .space 800\n\n");
	fprintf(fmips, ".text\n");
	fprintf(fmips, "la $gp, _global\n");
	fprintf(fmips, "la $sp, _stack\n");
	fprintf(fmips, "jal main\n");
	return 0;
}

// 中间代码分析
int analyseMedi() {
	bool isGlobal = true;
	char item[6][STRSIZE];
	char temp[STRSIZE * 8];
	int itemCount;

	while (!feof(fmedi)) {
		fgets(temp, STRSIZE * 8, fmedi);
		itemCount = sscanf(temp, "%s %s %s %s %s", item[1], item[2], item[3], item[4], item[5]);
		if (!strcmp(item[1], "@var"))									// 变量声明
		{
			if (isGlobal) {
				addSymbol(global_vars, item[3], strcmp(item[2], "int") ? VARIABLE_CHAR : VARIABLE_INT);
				addSymbolOffset(global_vars, item[3], gpOffset);
				gpOffset += 4;
			}
			else {
				addSymbol(local_vars, item[3], strcmp(item[2], "int") ? VARIABLE_CHAR : VARIABLE_INT);
				addSymbolOffset(local_vars, item[3], spOffset);
				spOffset += 4;
			}
		}
		else if (!strcmp(item[1], "@array"))							// 数组声明（未处理）
		{
			if (isGlobal) {
				addSymbol(global_vars, item[3], strcmp(item[2], "int[]") ? ARRAY_CHAR : ARRAY_INT);
				addSymbolIndex(global_vars, item[3], atoi(item[4]));
				addSymbolOffset(global_vars, item[3], gpOffset);
				gpOffset += 4 * atoi(item[4]);
			}
			else {
				addSymbol(local_vars, item[3], strcmp(item[2], "int[]") ? ARRAY_CHAR : ARRAY_INT);
				addSymbolIndex(global_vars, item[3], atoi(item[4]));
				addSymbolOffset(local_vars, item[3], spOffset);
				spOffset += 4 * atoi(item[4]);
			}
		}
		else if (!strcmp(item[1], "@exit"))								// 中止（未处理）
		{
			fprintf(fmips, "li $v0, 10\n");
			fprintf(fmips, "syscall\n\n");
		}
		else if (!strcmp(item[1], "@func"))								// 函数声明（未处理）
		{
			spOffset = 4;
			fprintf(fmips, "%s:\n", item[2]);
		}
		else if (!strcmp(item[1], "@para"))								// 函数变量声明（未处理）
		{
			
		}
		else if (!strcmp(item[1], "@call"))								// 函数调用（未处理）
		{
			if (isGlobal) { isGlobal = 1; beforeAnalyse(); }

			fprintf(fmips, "jal %s\n", item[2]);
		}
		else if (!strcmp(item[1], "@ret"))								// 函数返回（未处理）
		{
			
			fprintf(fmips, "jr $ra\n");
		}
		else if (!strcmp(item[1], "@push"))								// 函数传参（未处理）
		{

		}
		else if (!strcmp(item[1], "@get"))								// 接收返回值（未处理）
		{

		}
		else if (!strcmp(item[1], "@beqz"))								// 等于0跳转（处理）
		{
			loadReg("$t0", item[2], 0);
			fprintf(fmips, "beqz $t0 %s\n", item[3]);
		}
		else if (!strcmp(item[1], "@bnez"))								// 不等于0跳转（处理）
		{
			loadReg("$t0", item[2], 0);
			fprintf(fmips, "bnez $t0 %s\n", item[3]);
		}
		else if (!strcmp(item[1], "@beq"))								// 相等时跳转（未处理）
		{
			
		}
		else if (!strcmp(item[1], "@j"))								// 无条件跳转（处理）
		{
			fprintf(fmips, "j %s\n", item[2]);
		}
		else if (!strcmp(item[1], "@jal"))								// 无条件跳转并链接（未处理）
		{
			
		}
		else if (!strcmp(item[1], "@printf"))							// 输出
		{
			if (!strcmp(item[2], "int")) {
				fprintf(fmips, "li $v0, 1\n");
				loadReg("$a0", item[3], 0);
				fprintf(fmips, "syscall\n");
			}
			else if (!strcmp(item[2], "char")) {
				fprintf(fmips, "li $v0, 11\n");
				loadReg("$a0", item[3], 0);
				fprintf(fmips, "syscall\n");
			}
			else if (!strcmp(item[2], "string")) {
				fprintf(fmips, "li $v0, 4\n");
				fprintf(fmips, "la $a0, %s\n", item[4]);
				fprintf(fmips, "syscall\n");
			}
			else {
				printf("Medi_print error!");
			}
		}
		else if (!strcmp(item[1], "@newline"))							// 换行
		{
			fprintf(fmips, "li $v0, 11\n");
			fprintf(fmips, "li $a0, 10\n");
			fprintf(fmips, "syscall\n");
		}
		else if (!strcmp(item[1], "@scanf"))							// 输入
		{
			if (!strcmp(item[2], "int")) {
				fprintf(fmips, "li $v0, 5\n");
				fprintf(fmips, "syscall\n");
				saveReg("$v0", item[3], 0);
			}
			else if (!strcmp(item[2], "char")) {
				fprintf(fmips, "li $v0, 12\n");
				fprintf(fmips, "syscall\n");
				saveReg("$v0", item[3], 0);
			}
		}
		else if (!strcmp(item[2], ":"))									// 标签
		{
			fprintf(fmips, "%s:\n", item[1]);
		}
		else if (!strcmp(item[2], "="))									// 赋值或运算语句（未处理 数组）
		{
			if (itemCount == 3) {
				loadReg("$t1", item[3], 0);
				fprintf(fmips, "move $t0, $t1\n");
				saveReg("$t0", item[1], 0);
			}
			else if (itemCount == 5) {
				if (!strcmp(item[4], "ARGET")) {
					loadReg("$t0", item[3], atoi(item[5]));
					saveReg("$t0", item[1], 0);
				}
				else if (!strcmp(item[4], "ARSET")) {
					loadReg("$t0", item[5], 0);
					saveReg("$t0", item[1], atoi(item[3]));
				}
				else {
					loadReg("$t1", item[3], 0);
					loadReg("$t2", item[5], 0);
					if (!strcmp(item[4], "PLUS")) { fprintf(fmips, "add $t0, $t1, $t2\n"); }
					else if (!strcmp(item[4], "MINU")) { fprintf(fmips, "sub $t0, $t1, $t2\n"); }
					else if (!strcmp(item[4], "MULT")) { fprintf(fmips, "mult $t1, $t2\nmflo $t0\n"); }
					else if (!strcmp(item[4], "DIV")) { fprintf(fmips, "div $t1, $t2\nmflo $t0\n"); }
					else if (!strcmp(item[4], "LSS")) { fprintf(fmips, "slt $t0, $t1, $t2\n"); }
					else if (!strcmp(item[4], "LEQ")) { fprintf(fmips, "sle $t0, $t1, $t2\n"); }
					else if (!strcmp(item[4], "GRE")) { fprintf(fmips, "sgt $t0, $t1, $t2\n"); }
					else if (!strcmp(item[4], "GEQ")) { fprintf(fmips, "sge $t0, $t1, $t2\n"); }
					else if (!strcmp(item[4], "EQL")) { fprintf(fmips, "seq $t0, $t1, $t2\n"); }
					else if (!strcmp(item[4], "NEQ")) { fprintf(fmips, "sne $t0, $t1, $t2\n"); }
					else { printf("Analyse medi_calculation fault!"); }
					saveReg("$t0", item[1], 0);
				}
			}
			else {
				printf("Analyse medi_calculation fault!");
			}
		}
	}
	fprintf(fmips, "li $v0, 10\n");
	fprintf(fmips, "syscall\n");
	return 0;
}
