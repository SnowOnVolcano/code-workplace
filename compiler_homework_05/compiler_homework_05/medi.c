#define MIPS_OUTPUT(x) fprintf(fMediOut, "%s\n", x)

#include "medi.h"

int branch = 0;
int level;
int temp_count = 1;
int temps[64];
bool in_switch = false;


/*辅助函数*/
char* int2string(int n) {
	static char via[STRSIZE];
	_itoa(n, via, 10);
	return via;
}

// 初始化中间代码生成程序
int init_medi() {
	fMediOut = fopen("mips_mediate_code.txt", "w");
	return 0;
}

void init_temp() { puts("init_temp"); }

char* new_temp() {
	int tempno = temps[temp_count - 1];   // get last temp
	static char str_t[STRSIZE];
	strcpy(str_t, "#");
	strcat(str_t, int2string(tempno));
	temps[temp_count - 1] ++;
	return str_t;
}


// 新建一个Label
char* new_label(SymbolTable_t* func_item, char* info) {
	// return func_item->get_name() + "_L_" + int2string(branch++) + "_" + info;
	static char str_t[STRSIZE];
	strcpy(str_t, func_item->name);
	strcat(str_t, "_L_");
	strcat(str_t, int2string(branch++));
	strcat(str_t, "_");
	strcat(str_t, info);
	return str_t;
}

// 结束退出
void exit_medi() {
	MIPS_OUTPUT("@exit");
}

// 函数声明
void declare_func_medi(SymbolItem_t* func_item) {
	// MIPS_OUTPUT("@func " << func_item->get_name());
	char str_t[STRSIZE];
	strcpy(str_t, "@func ");
	strcat(str_t, func_item->key);
	MIPS_OUTPUT(str_t);
}

// 函数参数声明
void declare_para_medi(int type, char* name)
{
	// MIPS_OUTPUT("@para " << type2string(type) << " " << name);
	char str_t[STRSIZE];
	strcpy(str_t, "@para ");
	if (type == VARIABLE_INT) { strcat(str_t, "INT"); }
	else { strcat(str_t, "CHAR"); }
	strcat(str_t, " ");
	strcat(str_t, name);
	MIPS_OUTPUT(str_t);
}

// 变量声明
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
	if (var_item->type == ARRAY_INT || var_item->type == ARRAY_CHAR) {
		strcpy(str_t, "@array ");
		(var_item->type == ARRAY_INT) ? strcat(str_t, "int[] ") : strcat(str_t, "char[] ");
		strcat(str_t, var_item->key);
		strcat(str_t, " ");
		strcat(str_t, int2string(var_item->index));
		// strcat(str_s, var_item->array_index)
	}
	else {
		strcpy(str_t, "@var ");
		(var_item->type == VARIABLE_INT) ? strcat(str_t, "int ") : strcat(str_t, "char ");
		strcat(str_t, var_item->key);
	}
	MIPS_OUTPUT(str_t);
}

// 函数调用
void invoke_func_medi(char* name)
{
	// MIPS_OUTPUT("@call " + name);
	char str_t[STRSIZE];
	strcpy(str_t, "@call ");
	strcat(str_t, name);
	MIPS_OUTPUT(str_t);
}

// 返回语句
void return_medi_c(char* v)
{
	// MIPS_OUTPUT("@ret " << v);
	char str_t[STRSIZE];
	strcpy(str_t, "@ret ");
	strcat(str_t, v);
	MIPS_OUTPUT(str_t);
}
void return_medi_i(int v)
{
	// MIPS_OUTPUT("@ret " << v);
	char str_t[STRSIZE];
	strcpy(str_t, "@ret ");
	strcat(str_t, int2string(v));
	MIPS_OUTPUT(str_t);
}
void return_medi_f(SymbolItem_t* func_item)
{
	/* if (func_item->get_type() == VOID)
	{
		MIPS_OUTPUT("@ret");
	}
	else
	{
		MIPS_OUTPUT("@ret 0");
	}*/

	if (func_item->type != FUNC_NON_RETURN) {
		MIPS_OUTPUT("@ret 0");
	}
}
void return_medi_v() {
	MIPS_OUTPUT("@ret");
}

// 跳转标志符
void label_medi(char* label)
{
	// MIPS_OUTPUT(label << " :");
	char str_t[STRSIZE];
	strcpy(str_t, label);
	strcat(str_t, " :");
	MIPS_OUTPUT(str_t);
}

// 计算
void cal_medi_iccc(int op, char* result, char* a1, char* a2)
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
void cal_medi_icci(int op, char* result, char* a1, int a2)
{
	/*stringstream ss;
	  ss << a2;
	cal_medi(op, result, a1, ss.str());*/
	char str_t[STRSIZE];
	strcpy(str_t, result);
	strcat(str_t, " = ");
	strcat(str_t, a1);
	strcat(str_t, " ");
	strcat(str_t, mnemonices[op]);
	strcat(str_t, " ");
	strcat(str_t, int2string(a2));
	MIPS_OUTPUT(str_t);
}
void cal_medi_icic(int op, char* result, int a1, char* a2)
{
	/*stringstream ss;
	ss << a1;
	cal_medi(op, result, ss.str(), a2);*/
	char str_t[STRSIZE];
	strcpy(str_t, result);
	strcat(str_t, " = ");
	strcat(str_t, int2string(a1));
	strcat(str_t, " ");
	strcat(str_t, mnemonices[op]);
	strcat(str_t, " ");
	strcat(str_t, a2);
	MIPS_OUTPUT(str_t);
}

// 赋值语句
void assign_medi_cc(char* n1, char* n2)
{
	// MIPS_OUTPUT(n1 << " = " << n2);
	char str_t[STRSIZE];
	strcpy(str_t, n1);
	strcat(str_t, " = ");
	strcat(str_t, n2);
	MIPS_OUTPUT(str_t);
}
void assign_medi_ci(char* name, int value)
{
	// MIPS_OUTPUT(name << " = " << value);
	char str_t[STRSIZE];
	strcpy(str_t, name);
	strcat(str_t, " = ");
	strcat(str_t, int2string(value));
	MIPS_OUTPUT(str_t);
}

// 传入参数
void push_medi_c(char* name)
{
	// MIPS_OUTPUT("@push " << name);
	char str_t[STRSIZE];
	strcpy(str_t, "@push ");
	strcat(str_t, name);
	MIPS_OUTPUT(str_t);
}
void push_medi_i(int name)
{
	// MIPS_OUTPUT("@push " << name);
	char str_t[STRSIZE];
	strcpy(str_t, "@push ");
	strcat(str_t, int2string(name));
	MIPS_OUTPUT(str_t);
}

// 接受返回值
void return_get_medi(char* name)
{
	// MIPS_OUTPUT("@get " << name);
	char str_t[STRSIZE];
	strcpy(str_t, "@get ");
	strcat(str_t, name);
	MIPS_OUTPUT(str_t);
}

// 等于 0 时跳转
void branch_zero_medi(char* name, char* label)
{
	// MIPS_OUTPUT("@bz " << name << " " << label);
	char str_t[STRSIZE];
	strcpy(str_t, "@beqz ");
	strcat(str_t, name);
	strcat(str_t, " ");
	strcat(str_t, label);
	MIPS_OUTPUT(str_t);
}

// 不等于 0 时跳转
void branch_notzero_medi(char* name, char* label)
{
	// MIPS_OUTPUT("@bz " << name << " " << label);
	char str_t[STRSIZE];
	strcpy(str_t, "@bnez ");
	strcat(str_t, name);
	strcat(str_t, " ");
	strcat(str_t, label);
	MIPS_OUTPUT(str_t);
}

// 相等时跳转 
void branch_equal_medi(char* name, int value, char* label)
{
	// MIPS_OUTPUT("@be " << name << " " << value << " " << label);
	char str_t[STRSIZE];
	strcpy(str_t, "@beq ");
	strcat(str_t, name);
	strcat(str_t, " ");
	strcat(str_t, int2string(value));
	strcat(str_t, " ");
	strcat(str_t, label);
	MIPS_OUTPUT(str_t);
}

// 无条件跳转
void jump_medi(char* label)
{
	// MIPS_OUTPUT("@j " << label);
	char str_t[STRSIZE];
	strcpy(str_t, "@j ");
	strcat(str_t, label);
	MIPS_OUTPUT(str_t);
}

// 无条件跳转并链接
void jump_link_medi(char* label)
{
	// MIPS_OUTPUT("@jal " << label);
	char str_t[STRSIZE];
	strcpy(str_t, "@jal ");
	strcat(str_t, label);
	MIPS_OUTPUT(str_t);
}

// 数组取值
void array_get_medi_ccc(char* array_name, char* offset, char* result)
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
void array_get_medi_cic(char* array_name, int offset, char* result)
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

// 数组赋值
void array_set_medi_ccc(char* array_name, char* offset, char* value)
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
void array_set_medi_cic(char* array_name, int offset, char* value)
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
void array_set_medi_cci(char* array_name, char* offset, int value)
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
void array_set_medi_cii(char* array_name, int offset, int value)
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
// 输出语句
void printf_medi_ic(int type, char* v, int len)
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
	char str_t[STRSIZE];
	strcpy(str_t, "@printf ");
	if (type == STRCON) { 
		strcat(str_t, "STRING "); 
		strcat(str_t, int2string(len)); 
		strcat(str_t, " S_"); 
	}
	else if (type == INTCON) { strcat(str_t, "INT "); }
	else if (type == CHARCON) { strcat(str_t, "CHAR "); }
	strcat(str_t, v);
	MIPS_OUTPUT(str_t);
}
void printf_medi_ii(int type, int v)
{
	// MIPS_OUTPUT("@printf " << type2string(type) << " " << v);
	char str_t[STRSIZE];
	strcpy(str_t, "@printf ");
	if (type == INTCON) { strcat(str_t, "INT "); }
	else if (type == CHARCON) { strcat(str_t, "CHAR "); }
	strcat(str_t, int2string(v));
	MIPS_OUTPUT(str_t);
}

// 输入语句
void scanf_medi(int type, char* v)
{
	// MIPS_OUTPUT("@scanf " << type2string(type) << " " << v);
	char str_t[STRSIZE];
	strcpy(str_t, "@scanf ");
	if (type == INTCON) { strcat(str_t, "INT "); }
	else { strcat(str_t, "CHAR "); }
	strcat(str_t, v);
	MIPS_OUTPUT(str_t);
}

void medi(char* line)
{
	// MIPS_OUTPUT(line);
	MIPS_OUTPUT(line);
}


