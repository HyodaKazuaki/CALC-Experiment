// 丸括弧、+-符号に対応
// 小数に対応
%{
// lex+yacc を用いた式の計算の構文解析部分
#include <stdio.h>
#include <stdlib.h>

// yacc が定義する内部関数のプロトタイプ宣言
#define YYDEBUG 1
extern  int     yydebug ;
extern	int	yyerror( char const* ) ;
extern	char	*yytext ;
extern	int	yyparse( void ) ;
extern	FILE	*yyin ;
extern	int	yylex( void ) ;
%}

// 字句(トークン)の定義
%union {
	float	float_value;
}
%token	<float_value>     FLOAT_LITERAL
%token	ADD SUB MUL DIV CR PARL PARR
%type	<float_value>	expression term primary_expression parensis_expression literal_expression

%%
// 構文の定義
line_list	: line
			| line_list line
			;
line		: expression CR		{ printf( ">>%f\n" , $1 ) ; }
			;
expression	: term
			| expression ADD term	{
					$$ = $1 + $3 ;
				}
			| expression SUB term	{ $$ = $1 - $3 ; }
			;
term		: primary_expression
			| term MUL primary_expression	{ $$ = $1 * $3 ; }
			| term DIV primary_expression	{ $$ = $1 / $3 ; }
			;
primary_expression
			: ADD parensis_expression { $$ = $2; }
			| SUB parensis_expression { $$ = -$2; }
			| parensis_expression
			;
parensis_expression
			: PARL expression PARR { $$ = $2; }
			| literal_expression
			;
literal_expression
			: FLOAT_LITERAL
			;
%%

// 補助関数の定義
int yyerror( char const* str )
{
	fprintf( stderr , "parser error near %s\n" , yytext ) ;
	return 0 ;
}

int main( void )
{
	yydebug = 0 ;	// yydebug = 1 でデバッグ情報が見れる
	yyin = stdin ;	// 標準入力を使う
	if ( yyparse() ) {
		fprintf( stderr , "Error ! Error ! Error !\n" ) ;
		exit( 1 ) ;
	}
}

/** Local Variables: **/
/** mode: c++ **/
/** End: **/
