#include "medi_hash.h"

int main() {
	MediHashGrandpa_t* grandPa = new_MediHashGrandpa();
	
	add_NewHashTable(grandPa, "t1", 1);
	add_NewHashTable(grandPa, "t2", 2);
	add_NewHashTable(grandPa, "t3", 3);
	
	add_NewHashItem(get_TableFromGrandpa(grandPa, "t1"), "i1", 1);
	add_NewHashItem(get_TableFromGrandpa(grandPa, "t1"), "i2", 2);
	add_NewHashItem(get_TableFromGrandpa(grandPa, "t1"), "i3", 3);
	add_NewHashItem(get_TableFromGrandpa(grandPa, "t1"), "i4", 4);

	add_NewHashItem(get_TableFromGrandpa(grandPa, "t2"), "i1", 5);
	add_NewHashItem(get_TableFromGrandpa(grandPa, "t2"), "i2", 6);
	add_NewHashItem(get_TableFromGrandpa(grandPa, "t2"), "i3", 7);
	add_NewHashItem(get_TableFromGrandpa(grandPa, "t2"), "i4", 8);

	add_NewHashItem(get_TableFromGrandpa(grandPa, "t3"), "i1", 9);
	add_NewHashItem(get_TableFromGrandpa(grandPa, "t3"), "i2", 10);
	add_NewHashItem(get_TableFromGrandpa(grandPa, "t3"), "i3", 11);
	add_NewHashItem(get_TableFromGrandpa(grandPa, "t3"), "i4", 12);


	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t1"), "i1", 1);
	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t1"), "i2", 2);
	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t1"), "i3", 3);
	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t1"), "i4", 4);

	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t2"), "i1", 5);
	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t2"), "i2", 6);
	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t2"), "i3", 7);
	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t2"), "i4", 8);

	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t3"), "i1", 9);
	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t3"), "i2", 10);
	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t3"), "i3", 11);
	add_offsetToOldItem(get_TableFromGrandpa(grandPa, "t3"), "i4", 12);

	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t1"), "i1")->offset);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t1"), "i2")->offset);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t1"), "i3")->offset);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t1"), "i4")->offset);

	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t2"), "i1")->offset);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t2"), "i2")->offset);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t2"), "i3")->offset);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t2"), "i4")->offset);

	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t3"), "i1")->offset);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t3"), "i2")->offset);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t3"), "i3")->offset);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t3"), "i4")->offset);


	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t1"), "i1")->type);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t1"), "i2")->type);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t1"), "i3")->type);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t1"), "i4")->type);

	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t2"), "i1")->type);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t2"), "i2")->type);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t2"), "i3")->type);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t2"), "i4")->type);

	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t3"), "i1")->type);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t3"), "i2")->type);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t3"), "i3")->type);
	printf("%d\n", get_ItemFromTable(get_TableFromGrandpa(grandPa, "t3"), "i4")->type);
}