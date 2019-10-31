#include "parsing.h"

// 字符串		::= "｛十进制编码为32,33,35-126的ASCII字符｝"
void string() {
	(symbol == STRCON && print_sym()) ? getsym() : error();
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
	(symbol == CONSTTK && print_sym()) ? getsym() : error();
	con_definition();
	(symbol == SEMICN && print_sym()) ? getsym() : error();
	while (symbol == CONSTTK)
	{
		print_sym();
		getsym();
		con_definition();
		(symbol == SEMICN && print_sym()) ? getsym() : error();
	}
	fprintf(fpOut, "<常量说明>\n");
}

/* 常量定义		::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞} | 
						char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ } */
void con_definition() {
	if (symbol == INTTK) {
		do
		{
			print_sym();
			getsym();
			(symbol == IDENFR && print_sym()) ? add_sym(token, ST_CONST_IDEN) : error(); // 加入符号表
			getsym();			
			(symbol == ASSIGN && print_sym()) ? getsym() : error();
			integer();
		} while (symbol == COMMA);
	}
	else if (symbol == CHARTK) {
		do
		{
			print_sym();
			getsym(); 
			(symbol == IDENFR && print_sym()) ? add_sym(token, ST_CONST_IDEN) : error(); // 加入符号表
			getsym();
			(symbol == ASSIGN && print_sym()) ? getsym() : error();
			(symbol == CHARCON && print_sym()) ? getsym() : error();
		} while (symbol == COMMA);
	}
	else {
		error();
	}
	fprintf(fpOut, "<常量定义>\n");
}

// 无符号整数	::= ＜非零数字＞｛＜数字＞｝| 0
void unsigned_integer() {
	(symbol == INTCON && print_sym()) ? getsym() : error();
	fprintf(fpOut, "<无符号整数>\n");
}

// 整数			::= ［＋｜－］＜无符号整数＞
void integer() {
	if (symbol == PLUS || symbol == MINU) {
		print_sym();
		getsym();
	}
	unsigned_integer();
	fprintf(fpOut, "<整数>\n");
}

// 声明头部		::=  int＜标识符＞ | char＜标识符＞
void declarator() {
	((symbol == INTTK || symbol == CHARTK) && print_sym()) ? getsym() : error();
	(symbol == IDENFR && print_sym()) ? add_sym(token, ST_HAS_RETURN_FUNC) : error();		// 加入符号表
	getsym();	
	fprintf(fpOut, "<声明头部>\n");
}

// 变量说明		::= ＜变量定义＞;{＜变量定义＞;}
void var_info() {
	var_definition();
	(symbol == SEMICN && print_sym()) ? getsym() : error();
	while (symbol == INTTK || symbol == CHARTK) {
		saveCurrentSym();
		getsym();
		getsym();
		if (symbol != LPARENT) {
			restorePreviousSym();
			var_definition();
			(symbol == SEMICN && print_sym()) ? getsym() : error();
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
	if (symbol == INTTK || symbol == CHARTK) {
		do {
			print_sym();
			getsym();
			(symbol == IDENFR && print_sym()) ? add_sym(token, ST_VARIABLE_IDEN) : error(); // 加入符号表
			getsym();
			if (symbol == LBRACK) {
				print_sym();
				getsym();
				unsigned_integer();
				(symbol == RBRACK && print_sym()) ? getsym() : error();
			}
		} while (symbol == COMMA);
	}
	else {
		error();
	}

	fprintf(fpOut, "<变量定义>\n");
}

// 类型标识符	::=  int | char


// 有返回值函数定义	::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
void re_func_definition() {
	declarator();
	(symbol == LPARENT && print_sym()) ? getsym() : error();
	paraList();
	(symbol == RPARENT && print_sym()) ? getsym() : error();
	(symbol == LBRACE && print_sym()) ? getsym() : error();
	compoundStatement();
	(symbol == RBRACE && print_sym()) ? getsym() : error();
	fprintf(fpOut, "<有返回值函数定义>\n");
}

// 无返回值函数定义	::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void unre_func_definition() {
	(symbol == VOIDTK && print_sym()) ? getsym() : error();
	(symbol == IDENFR && print_sym()) ? add_sym(token, ST_NON_RETURN_FUNC) : error(); // 加入符号表
	getsym();
	(symbol == LPARENT && print_sym()) ? getsym() : error();
	paraList();
	(symbol == RPARENT && print_sym()) ? getsym() : error();
	(symbol == LBRACE && print_sym()) ? getsym() : error();
	compoundStatement();
	(symbol == RBRACE && print_sym()) ? getsym() : error();
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
	if (symbol == INTTK || symbol == CHARTK) {
		print_sym();
		getsym();
		(symbol == IDENFR && print_sym()) ? add_sym(token, ST_VARIABLE_IDEN) : error();
		getsym();
		while (symbol == COMMA) {
			print_sym();
			getsym();
			((symbol == INTTK || symbol == CHARTK) && print_sym()) ? getsym() : error();
			(symbol == IDENFR && print_sym()) ? add_sym(token, ST_VARIABLE_IDEN) : error();
			getsym();			// 局部变量在符号表中应该怎么处置？
		}
	}
	else {
		// 空
	}
	fprintf(fpOut, "<参数表>\n");
}

// 主函数		::= void main'('')' '{'＜复合语句＞'}'
void mainFunction() {
	(symbol == VOIDTK && print_sym()) ? getsym() : error();
	(symbol == MAINTK && print_sym()) ? getsym() : error();
	(symbol == LPARENT && print_sym()) ? getsym() : error();
	(symbol == RPARENT && print_sym()) ? getsym() : error();
	(symbol == LBRACE && print_sym()) ? getsym() : error();
	compoundStatement();
	(symbol == RBRACE && print_sym()) ? getsym() : error();
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
		if (get_symType(token) == ST_HAS_RETURN_FUNC) { // 有返回值函数调用语句
			refunc_callStatement();
		}
		else {							// 标识符 or 标识符'['表达式']' 
			print_sym();
			getsym();
			if (symbol == LBRACK) {
				print_sym();
				getsym();
				expression();
				(symbol == RBRACK && print_sym()) ? getsym() : error();
			}
		}
	}
	else if (symbol == LPARENT) {		// '('表达式ʽ')'
		print_sym();
		getsym();
		expression();
		(symbol == RPARENT && print_sym()) ? getsym() : error();
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
		(symbol == RBRACE && print_sym()) ? getsym() : error();
		stateType = S_STATECOLUMN;
	}
	else if (symbol == IDENFR) {		// 有、无返回值函数调用语句 | 赋值语句
		int temp = iden_statement();
		(symbol == SEMICN && print_sym()) ? getsym() : error();
		stateType = temp;
	}
	else if (symbol == SCANFTK) {		// 读语句
		readStatement();
		(symbol == SEMICN && print_sym()) ? getsym() : error();
		stateType = S_READ;
	}
	else if (symbol == PRINTFTK) {		// 写语句
		printStatement();
		(symbol == SEMICN && print_sym()) ? getsym() : error();
		stateType = S_PRINT;
	}
	else if (symbol == SEMICN) {		// 空语句
		print_sym();
		getsym();
		stateType = S_NULL;
	}
	else if (symbol == RETURNTK) {		// 返回语句
		returnStatement();
		(symbol == SEMICN && print_sym()) ? getsym() : error();
		stateType = S_RETURN;
	}
	else {
		error();
		return _ERROR;
	}
	fprintf(fpOut, "<语句>\n");
	return stateType;
}

// 标识符开头的语句
int iden_statement() {
	if (get_symType(token) < 0) {
		error();
		return _ERROR;
	}
	else if (get_symType(token) == ST_HAS_RETURN_FUNC/*identifier is re*/) {
		refunc_callStatement();
		return S_REFUNCCALL;
	}
	else if (get_symType(token) == ST_NON_RETURN_FUNC/*identifier is unre*/) {
		unrefunc_callStatement();
		return S_UNREFUNCCALL;
	}
	else /*other identifiers*/{
		assignStatement();
		return S_ASSIGN;
	}
}

// 赋值语句		::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
void assignStatement() {
	(symbol == IDENFR && print_sym()) ? getsym() : error();
	if (symbol == ASSIGN) {
		print_sym();
		getsym();
		expression();
	}
	else if (symbol == LBRACK) {
		print_sym();
		getsym();
		expression();
		(symbol == RBRACK && print_sym()) ? getsym() : error();
		(symbol == ASSIGN && print_sym()) ? getsym() : error();
		expression();
	}
	else {
		error();
	}
	fprintf(fpOut, "<赋值语句>\n");
}

// 条件语句		::= if '('＜条件＞')'＜语句＞［else＜语句＞］
void conditionalStatement() {
	(symbol == IFTK && print_sym()) ? getsym() : error();
	(symbol == LPARENT && print_sym()) ? getsym() : error();
	condition();
	(symbol == RPARENT && print_sym()) ? getsym() : error();
	statement();
	if (symbol == ELSETK) {
		print_sym();
		getsym();
		statement();
	}
	fprintf(fpOut, "<条件语句>\n");
}

/* 条件			::=  ＜表达式＞＜关系运算符＞＜表达式＞ #整型表达式之间才能进行关系运算# ｜
					 ＜表达式＞    #表达式为整型，其值为0条件为假，值不为0时条件为真#  */
void condition() {
	expression();
	if (symbol >= LSS && symbol <= NEQ) {
		print_sym();
		getsym();
		expression();
	}
	fprintf(fpOut, "<条件>\n");
}

/* 循环语句		::=  while '('＜条件＞')'＜语句＞ | 
					 do＜语句＞while '('＜条件＞')' |
					 for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞ */
void loopStatement() {
	if (symbol == WHILETK) {
		print_sym();
		getsym();
		(symbol == LPARENT && print_sym()) ? getsym() : error();
		condition();
		(symbol == RPARENT && print_sym()) ? getsym() : error();
		statement();
	}
	else if (symbol == DOTK) {
		print_sym();
		getsym();
		statement();
		(symbol == WHILETK && print_sym()) ? getsym() : error();
		(symbol == LPARENT && print_sym()) ? getsym() : error();
		condition();
		(symbol == RPARENT && print_sym()) ? getsym() : error();
	}
	else if (symbol == FORTK)
	{
		print_sym();
		getsym();
		(symbol == LPARENT && print_sym()) ? getsym() : error();
		(symbol == IDENFR && print_sym()) ? getsym() : error();
		(symbol == ASSIGN && print_sym()) ? getsym() : error();
		expression();
		(symbol == SEMICN && print_sym()) ? getsym() : error();
		condition();
		(symbol == SEMICN && print_sym()) ? getsym() : error();
		(symbol == IDENFR && print_sym()) ? getsym() : error();
		(symbol == ASSIGN && print_sym()) ? getsym() : error();
		(symbol == IDENFR && print_sym()) ? getsym() : error();
		((symbol == PLUS || symbol == MINU) && print_sym()) ? getsym() : error();
		stride();
		(symbol == RPARENT && print_sym()) ? getsym() : error();
		statement();
	}
	else {
		error();
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
	(symbol == IDENFR && print_sym()) ? getsym() : error();
	(symbol == LPARENT && print_sym()) ? getsym() : error();

	valueparaList();
	(symbol == RPARENT && print_sym()) ? getsym() : error();

	fprintf(fpOut, "<有返回值函数调用语句>\n");
}

// 无返回值函数调用语句	::= ＜标识符＞'('＜值参数表＞')'
void unrefunc_callStatement() {
	(symbol == IDENFR && print_sym()) ? getsym() : error();
	(symbol == LPARENT && print_sym()) ? getsym() : error();

	valueparaList();
	(symbol == RPARENT && print_sym()) ? getsym() : error();

	fprintf(fpOut, "<无返回值函数调用语句>\n");
}

// 值参数表		::= ＜表达式＞{,＜表达式＞}｜＜空＞
void valueparaList() {
	if (symbol != RPARENT /* 值参数表不为空 */) {
		expression();
		while (symbol == COMMA) {
			print_sym();
			getsym();
			expression();
		}
	}
	fprintf(fpOut, "<值参数表>\n");
}

// 语句列		::= ｛＜语句＞｝
void statecolumn() {
	for (;statement() >= 0;);
	fprintf(fpOut, "<语句列>\n");
}

// 读语句		::=  scanf '('＜标识符＞{,＜标识符＞}')'
void readStatement() {
	(symbol == SCANFTK && print_sym()) ? getsym() : error();
	(symbol == LPARENT && print_sym()) ? getsym() : error();
	(symbol == IDENFR && print_sym()) ? getsym() : error();
	while (symbol == COMMA)
	{
		print_sym();
		getsym();
		(symbol == IDENFR && print_sym()) ? getsym() : error();
	}
	(symbol == RPARENT && print_sym()) ? getsym() : error();
	fprintf(fpOut, "<读语句>\n");
}

// 写语句		::= printf '(' ＜字符串＞,＜表达式＞ ')'| printf '('＜字符串＞ ')'| printf '('＜表达式＞')'
void printStatement() {
	(symbol == PRINTFTK && print_sym()) ? getsym() : error();
	(symbol == LPARENT && print_sym()) ? getsym() : error();
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
	(symbol == RPARENT && print_sym()) ? getsym() : error();
	fprintf(fpOut, "<写语句>\n");
}

// 返回语句		::=  return['('＜表达式＞')']  
void returnStatement() {
	(symbol == RETURNTK && print_sym()) ? getsym() : error();
	while (symbol == LPARENT)
	{
		print_sym();
		getsym();
		expression();
		(symbol == RPARENT && print_sym()) ? getsym() : error();
	}
	fprintf(fpOut, "<返回语句>\n");
}

