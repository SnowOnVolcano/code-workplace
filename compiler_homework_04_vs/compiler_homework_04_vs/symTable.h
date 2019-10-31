#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include "head.h"
#include "uthash-master/uthash.h"

// ���ű�
enum typesOfItemsInSymbolTable {
	ST_HAS_RETURN_FUNC		= 0b10,
	ST_HAS_RETURN_FUNC_INT	= 0b10,
	ST_HAS_RETURN_FUNC_CHAR = 0b11,
	ST_NON_RETURN_FUNC		= 0b1000,
	ST_VARIABLE_IDEN		= 0b100000,
	ST_VARIABLE_IDEN_INT	= 0b100000,
	ST_VARIABLE_IDEN_CHAR	= 0b110000,
	ST_CONST_IDEN			= 0b10000000,
	ST_CONST_IDEN_INT		= 0b10000000,
	ST_CONST_IDEN_CHAR		= 0b11000000,
	ST_ARRAY_IDEN			= 0b1000000000,
	ST_ARRAY_IDEN_INT		= 0b1000000000,
	ST_ARRAY_IDEN_CHAR		= 0b1100000000,
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
extern void add_sym(char* key, int type);

// ���ָ����ʶ��������
extern void change_symType(char* key, int newType);

// ���ָ����ʶ����ֵ�б�
extern void add_symValueList(char* key, struct list* valueList);

extern void delete_sym(char* key);

#endif // !_SYMTABLE_H_