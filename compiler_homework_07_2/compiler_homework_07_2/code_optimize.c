#include "code_optimize.h"

int coisInteger(char str[]) {
	return (str[0] == '-' || str[0] == '+' || isdigit(str[0]));
}

int coisGlobalVar(char* var_name) {
	int r = getSymbolType(symbolTables[0], var_name);
	return r != -1;
}

int coisLocalVar(char* func_name, char* var_name) {
	int i = getSymbolTableIndexByName(func_name);
	int r = getSymbolType(symbolTables[i], var_name);
	return r >= 0;
}


// （优化中间代码） #1 = #2, #3 = #2	--->  #3 = #1
int optimizeMedi_1(char* fileIn, char* fileOut) {
	FILE* moFileIn = fopen(fileIn, "rb");
	FILE* moFileOut = fopen(fileOut, "w");

	char item[6][STRSIZE];
	char temp[STRSIZE * 8];
	int itemCount = 0;

	char nextItem[6][STRSIZE];
	char nextTemp[STRSIZE * 8];
	int nextItemCount = 0;

	memset(temp, 0, STRSIZE * 8);
	memset(item, 0, 6 * STRSIZE);
	fgets(temp, STRSIZE * 8, moFileIn);
	itemCount = sscanf(temp, "%s %s %s %s %s", item[1], item[2], item[3], item[4], item[5]);

	while (!feof(moFileIn)) {
		memset(nextTemp, 0, STRSIZE * 8);
		memset(nextItem, 0, 6 * STRSIZE);
		fgets(nextTemp, STRSIZE * 8, moFileIn);
		nextItemCount = sscanf(nextTemp, "%s %s %s %s %s", nextItem[1], nextItem[2], nextItem[3], nextItem[4], nextItem[5]);
		if (itemCount == 3 && nextItemCount == 3
			&& !strcmp(item[2], "=") && !strcmp(nextItem[2], "=")
			&& !strcmp(item[1], nextItem[3])
			&& item[1][0] == '#') {
			strcpy(nextItem[3], item[3]);
		}
		else {
			fprintf(moFileOut, "%s %s %s %s %s\n", item[1], item[2], item[3], item[4], item[5]);
		}
		for (int i = 0; i < 6; i++) { strcpy(item[i], nextItem[i]); }
		itemCount = nextItemCount;
		strcpy(temp, nextTemp);
	}

	if (nextItemCount) { fprintf(moFileOut, "%s %s %s %s %s\n", nextItem[1], nextItem[2], nextItem[3], nextItem[4], nextItem[5]); }
	else { fprintf(moFileOut, "%s %s %s %s %s\n", item[1], item[2], item[3], item[4], item[5]); }

	fclose(moFileIn);
	fclose(moFileOut);
	return 0;
}

// （优化中间代码）函数内联
int optimizeMedi_2(char* fileIn, char* fileOut) {
	FILE* moFileIn = fopen(fileIn, "rb");
	FILE* moFileOut = fopen(fileOut, "wb");

	char temp[STRSIZE * 8];
	char item[6][STRSIZE];
	int itemCount = 0;

	char viaI[0x100];
	char tempS[0x800];

	List_t* pushStack = newList();

	MediHashGrandpa_t* functions = new_MediHashGrandpa();
	MediHashTable_t* curFunction = NULL;

	while (!feof(moFileIn))
	{
		memset(temp, 0, STRSIZE * 8);
		memset(item, 0, 6 * STRSIZE);
		fgets(temp, STRSIZE * 8, moFileIn);
		itemCount = sscanf(temp, "%s %s %s %s %s", item[1], item[2], item[3], item[4], item[5]);

		if (!strcmp(item[1], "@call"))										// 函数调用
		{
			MediHashTable_t* tempT = get_TableFromGrandpa(functions, item[2]);
			List_t* tempL = (tempT == NULL) ? NULL : tempT->list;

			if (curFunction == NULL) {
				fprintf(moFileOut, temp);
			}
			else if (tempL != NULL) {
				curFunction->type = -1;

				Node_t* tempN1 = getListTopNode(tempL);
				Node_t* tempN2 = getListTailNode(pushStack);
				
				int paraCount = 0;
				for (; paraCount < tempL->n; paraCount++) {
					if (tempN1->name[0] != '@' || tempN1->name[1] != 'p' || tempN1->name[2] != 'a') { break; }
					else { tempN1 = getListNextNode(tempN1); }
				}
				tempN1 = getListTopNode(tempL);

				// 指针指向被调用函数的第一个参数
				for (int i = 0; i < paraCount - 1; i++) { tempN2 = getListPrevNode(tempN2); }
					
				if (tempT->type != -1) {// 如果该函数为内联函数，输出 tempL ，并对每一个参数进行赋值
					char tempPara[4][STRSIZE];
					for (int i = 0; i < paraCount; i++) {
						sscanf(tempN1->name, "%s %s %s", tempPara[1], tempPara[2], tempPara[3]);
						//fprintf(moFileOut, "@var %s %s\n", tempPara[2], tempPara[3]);
						fprintf(moFileOut, "%s = %s\n", tempPara[3], tempN2->name);
						tempN1 = getListNextNode(tempN1);
						tempN2 = getListNextNode(tempN2);
					}
					for (int i = paraCount; i < tempL->n; i++) {
						fprintf(moFileOut, "%s\n", tempN1->name);
						tempN1 = getListNextNode(tempN1);
					}
				}
				else {					// 如果改函数不是内联函数，则弹出pushStack中指定数目的参数，@CALL
					for (int i = 0; i < paraCount; i++) {
						fprintf(moFileOut, "@push %s\n", tempN2->name);
						tempN2 = getListNextNode(tempN2);
					}
					fprintf(moFileOut, temp);
				}
				for (int i = 0; i < paraCount; i++) { deleteListTail(pushStack); }
			}
		}
		else if (!strcmp(item[1], "@push"))									// 函数传参
		{
			appendListTailWithName(pushStack, item[2], 0);
		}
		else {
			if (!strcmp(item[1], "@var") && curFunction != NULL)			// 变量声明
			{
				sprintf(tempS, "%s %s #%s", item[1], item[2], item[3]);
				add_paraToOldTable(curFunction, tempS, 0);
			}
			else if (!strcmp(item[1], "@array") && curFunction != NULL)		// 数组声明
			{
				curFunction->type = -1;
			}
			else if (!strcmp(item[1], "@exit"))								// 中止
			{
			}
			else if (!strcmp(item[1], "@func"))								// 函数声明
			{
				add_NewHashTable(functions, item[2], 0);
				curFunction = get_TableFromGrandpa(functions, item[2]);
			}
			else if (!strcmp(item[1], "@para"))								// 函数参数声明
			{
				sprintf(tempS, "%s %s #%s", item[1], item[2], item[3]);
				add_paraToOldTable(curFunction, tempS, 0);
			}
			else if (!strcmp(item[1], "@ret"))								// 函数返回
			{
				if (itemCount > 1) {
					if (coisInteger(item[2])) { 
						sprintf(tempS, "@halfret %s", item[2]);
						add_paraToOldTable(curFunction, tempS, 0);
					}
					else if (coisGlobalVar(item[2])) { curFunction->type = -1; }
					else {
						//sprintf(tempS, "%s #%s", item[1], item[2]);
						sprintf(tempS, "@halfret #%s", item[2]);
						add_paraToOldTable(curFunction, tempS, 0);
					}
				}
			}
			else if (!strcmp(item[1], "@get"))								// 接收返回值
			{
			}
			else if (!strcmp(item[1], "@beqz") || !strcmp(item[1], "@bnez"))// 等于0跳转 || 不等于0跳转
			{
				curFunction->type = -1;		// 暂时不处理跳转
		
				if (coisInteger(item[2])) { 
					sprintf(tempS, "%s %s %s", item[1], item[2], item[3]);
					add_paraToOldTable(curFunction, tempS, 0);
				}
				else if (coisGlobalVar(item[2])) { curFunction->type = -1; }
				else {
					sprintf(tempS, "%s #%s %s", item[1], item[2], item[3]);
					add_paraToOldTable(curFunction, tempS, 0);
				}
			}
			else if (!strcmp(item[1], "@beq"))								// 相等时跳转
			{
				curFunction->type = -1;		// 暂时不处理跳转

				if (coisGlobalVar(item[2]) || coisGlobalVar(item[3])) { curFunction->type = -1; }
				else {
					if (!coisInteger(item[2])) { sprintf(viaI, "#%s", item[2]); strcpy(item[2], viaI); }
					if (!coisInteger(item[3])) { sprintf(viaI, "#%s", item[3]); strcpy(item[3], viaI); }
					sprintf(tempS, "%s %s %s %s", item[1], item[2], item[3], item[4]);
					add_paraToOldTable(curFunction, tempS, 0);
				}
			}
			else if (!strcmp(item[1], "@j"))								// 无条件跳转
			{
				curFunction->type = -1;		// 暂时不处理跳转
				
				sprintf(tempS, "%s %s", item[1], item[2]);
				add_paraToOldTable(curFunction, tempS, 0);
			}
			else if (!strcmp(item[1], "@jal"))								// 无条件跳转并链接
			{

			}
			else if (!strcmp(item[1], "@printf"))							// 输出
			{
				if (!strcmp(item[2], "string")) { 
					sprintf(tempS, "%s %s %s %s", item[1], item[2], item[3], item[4]);
					add_paraToOldTable(curFunction, tempS, 0);
				}
				else if (!strcmp(item[2], "int") || !strcmp(item[2], "char")) {
					if (coisGlobalVar(item[3])) { curFunction->type = -1; }
					else if (!coisInteger(item[3])) {
						sprintf(tempS, "%s %s #%s", item[1], item[2], item[3]);
						add_paraToOldTable(curFunction, tempS, 0);
					}
				}
			}
			else if (!strcmp(item[1], "@newline"))							// 换行
			{
				add_paraToOldTable(curFunction, item[1], 0);
			}
			else if (!strcmp(item[1], "@scanf"))							// 输入
			{
				if (coisGlobalVar(item[3])) { curFunction->type = -1; }
				else if (!coisInteger(item[3])) {
					sprintf(tempS, "%s %s #%s", item[1], item[2], item[3]);
					add_paraToOldTable(curFunction, tempS, 0);
				}
			}
			else if (!strcmp(item[2], ":"))									// 标签
			{
				curFunction->type = -1;		// 暂时不处理跳转
				
				sprintf(tempS, "%s %s", item[1], item[2]);
				add_paraToOldTable(curFunction, tempS, 0);
			}
			else if (!strcmp(item[2], "="))									// 赋值或运算语句
			{
				if (itemCount == 3) {
					if (coisGlobalVar(item[1]) || coisGlobalVar(item[3])) { curFunction->type = -1; }
					else {
						if (!coisInteger(item[1])) { sprintf(viaI, "#%s", item[1]); strcpy(item[1], viaI); }
						if (!coisInteger(item[3])) { sprintf(viaI, "#%s", item[3]); strcpy(item[3], viaI); }
						sprintf(tempS, "%s %s %s", item[1], item[2], item[3]);
						add_paraToOldTable(curFunction, tempS, 0);
					}
				}
				else {
					if (!strcmp(item[4], "ARGET") || !strcmp(item[4], "ARSET")) { curFunction->type = -1; }
					else {
						if (coisGlobalVar(item[1]) || coisGlobalVar(item[3]) || coisGlobalVar(item[5])) { curFunction->type = -1; }
						else {
							if (!coisInteger(item[1])) { sprintf(viaI, "#%s", item[1]); strcpy(item[1], viaI); }
							if (!coisInteger(item[3])) { sprintf(viaI, "#%s", item[3]); strcpy(item[3], viaI); }
							if (!coisInteger(item[5])) { sprintf(viaI, "#%s", item[5]); strcpy(item[5], viaI); }
							sprintf(tempS, "%s %s %s %s %s", item[1], item[2], item[3], item[4], item[5]);
							add_paraToOldTable(curFunction, tempS, 0);
						}
					}
				}
			}
			fprintf(moFileOut, temp);
		}
	}

	fclose(moFileIn);
	fclose(moFileOut);
}

// （优化中间代码）临时变量传递
int optimizeMedi_3(char* fileIn, char* fileOut) {
	FILE* moFileIn = fopen(fileIn, "rb");
	FILE* moFileOut = fopen(fileOut, "w");

	char item[6][STRSIZE];
	char temp[STRSIZE * 8];
	int itemCount = 0;

	MediHashTable_t* pool = new_MediHashTable("", 0);

	while (!feof(moFileIn))
	{
		memset(temp, 0, STRSIZE * 8);
		memset(item, 0, 6 * STRSIZE);
		fgets(temp, STRSIZE * 8, moFileIn);
		itemCount = sscanf(temp, "%s %s %s %s %s", item[1], item[2], item[3], item[4], item[5]);

		if (itemCount == 3 && !strcmp(item[2], "=") && item[1][0] == '#') {
			MediHashItem_t* tempI1 = get_ItemFromTable(pool, item[1]);
			MediHashItem_t* tempI2 = get_ItemFromTable(pool, item[3]);
			if (tempI1 == NULL) { add_NewHashItem(pool, item[1], 0); }
			if (tempI2 != NULL) { strcpy(item[3], tempI2->pal); }
			add_palToOldItem(pool, item[1], item[3]);
		}
		else if (!strcmp(item[2], ".asciiz")) {
			temp[strlen(temp) - 1] = '\0';
			fprintf(moFileOut, temp);
		}
		else {
			if (!strcmp(item[1], "@func")) {							// 新的函数，重新储存赋值关系
				pool = new_MediHashTable(item[2], 0);
			}
			else if (!strcmp(item[1], "@scanf")) {						// scanf <type> <iden>
				MediHashItem_t* tempI = get_ItemFromTable(pool, item[3]);
				if (tempI != NULL) { delete_MediHashItem(pool, item[3]); }
			}
			else if (itemCount == 5 && !strcmp(item[2], "=") && item[1][0] == '#') {	// <iden> = <iden> <oper> <iden>
				MediHashItem_t* tempI1 = get_ItemFromTable(pool, item[3]);
				MediHashItem_t* tempI2 = get_ItemFromTable(pool, item[5]);
				if (tempI1 != NULL) { strcpy(item[3], tempI1->pal); }
				if (tempI2 != NULL) { strcpy(item[5], tempI2->pal); }

				if (get_ItemFromTable(pool, item[1]) != NULL) { delete_MediHashItem(pool, item[1]); }
			}
			else {
				MediHashItem_t* tempI;
				for (int i = 1; i <= itemCount; i++) {
					if (item[i][0] == '#') {
						tempI = get_ItemFromTable(pool, item[i]);
						if (tempI != NULL) { strcpy(item[i], tempI->pal); }
					}
				}
			}
			fprintf(moFileOut, "%s %s %s %s %s\n", item[1], item[2], item[3], item[4], item[5]);
		}
	}

	fclose(moFileIn);
	fclose(moFileOut);
	return 0;
}

// 优化中间代码
int optimizeMedi() {
	optimizeMedi_1("medi_beta.txt", "medi_1.txt");
	optimizeMedi_2("medi_1.txt", "medi_2.txt");
	optimizeMedi_3("medi_2.txt", "medi.txt");

	return 0;
}


// （优化汇编代码）
int optimizeMips_1(char* fileIn, char* fileOut) {
	FILE* mipsFileIn = fopen(fileIn, "rb");
	FILE* mipsFileOut = fopen(fileOut, "wb");

	char lastItem[6][STRSIZE];
	int lastItemCount = 0;

	char item[6][STRSIZE];
	char temp[STRSIZE * 8];
	int itemCount = 0;

	while (!feof(mipsFileIn))
	{
		memset(temp, 0, STRSIZE * 8);
		memset(item, 0, 6 * STRSIZE);
		fgets(temp, STRSIZE * 8, mipsFileIn);
		itemCount = sscanf(temp, "%s %s %s %s %s", item[1], item[2], item[3], item[4], item[5]);

		// sw $t0, -24($sp)
		// lw $t1, -24($sp)

		if (itemCount == 3 && lastItemCount == 3
			&& !strcmp(item[1], "lw") && !strcmp(lastItem[1], "sw")
			&& !strcmp(item[3], lastItem[3])) {
			fprintf(mipsFileOut, "move %s %c%c%c\n", item[2], lastItem[2][0], lastItem[2][1], lastItem[2][2]);
		}
		else if (!strcmp(item[2], ".asciiz")) {
			fprintf(mipsFileOut, temp);
		}
		else {
			fprintf(mipsFileOut, "%s %s %s %s %s\n", item[1], item[2], item[3], item[4], item[5]);
		}

		for (int i = 0; i < 6; i++) { strcpy(lastItem[i], item[i]); }
		lastItemCount = itemCount;
	}

	fclose(mipsFileIn);
	fclose(mipsFileOut);
	return 0;
}

// （优化汇编代码）	move $t2, $t1 ; move $t3, $t2 ---> move $t3, $t1
int optimizeMips_2(char* fileIn, char* fileOut) {
	FILE* mipsFileIn = fopen(fileIn, "rb");
	FILE* mipsFileOut = fopen(fileOut, "wb");

	char item[6][STRSIZE];
	char temp[STRSIZE * 8];
	int itemCount = 0;

	char nextItem[6][STRSIZE];
	char nextTemp[STRSIZE * 8];
	int nextItemCount = 0;

	memset(temp, 0, STRSIZE * 8);
	memset(item, 0, 6 * STRSIZE);
	fgets(temp, STRSIZE * 8, mipsFileIn);
	itemCount = sscanf(temp, "%s %s %s %s %s", item[1], item[2], item[3], item[4], item[5]);

	while (!feof(mipsFileIn)) {
		memset(nextTemp, 0, STRSIZE * 8);
		memset(nextItem, 0, 6 * STRSIZE);
		fgets(nextTemp, STRSIZE * 8, mipsFileIn);
		nextItemCount = sscanf(nextTemp, "%s %s %s %s %s", nextItem[1], nextItem[2], nextItem[3], nextItem[4], nextItem[5]);
		if (itemCount == 3 && nextItemCount == 3
			&& !strcmp(item[1], "move") && !strcmp(nextItem[1], "move")
			/*&& item[2][1] == 't'*/ && item[2][1] == nextItem[3][1] && item[2][2] == nextItem[3][2]) {
			nextItem[3][1] = item[3][1];
			nextItem[3][2] = item[3][2];
		}
		else {
			if (!strcmp(item[2], ".asciiz")) { fprintf(mipsFileOut, temp); }
			else if (!strcmp(item[1], "move") && item[2][1] == item[3][1] && item[2][2] == item[3][2]) {
			}
			else { fprintf(mipsFileOut, "%s %s %s %s %s\n", item[1], item[2], item[3], item[4], item[5]); }
		}
		for (int i = 0; i < 6; i++) { strcpy(item[i], nextItem[i]); }
		itemCount = nextItemCount;
		strcpy(temp, nextTemp);
	}

	if (nextItemCount) { fprintf(mipsFileOut, "%s %s %s %s %s\n", nextItem[1], nextItem[2], nextItem[3], nextItem[4], nextItem[5]); }
	else { fprintf(mipsFileOut, "%s %s %s %s %s\n", item[1], item[2], item[3], item[4], item[5]); }

	fclose(mipsFileIn);
	fclose(mipsFileOut);
	return 0;
}

// （优化汇编代码）	sw $t0, <int>($sp) ; b $t0, <label> ----> b $t0, <label>
int optimizeMips_3(char* fileIn, char* fileOut) {
	FILE* mipsFileIn = fopen(fileIn, "rb");
	FILE* mipsFileOut = fopen(fileOut, "wb");

	char item[6][STRSIZE];
	char temp[STRSIZE * 8];
	int itemCount = 0;

	char nextItem[6][STRSIZE];
	char nextTemp[STRSIZE * 8];
	int nextItemCount = 0;

	memset(temp, 0, STRSIZE * 8);
	memset(item, 0, 6 * STRSIZE);
	fgets(temp, STRSIZE * 8, mipsFileIn);
	itemCount = sscanf(temp, "%s %s %s %s %s", item[1], item[2], item[3], item[4], item[5]);

	while (!feof(mipsFileIn)) {
		memset(nextTemp, 0, STRSIZE * 8);
		memset(nextItem, 0, 6 * STRSIZE);
		fgets(nextTemp, STRSIZE * 8, mipsFileIn);
		nextItemCount = sscanf(nextTemp, "%s %s %s %s %s", nextItem[1], nextItem[2], nextItem[3], nextItem[4], nextItem[5]);

		if (!strcmp(item[1], "sw") && (!strcmp(nextItem[1], "beqz") || !strcmp(nextItem[1], "bnez"))
			&& !strcmp(item[2], "$t0,") && !strcmp(nextItem[2], "$t0,")) {
		}
		else { fprintf(mipsFileOut, temp); }

		for (int i = 0; i < 6; i++) { strcpy(item[i], nextItem[i]); }
		itemCount = nextItemCount;
		strcpy(temp, nextTemp);
	}

	if (nextItemCount) { fprintf(mipsFileOut, "%s %s %s %s %s\n", nextItem[1], nextItem[2], nextItem[3], nextItem[4], nextItem[5]); }
	else { fprintf(mipsFileOut, "%s %s %s %s %s\n", item[1], item[2], item[3], item[4], item[5]); }

	fclose(mipsFileIn);
	fclose(mipsFileOut);
	return 0;
}

// 优化汇编代码
int optimizeMips() {
	optimizeMips_1("mips_beta.txt", "mips_1.txt");
	optimizeMips_2("mips_1.txt", "mips_2.txt");
	optimizeMips_3("mips_2.txt", "mips.txt");
	return 0;
}