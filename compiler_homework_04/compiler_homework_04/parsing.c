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
				mainFunction();
			}
			else {
				restorePreviousSym();
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
	if (symbol == INTTK) {
		do
		{
			print_sym();
			getsym();
			
			(symbol == IDENFR && print_sym()) /*加入符号表*/
				? (r = addSymbol(symbolTables[stIndex], token, CONST_INT))
				: error(_ERROR);
			
			/*错误处理*/ /*名字重定义*/
			if (r < 0) { error(ERROR_B); }
			
			getsym();
			(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();
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

			(symbol == IDENFR && print_sym()) /*加入符号表*/
				? (r = addSymbol(symbolTables[stIndex], token, CONST_CHAR))
				: error(_ERROR);
			
			/*错误处理*/ /*名字重定义*/
			if (r < 0) error(ERROR_B);

			getsym();
			(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();
			(symbol == CHARCON && print_sym()) ? getsym() : error_and_getsym();

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
		do {
			int type = (symbol == INTTK) ? VARIABLE_INT : VARIABLE_CHAR;
			print_sym();
			getsym();

			(symbol == IDENFR && print_sym()) /*加入符号表*/
				? (r = addSymbol(symbolTables[stIndex], token, type))
				: error(_ERROR);
			
			/*错误处理*/ /*名字重定义*/
			if (r < 0) error(ERROR_B);

			char curname[TOKENSIZE];
			strcpy(curname, token);

			getsym();
			if (symbol == LBRACK) {
				type = (type == VARIABLE_INT) ? ARRAY_INT : ARRAY_CHAR;
				addSymbolType(symbolTables[stIndex], curname, type); /*加入符号表*/
				
				print_sym();
				getsym();
				unsigned_integer();
				(symbol == RBRACK && print_sym()) ? getsym() : error(ERROR_M);
			}
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
	
	getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	paraList();
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	(symbol == LBRACE && print_sym()) ? getsym() : error_and_getsym();
	compoundStatement();
	(symbol == RBRACE && print_sym()) ? getsym() : error_and_getsym();
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

	getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	(symbol == LBRACE && print_sym()) ? getsym() : error_and_getsym();
	compoundStatement();

	(symbol == RBRACE && print_sym()) ? getsym() : error_and_getsym();
	fprintf(fpOut, "<主函数>\n");
}

// 表达式		::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   #[+|-]只作用于第一个<项>#
int expression() {
	int type = 0;
	if (symbol == PLUS || symbol == MINU) {
		type = 1;
		print_sym();
		getsym();
	}
	type = term() | type;
	if (symbol == PLUS || symbol == MINU) { type = 1; }
	while (symbol == PLUS || symbol == MINU)
	{
		print_sym();
		getsym();
		term();
	}
	fprintf(fpOut, "<表达式>\n");
	return type;
}

// 项			::= ＜因子＞{＜乘法运算符＞＜因子＞}
int term() {
	int type = 0;
	type = factor() | type;
	if (symbol == MULT || symbol == DIV) { type = 1; }
	while (symbol == MULT || symbol == DIV)
	{
		print_sym();
		getsym();
		factor();
	}
	fprintf(fpOut, "<项>\n");
	return type;
}

// 因子			::= ＜标识符＞ ｜ ＜标识符＞'['＜表达式＞']' | '('＜表达式＞')' ｜ ＜整数＞ | ＜字符＞ ｜ ＜有返回值函数调用语句＞
int factor() {
	int type = 0;
	if (symbol == IDENFR) {
		if (getSymbolType(symbolTables[0], token) == FUNC_HAS_RETURN_INT
			|| getSymbolType(symbolTables[0], token) == FUNC_HAS_RETURN_CHAR) {
			// 有返回值函数调用语句
			type = (getSymbolType(symbolTables[0], token) == FUNC_HAS_RETURN_INT);
			refunc_callStatement();
		}
		else if (getSymbolTypeFromTwo() == VARIABLE_INT 
			|| getSymbolTypeFromTwo() == VARIABLE_CHAR
			|| getSymbolTypeFromTwo() == CONST_INT 
			|| getSymbolTypeFromTwo() == CONST_CHAR) {
			// 变量或常量
			type = (getSymbolTypeFromTwo() == VARIABLE_INT 
				|| getSymbolTypeFromTwo() == CONST_INT);
			print_sym();
			getsym();
		}
		else if (getSymbolTypeFromTwo() == ARRAY_INT
			|| getSymbolTypeFromTwo() == ARRAY_CHAR) {
			// 数组
			type = (getSymbolTypeFromTwo() == ARRAY_INT);
			print_sym();
			getsym();
			(symbol == LBRACK && print_sym()) ? getsym() : error_and_getsym();
			type = expression();

			/*错误处理*/ /*数组元素的下标只能是整型表达式*/
			if (type == 0) { error(ERROR_I); }

			(symbol == RBRACK && print_sym()) ? getsym() : error(ERROR_M);
		}
		else {	
			type = -1;
			
			/*错误处理*/ /*未定义的名字*/
			if (getSymbolTypeFromTwo() < 0) { error(ERROR_C); getsym(); }
			
			else { error_and_getsym(); }
		}
	}
	else if (symbol == LPARENT) {		// '('表达式')'
		type = 1;
		print_sym();
		getsym();
		expression();
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	}
	else if (symbol == CHARCON)			// 字符
	{
		type = 0;
		print_sym();
		getsym();
	}
	else {								// 整数
		type = 1;
		integer();
	}
	fprintf(fpOut, "<因子>\n");
	return type;
}

/* 语句			::= ＜条件语句＞｜＜循环语句＞| '{'＜语句列＞'}' | 
					＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞;｜＜赋值语句＞; ｜
					＜读语句＞; ｜＜写语句＞; ｜＜空＞; | ＜返回语句＞; */
int statement() {
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
			getsym();
			stateType = _ERROR_GET;
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
	return stateType;
}

// 标识符开头的语句
int iden_statement() {
	if (getSymbolType(symbolTables[0], token) == FUNC_HAS_RETURN_INT
		|| getSymbolType(symbolTables[0], token) == FUNC_HAS_RETURN_CHAR) {
		// 有返回值函数调用语句
		refunc_callStatement();
		return S_REFUNCCALL;
	}
	else if (getSymbolType(symbolTables[0], token) == FUNC_NON_RETURN) {
		// 无返回值函数调用语句
		unrefunc_callStatement();
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
	if (getSymbolTypeFromTwo() == VARIABLE_INT 
		|| getSymbolTypeFromTwo() == VARIABLE_CHAR) {
		print_sym();
		getsym();
		(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();
		expression();
	}
	else if (getSymbolTypeFromTwo() == ARRAY_INT 
		||getSymbolTypeFromTwo() == ARRAY_CHAR) {
		print_sym();
		getsym();
		(symbol == LBRACK && print_sym()) ? getsym() : error_and_getsym();
		type = expression();

		/*错误处理*/ /*数组元素的下标只能是整型表达式*/
		if (type == 0) { error(ERROR_I); }

		(symbol == RBRACK && print_sym()) ? getsym() : error(ERROR_M);
		(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();
		expression();
	}
	else if (getSymbolTypeFromTwo() == CONST_INT
		|| getSymbolTypeFromTwo() == CONST_CHAR) {
		
		/*错误处理*/ /*不能改变常量的值*/
		error(ERROR_J);
	}
	else {
		error(_ERROR);
	}
	fprintf(fpOut, "<赋值语句>\n");
}

// 条件语句		::= if '('＜条件＞')'＜语句＞［else＜语句＞］
void conditionalStatement() {
	(symbol == IFTK && print_sym()) ? getsym() : error_and_getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	condition();
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	statement();
	if (symbol == ELSETK) {
		print_sym();
		getsym();
		statement();
	}
	fprintf(fpOut, "<条件语句>\n");
}

/* 条件			::=  ＜表达式＞＜关系运算符＞＜表达式＞ #整型表达式之间才能进行关系运算# ｜
					 ＜表达式＞    #表达式为整型，其值为0条件为假，值不为0时条件为真# */
void condition() {
	int type = 0;
	type = expression();

	/*错误处理*/ /*条件判断中出现不合法的类型*/
	if (type == 0) { error(ERROR_F); }

	if (symbol >= LSS && symbol <= NEQ) {
		print_sym();
		getsym();
		type = expression();

		/*错误处理*/ /*条件判断中出现不合法的类型*/
		if (type == 0) { error(ERROR_F); }

	}
	fprintf(fpOut, "<条件>\n");
}

/* 循环语句		::=  while '('＜条件＞')'＜语句＞ | 
					 do＜语句＞while '('＜条件＞')' | 
					 for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞*/
void loopStatement() {
	if (symbol == WHILETK) {
		print_sym();
		getsym();
		(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
		condition();
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
		statement();
	}
	else if (symbol == DOTK) {
		print_sym();
		getsym();
		statement();
		
		/*错误处理*/ /*do-while应为语句中缺少while*/
		(symbol == WHILETK && print_sym()) ? getsym() : error(ERROR_N);
		
		(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
		condition();
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	}
	else if (symbol == FORTK)
	{
		print_sym();
		getsym();
		(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
		
		/*错误处理*/ /*未定义的名字*/
		if (getSymbolTypeFromTwo() < 0) { error(ERROR_C); getsym(); }
		
		else (getSymbolTypeFromTwo() == VARIABLE_INT && print_sym()) ? getsym() : error_and_getsym();
		
		(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();
		expression();
		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
		condition();
		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);

		/*错误处理*/ /*未定义的名字*/
		if (getSymbolTypeFromTwo() < 0) { error(ERROR_C); getsym(); }
		
		else (getSymbolTypeFromTwo() == VARIABLE_INT && print_sym()) ? getsym() : error_and_getsym();
		
		(symbol == ASSIGN && print_sym()) ? getsym() : error_and_getsym();
		
		/*错误处理*/ /*未定义的名字*/
		if (getSymbolTypeFromTwo() < 0) { error(ERROR_C); getsym(); }
		
		else (getSymbolTypeFromTwo() == VARIABLE_INT && print_sym()) ? getsym() : error_and_getsym();
		
		((symbol == PLUS || symbol == MINU) && print_sym()) ? getsym() : error_and_getsym();
		stride();
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
		statement();
	}
	else {
		error(_ERROR);
	}
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
	Node_t* node = getListTailNode(paraList);

	/*错误处理*/ /*函数参数个数不匹配*/
	if ((node == NULL && symbol != RPARENT)
		|| (node != NULL && symbol == RPARENT))
	{
		error(ERROR_D);
		while (symbol != RPARENT) { getsym(); }
	}

	else if (node != NULL && symbol != RPARENT) {
		int nodeType = node->data;
		sum++;
		type = expression();

		/*错误分析*/ /*函数参数类型不匹配*/
		if ((node->data == VARIABLE_INT && type != 1)
			|| (node->data == VARIABLE_CHAR && type != 0)) {
			error(ERROR_E);
		}

		while (symbol == COMMA) {
			print_sym();
			getsym();
			sum++;
			type = expression();

			/*错误分析*/ /*函数参数类型不匹配*/
			if ((node->data == VARIABLE_INT && type != 1)
				|| (node->data == VARIABLE_CHAR && type != 0)) {
				error(ERROR_E);
			}
		}

		/*错误处理*/ /*函数参数个数不匹配*/
		if (sum != paraList->n) { error(ERROR_D); }
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
	(symbol == SCANFTK && print_sym()) ? getsym() : error_and_getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	
	/*错误处理*/ /*未定义的名字*/
	if (getSymbolTypeFromTwo() < 0) { error(ERROR_C); getsym(); }

	else (getSymbolTypeFromTwo() == VARIABLE_INT
		|| getSymbolTypeFromTwo() == VARIABLE_CHAR)
		? getsym() : error_and_getsym();

	while (symbol == COMMA)
	{
		print_sym();
		getsym();

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
	(symbol == PRINTFTK && print_sym()) ? getsym() : error_and_getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error_and_getsym();
	if (symbol == STRCON) {
		string();
		if (symbol == COMMA) {
			print_sym();
			getsym();
			expression();
		}
	}
	else {
		expression();
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
		int returnType = expression();

		/*错误处理*/ /*无返回值的函数存在不匹配的return语句*/
		if (type == FUNC_NON_RETURN) { error(ERROR_G); }
		
		/*错误处理*/ /*有返回值的函数缺少return语句或存在不匹配的return语句*/
		else if (type == FUNC_HAS_RETURN_INT && returnType != 1) { error(ERROR_H); }
		else if (type == FUNC_HAS_RETURN_CHAR && returnType != 0) { error(ERROR_H); }

		symbolTables[stIndex]->returnType = 
			(returnType == 1) ? FUNC_HAS_RETURN_INT : FUNC_HAS_RETURN_CHAR;
		
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	}
	fprintf(fpOut, "<返回语句>\n");
}


	