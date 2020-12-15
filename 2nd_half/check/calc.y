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
	int	int_value;
}
%token	<int_value>     INT_LITERAL
%token	ADD SUB MUL DIV CR
%type	<int_value>	expression term primary_expression

%%
// 構文の定義
line_list	: line
		| line_list line
		;
line		: expression CR		{ printf( ">>%d\n" , $1 ) ; }
		;
expression	: term
		| expression ADD term	{
						$$ = $1 + $3 ;
						printf( "%d+%d=%d\n" , $1 , $3 , $$ ) ;
					}
		| expression SUB term	{ $$ = $1 - $3 ; }
		;
term		: primary_expression
		| term MUL primary_expression	{ $$ = $1 * $3 ; }
		| term DIV primary_expression	{ $$ = $1 / $3 ; }
		;
primary_expression
		: INT_LITERAL
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
