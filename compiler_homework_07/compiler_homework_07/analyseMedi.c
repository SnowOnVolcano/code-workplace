#include "analyseMedi.h"
static char curFuncName[STRSIZE];

// 初始化中间代码分析器
int init_analyseMedi() {
	if ((fmedi = fopen("medi.txt", "rb")) < 0) {
		printf("Cannot open the file!\n");
		exit(0);
	}
	fmips = fopen("mips_beta.txt", "w");
	global_pool = new_MediHashTable("#GlobalVars#", 0);
	func_pool = new_MediHashGrandpa();
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
		MediHashTable_t* func = get_TableFromGrandpa(func_pool, curFuncName);
		if (get_ItemFromTable(func, var_name) == NULL) {
			fprintf(fmips, "lw %s, %d($gp)\n", reg_name, index * 4 + get_ItemFromTable(global_pool, var_name)->offset);
		}
		else {
			fprintf(fmips, "lw %s, -%d($sp)\n", reg_name, 4 + index * 4 + get_ItemFromTable(func, var_name)->offset);
		}
	}
}

void saveReg(char* reg_name, char* var_name, int index) {
	MediHashTable_t* func = get_TableFromGrandpa(func_pool, curFuncName);
	if (get_ItemFromTable(func, var_name) == NULL) {
		if (get_ItemFromTable(global_pool, var_name) == NULL) {
			add_NewHashItem(func, var_name, VARIABLE_INT);
			add_offsetToOldItem(func, var_name, spOffset);
			spOffset += 4;
			fprintf(fmips, "sw %s, -%d($sp)\n", reg_name, 4 + index * 4 + get_ItemFromTable(func, var_name)->offset);
		}
		else {
			fprintf(fmips, "sw %s, %d($gp)\n", reg_name, index * 4 + get_ItemFromTable(global_pool, var_name)->offset);
		}
	}
	else {
		fprintf(fmips, "sw %s, -%d($sp)\n", reg_name, 4 + index * 4 + get_ItemFromTable(func, var_name)->offset);
	}
}

int getItemOffsetByName(char* item_name) {
	MediHashTable_t* tempF = get_TableFromGrandpa(func_pool, curFuncName);
	MediHashItem_t* tempI1 = get_ItemFromTable(tempF, item_name);
	MediHashItem_t* tempI2 = get_ItemFromTable(global_pool, item_name);
	if (tempI1 == NULL) {
		if (tempI2 == NULL) {
			add_NewHashItem(tempF, item_name, VARIABLE_INT);
			add_offsetToOldItem(tempF, item_name, spOffset);
			spOffset += 4;
			return get_ItemFromTable(tempF, item_name)->offset;
		}
		else { return tempI2->offset; }
	}
	else { return tempI1->offset; }
}

int getFuncIndexByName(char* table_name) {
	for (int i = 0; i < func_pool->count; i++) {
		if (!strcmp(func_pool->tableNames[i], table_name)) { return i; }
	}
	printf("No such Func in func_pool!");
	return -1;
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
	fprintf(fmips, "_stack: .space 200\n\n");
	fprintf(fmips, ".text\n");
	fprintf(fmips, "la $gp, _global\n");
	fprintf(fmips, "la $fp, _stack\n");
	return 0;
}

// 中间代码分析
int analyseMedi() {
	bool isGlobal = true;
	char item[6][STRSIZE];
	char temp[STRSIZE * 8];
	int itemCount;
	

	while (!feof(fmedi)) {
		memset(temp, 0, STRSIZE * 8);
		memset(item, 0, 6 * STRSIZE);
		fgets(temp, STRSIZE * 8, fmedi);
		itemCount = sscanf(temp, "%s %s %s %s %s", item[1], item[2], item[3], item[4], item[5]);
		if (!strcmp(item[1], "@var"))									// 变量声明
		{
			if (isGlobal) {
				add_NewHashItem(global_pool, item[3], strcmp(item[2], "int") ? VARIABLE_CHAR : VARIABLE_INT);
				add_offsetToOldItem(global_pool, item[3], gpOffset);
				gpOffset += 4;
			}
			else {
				MediHashTable_t* func = get_TableFromGrandpa(func_pool, curFuncName);
				add_NewHashItem(func, item[3], strcmp(item[2], "int") ? VARIABLE_CHAR : VARIABLE_INT);
				add_offsetToOldItem(func, item[3], spOffset);
				spOffset += 4;
			}
		}
		else if (!strcmp(item[1], "@array"))							// 数组声明（未处理）
		{
			if (isGlobal) {
				add_NewHashItem(global_pool, item[3], strcmp(item[2], "int[]") ? ARRAY_CHAR : ARRAY_INT);
				add_offsetToOldItem(global_pool, item[3], gpOffset);
				gpOffset += 4 * atoi(item[4]);
			}
			else {
				MediHashTable_t* func = get_TableFromGrandpa(func_pool, curFuncName);
				add_NewHashItem(func, item[3], strcmp(item[2], "int[]") ? ARRAY_CHAR : ARRAY_INT);
				add_offsetToOldItem(func, item[3], spOffset);
				spOffset += 4 * atoi(item[4]);
			}
		}
		else if (!strcmp(item[1], "@exit"))								// 中止（处理）
		{
			fprintf(fmips, "li $v0, 10\n");
			fprintf(fmips, "syscall\n\n");
		}
		else if (!strcmp(item[1], "@func"))								// 函数声明（未处理）
		{
			strcpy(curFuncName, item[2]);
			add_NewHashTable(func_pool, item[2], 0);

			fprintf(fmips, "\n");
			fprintf(fmips, "%s:\n", item[2]);
			spOffset = 4 * 2;

			List_t* tempL = getSymbolParaList(symbolTables[0], item[2]);
			fprintf(fmips, "addi $fp, $fp, %d\n", -4 * (tempL == NULL ? 0 : tempL->n));
		}
		else if (!strcmp(item[1], "@para"))								// 函数参数声明（处理）
		{
			add_NewHashItem(get_TableFromGrandpa(func_pool, curFuncName), item[3], strcmp(item[2], "int") ? VARIABLE_CHAR : VARIABLE_INT);
			add_offsetToOldItem(get_TableFromGrandpa(func_pool, curFuncName), item[3], spOffset);
			add_paraToOldTable(get_TableFromGrandpa(func_pool, curFuncName), item[3], spOffset);
			spOffset += 4;
			
			// fprintf(fmips, "subi $fp, $fp, 4\n");
			fprintf(fmips, "lw $t0, %d($fp)\n", spOffset - 12);
			saveReg("$t0", item[3], 0);
		}
		else if (!strcmp(item[1], "@call"))								// 函数调用（未处理）
		{
			if (isGlobal) { isGlobal = false; beforeAnalyse(); }
			
			fprintf(fmips, "sw $ra, -4($sp)\n");
			fprintf(fmips, "move $t0, $sp\n");
			fprintf(fmips, "addi $sp, $sp, %d\n", -spOffset);
			fprintf(fmips, "sw $t0, -8($sp)\n");

			fprintf(fmips, "jal %s\n", item[2]);

			fprintf(fmips, "lw $sp, -8($sp)\n");
			fprintf(fmips, "lw $ra, -4($sp)\n");
		}
		else if (!strcmp(item[1], "@ret"))								// 函数返回（未处理）
		{
			if (itemCount > 1) {
				loadReg("$v0", item[2], 0);
			}
			
			fprintf(fmips, "jr $ra\n");
		}
		else if (!strcmp(item[1], "@push"))								// 函数传参（未处理）
		{
			loadReg("$t0", item[2], 0);
			fprintf(fmips, "sw $t0, 0($fp)\n");
			fprintf(fmips, "addi $fp, $fp, 4\n");
		}
		else if (!strcmp(item[1], "@get"))								// 接收返回值（未处理）
		{
			saveReg("$v0", item[2], 0);
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
					loadReg("$t1", item[5], 0);
					fprintf(fmips, "sll $t1, $t1, 2\n");
					
					if (get_ItemFromTable(global_pool, item[3]) == NULL) {
						MediHashTable_t* func = get_TableFromGrandpa(func_pool, curFuncName);
						int tempO = 4 + get_ItemFromTable(func, item[3])->offset;
						fprintf(fmips, "li $t2, %d\n", tempO);
						fprintf(fmips, "add $t2, $t1, $t2\n");
						fprintf(fmips, "sub $t2, $sp, $t2\n");
					}
					else {
						int tempO = get_ItemFromTable(global_pool, item[3])->offset;
						fprintf(fmips, "li $t2, %d\n", tempO);
						fprintf(fmips, "add $t2, $t1, $t2\n");
						fprintf(fmips, "add $t2, $gp, $t2\n");
					}

					fprintf(fmips, "lw $t0, 0($t2)\n");
					saveReg("$t0", item[1], 0);
				}
				else if (!strcmp(item[4], "ARSET")) {
					loadReg("$t0", item[5], 0);
					loadReg("$t1", item[3], 0);
					fprintf(fmips, "sll $t1, $t1, 2\n");

					if (get_ItemFromTable(global_pool, item[1]) == NULL) {
						MediHashTable_t* func = get_TableFromGrandpa(func_pool, curFuncName);
						int tempO = 4 + get_ItemFromTable(func, item[1])->offset;
						fprintf(fmips, "li $t2, %d\n", tempO);
						fprintf(fmips, "add $t2, $t1, $t2\n");
						fprintf(fmips, "sub $t2, $sp, $t2\n");
					}
					else { 
						int tempO = get_ItemFromTable(global_pool, item[1])->offset;
						fprintf(fmips, "li $t2, %d\n", tempO);
						fprintf(fmips, "add $t2, $t1, $t2\n");
						fprintf(fmips, "add $t2, $gp, $t2\n");
					}

					fprintf(fmips, "sw $t0, 0($t2)\n");
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
	return 0;
}
