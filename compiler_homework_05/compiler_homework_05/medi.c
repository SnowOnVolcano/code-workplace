#define MIPS_OUTPUT(x) fputs(fMediOut, x)
#include "medi.h"

int branch = 0;
int level;
int temp_count = 0;
bool in_switch = false;

// ��ʼ���м�������ɳ���
int init_medi() {
	fMediOut = fopen("mips_mediate_code.txt", "w");
	return 0;
}

void init_temp();
char* new_temp();

// �½�һ��Label
char* new_label(SymbolItem_t* func_item, char* info) {
	// return func_item->get_name() + "_L_" + int2string(branch++) + "_" + info;
	char str_t[STRSIZE];
	strcpy(str_t, func_item->key);
	strcat(str_t, "_L_");
	strcat(str_t, int2string(branch++));
	strcat(str_t, "_");
	strcat(str_t, info);
	return str_t;
}

// �����˳�
void exit_medi() {
	MIPS_OUTPUT("@exit");
}

// ��������
void declare_func_medi(SymbolItem_t* func_item) {
	// MIPS_OUTPUT("@func " << func_item->get_name());
	char str_t[STRSIZE];
	strcpy(str_t, "@func ");
	strcat(str_t, func_item->key);
	MIPS_OUTPUT(str_t);
}

// ������������
void declare_para_medi(int type, char* name)
{
	// MIPS_OUTPUT("@para " << type2string(type) << " " << name);
	char str_t[STRSIZE];
	strcpy(str_t, "@para ");
	strcat(str_t, type2string(type));
	strcat(str_t, " ");
	strcat(str_t, name);
	MIPS_OUTPUT(str_t);
}

// ��������
void declare_var_medi(SymbolItem_t* var_item)
{
	/*if (var_item->isarray())
	{
		MIPS_OUTPUT("@array " << type2string(var_item->get_type()) << "[] " << var_item->get_name() << " " << var_item->get_len());
	}
	else
	{
		MIPS_OUTPUT("@var " << type2string(var_item->get_type()) << " " << var_item->get_name());
	}*/
	char str_t[STRSIZE];
	if (var_item->type == ARRAY_INT || var_item == ARRAY_CHAR) {
		strcpy(str_t, "@array ");
		(var_item->type == ARRAY_INT) ? strcat(str_t, "int[] ") : strcat(str_t, "char[] ");
		strcat(str_t, var_item->key);
		// strcat(str_s, var_item->array_index)
	}
	else {
		strcpy(str_t, "@var ");
		(var_item->type == VARIABLE_INT) ? strcat(str_t, "int ") : strcat(str_t, "char ");
		strcat(str_t, var_item->key);
	}
	MIPS_OUTPUT(str_t);
}

// ��������
void invoke_func_medi(char* name)
{
	// MIPS_OUTPUT("@call " + name);
	char str_t[STRSIZE];
	strcpy(str_t, "@call ");
	strcat(str_t, name);
	MIPS_OUTPUT(str_t);
}

// �������
void return_medi(char* v)
{
	// MIPS_OUTPUT("@ret " << v);
	char str_t[STRSIZE];
	strcpy(str_t, "@ret ");
	strcat(str_t, v);
	MIPS_OUTPUT(str_t);
}
void return_medi(int v)
{
	// MIPS_OUTPUT("@ret " << v);
	char str_t[STRSIZE];
	strcpy(str_t, "@ret ");
	strcat(str_t, int2string(v));
	MIPS_OUTPUT(str_t);
}
void return_medi(SymbolItem_t* func_item)
{
	/* if (func_item->get_type() == VOID)
	{
		MIPS_OUTPUT("@ret");
	}
	else
	{
		MIPS_OUTPUT("@ret 0");
	}*/

	if (func_item->type == FUNC_NON_RETURN) {
		MIPS_OUTPUT("@ret");
	}
	else {
		MIPS_OUTPUT("@ret 0");
	}
}

// ��ת��־��
void label_medi(char* label)
{
	// MIPS_OUTPUT(label << " :");
	char str_t[STRSIZE];
	strcpy(str_t, label);
	strcat(str_t, " :");
	MIPS_OUTPUT(str_t);
}

// ����ʽ
// cond must not be zero
void cal_medi(int op, char* result, char* a1, char* a2)
{
	// MIPS_OUTPUT(result << " = " << a1 << " " << symbol2string(op) << " " << a2);
	char str_t[STRSIZE];
	strcpy(str_t, result);
	strcat(str_t, " = ");
	strcat(str_t, a1);
	strcat(str_t, " ");
	strcat(str_t, mnemonices[op]);
	strcat(str_t, " ");
	strcat(str_t, a2);
	MIPS_OUTPUT(str_t);
}
void cal_medi(int op, char* result, char* a1, int a2)
{
	/*stringstream ss;
	  ss << a2;
	cal_medi(op, result, a1, ss.str());*/
}
void cal_medi(int op, char* result, int a1, char* a2)
{
	/*stringstream ss;
	ss << a1;
	cal_medi(op, result, ss.str(), a2);*/
}

// ��ֵ���
void assign_medi(char* n1, char* n2)
{
	// MIPS_OUTPUT(n1 << " = " << n2);
	char str_t[STRSIZE];
	strcpy(str_t, n1);
	strcat(str_t, " = ");
	strcat(str_t, n2);
	MIPS_OUTPUT(str_t);
}
void assign_medi(char* name, int value)
{
	// MIPS_OUTPUT(name << " = " << value);
	char str_t[STRSIZE];
	strcpy(str_t, name);
	strcat(str_t, " = ");
	strcat(str_t, int2string(value));
	MIPS_OUTPUT(str_t);
}

// �������
void push_medi(char* name)
{
	// MIPS_OUTPUT("@push " << name);
	char str_t[STRSIZE];
	strcpy(str_t, "@push ");
	strcat(str_t, name);
	MIPS_OUTPUT(str_t);
}
void push_medi(int name)
{
	// MIPS_OUTPUT("@push " << name);
	char str_t[STRSIZE];
	strcpy(str_t, "@push ");
	strcat(str_t, int2string(name));
	MIPS_OUTPUT(str_t);
}

// ���ܷ���ֵ
void return_get_medi(char* name)
{
	// MIPS_OUTPUT("@get " << name);
	char str_t[STRSIZE];
	strcpy(str_t, "@get ");
	strcat(str_t, name);
	MIPS_OUTPUT(str_t);
}

// ���� 0 ʱ��ת
void branch_zero_medi(char* name, char* label)
{
	// MIPS_OUTPUT("@bz " << name << " " << label);
	char str_t[STRSIZE];
	strcpy(str_t, "@bz ");
	strcat(str_t, name);
	strcat(str_t, " ");
	strcat(str_t, label);
	MIPS_OUTPUT(str_t);
}

// ���ʱ��ת 
void branch_equal_medi(char* name, int value, char* label)
{
	// MIPS_OUTPUT("@be " << name << " " << value << " " << label);
	char str_t[STRSIZE];
	strcpy(str_t, "@be ");
	strcat(str_t, name);
	strcat(str_t, " ");
	strcat(str_t, int2string(value));
	strcat(str_t, " ");
	strcat(str_t, label);
	MIPS_OUTPUT(str_t);
}

// ��������ת
void jump_medi(char* label)
{
	// MIPS_OUTPUT("@j " << label);
	char str_t[STRSIZE];
	strcpy(str_t, "@j ");
	strcat(str_t, label);
	MIPS_OUTPUT(str_t);
}

// ��������ת������
void jump_link_medi(char* label)
{
	// MIPS_OUTPUT("@jal " << label);
	char str_t[STRSIZE];
	strcpy(str_t, "@jal ");
	strcat(str_t, label);
	MIPS_OUTPUT(str_t);
}

// ����ȡֵ
void array_get_medi(char* array_name, char* offset, char* result)
{
	// MIPS_OUTPUT(result << " = " << array_name << " ARGET " << offset);
	char str_t[STRSIZE];
	strcpy(str_t, result);
	strcat(str_t, " = ");
	strcat(str_t, array_name);
	strcat(str_t, " ARGET ");
	strcat(str_t, offset);
	MIPS_OUTPUT(str_t);
}
void array_get_medi(char* array_name, int offset, char* result)
{
	// MIPS_OUTPUT(result << " = " << array_name << " ARGET " << offset);
	char str_t[STRSIZE];
	strcpy(str_t, result);
	strcat(str_t, " = ");
	strcat(str_t, array_name);
	strcat(str_t, " ARGET ");
	strcat(str_t, int2string(offset));
	MIPS_OUTPUT(str_t);
}

// ���鸳ֵ
void array_set_medi(char* array_name, char* offset, char* value)
{
	// MIPS_OUTPUT(array_name << " = " << offset << " ARSET " << value);
	char str_t[STRSIZE];
	strcpy(str_t, array_name);
	strcat(str_t, " = ");
	strcat(str_t, offset);
	strcat(str_t, " ARSET ");
	strcat(str_t, value);
	MIPS_OUTPUT(str_t);
}
void array_set_medi(char* array_name, int offset, char* value)
{
	// MIPS_OUTPUT(array_name << " = " << offset << " ARSET " << value);
	char str_t[STRSIZE];
	strcpy(str_t, array_name);
	strcat(str_t, " = ");
	strcat(str_t, int2string(offset));
	strcat(str_t, " ARSET ");
	strcat(str_t, value);
	MIPS_OUTPUT(str_t);
}
void array_set_medi(char* array_name, char* offset, int value)
{
	// MIPS_OUTPUT(array_name << " = " << offset << " ARSET " << value);
	char str_t[STRSIZE];
	strcpy(str_t, array_name);
	strcat(str_t, " = ");
	strcat(str_t, offset);
	strcat(str_t, " ARSET ");
	strcat(str_t, int2string(value));
	MIPS_OUTPUT(str_t);
}
void array_set_medi(char* array_name, int offset, int value)
{
	// MIPS_OUTPUT(array_name << " = " << offset << " ARSET " << value);
	char str_t[STRSIZE];
	strcpy(str_t, array_name);
	strcat(str_t, " = ");
	strcat(str_t, int2string(offset));
	strcat(str_t, " ARSET ");
	strcat(str_t, int2string(value));
	MIPS_OUTPUT(str_t);
}


// vector<char*> str_set;
// ������
void printf_medi(int type, char* v)
{
	/*if (type == STRING)
	{
		int len = str_set.size();
		for (int i = 0; i < len; i++)
		{
			if (str_set[i] == v)
			{
				MIPS_OUTPUT("@printf " << type2string(type) << " " << "S_" << i);
				return;
			}
		}
		if (LINE)
		{
			str_set.push_back("\\n" + v);
		}
		else
		{
			str_set.push_back(v);
		}

		MIPS_OUTPUT("@printf " << type2string(type) << " S_" << len);

	}
	else
	{
		MIPS_OUTPUT("@printf " << type2string(type) << " " << v);
	}*/
}
void printf_medi(int type, int v)
{
	// MIPS_OUTPUT("@printf " << type2string(type) << " " << v);
	char str_t[STRSIZE];
	strcpy(str_t, "@printf ");
	strcat(str_t, type2string(type));
	strcat(str_t, " ");
	strcat(str_t, int2string(v));
	MIPS_OUTPUT(str_t);
}

// �������
void scanf_medi(int type, char* v)
{
	// MIPS_OUTPUT("@scanf " << type2string(type) << " " << v);
	char str_t[STRSIZE];
	strcpy(str_t, "@scanf ");
	strcat(str_t, type2string(type));
	strcat(str_t, " ");
	strcat(str_t, v);
}

void medi(char* line)
{
	// MIPS_OUTPUT(line);
	MIPS_OUTPUT(line);
}


/*��������*/
char* int2string(int n) {
	char via[STRSIZE];
	itoa(n, via, 10);
	return via;
}

char* type2string(int type) {
	/* switch (type)
	{
	case INT:
		return "int";
	case CHAR:
		return "char";
	case VOID:
		return "void";
	case STRING:
		return "string";
	}*/
	return NULL;
}