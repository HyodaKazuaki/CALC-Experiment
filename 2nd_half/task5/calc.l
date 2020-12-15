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
"=" return SSTI;

[0-9][0-9]*(\.[0-9][0-9]*)? {
		float temp ;
		sscanf( yytext , "%f" , &temp ) ;
		yylval.float_value = temp ;
		return FLOAT_LITERAL;
	}

[A-Za-z] {
	char temp;
	temp = yytext[0];
	yylval.char_value = temp;
	return CHAR_LITERAL;
}

%%

/** Local Variables: **/
/** mode: c++ **/
/** End: **/
