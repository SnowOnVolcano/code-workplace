#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include "head.h"
#include "uthash-master/uthash.h"

// ���ű�
enum typesOfItemsInSymbolTable {
	FUNC_HAS_RETURN_INT,
	FUNC_HAS_RETURN_CHAR,
	FUNC_NON_RETURN,
	VARIABLE_INT,
	VARIABLE_CHAR,
	CONST_INT,
	CONST_CHAR,
	ARRAY_INT,
	ARRAY_CHAR
};

struct SymbolTable
{
	char key[64];
	int type;
	struct list* valueList;
	UT_hash_handle hh;
};

extern struct SymbolTable* symtabler;

extern void init_symtable();

extern struct SymbolTable* find_sym(char* key);

// ��ȡ��ʶ��������
extern int get_symType(char* key);

// ��ȡ��ʶ����ֵ�б�
extern struct list* get_symValueList(char* key);

// ����µı�ʶ��
extern int add_sym(char* key, int type);

// ���ָ����ʶ��������
extern void change_symType(char* key, int newType);

// ɾ����ʶ����
extern void delete_sym(char* key);

#endif // !_SYMTABLE_H_