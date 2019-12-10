#include "code_optimize.h"



int optimizeMedi_1() {
	FILE* moFileIn = fopen("medi_beta.txt", "rb");
	FILE* moFileOut = fopen("medi_1.txt", "w");

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

int optimizeMedi_2() {
	FILE* moFileIn = fopen("medi_1.txt", "rb");
	FILE* moFileOut = fopen("medi.txt", "w");
	
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
			if (!strcmp(item[1], "@func")) {
				pool = new_MediHashTable(item[2], 0);
			}
			else if (!strcmp(item[1], "@scanf")) {
				MediHashItem_t* tempI = get_ItemFromTable(pool, item[3]);
				if (tempI != NULL) { delete_MediHashItem(pool, item[3]); }
			}
			else if (itemCount == 5 && !strcmp(item[2], "=") && item[1][0] == '#') {
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
	optimizeMedi_1();
	optimizeMedi_2();
	return 0;
}

// 优化汇编代码
int optimizeMips() {
	FILE* mipsFileIn = fopen("mips_beta.txt", "rb");
	FILE* mipsFileOut = fopen("mips.txt", "w");

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
			// temp[strlen(temp) - 1] = '\0';
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