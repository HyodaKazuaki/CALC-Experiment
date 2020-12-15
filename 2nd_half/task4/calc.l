%{
// lex+yacc を用いた式の計算の字句解析部分
#include <stdio.h>

// yaccが出力するヘッダファイル
#include "y.tab.h"

int yywrap( void )
{
	// 1: スキャナ終了
	// 0: yyin を切り替えて継続
	return 1 ;
}
%}

%%
"+"	return ADD ;
"-"	return SUB ;
"*"	return MUL ;
"/"	return DIV ;
"%" return MOD;
"\n"	return CR  ;
"("	return PARL;
")"	return PARR;
"^" return EXP;
"E" return SCI;

[0-9][0-9]*(\.[0-9][0-9]*)? {
		float temp ;
		sscanf( yytext , "%f" , &temp ) ;
		yylval.float_value = temp ;
		return FLOAT_LITERAL;
	}

%%

/** Local Variables: **/
/** mode: c++ **/
/** End: **/
