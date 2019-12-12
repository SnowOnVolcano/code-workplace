#pragma once

#include "linkList.h"
#include "uthash/uthash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define KEYSIZE 0x80

typedef struct mediHashItem
{
	char name[KEYSIZE];
	char pal[KEYSIZE];
	int type;
	int offset;

	UT_hash_handle hh;
} MediHashItem_t;

typedef struct mediHashTable
{
	char name[KEYSIZE];
	int type;
	int count;
	List_t* list;
	struct mediHashItem* head;

	UT_hash_handle hh;
} MediHashTable_t;

typedef struct mediHashGrandpa
{
	int count;
	char tableNames[KEYSIZE][KEYSIZE];
	struct mediHashTable* head;
} MediHashGrandpa_t;


extern MediHashGrandpa_t* new_MediHashGrandpa();
extern MediHashTable_t* new_MediHashTable(char* name, int type);
extern MediHashItem_t* new_MediHashItem(char* name, int type);

extern int add_NewHashTable(MediHashGrandpa_t* grandPa, char* table_name, int type);
extern int add_NewHashItem(MediHashTable_t* table, char* item_name, int type);

extern int add_paraToOldTable(MediHashTable_t* table, char* para_name, int offset);

extern int add_typeToOldItem(MediHashTable_t* table, char* item_name, int type);
extern int add_offsetToOldItem(MediHashTable_t* table, char* item_name, int offset);
extern int add_palToOldItem(MediHashTable_t* table, char* item_name, char* item_pal);

extern MediHashTable_t* get_TableFromGrandpa(MediHashGrandpa_t* grandPa, char* table_name);
extern MediHashItem_t* get_ItemFromTable(MediHashTable_t* table, char* item_name);

extern int delete_MediHashItem(MediHashTable_t* table, char* item_name);






