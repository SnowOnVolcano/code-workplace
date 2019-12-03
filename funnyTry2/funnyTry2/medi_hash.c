#include "medi_hash.h"

MediHashGrandpa_t* new_MediHashGrandpa() {
	MediHashGrandpa_t* newone;
	newone = (MediHashGrandpa_t*)malloc(sizeof(MediHashGrandpa_t));
	newone->count = 0;
	newone->head = NULL;
	return newone;
}

MediHashTable_t* new_MediHashTable(char* name, int type) {
	MediHashTable_t* newone;
	newone = (MediHashTable_t*)malloc(sizeof(MediHashTable_t));
	strcpy(newone->name, name);
	newone->type = type;
	newone->count = 0;
	newone->list = (List_t*)malloc(sizeof(List_t));
	newone->head = NULL;
	return newone;
}

MediHashItem_t* new_MediHashItem(char* name, int type) {
	MediHashItem_t* newone;
	newone = (MediHashItem_t*)malloc(sizeof(MediHashItem_t));
	strcpy(newone->name, name);
	newone->type = type;
	newone->offset = 0;
	return newone;
}


int add_NewHashTable(MediHashGrandpa_t* grandPa, char* table_name, int type) {
	MediHashTable_t* table;
	HASH_FIND_STR(grandPa->head, table_name, table);
	if (table != NULL) { return -1; }
	else { 
		table = new_MediHashTable(table_name, type);
		HASH_ADD_STR(grandPa->head, name, table);
		if (grandPa->count == 0) { grandPa->head = table; }
		grandPa->count++;
	}
	return 0;
}

int add_NewHashItem(MediHashTable_t* table, char* item_name, int type) {
	MediHashItem_t* item;
	HASH_FIND_STR(table->head, item_name, item);
	if (item != NULL) { return -1; }
	else {
		item = new_MediHashItem(item_name, type);
		HASH_ADD_STR(table->head, name, item);
		if (table->count == 0) { table->head = item; }
		table->count++;
	}
	return 0;
}


int add_paraToOldTable(MediHashTable_t* table, char* para_name, int offset) {
	return appendListTailWithName(table->list, para_name, offset);
}


int add_typeToOldItem(MediHashTable_t* table, char* item_name, int type) {
	MediHashItem_t* item = get_ItemFromTable(table, item_name);
	if (item == NULL) { return -1; }
	else { item->type = type; }
	return 0;
}

int add_offsetToOldItem(MediHashTable_t* table, char* item_name, int offset) {
	MediHashItem_t* item = get_ItemFromTable(table, item_name);
	if (item == NULL) { return -1; }
	else { item->offset = offset; }
	return 0;
}

MediHashTable_t* get_TableFromGrandpa(MediHashGrandpa_t* grandPa, char* table_name) {
	MediHashTable_t* table;
	HASH_FIND_STR(grandPa->head, table_name, table);
	if (table == NULL) { printf("ERROR: Don't have that HashTable.\n"); }
	return table;
}

MediHashItem_t* get_ItemFromTable(MediHashTable_t* table, char* item_name) {
	MediHashItem_t* item;
	HASH_FIND_STR(table->head, item_name, item);
	if (item == NULL) { printf("ERROR: Don't have that HashItem.\n"); }
	return item;
}
