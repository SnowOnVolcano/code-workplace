#include "parsing.h"

/*辅助函数*/
int init_parsing() {
	stIndex = -1;
	return 0;
}
int getSymbolTypeFromTwo() {
	int r;
	if ((r = getSymbolType(symbolTables[stIndex], token)) < 0) {
		if ((r = getSymbolType(symbolTables[0], token)) < 0) {
			return -1;
		}
		else {
			return r;
		}
	}
	else {
		return r;
	}
}
int getSymbolValueFromTwo() {
	List_t* r;
	if ((r = getSymbolParaList(symbolTables[stIndex], token)) == NULL) {
		if ((r = getSymbolParaList(symbolTables[0], token)) == NULL) { return 0; }
		else { return getListTailNode(r)->data; }
	}
	else { return getListTailNode(r)->data; }
}
int error_and_getsym() {
	error(_ERROR_GET);
	getsym();
	return 0;
}


/*分析程序*/
// 字符串		::= "｛十进制编码为32,33,35-126的ASCII字符｝"
void string() {
	(symbol == STRCON && print_sym()) ? getsym() : error_and_getsym();
	fprintf(fpOut, "<字符串>\n");
}

// 程序			::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void program() {
	int isFuncStart = 0;
	/*———————— 新建一个符号表 - program ——————————*/
	symbolTables[++stIndex] = newSymbolTable("");
	/*------------------------------------------------------------*/
	while (symbol == CONSTTK) {
		con_info();
	}
	while (symbol == INTTK || symbol == CHARTK || symbol == VOIDTK) {
		if (symbol == INTTK || symbol == CHARTK) {
			saveCurrentSym();
			getsym();
			getsym();
			if (symbol == LPARENT) {
				restorePreviousSym();
				temps[++temp_count] = 0;
				if (isFuncStart == 0) { 
					isFuncStart = 1; 
					invoke_func_medi("main");
					exit_medi();
				}
				re_func_definition();
			}
			else {
				restorePreviousSym();
				var_info();
			}
		}
		else {
			saveCurrentSym();
			getsym();
			if (symbol == MAINTK) {
				restorePreviousSym();
				temps[++temp_count] = 0;
				if (isFuncStart == 0) {
					isFuncStart = 1;
					invoke_func_medi("main");
					exit_medi();
				}
				mainFunction();
			}
			else {
				restorePreviousSym();
				temps[++temp_count] = 0;
				if (isFuncStart == 0) {
					isFuncStart = 1;
					invoke_func_medi("main");
					exit_medi();
				}
				unre_func_definition();
			}
		}

	}
	fprintf(fpOut, "<程序>\n");
}

// 常量说明		::=  const＜常量定义＞;{ const＜常量定义＞;}
void con_info() {
	(symbol == CONSTTK && print_sym()) ? getsym() : error_and_getsym();
	con_definition();
	(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
	while (symbol == CONSTTK)
	{
		print_sym();
		getsym();
		con_definition();
		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
	}
	fprintf(fpOut, "<常量说明>\n");
}

/* 常量定义		::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞} |
						char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ } */
void con_definition() {
	int r;
	char const_name[TOKENSIZE];
	if (symbol == INTTK) {
		do
		{
			print_sym();
			getsym();

			strcpy(const_name, token);
			(symbol == IDENFR && print_sym()) /*加入符号表*/
				? (r = addSymbol(symbolTables[stIndex], token, CONST_INT))
				: error(_ERROR);

			/*错误处理*/ /*名字重定义*/
			if (r < 0) { error(ERROR_B); }

			getsym();
			(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();
			
			saveCurrentSym();	/*加入符号表*/
			char str_t[STRSIZE] = { 0 };
			if (symbol == PLUS || symbol == MINU) { strcat(str_t, token); getsym(); }
			strcat(str_t, token);
			int const_value = atoi(str_t);
			addSymbolParaListItem(symbolTables[stIndex], const_name, const_value);
			restorePreviousSym();

			r = integer();
			/*错误处理*/ /*常量定义中=后面只能是整型或字符型常量*/
			if (r < 0) { error(ERROR_O); }

		} while (symbol == COMMA);
	}
	else if (symbol == CHARTK) {
		do
		{
			print_sym();
			getsym();

			strcpy(const_name, token);
			(symbol == IDENFR && print_sym()) /*加入符号表*/
				? (r = addSymbol(symbolTables[stIndex], token, CONST_CHAR))
				: error(_ERROR);

			/*错误处理*/ /*名字重定义*/
			if (r < 0) error(ERROR_B);

			getsym();
			(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();

			/*错误处理*/ /*常量定义中=后面只能是整型或字符型常量*/
			if (symbol != CHARCON) { error(ERROR_O); }
			
			addSymbolParaListItem(symbolTables[stIndex], const_name, (int)token[0]);	/*加入符号表*/

			getsym();

		} while (symbol == COMMA);
	}
	else {
		error(_ERROR);
	}
	fprintf(fpOut, "<常量定义>\n");
}

// 无符号整数	::= ＜非零数字＞｛＜数字＞｝| 0
int unsigned_integer() {
	int r = (symbol == INTCON);
	print_sym();
	getsym();
	fprintf(fpOut, "<无符号整数>\n");
	return r - 1;
}

// 整数			::= ［＋｜－］＜无符号整数＞
int integer() {
	if (symbol == PLUS || symbol == MINU) {
		print_sym();
		getsym();
	}
	int r = unsigned_integer();
	fprintf(fpOut, "<整数>\n");
	return r;
}

// 变量说明		::= ＜变量定义＞;{＜变量定义＞;}
void var_info() {
	var_definition();
	(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
	while (symbol == INTTK || symbol == CHARTK) {
		saveCurrentSym();
		getsym();
		getsym();
		if (symbol != LPARENT) {
			restorePreviousSym();
			var_definition();
			(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
		}
		else {
			restorePreviousSym();
			break;
		}
	}
	fprintf(fpOut, "<变量说明>\n");
}

// 变量定义		::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )}
void var_definition() {
	int r;
	if (symbol == INTTK || symbol == CHARTK) {
		int vtype = (symbol == INTTK) ? VARIABLE_INT : VARIABLE_CHAR;
		int atype = (symbol == INTTK) ? ARRAY_INT : ARRAY_CHAR;
		do {
			print_sym();
			getsym();

			(symbol == IDENFR && print_sym()) /*加入符号表*/
				? (r = addSymbol(symbolTables[stIndex], token, vtype))
				: error(_ERROR);

			/*错误处理*/ /*名字重定义*/
			if (r < 0) error(ERROR_B);

			char var_name[TOKENSIZE];
			strcpy(var_name, token);

			getsym();
			if (symbol == LBRACK) {
				addSymbolType(symbolTables[stIndex], var_name, atype); /*加入符号表*/

				print_sym();
				getsym();

				addSymbolIndex(symbolTables[stIndex], var_name, atoi(token));

				unsigned_integer();
				(symbol == RBRACK && print_sym()) ? getsym() : error(ERROR_M);
			}

			declare_var_medi(getSymbol(symbolTables[stIndex], var_name));
		} while (symbol == COMMA);
	}
	else {
		error(_ERROR);
	}

	fprintf(fpOut, "<变量定义>\n");
}

// 类型标识符	::=  int | char

// 声明头部		::=  int＜标识符＞ | char＜标识符＞
void declarator() {
	int r = 0;
	int type = (symbol == INTTK) ? FUNC_HAS_RETURN_INT : FUNC_HAS_RETURN_CHAR;
	((symbol == INTTK || symbol == CHARTK) && print_sym()) ? getsym() : error_and_getsym();

	/*———————— 新建一个符号表 - token ———————————*/
	symbolTables[++stIndex] = newSymbolTable(token);
	/*------------------------------------------------------------*/

	(symbol == IDENFR && print_sym()) /*加入符号表*/ /*加入总符号表*/
		? (r = addSymbol(symbolTables[0], token, type))
		: error(_ERROR);

	/*错误处理*/ /*名字重定义*/
	if (r < 0) error(ERROR_B);

	declare_func_medi(getSymbol(symbolTables[0], token));

	getsym();
	fprintf(fpOut, "<声明头部>\n");
}

// 有返回值函数定义	::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
void re_func_definition() {
	declarator();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	paraList();
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	(symbol == LBRACE && print_sym()) ? getsym() : error_and_getsym();
	compoundStatement();

	/*错误处理*/ /*有返回值的函数缺少return语句或存在不匹配的return语句*/
	if (symbolTables[stIndex]->returnType == 0) { error(ERROR_H); }

	(symbol == RBRACE && print_sym()) ? getsym() : error_and_getsym();
	fprintf(fpOut, "<有返回值函数定义>\n");
}

// 无返回值函数定义	::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void unre_func_definition() {
	int r = 0;
	(symbol == VOIDTK && print_sym()) ? getsym() : error_and_getsym();

	/*———————— 新建一个符号表 - token ———————————*/
	symbolTables[++stIndex] = newSymbolTable(token);
	/*------------------------------------------------------------*/

	(symbol == IDENFR && print_sym()) /*加入符号表*/ /*加入总符号表*/
		? (r = addSymbol(symbolTables[0], token, FUNC_NON_RETURN))
		: error(_ERROR);

	/*错误处理*/ /*名字重定义*/
	if (r < 0) error(ERROR_B);

	declare_func_medi(getSymbol(symbolTables[0], token));

	getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	paraList();
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	(symbol == LBRACE && print_sym()) ? getsym() : error_and_getsym();
	compoundStatement();
	(symbol == RBRACE && print_sym()) ? getsym() : error_and_getsym();

	return_medi_v();

	fprintf(fpOut, "<无返回值函数定义>\n");
}

// 复合语句		::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
void compoundStatement() {
	if (symbol == CONSTTK) {
		con_info();
	}
	if (symbol == INTTK || symbol == CHARTK) {
		var_info();
	}
	statecolumn();
	fprintf(fpOut, "<复合语句>\n");
}

// 参数表		::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞} | ＜空＞
void paraList() {
	int r = 0;	
	if (symbol == INTTK || symbol == CHARTK) {
		int type = (symbol == INTTK) ? VARIABLE_INT : VARIABLE_CHAR;
		print_sym();
		getsym();

		(symbol == IDENFR && print_sym()) /*加入符号表*/
			? (r = addSymbol(symbolTables[stIndex], token, type))
			: error(_ERROR);

		/*错误处理*/ /*名字重定义*/ /*如未出错则更新函数参数表*/
		if (r < 0) error(ERROR_B);
		else addSymbolParaListItem(symbolTables[0], symbolTables[stIndex]->name, type);

		declare_para_medi(type, token);

		getsym();
		while (symbol == COMMA) {
			print_sym();
			getsym();
			type = (symbol == INTTK) ? VARIABLE_INT : VARIABLE_CHAR;
			((symbol == INTTK || symbol == CHARTK) && print_sym()) ? getsym() : error_and_getsym();

			(symbol == IDENFR && print_sym()) /*加入符号表*/
				? (r = addSymbol(symbolTables[stIndex], token, type))
				: error_and_getsym();

			/*错误处理*/ /*名字重定义*/  /*如未出错则更新函数参数表*/
			if (r < 0) error(ERROR_B);
			else addSymbolParaListItem(symbolTables[0], symbolTables[stIndex]->name, type);

			getsym();
		}
	}
	else {
		// 空
	}
	fprintf(fpOut, "<参数表>\n");
}

// 主函数		::= void main'('')' '{'＜复合语句＞'}'
void mainFunction() {
	int r = 0;

	/*———————— 新建一个符号表 - token ———————————*/
	symbolTables[++stIndex] = newSymbolTable("main");
	/*------------------------------------------------------------*/

	(symbol == VOIDTK && print_sym()) ? getsym() : error_and_getsym();

	(symbol == MAINTK && print_sym()) /*加入符号表*/ /*加入总符号表*/
		? (r = addSymbol(symbolTables[0], token, FUNC_NON_RETURN))
		: error(_ERROR);

	/*错误处理*/ /*名字重定义*/
	if (r < 0) error(ERROR_B);

	declare_func_medi(getSymbol(symbolTables[0], token));

	getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	(symbol == LBRACE && print_sym()) ? getsym() : error_and_getsym();
	compoundStatement();

	(symbol == RBRACE && print_sym()) ? getsym() : error_and_getsym();
	
	return_medi_v();

	fprintf(fpOut, "<主函数>\n");
}

// 表达式		::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   #[+|-]只作用于第一个<项>#
int expression(int* value, bool* certain, char* name) {
	int type = 0;
	
	int cur_value;
	bool cur_certain;
	int cur_op = PLUS;
	*value = 0;
	*certain = true;
	char* itm_name = (char*)malloc(STRSIZE * sizeof(char));
	bool first_uncertain = true;

	if (symbol == PLUS || symbol == MINU) {
		type = 1;
		cur_op = symbol;
		print_sym();
		getsym();
	}
	do {
		type = term(&cur_value, &cur_certain, itm_name) | type;
		
		*certain &= cur_certain;
		if (*certain) {
			if (cur_op == PLUS) { *value += cur_value; }
			else if (cur_op == MINU) { *value -= cur_value; }
			else { puts("cur_op failed at expr()"); }
		}
		else if (first_uncertain) {
			first_uncertain = false;
			strcpy(name, new_temp());
			if (*value == 0) {
				if (cur_op == PLUS) { assign_medi_cc(name, itm_name); }
				else { cal_medi_icic(cur_op, name, 0, itm_name); }
			}
			else {
				assign_medi_ci(name, *value);
				cal_medi_iccc(cur_op, name, name, itm_name);
			}
		}
		else if (!cur_certain) { cal_medi_iccc(cur_op, name, name, itm_name); }
		else { cal_medi_icci(cur_op, name, name, cur_value); }

		if (symbol == PLUS || symbol == MINU) {
			type = 1;
			cur_op = symbol;
			print_sym();
			getsym();
		}
		else { break; }
	} while (true);

	free(itm_name);
	fprintf(fpOut, "<表达式>\n");
	return type;
}

// 项			::= ＜因子＞{＜乘法运算符＞＜因子＞}
int term(int* value, bool* certain, char* name) {
	int type = 0;

	int cur_value;
	bool cur_certain;
	int cur_op = MULT;
	*value = 1;
	*certain = true;
	char* fac_name = (char*)malloc(STRSIZE * sizeof(char));
	bool first_uncertain = true;

	do
	{
		type = factor(&cur_value, &cur_certain, fac_name) | type;
		
		*certain &= cur_certain;
		if (cur_op == DIV && cur_certain && cur_value == 0) { cur_value = 1; }
		if (*certain) {
			if (cur_op == MULT) { *value *= cur_value; }
			else if (cur_op == DIV) { *value /= cur_value; }
			else { puts("error at item()"); }
		}
		else if (first_uncertain) {
			first_uncertain = false;
			strcpy(name, new_temp());
			if (*value == 1 && cur_op == MULT) { 
				assign_medi_cc(name, fac_name); 
			}
			else {
				assign_medi_ci(name, *value);
				cal_medi_iccc(cur_op, name, name, fac_name);
			}
		}
		else if (!cur_certain) { cal_medi_iccc(cur_op, name, name, fac_name); }
		else { cal_medi_icci(cur_op, name, name, cur_value); }

		if (symbol == MULT || symbol == DIV) {
			type = 1;
			print_sym();
			getsym();
		}
		else { break; }
	} while (true);

	free(fac_name);
	fprintf(fpOut, "<项>\n");
	return type;
}

// 因子			::= ＜标识符＞ ｜ ＜标识符＞'['＜表达式＞']' | '('＜表达式＞')' ｜ ＜整数＞ | ＜字符＞ ｜ ＜有返回值函数调用语句＞
int factor(int* value, bool* certain, char* name) {
	int type = 0;
	int r = 0;

	*certain = false;

	if (symbol == IDENFR) {
		if ((r = getSymbolType(symbolTables[0], token)) == FUNC_HAS_RETURN_INT || r == FUNC_HAS_RETURN_CHAR) {
			// 有返回值函数调用语句
			type = (r == FUNC_HAS_RETURN_INT);

			char func_name[STRSIZE];
			strcpy(func_name, token);

			refunc_callStatement();
			
			invoke_func_medi(func_name);
			strcpy(name, new_temp());
			return_get_medi(name);
			*certain = false;
		}
		else if ((r = getSymbolTypeFromTwo()) == VARIABLE_INT || r == VARIABLE_CHAR) {
			// 变量或常量
			type = (r == VARIABLE_INT);

			strcpy(name, token);
			*certain = false;
			
			print_sym();
			getsym();
		}
		else if (r == CONST_INT || r == CONST_CHAR) {
			type = (r == CONST_INT);
			
			*value = getSymbolValueFromTwo();
			*certain = true;

			print_sym();
			getsym();
		}
		else if (r == ARRAY_INT || r == ARRAY_CHAR) {
			// 数组
			char array_name[STRSIZE];
			strcpy(array_name, token);

			type = (r == ARRAY_INT);
			print_sym();
			getsym();
			(symbol == LBRACK && print_sym()) ? getsym() : error_and_getsym();
			
			int index_value;
			bool index_certain;
			char* index_name = (char*)malloc(STRSIZE * sizeof(char));

			type = expression(&index_value, &index_certain, index_name);

			/*错误处理*/ /*数组元素的下标只能是整型表达式*/
			if (type == 0) { error(ERROR_I); }

			if (index_certain) {
				strcpy(index_name, new_temp());
				assign_medi_ci(index_name, index_value);
			}
			strcpy(name, new_temp());
			array_get_medi_ccc(array_name, index_name, name);
			*certain = false;
			free(index_name);

			(symbol == RBRACK && print_sym()) ? getsym() : error(ERROR_M);
		}
		else {
			type = 1;

			*certain = true;
			*value = 0;

			/*错误处理*/ /*未定义的名字*/
			if (r < 0) { error(ERROR_C); getsym(); }

			else { error_and_getsym(); }
		}
	}
	else if (symbol == LPARENT) {		// '('表达式')'
		type = 1;
		print_sym();
		getsym();
		expression(value, certain, name);
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	}
	else if (symbol == CHARCON) {		// 字符
		type = 0;

		*value = (int)token[0];
		*certain = true;

		print_sym();
		getsym();
	}
	else {								// 整数
		type = 1;

		saveCurrentSym();
		char str_t[STRSIZE] = { 0 };
		if (symbol == PLUS || symbol == MINU) { strcat(str_t, token); getsym(); }
		strcat(str_t, token);
		*value = atoi(str_t);
		*certain = true;
		restorePreviousSym();

		integer();
	}
	fprintf(fpOut, "<因子>\n");
	return type;
}

/* 语句			::= ＜条件语句＞｜＜循环语句＞| '{'＜语句列＞'}' |
					＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞;｜＜赋值语句＞; ｜
					＜读语句＞; ｜＜写语句＞; ｜＜空＞; | ＜返回语句＞; */
int statement() {
	temp_count++;
	temps[temp_count] = temps[temp_count - 1];
	int stateType;
	if (symbol == IFTK) {				// 条件语句
		conditionalStatement();
		stateType = S_CONDITIONAL;
	}
	else if (symbol == WHILETK || symbol == DOTK || symbol == FORTK) {
		loopStatement();				// 循环语句
		stateType = S_LOOP;
	}
	else if (symbol == LBRACE) {		// 语句列
		print_sym();
		getsym();
		statecolumn();
		(symbol == RBRACE && print_sym()) ? getsym() : error_and_getsym();
		stateType = S_STATECOLUMN;
	}
	else if (symbol == IDENFR) {		// 有、无返回值函数调用语句 | 赋值语句
		/*错误处理*/ /*未定义的名字*/
		if (getSymbolTypeFromTwo() < 0) {
			error(ERROR_C);
			while (symbol != SEMICN) getsym();
			stateType = _NORMAL_EXIT;
		}

		else {
			int temp = iden_statement();
			(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
			stateType = temp;
		}
	}
	else if (symbol == SCANFTK) {		// 读语句
		readStatement();
		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
		stateType = S_READ;
	}
	else if (symbol == PRINTFTK) {		// 写语句
		printStatement();
		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
		stateType = S_PRINT;
	}
	else if (symbol == SEMICN) {		// 空语句
		print_sym();
		getsym();
		stateType = S_NULL;
	}
	else if (symbol == RETURNTK) {		// 返回语句
		returnStatement();
		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
		stateType = S_RETURN;
	}
	else {
		return _ERROR;
	}
	fprintf(fpOut, "<语句>\n");
	temps[temp_count] = 0;
	temp_count--;
	return stateType;
}

// 标识符开头的语句
int iden_statement() {
	if (getSymbolType(symbolTables[0], token) == FUNC_HAS_RETURN_INT
		|| getSymbolType(symbolTables[0], token) == FUNC_HAS_RETURN_CHAR) {
		// 有返回值函数调用语句
		char func_name[STRSIZE];
		strcpy(func_name, token);
		refunc_callStatement();
		invoke_func_medi(func_name);
		return S_REFUNCCALL;
	}
	else if (getSymbolType(symbolTables[0], token) == FUNC_NON_RETURN) {
		// 无返回值函数调用语句
		char func_name[STRSIZE];
		strcpy(func_name, token);
		unrefunc_callStatement();
		invoke_func_medi(func_name);
		return S_UNREFUNCCALL;
	}
	else {
		// 赋值语句
		assignStatement();
		return S_ASSIGN;
	}
}

// 赋值语句		::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
void assignStatement() {
	int type = 0;
	
	char var_name[TOKENSIZE];
	strcpy(var_name, token);

	char* name = (char*)malloc(STRSIZE * sizeof(char));
	int assign_value;
	bool assign_certain;
	int index_value;
	bool index_certain;
	char* index_name = (char*)malloc(STRSIZE * sizeof(char));
	
	if (getSymbolTypeFromTwo() == VARIABLE_INT
		|| getSymbolTypeFromTwo() == VARIABLE_CHAR) {
		print_sym();
		getsym();
		(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();
		expression(&assign_value, &assign_certain, name);

		if (assign_certain) { assign_medi_ci(var_name, assign_value); }
		else { assign_medi_cc(var_name, name); }
	}
	else if (getSymbolTypeFromTwo() == ARRAY_INT
		|| getSymbolTypeFromTwo() == ARRAY_CHAR) {
		print_sym();
		getsym();
		(symbol == LBRACK && print_sym()) ? getsym() : error_and_getsym();
		
		type = expression(&index_value, &index_certain, index_name);

		/*错误处理*/ /*数组元素的下标只能是整型表达式*/
		if (type == 0) { error(ERROR_I); }

		if (index_certain) {
			strcpy(index_name, new_temp());
			assign_medi_ci(index_name, index_value);
		}

		(symbol == RBRACK && print_sym()) ? getsym() : error(ERROR_M);
		(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();
		expression(&assign_value, &assign_certain, name);

		if (assign_certain) { array_set_medi_cci(var_name, index_name, assign_value); }
		else { array_set_medi_ccc(var_name, index_name, name); }
	}
	else if (getSymbolTypeFromTwo() == CONST_INT
		|| getSymbolTypeFromTwo() == CONST_CHAR) {

		/*错误处理*/ /*不能改变常量的值*/
		error(ERROR_J);
		while (symbol != SEMICN) { getsym(); }
	}
	else {
		error(_ERROR);
	}

	free(index_name);
	fprintf(fpOut, "<赋值语句>\n");
}

// 条件语句		::= if '('＜条件＞')'＜语句＞［else＜语句＞］
void conditionalStatement() {
	(symbol == IFTK && print_sym()) ? getsym() : error_and_getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	
	char* cond_name = (char*)malloc(STRSIZE * sizeof(char));
	char over_label[STRSIZE];
	char else_label[STRSIZE];
	int cond_value;
	bool cond_certain;

	condition(&cond_value, &cond_certain, cond_name);
	
	strcpy(else_label, new_label(symbolTables[stIndex], "else_begin"));   // set label
	strcpy(over_label, new_label(symbolTables[stIndex], "else_over"));   // set label
	
	if (cond_certain && cond_value == 0) { jump_medi(else_label); }
	else if (!cond_certain) { branch_zero_medi(cond_name, else_label); }
	free(cond_name);
	
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	statement();

	jump_medi(over_label);

	if (symbol == ELSETK) {
		print_sym();
		getsym();
		label_medi(else_label);
		statement();
		label_medi(over_label);
	}
	fprintf(fpOut, "<条件语句>\n");
}

/* 条件			::=  ＜表达式＞＜关系运算符＞＜表达式＞ #整型表达式之间才能进行关系运算# ｜
					 ＜表达式＞    #表达式为整型，其值为0条件为假，值不为0时条件为真# */
void condition(int* value, bool* certain, char* name) {
	int type = 0;

	int cmp_op;
	char* left_name = (char*)malloc(STRSIZE * sizeof(char));

	type = expression(value, certain, left_name);

	/*错误处理*/ /*条件判断中出现不合法的类型*/
	if (type == 0) { error(ERROR_F); }

	if (symbol >= LSS && symbol <= NEQ) {
		cmp_op = symbol;
		
		print_sym();
		getsym();

		bool left_certain = *certain;
		int left_value = *value;
		char* right_name = (char*)malloc(STRSIZE * sizeof(char));

		type = expression(value, certain, right_name);
		
		/*错误处理*/ /*条件判断中出现不合法的类型*/
		if (type == 0) { error(ERROR_F); }

		bool right_certain = *certain;
		int right_value = *value;
		*certain = false;

		if (left_certain && right_certain) {
			*certain = true;
			if (cmp_op == LSS) { *value = (left_value < right_value); }
			else if (cmp_op == LEQ) { *value = (left_value <= right_value); }
			else if (cmp_op == GRE) { *value = (left_value > right_value); }
			else if (cmp_op == GEQ) { *value = (left_value >= right_value); }
			else if (cmp_op == EQL) { *value = (left_value == right_value); }
			else if (cmp_op == NEQ) { *value = (left_value != right_value); }
		}
		else if (left_certain && !right_certain) {
			cal_medi_icic(cmp_op, right_name, left_value, right_name);
			strcpy(name, right_name);
		}
		else if (!left_certain && right_certain) {
			cal_medi_icci(cmp_op, left_name, left_name, right_value);
			strcpy(name, left_name);
		}
		else {
			cal_medi_iccc(cmp_op, left_name, left_name, right_name);
			strcpy(name, left_name);
		}
		free(right_name);
	}
	else { strcpy(name, left_name); }

	free(left_name);
	fprintf(fpOut, "<条件>\n");
}

/* 循环语句		::=  while '('＜条件＞')'＜语句＞ |
					 do＜语句＞while '('＜条件＞')' |
					 for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞*/
void loopStatement() {
	char begin_label[STRSIZE], over_label[STRSIZE];
	char* cond_name = (char*)malloc(STRSIZE * sizeof(char));
	int cond_value;
	bool cond_certain;

	if (symbol == WHILETK) {
		print_sym();
		getsym();
		
		strcpy(begin_label, new_label(symbolTables[stIndex], "while_begin"));
		strcpy(over_label, new_label(symbolTables[stIndex], "while_over"));
		label_medi(begin_label);   // set label

		(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
		condition(&cond_value, &cond_certain, cond_name);
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);

		if (cond_certain && cond_value == 0) { jump_medi(over_label); }
		else if (!cond_certain) { branch_zero_medi(cond_name, over_label); }

		statement();

		jump_medi(begin_label);
		label_medi(over_label);
	}
	else if (symbol == DOTK) {	
		print_sym();
		getsym();

		strcpy(begin_label, new_label(symbolTables[stIndex], "do_begin"));
		label_medi(begin_label);

		statement();

		/*错误处理*/ /*do-while应为语句中缺少while*/
		(symbol == WHILETK && print_sym()) ? getsym() : error(ERROR_N);

		(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
		condition(&cond_value, &cond_certain, cond_name);
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);

		if (cond_certain && cond_value == 1) { jump_medi(begin_label); }
		else if (!cond_certain) { branch_notzero_medi(cond_name, begin_label); }
	}
	else if (symbol == FORTK)	
	{
		print_sym();
		getsym();

		strcpy(begin_label, new_label(symbolTables[stIndex], "for_begin"));
		strcpy(over_label, new_label(symbolTables[stIndex], "for_over"));

		(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
		
		int r = getSymbolTypeFromTwo();
		
		/*错误处理*/ /*未定义的名字*/
		if (r < 0) { error(ERROR_C); }
		/*错误处理*/ /*不能改变常量的值*/
		else if (r == CONST_CHAR || r == CONST_INT) { error(ERROR_J); }
		
		char var_name[STRSIZE];
		strcpy(var_name, token);

		getsym();
		(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();
		
		int expr_value, expr_certain;
		char* expr_name = (char*)malloc(STRSIZE * sizeof(char));
		expression(&expr_value, &expr_certain, expr_name);
		
		if (expr_certain) { assign_medi_ci(var_name, expr_value); }
		else { assign_medi_cc(var_name, expr_name); }
		free(expr_name);

		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);

		label_medi(begin_label);

		condition(&cond_value, &cond_certain, cond_name);
		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);

		branch_zero_medi(cond_name, over_label);

		r = getSymbolTypeFromTwo();

		/*错误处理*/ /*未定义的名字*/
		if (getSymbolTypeFromTwo() < 0) { error(ERROR_C); }

		/*错误处理*/ /*不能改变常量的值*/
		else if (r == CONST_CHAR || r == CONST_INT) { error(ERROR_J); }

		char var1_name[STRSIZE];
		strcpy(var1_name, token);

		getsym();
		(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();

		/*错误处理*/ /*未定义的名字*/
		if (getSymbolTypeFromTwo() < 0) { error(ERROR_C); }

		char var2_name[STRSIZE];
		strcpy(var2_name, token);
		int cur_type;
		int cur_value = 0;
		int const_value = 0;
		if ((cur_type = getSymbolTypeFromTwo()) == CONST_INT || cur_type == CONST_CHAR) {
			const_value = getSymbolValueFromTwo();
		}

		getsym();
		
		int cur_op = symbol;
		((symbol == PLUS || symbol == MINU) && print_sym()) ? getsym() : error_and_getsym();
		

		cur_value = atoi(token);
		stride();
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);

		statement();

		if (cur_type == CONST_INT || cur_type == CONST_CHAR) { 
			if (cur_op == PLUS) { assign_medi_ci(var1_name, const_value + cur_value); }
			else { assign_medi_ci(var1_name, const_value - cur_value); }
		}
		else {
			cal_medi_icci(cur_op, var1_name, var2_name, cur_value);
		}

		jump_medi(begin_label);
		label_medi(over_label);
	}
	else {
		error(_ERROR);
	}

	free(cond_name);
	fprintf(fpOut, "<循环语句>\n");
}

// 步长			::= ＜无符号整数＞
void stride() {
	unsigned_integer();
	fprintf(fpOut, "<步长>\n");
}

// 有返回值函数调用语句	::= ＜标识符＞'('＜值参数表＞')'
void refunc_callStatement() {
	List_t* paraList = getSymbolParaList(symbolTables[0], token);

	(symbol == IDENFR && print_sym()) ? getsym() : error_and_getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();

	valueparaList(paraList);

	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);

	fprintf(fpOut, "<有返回值函数调用语句>\n");
}

// 无返回值函数调用语句	::= ＜标识符＞'('＜值参数表＞')'
void unrefunc_callStatement() {
	List_t* paraList = getSymbolParaList(symbolTables[0], token);

	(symbol == IDENFR && print_sym()) ? getsym() : error_and_getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();

	valueparaList(paraList);

	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);

	fprintf(fpOut, "<无返回值函数调用语句>\n");
}

// 值参数表		::= ＜表达式＞{,＜表达式＞}｜＜空＞
void valueparaList(List_t* paraList) {
	int sum = 0;
	int type = 0;
	Node_t* node = getListTopNode(paraList);

	if (node != NULL && symbol != RPARENT) {
		node = getListPrevNode(node); // 回退
		do
		{
			sum++; 
			
			int para_value;
			bool para_certain;
			char* para_name = (char*)malloc(STRSIZE * sizeof(char));
			
			type = expression(&para_value, &para_certain, para_name);

			if (para_certain) { push_medi_i(para_value); }
			else { push_medi_c(para_name); }
			free(para_name);

			node = getListNextNode(node);
			/*错误处理*/ /*函数参数个数不匹配*/
			if (node == NULL || sum > paraList->n)
			{
				error(ERROR_D);
				while (symbol != RPARENT) { getsym(); }
				break;
			}
			int nodeType = node->data;
			/*错误分析*/ /*函数参数类型不匹配*/
			if ((nodeType == VARIABLE_INT && type != 1)
				|| (nodeType == VARIABLE_CHAR && type != 0)) {
				error(ERROR_E);
				while (symbol != RPARENT) { getsym(); }
			}

			if (symbol == COMMA) {
				print_sym();
				getsym();
			}
			else { break; }
		} while (true);

		/*错误处理*/ /*函数参数个数不匹配*/
		if (sum < paraList->n) { error(ERROR_D); }
	}
	else if ((node == NULL && symbol != RPARENT) || (node != NULL && symbol == RPARENT))
	{
		/*错误处理*/ /*函数参数个数不匹配*/
		error(ERROR_D);
		while (symbol != RPARENT) { getsym(); }
	}

	fprintf(fpOut, "<值参数表>\n");
}

// 语句列		::= ｛＜语句＞｝
void statecolumn() {
	for (; statement() >= 0;);
	fprintf(fpOut, "<语句列>\n");
}

// 读语句		::=  scanf '('＜标识符＞{,＜标识符＞}')'
void readStatement() {
	int scanf_type;

	(symbol == SCANFTK && print_sym()) ? getsym() : error_and_getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	
	scanf_type = (getSymbolTypeFromTwo() == VARIABLE_INT) ? INTCON : CHARCON;
	scanf_medi(scanf_type, token);

	/*错误处理*/ /*未定义的名字*/
	if (getSymbolTypeFromTwo() < 0) { error(ERROR_C); getsym(); }

	else {
		(getSymbolTypeFromTwo() == VARIABLE_INT
			|| getSymbolTypeFromTwo() == VARIABLE_CHAR)
			? getsym() : error_and_getsym();
	}

	while (symbol == COMMA)
	{
		print_sym();
		getsym();

		scanf_type = (getSymbolTypeFromTwo() == VARIABLE_INT) ? INTCON : CHARCON;
		scanf_medi(scanf_type, token);

		/*错误处理*/ /*未定义的名字*/
		if (getSymbolTypeFromTwo() < 0) { error(ERROR_C); getsym(); }

		else (getSymbolTypeFromTwo() == VARIABLE_INT
			|| getSymbolTypeFromTwo() == VARIABLE_CHAR)
			? getsym() : error_and_getsym();
	}

	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	fprintf(fpOut, "<读语句>\n");
}

// 写语句		::= printf '(' ＜字符串＞,＜表达式＞ ')'| printf '('＜字符串＞ ')'| printf '('＜表达式＞')'
void printStatement() {
	bool is_expr = false;
	bool is_str = false; 
	char str_t[STRSIZE];  

	(symbol == PRINTFTK && print_sym()) ? getsym() : error_and_getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	if (symbol == STRCON) {
		is_str = true; 
		strcpy(str_t, token); 
		string();
		if (symbol == COMMA) {
			is_expr = true;
			print_sym();
			getsym();
		}
	}
	else { is_expr = true; }

	if (is_str && !is_expr) { printf_medi_ic(STRCON, str_t, strlen(str_t)); }
	if (is_expr) {
		char* print_name = (char*)malloc(STRSIZE * sizeof(char));
		int print_value;
		bool print_certain;
		int print_type = expression(&print_value, &print_certain, print_name);

		print_type = (print_type == 1) ? INTCON : CHARCON;

		if (is_str) { printf_medi_ic(STRCON, str_t, strlen(str_t)); }
		if (print_certain) { printf_medi_ii(print_type, print_value);}
		else { printf_medi_ic(print_type, print_name, 0); }
		free(print_name);
	}

	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	fprintf(fpOut, "<写语句>\n");
}

// 返回语句		::=  return['('＜表达式＞')']  
void returnStatement() {
	int type = getSymbolType(symbolTables[0], symbolTables[stIndex]->name);

	(symbol == RETURNTK && print_sym()) ? getsym() : error_and_getsym();
	if (symbol == LPARENT)
	{
		print_sym();
		getsym();

		char* return_name = (char*)malloc(STRSIZE * sizeof(char));
		int expr_value;
		bool expr_certain;

		int returnType = expression(&expr_value, &expr_certain, return_name);

		if (expr_certain) { return_medi_i(expr_value); }
		else { return_medi_c(return_name); }
		free(return_name);

		/*错误处理*/ /*无返回值的函数存在不匹配的return语句*/
		if (type == FUNC_NON_RETURN) { error(ERROR_G); }

		/*错误处理*/ /*有返回值的函数缺少return语句或存在不匹配的return语句*/
		else if (type == FUNC_HAS_RETURN_INT && returnType != 1) { error(ERROR_H); }
		else if (type == FUNC_HAS_RETURN_CHAR && returnType != 0) { error(ERROR_H); }

		symbolTables[stIndex]->returnType =
			(returnType == 1) ? FUNC_HAS_RETURN_INT : FUNC_HAS_RETURN_CHAR;

		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	}
	else { return_medi_f(getSymbol(symbolTables[0], symbolTables[stIndex]->name)); }
	
	fprintf(fpOut, "<返回语句>\n");
}


