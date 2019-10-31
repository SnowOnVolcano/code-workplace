#include "parsing.h"


// 标识符类型
int temp_idenType = 0;
// 当前的函数名
char temp_idenName[TOKENSIZE];
// 当前函数的值列表
struct list* temp_funcParaList;


// 字符串		::= "｛十进制编码为32,33,35-126的ASCII字符｝"
void string() {
	(symbol == STRCON && print_sym()) ? getsym() : error(_ERROR);
	fprintf(fpOut, "<字符串>\n");
}

// 程序			::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void program() {
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
	(symbol == CONSTTK && print_sym()) ? getsym() : error(_ERROR);
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

// 常量定义		::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞} | \
						char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ } 
void con_definition() {
	if (symbol == INTTK) {
		do
		{
			print_sym();
			getsym();
			(symbol == IDENFR && print_sym()) ? add_sym(token, ST_CONST_IDEN_INT) : error(_ERROR); // 加入符号表
			getsym();
			(symbol == ASSIGN && print_sym()) ? getsym() : error(_ERROR);
			if (!integer()) {
				error(ERROR_O);
			}
		} while (symbol == COMMA);
	}
	else if (symbol == CHARTK) {
		do
		{
			print_sym();
			getsym();
			(symbol == IDENFR && print_sym()) ? add_sym(token, ST_CONST_IDEN_CHAR) : error(_ERROR); // 加入符号表
			getsym();
			(symbol == ASSIGN && print_sym()) ? getsym() : error(_ERROR);
			(symbol == CHARCON && print_sym()) ? getsym() : error(ERROR_O);
		} while (symbol == COMMA);
	}
	else {
		error(_ERROR);
	}
	fprintf(fpOut, "<常量定义>\n");
}

// 无符号整数	::= ＜非零数字＞｛＜数字＞｝| 0
bool unsigned_integer() {
	(symbol == INTCON && print_sym()) ? getsym() : error(_ERROR);
	fprintf(fpOut, "<无符号整数>\n");
	return true;
}

// 整数			::= ［＋｜－］＜无符号整数＞
bool integer() {
	if (symbol == PLUS || symbol == MINU) {
		print_sym();
		getsym();
	}
	int temp = unsigned_integer();
	fprintf(fpOut, "<整数>\n");
	return temp;
}

// 声明头部		::=  int＜标识符＞ | char＜标识符＞
void declarator() {
	/********************* 记 录 函 数 返 回 值 类 型 ***********************/
	temp_idenType = (symbol == INTTK) ? ST_HAS_RETURN_FUNC_INT : ST_HAS_RETURN_FUNC_CHAR;
	/**********************************************************************/
	((symbol == INTTK || symbol == CHARTK) && print_sym()) ? getsym() : error(_ERROR);
	/************************ 记 录 函 数 名 称 ****************************/
	strcpy(temp_idenName, token);
	/**********************************************************************/
	(symbol == IDENFR && print_sym()) ? add_sym(temp_idenName, temp_idenType) : error(_ERROR);		// 加入符号表
	getsym();
	fprintf(fpOut, "<声明头部>\n");
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

// 变量定义		::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']') \
					{,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )}
void var_definition() {
	if (symbol == INTTK || symbol == CHARTK) {
		do {
			temp_idenType = (symbol == INTTK) ? ST_VARIABLE_IDEN_INT : ST_VARIABLE_IDEN_CHAR;
			print_sym();
			getsym();
			strcpy(temp_idenName, token);
			(symbol == IDENFR && print_sym()) ? add_sym(temp_idenName, temp_idenType) : error(_ERROR); // 加入符号表
			getsym();
			if (symbol == LBRACK) {
				temp_idenType = (temp_idenType == ST_VARIABLE_IDEN_INT) ? ST_ARRAY_IDEN_INT : ST_ARRAY_IDEN_CHAR;
				change_symType(temp_idenName, temp_idenType);
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


// 有返回值函数定义	::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
void re_func_definition() {
	declarator();
	(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
	paraList();
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	(symbol == LBRACE && print_sym()) ? getsym() : error(_ERROR);
	compoundStatement();
	(symbol == RBRACE && print_sym()) ? getsym() : error(_ERROR);
	fprintf(fpOut, "<有返回值函数定义>\n");
}

// 无返回值函数定义	::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void unre_func_definition() {
	(symbol == VOIDTK && print_sym()) ? getsym() : error(_ERROR);
	(symbol == IDENFR && print_sym()) ? add_sym(token, ST_NON_RETURN_FUNC) : error(_ERROR); // 加入符号表
	getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
	paraList();
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	(symbol == LBRACE && print_sym()) ? getsym() : error(_ERROR);
	compoundStatement();
	(symbol == RBRACE && print_sym()) ? getsym() : error(_ERROR);
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
	temp_funcParaList = list_new();
	if (symbol == INTTK || symbol == CHARTK) {
		/************************* 加 入 函 数 参 数 表*************************/
		temp_idenType = (symbol == INTTK) ? ST_VARIABLE_IDEN_INT : ST_VARIABLE_IDEN_CHAR;
		listnode_add(temp_funcParaList, temp_idenType);
		/**********************************************************************/
		print_sym();
		getsym();
		(symbol == IDENFR && print_sym()) ? add_sym(token, temp_idenType) : error(_ERROR);
		getsym();
		while (symbol == COMMA) {
			print_sym();
			getsym();
			/*********************** 加 入 函 数 参 数 表 *********************/
			temp_idenType = (symbol == INTTK) ? ST_VARIABLE_IDEN_INT : ST_VARIABLE_IDEN_CHAR;
			listnode_add(temp_funcParaList, temp_idenType);
			/*****************************************************************/
			((symbol == INTTK || symbol == CHARTK) && print_sym()) ? getsym() : error(_ERROR);
			(symbol == IDENFR && print_sym()) ? add_sym(token, temp_idenType) : error(_ERROR);
			getsym();			// 局部变量在符号表中应该怎么处置？
		}

		/**************** 向 符 号 表 添 加 进 函 数 的 参 数 列 表 ****************/
		struct SymbolTable* s = find_sym(temp_idenName);
		s->valueList = list_new();
		list_add_list(s->valueList, temp_funcParaList);
		list_free(temp_funcParaList);
		/**********************************************************************/
	}
	else {
		// 空
	}
	fprintf(fpOut, "<参数表>\n");
}

// 主函数		::= void main'('')' '{'＜复合语句＞'}'
void mainFunction() {
	(symbol == VOIDTK && print_sym()) ? getsym() : error(_ERROR);
	(symbol == MAINTK && print_sym()) ? getsym() : error(_ERROR);
	(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	(symbol == LBRACE && print_sym()) ? getsym() : error(_ERROR);
	compoundStatement();
	(symbol == RBRACE && print_sym()) ? getsym() : error(_ERROR);
	fprintf(fpOut, "<主函数>\n");
}

// 表达式		::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   #[+|-]只作用于第一个<项>#
void expression() {
	if (symbol == PLUS || symbol == MINU) {
		print_sym();
		getsym();
	}
	term();
	while (symbol == PLUS || symbol == MINU)
	{
		print_sym();
		getsym();
		term();
	}
	fprintf(fpOut, "<表达式>\n");
}

// 项			::= ＜因子＞{＜乘法运算符＞＜因子＞}
void term() {
	factor();
	while (symbol == MULT || symbol == DIV)
	{
		print_sym();
		getsym();
		factor();
	}
	fprintf(fpOut, "<项>\n");
}

// 因子			::= ＜标识符＞ ｜ ＜标识符＞'['＜表达式＞']' | '('＜表达式＞')' ｜ ＜整数＞ | ＜字符＞ ｜ ＜有返回值函数调用语句＞
void factor() {
	if (symbol == IDENFR) {
		if (find_sym == NULL) {
			error(ERROR_C);
		}
		else {
			if (get_symType(token) & ST_HAS_RETURN_FUNC) {	// 有返回值函数调用语句
				refunc_callStatement();
			}
			else if (get_symType(token) & ST_ARRAY_IDEN){	// 标识符'['表达式']' 
				print_sym();
				getsym();
				(symbol == LBRACK && print_sym()) ? getsym() : error(ERROR_M);
				if (!expression_int()) {
					error(ERROR_I);
				}
				(symbol == RBRACK && print_sym()) ? getsym() : error(ERROR_M);
			}
			else {											// 标识符
				print_sym();
				getsym();
			}
		}
	}
	else if (symbol == LPARENT) {		// '('表达式ʽ')'
		print_sym();
		getsym();
		expression();
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	}
	else if (symbol == CHARCON)			// 字符
	{
		print_sym();
		getsym();
	}
	else {								// 整数
		integer();
	}
	fprintf(fpOut, "<因子>\n");
}

// 语句			::= ＜条件语句＞｜＜循环语句＞| '{'＜语句列＞'}' |\
					＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞;｜＜赋值语句＞; ｜\
					＜读语句＞; ｜＜写语句＞; ｜＜空＞; | ＜返回语句＞;
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
		(symbol == RBRACE && print_sym()) ? getsym() : error(_ERROR);
		stateType = S_STATECOLUMN;
	}
	else if (symbol == IDENFR) {		// 有、无返回值函数调用语句 | 赋值语句
		int temp = iden_statement();
		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
		stateType = temp;
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
	if (find_sym(token) == NULL) {
		error(ERROR_C);
	}
	else {
		if (get_symType(token) & ST_HAS_RETURN_FUNC/*identifier is re*/) {
			refunc_callStatement();
			return S_REFUNCCALL;
		}
		else if (get_symType(token) == ST_NON_RETURN_FUNC/*identifier is unre*/) {
			unrefunc_callStatement();
			return S_UNREFUNCCALL;
		}
		else /*other identifiers*/ {
			assignStatement();
			return S_ASSIGN;
		}
	}
}

// 赋值语句		::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
void assignStatement() {
	if (get_symType(token) & ST_CONST_IDEN) {
		error(ERROR_J);
	}
	else {
		print_sym();
		getsym();
		if (symbol == ASSIGN) {
			print_sym();
			getsym();
			expression();
		}
		else if (symbol == LBRACK) {
			print_sym();
			getsym();
			if (!expression_int()) {
				error(ERROR_I);
			}
			(symbol == RBRACK && print_sym()) ? getsym() : error(ERROR_M);
			(symbol == ASSIGN && print_sym()) ? getsym() : error(_ERROR);
			expression();
		}
		else {
			error(_ERROR);
		}
	}
	fprintf(fpOut, "<赋值语句>\n");
}

// 条件语句		::= if '('＜条件＞')'＜语句＞［else＜语句＞］
void conditionalStatement() {
	(symbol == IFTK && print_sym()) ? getsym() : error(_ERROR);
	(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
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

// 条件			::=  ＜表达式＞＜关系运算符＞＜表达式＞ #整型表达式之间才能进行关系运算# ｜\
					 ＜表达式＞    #表达式为整型，其值为0条件为假，值不为0时条件为真#  
void condition() {
	if (!expression_int()) {
		error(ERROR_F);
	}
	if (symbol >= LSS && symbol <= NEQ) {
		print_sym();
		getsym();
		if (!expression_int()) {
			error(ERROR_F);
		}
	}
	fprintf(fpOut, "<条件>\n");
}

// 循环语句		::=  while '('＜条件＞')'＜语句＞ |\
					 do＜语句＞while '('＜条件＞')' |\
					 for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
void loopStatement() {
	if (symbol == WHILETK) {
		print_sym();
		getsym();
		(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
		condition();
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
		statement();
	}
	else if (symbol == DOTK) {
		print_sym();
		getsym();
		statement();
		(symbol == WHILETK && print_sym()) ? getsym() : error(ERROR_N);
		(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
		condition();
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	}
	else if (symbol == FORTK)
	{
		print_sym();
		getsym();
		(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
		(find_sym(token) != NULL && print_sym()) ? getsym() : error(_ERROR);
		(symbol == ASSIGN && print_sym()) ? getsym() : error(_ERROR);
		expression();
		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
		condition();
		(symbol == SEMICN && print_sym()) ? getsym() : error(ERROR_K);
		(find_sym(token) != NULL && print_sym()) ? getsym() : error(_ERROR);
		(symbol == ASSIGN && print_sym()) ? getsym() : error(_ERROR);
		(find_sym(token) != NULL && print_sym()) ? getsym() : error(_ERROR);
		((symbol == PLUS || symbol == MINU) && print_sym()) ? getsym() : error(_ERROR);
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
	strcpy(temp_idenName, token);
	(find_sym(temp_idenName) != NULL && print_sym()) ? getsym() : error(ERROR_C);
	(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
	valueparaList();
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	fprintf(fpOut, "<有返回值函数调用语句>\n");
}

// 无返回值函数调用语句	::= ＜标识符＞'('＜值参数表＞')'
void unrefunc_callStatement() {
	strcpy(temp_idenName, token);
	(find_sym(temp_idenName) != NULL && print_sym()) ? getsym() : error(ERROR_C);
	(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
	valueparaList();
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	fprintf(fpOut, "<无返回值函数调用语句>\n");
}

// 值参数表		::= ＜表达式＞{,＜表达式＞}｜＜空＞
void valueparaList() {
	int temp_count = 0;
	struct listnode* node = listhead(get_symValueList(temp_idenName));
	if (symbol != RPARENT /* 值参数表不为空 */) {
		temp_count++;
		if (node->data == ST_VARIABLE_IDEN_INT) {
			if (!expression_int()) {
				error(ERROR_E);
			}
		}
		else {
			if (!expression_char()) {
				error(ERROR_E);
			}
		}
		while (symbol == COMMA) {
			print_sym();
			getsym();
			temp_count++;
			expression();
		}
	}
	// 参数个数不匹配
	if (temp_count + 1 != find_sym(temp_idenName)->valueList->count) {
		error(ERROR_D);
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
	(symbol == SCANFTK && print_sym()) ? getsym() : error(_ERROR);
	(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
	(find_sym(token) != NULL && print_sym()) ? getsym() : error(_ERROR);
	while (symbol == COMMA)
	{
		print_sym();
		getsym();
		(find_sym(token) != NULL && print_sym()) ? getsym() : error(_ERROR);
	}
	(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	fprintf(fpOut, "<读语句>\n");
}

// 写语句		::= printf '(' ＜字符串＞,＜表达式＞ ')'| printf '('＜字符串＞ ')'| printf '('＜表达式＞')'
void printStatement() {
	(symbol == PRINTFTK && print_sym()) ? getsym() : error(_ERROR);
	(symbol == LPARENT && print_sym()) ? getsym() : error(_ERROR);
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
	(symbol == RETURNTK && print_sym()) ? getsym() : error(_ERROR);
	while (symbol == LPARENT)
	{
		print_sym();
		getsym();
		expression();
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	}
	fprintf(fpOut, "<返回语句>\n");
}


// 整型表达式	::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   #[+|-]只作用于第一个<项>#
bool expression_int() {
	if (symbol == PLUS || symbol == MINU) {
		print_sym();
		getsym();
	}
	if (!term_int()) {
		return false;
	}
	while (symbol == PLUS || symbol == MINU)
	{
		print_sym();
		getsym();
		term();
	}
	fprintf(fpOut, "<表达式>\n");
	return true;
}

// 整型项		::= ＜因子＞{＜乘法运算符＞＜因子＞}
bool term_int() {
	if (!factor_int()) {
		return false;
	}
	while (symbol == MULT || symbol == DIV)
	{
		print_sym();
		getsym();
		factor();
	}
	fprintf(fpOut, "<项>\n");
	return true;
}

// ?? 整型因子	::= ＜标识符＞ ｜ ＜标识符＞'['＜表达式＞']' | '('＜表达式＞')' ｜ ＜整数＞ | ＜字符＞ ｜ ＜有返回值函数调用语句＞
bool factor_int() {
	if (symbol == IDENFR) {
		if (find_sym == NULL) {
			error(ERROR_C);
		}
		else {
			// ？？分开考虑？？
			if (get_symType(token) == ST_HAS_RETURN_FUNC_CHAR ||
				get_symType(token) == ST_VARIABLE_IDEN_CHAR ||
				get_symType(token) == ST_CONST_IDEN_CHAR ||
				get_symType(token) == ST_ARRAY_IDEN_CHAR) {
				return false;
			}
			else if (get_symType(token) & ST_HAS_RETURN_FUNC) {	// 有返回值函数调用语句
					refunc_callStatement();
			}
			else if (get_symType(token) & ST_ARRAY_IDEN) {	// 标识符'['表达式']' 
				print_sym();
				getsym();
				(symbol == LBRACK && print_sym()) ? getsym() : error(ERROR_M);
				if (!expression_int()) {
					error(ERROR_I);
				}
				(symbol == RBRACK && print_sym()) ? getsym() : error(ERROR_M);
			}
			else {											// 标识符
				print_sym();
				getsym();
			}
		}
	}
	else if (symbol == LPARENT) {		// '('表达式ʽ')'
		print_sym();
		getsym();
		expression();
		(symbol == RPARENT && print_sym()) ? getsym() : error(ERROR_L);
	}
	else if (symbol == CHARCON)			// 字符
	{
		return false;
	}
	else {								// 整数
		integer();
	}
	fprintf(fpOut, "<因子>\n");
	return true;
}
