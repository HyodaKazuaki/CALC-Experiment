// 丸括弧、+-符号に対応
// 小数に対応
// べき演算、剰余演算に対応
// 科学的表記に対応
// 変数代入に対応
// 整数と小数の表記切り替えに対応
%{
// lex+yacc を用いた式の計算の構文解析部分
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// yacc が定義する内部関数のプロトタイプ宣言
#define YYDEBUG 1
extern  int     yydebug ;
extern	int	yyerror( char const* ) ;
extern	char	*yytext ;
extern	int	yyparse( void ) ;
extern	FILE	*yyin ;
extern	int	yylex( void ) ;
extern 	float variables[26];
%}

// 字句(トークン)の定義
%union {
	float	float_value;
	char	char_value;
}
%token	<float_value>   FLOAT_LITERAL
%token	<char_value>	CHAR_LITERAL
%token					ADD SUB MUL DIV MOD CR PARL PARR EXP SCI SSTI
%type	<float_value>	expression term primary_expression parensis_expression exponential_expression scientific_notation float_expression literal

%%
// 構文の定義
line_list	: line
			| line_list line
			;
line		: CHAR_LITERAL SSTI expression CR	{ variables[$1 - 'a'] = $3; }
			| expression CR		{ if((int)$1 != $1) printf( ">>%f\n" , $1 ); else printf(">>%d\n", (int)$1); }
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
			| term MOD primary_expression	{ $$ = (int)$1 % (int)$3; }
			;
primary_expression
			: ADD parensis_expression { $$ = $2; }
			| SUB parensis_expression { $$ = -$2; }
			| parensis_expression
			;
parensis_expression
			: PARL expression PARR { $$ = $2; }
			| exponential_expression
			;
exponential_expression
			: primary_expression EXP primary_expression { $$ = powf($1, $3); }
			| scientific_notation
			;
scientific_notation
			: FLOAT_LITERAL SCI float_expression { 
				if((int)$3 - $3 == 0.0)
					$$ = $1 * powf(10.0, (float)$3); 
				else{
					printf("Error\n");
					$$ = 0;
				}
			}
			| literal
			;
float_expression
			: ADD literal { $$ = $2; }
			| SUB literal { $$ = -$2; }
			| literal
			;
literal 	: CHAR_LITERAL { $$ = variables[$1 - 'a']; }
			| FLOAT_LITERAL
			;
%%
// 変数
float variables[26];

// 補助関数の定義
int yyerror( char const* str )
{
	fprintf( stderr , "parser error near %s\n" , yytext ) ;
	return 0 ;
}

int initialize_variables(){
	int i;
	for(i = 0; i < 26; i++)
		variables[i] = 0.0;
	return 0;
}

int main( void )
{
	// 変数の初期化
	initialize_variables();
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
