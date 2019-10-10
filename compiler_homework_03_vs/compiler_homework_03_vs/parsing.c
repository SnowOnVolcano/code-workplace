#include "parsing.h"

// 程序
void program() {
	if (symbol == CONSTTK) {
		con_info();
	}
	if (symbol == INTTK || symbol == CHARTK) {
		saveCurrentSym();
		getsym_noprint();
		getsym_noprint();
		if (symbol == LPARENT) {
			restorePreviousSym();
			re_func_definition();
		}
		else {
			restorePreviousSym();
			var_info();
		}
	}
	if (symbol == VOIDTK) {
		saveCurrentSym();
		getsym_noprint();
		if (symbol == MAINTK) {
			restorePreviousSym();
			mainFunction();
		}
		else {
			restorePreviousSym();
			unre_func_definition();
			(symbol == VOIDTK) ? getsym() : error();
			(symbol == MAINTK) ? getsym() : error();
			mainFunction();
		}
	}
	else {
		error();
	}
	fprintf(fpOut, "<程序>\n");
}

// 常量说明
void con_info() {
	(symbol == CONSTTK) ? getsym() : error();
	con_definition();
	(symbol == SEMICN) ? getsym() : error();
	while (symbol == CONSTTK)
	{
		getsym();
		con_definition();
		(symbol == SEMICN) ? getsym() : error();
	}
	fprintf(fpOut, "<常量说明>\n");
}

// 常量定义
void con_definition() {
	if (symbol == INTTK) {
		do
		{
			getsym();
			(symbol == IDENFR) ? add_sym(token, ST_CONST_IDEN) : error(); // 加入符号表
			getsym();			
			(symbol == ASSIGN) ? getsym() : error();
			integer();
		} while (symbol == COMMA);
		
	}
	else if (symbol == CHARTK) {
		getsym();
		do
		{
			(symbol == IDENFR) ? add_sym(token, ST_CONST_IDEN) : error(); // 加入符号表
			getsym();
			(symbol == ASSIGN) ? getsym() : error();
			(symbol == CHARCON) ? getsym() : error();
		} while (symbol == COMMA);
	}
	else {
		error();
	}
	fprintf(fpOut, "<常量定义>\n");
}

// 无符号整数
void unsigned_integer() {
	(symbol == INTCON) ? getsym() : error();
	fprintf(fpOut, "<无符号整数>\n");
}

// 整数
void integer() {
	if (symbol == PLUS || symbol == MINU) {
		getsym();
	}
	unsigned_integer();
	fprintf(fpOut, "<整数>\n");
}

// 声明头部
void declarator() {
	(symbol == INTTK || symbol == CHARTK) ? getsym() : error();
	(symbol == IDENFR) ? add_sym(token, ST_HAS_RETURN_FUNC) : error();		// 加入符号表
	getsym();	
	fprintf(fpOut, "<声明头部>\n");
}

// 变量说明
void var_info() {
	var_definition();
	(symbol == SEMICN) ? getsym() : error();
	while (symbol == INTTK || symbol == CHARTK) {
		var_definition();
		(symbol == SEMICN) ? getsym() : error();
	}
	fprintf(fpOut, "<变量说明>\n");
}

// 变量定义
void var_definition() {
	(symbol == INTTK || symbol == CHARTK) ? getsym() : error();
	do
	{
		(symbol == IDENFR) ? add_sym(token, ST_VARIABLE_IDEN) : error(); // 加入符号表
		getsym();
		if (symbol == LBRACK) {
			getsym();
			unsigned_integer();
			(symbol == RBRACK) ? getsym() : error();
		}
	} while (symbol == COMMA);
	fprintf(fpOut, "<变量定义>\n");
}

// 有返回值函数定义
void re_func_definition() {
	declarator();
	(symbol == LPARENT) ? getsym() : error();
	paraList();
	(symbol == RPARENT) ? getsym() : error();
	(symbol == LBRACE) ? getsym() : error();
	compoundStatement();
	(symbol == RBRACE) ? getsym() : error();
	fprintf(fpOut, "<有返回值函数定义>\n");
}

// 无返回值函数定义
void unre_func_definition() {
	(symbol == VOIDTK) ? getsym() : error();
	(symbol == IDENFR) ? add_sym(token, ST_NON_RETURN_FUNC) : error(); // 加入符号表
	getsym();
	(symbol == LPARENT) ? getsym() : error();
	paraList();
	(symbol == RPARENT) ? getsym() : error();
	(symbol == LBRACE) ? getsym() : error();
	compoundStatement();
	(symbol == RBRACE) ? getsym() : error();
	fprintf(fpOut, "<无返回值函数定义>\n");
}

// 复合语句
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

// 参数表
void paraList() {
	if (symbol == INTTK || symbol == CHARTK) {
		(symbol == IDENFR) ? getsym() : error();
		while (symbol == COMMA) {
			getsym();
			(symbol == INTTK || symbol == CHARTK) ? getsym() : error();
			(symbol == IDENFR) ? getsym() : error();
		}
	}
	else {
		// 空
	}
	fprintf(fpOut, "<参数表>\n");
}

// 主函数
void mainFunction() {
	(symbol == VOIDTK) ? getsym() : error();
	(symbol == MAINTK) ? getsym() : error();
	(symbol == LPARENT) ? getsym() : error();
	(symbol == RPARENT) ? getsym() : error();
	(symbol == LBRACE) ? getsym() : error();
	compoundStatement();
	(symbol == RBRACE) ? getsym() : error();
	fprintf(fpOut, "<主函数>\n");
}

// 表达式
void expression() {
	if (symbol == PLUS || symbol == MINU) {
		getsym();
	}
	term();
	while (symbol == PLUS || symbol == MINU)
	{
		getsym();
		term();
	}
	fprintf(fpOut, "<表达式>\n");
}

// 项
void term() {
	factor();
	while (symbol == MULT || symbol == DIV)
	{
		getsym();
		factor();
	}
	fprintf(fpOut, "<项>\n");
}

// 因子
void factor() {
	if (symbol == IDENFR) {				// 标识符 or 标识符'['表达式']' or 有返回值函数调用语句
		if (get_symType(token) == ST_HAS_RETURN_FUNC/*如果此时token是函数声明标识符*/) {
			refunc_callStatement();
		}
		else {
			getsym();
			if (symbol == LBRACK) {
				expression();
				(symbol == RBRACK) ? getsym() : error();
			}
		}
	}
	else if (symbol == LPARENT) {		// '('表达式ʽ')'
		expression();
		(symbol == RPARENT) ? getsym() : error();
	}
	else if (symbol == CHARCON)			// 字符
	{
		getsym();
	}
	else {								// 整数
		integer();
	}
	fprintf(fpOut, "<因子>\n");
}

// 语句
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
		getsym();
		statecolumn();
		(symbol == RBRACE) ? getsym() : error();
		stateType = S_STATECOLUMN;
	}
	else if (symbol == IDENFR) {		// 有、无返回值函数调用语句 | 赋值语句
		int temp = iden_statement();
		(symbol == SEMICN) ? getsym() : error();
		stateType = temp;
	}
	else if (symbol == SCANFTK) {		// 读语句
		getsym();
		readStatement();
		(symbol == SEMICN) ? getsym() : error();
		stateType = S_READ;
	}
	else if (symbol == PRINTFTK) {		// 写语句
		getsym();
		printStatement();
		(symbol == SEMICN) ? getsym() : error();
		stateType = S_PRINT;
	}
	else if (symbol == SEMICN) {		// 空语句
		getsym();
		stateType = S_NULL;
	}
	else if (symbol == RETURNTK) {		// 返回语句
		getsym();
		readStatement();
		(symbol == SEMICN) ? getsym() : error();
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

// 赋值语句
void assignStatement() {
	(symbol == IDENFR) ? getsym() : error();
	if (symbol == ASSIGN) {
		getsym();
		expression();
	}
	else if (symbol == LBRACK) {
		getsym();
		expression();
		(symbol == RBRACK) ? getsym() : error();
		(symbol == ASSIGN) ? getsym() : error();
		expression();
	}
	else {
		error();
	}
	fprintf(fpOut, "<赋值语句>\n");
}

// 有返回值函数调用语句
void refunc_callStatement() {
	(symbol == IDENFR) ? getsym() : error();
	(symbol == LPARENT) ? getsym() : error();

	valueparaList();
	(symbol == RPARENT) ? getsym() : error();

	fprintf(fpOut, "<有返回值函数调用语句>\n");
}

// 无返回值函数调用语句
void unrefunc_callStatement() {
	(symbol == IDENFR) ? getsym() : error();
	(symbol == LPARENT) ? getsym() : error();

	valueparaList();
	(symbol == RPARENT) ? getsym() : error();

	fprintf(fpOut, "<无返回值函数调用语句>\n");
}

// 条件语句
void conditionalStatement() {
	(symbol == IFTK) ? getsym() : error();
	(symbol == LPARENT) ? getsym() : error();
	condition();
	(symbol == RPARENT) ? getsym() : error();
	statement();
	if (symbol == ELSETK) {
		getsym();
		statement();
	}
	fprintf(fpOut, "<条件语句>\n");
}

// 条件
void condition() {
	expression();
	if (symbol >= LSS && symbol <= NEQ) {
		getsym();
		expression();
	}
	fprintf(fpOut, "<条件>\n");
}

// 循环语句
void loopStatement() {
	if (symbol == WHILETK) {
		getsym();
		(symbol == LPARENT) ? getsym() : error();
		condition();
		(symbol == RPARENT) ? getsym() : error();
		statement();
	}
	else if (symbol == DOTK) {
		getsym();
		statement();
		(symbol == WHILETK) ? getsym() : error();
		(symbol == LPARENT) ? getsym() : error();
		condition();
		(symbol == RPARENT) ? getsym() : error();
	}
	else if (symbol == FORTK)
	{
		getsym();
		(symbol == LPARENT) ? getsym() : error();
		(symbol == IDENFR) ? getsym() : error();
		(symbol == ASSIGN) ? getsym() : error();
		expression();
		(symbol == SEMICN) ? getsym() : error();
		condition();
		(symbol == SEMICN) ? getsym() : error();
		(symbol == IDENFR) ? getsym() : error();
		(symbol == ASSIGN) ? getsym() : error();
		(symbol == IDENFR) ? getsym() : error();
		(symbol == PLUS || symbol == MINU) ? getsym() : error();
		stride();
		(symbol == RPARENT) ? getsym() : error();
		statement();
	}
	else {
		error();
	}
	fprintf(fpOut, "<循环语句>\n");
}

// 步长
void stride() {
	unsigned_integer();
}

// 值参数表
void valueparaList() {
	if (symbol != RPARENT /* 值参数表不为空 */) {
		do
		{
			expression();
		} while (symbol == COMMA);
	}
	fprintf(fpOut, "<值参数表>\n");
}

// 语句列
void statecolumn() {
	for (;statement() >= 0;);
	fprintf(fpOut, "<语句列>\n");
}

// 读语句
void readStatement() {
	(symbol == LPARENT) ? getsym() : error();
	(symbol == IDENFR) ? getsym() : error();
	while (symbol == COMMA)
	{
		getsym();
		(symbol == IDENFR) ? getsym() : error();
	}
	(symbol == RPARENT) ? getsym() : error();
	fprintf(fpOut, "<读语句>\n");
}

// 写语句
void printStatement() {
	(symbol == LPARENT) ? getsym() : error();
	if (symbol == STRCON) {
		getsym();
		if (symbol == COMMA) {
			getsym();
			expression();
		}
	}
	else {
		expression();
	}
	(symbol == RPARENT) ? getsym() : error();
	fprintf(fpOut, "<写语句>\n");
}

// 返回语句
void returnStatement() {
	while (symbol == LPARENT)
	{
		getsym();
		expression();
		(symbol == RPARENT) ? getsym() : error();
	}
	fprintf(fpOut, "<返回语句>\n");
}

