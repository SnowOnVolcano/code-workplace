#include "parsing.h"

// 字符串
void string() {
	(symbol == STRCON && print_sym()) ? getsym() : error();
	fprintf(fpOut, "<字符串>\n");
}

// 程序
void program() {
	while (symbol == CONSTTK) {
		con_info();
	}
	while (symbol == INTTK || symbol == CHARTK) {
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
	while (symbol == VOIDTK) {
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
	fprintf(fpOut, "<程序>\n");
}

// 常量说明
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

// 常量定义
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

// 无符号整数
void unsigned_integer() {
	(symbol == INTCON && print_sym()) ? getsym() : error();
	fprintf(fpOut, "<无符号整数>\n");
}

// 整数
void integer() {
	if (symbol == PLUS || symbol == MINU) {
		print_sym();
		getsym();
	}
	unsigned_integer();
	fprintf(fpOut, "<整数>\n");
}

// 声明头部
void declarator() {
	((symbol == INTTK || symbol == CHARTK) && print_sym()) ? getsym() : error();
	(symbol == IDENFR && print_sym()) ? add_sym(token, ST_HAS_RETURN_FUNC) : error();		// 加入符号表
	getsym();	
	fprintf(fpOut, "<声明头部>\n");
}

// 变量说明
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

// 变量定义
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

// 有返回值函数定义
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

// 无返回值函数定义
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
		print_sym();
		getsym();
		(symbol == IDENFR && print_sym()) ? getsym() : error();
		while (symbol == COMMA) {
			print_sym();
			getsym();
			((symbol == INTTK || symbol == CHARTK) && print_sym()) ? getsym() : error();
			(symbol == IDENFR && print_sym()) ? getsym() : error(); // 局部变量在符号表中应该怎么处置？
		}
	}
	else {
		// 空
	}
	fprintf(fpOut, "<参数表>\n");
}

// 主函数
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

// 表达式
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

// 项
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

// 因子
void factor() {
	if (symbol == IDENFR) {				// 标识符 or 标识符'['表达式']' or 有返回值函数调用语句
		if (get_symType(token) == ST_HAS_RETURN_FUNC/*如果此时token是函数声明标识符*/) {
			refunc_callStatement();
		}
		else {
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

// 赋值语句
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

// 有返回值函数调用语句
void refunc_callStatement() {
	(symbol == IDENFR && print_sym()) ? getsym() : error();
	(symbol == LPARENT && print_sym()) ? getsym() : error();

	valueparaList();
	(symbol == RPARENT && print_sym()) ? getsym() : error();

	fprintf(fpOut, "<有返回值函数调用语句>\n");
}

// 无返回值函数调用语句
void unrefunc_callStatement() {
	(symbol == IDENFR && print_sym()) ? getsym() : error();
	(symbol == LPARENT && print_sym()) ? getsym() : error();

	valueparaList();
	(symbol == RPARENT && print_sym()) ? getsym() : error();

	fprintf(fpOut, "<无返回值函数调用语句>\n");
}

// 条件语句
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

// 条件
void condition() {
	expression();
	if (symbol >= LSS && symbol <= NEQ) {
		print_sym();
		getsym();
		expression();
	}
	fprintf(fpOut, "<条件>\n");
}

// 循环语句
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

// 步长
void stride() {
	unsigned_integer();
	fprintf(fpOut, "<步长>\n");
}

// 值参数表
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

// 语句列
void statecolumn() {
	for (;statement() >= 0;);
	fprintf(fpOut, "<语句列>\n");
}

// 读语句
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

// 写语句
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

// 返回语句
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

