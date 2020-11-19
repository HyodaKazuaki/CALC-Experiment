// 再帰下読みの基本の一番短いサンプル
//
//   字句解析は面倒なので、
//   演算子は1文字のみ、定数は数字1桁だけとする。

#include <stdio.h>
#include <ctype.h>

// 課題(1): () を含む処理に対応
//
// exp_PLUS_MINUS ::= exp_MUL_DIV '+' exp_PLUS_MINUS
//                  | exp_MUL_DIV '-' exp_PLUS_MINUS
//                  | exp_MUL_DIV
//                  ;
// exp_MUL_DIV    ::= BRACKETED '*' exp_MUL_DIV
//                  | BRACKETED '/' exp_MUL_DIV
//                  | BRACKETED
//                  ;
// BRACKETED      ::= '(' exp_PLUS_MINUS ')'
//                  | DIGIT
// DIGIT          ::= [0-9] ;

// 構文解析に使う関数のプロトタイプ宣言
int exp_PLUS_MINUS( const char* , const char** ) ;
int exp_MUL_DIV( const char* , const char** ) ;
int BRACKETED( const char* , const char** ) ;
int DIGIT( const char* , const char** ) ;

// PLUS,MINUSだけの式
//   構文解析関数の引数
//   pc:   解析する文字列の先頭
//   endp: 解析が終わった場所
int exp_PLUS_MINUS( const char* pc , const char** endp ) {
   // left=乗除算式
   int left = exp_MUL_DIV( pc , endp ) ;

   if ( **endp == '+' ) {
      // right=加減算式
      int right = exp_PLUS_MINUS( *endp + 1 , endp ) ;
      return left + right ;
   } else if ( **endp == '-' ) {
      // right=加減算式
      int right = exp_PLUS_MINUS( *endp + 1 , endp ) ;
      return left - right ;
   } else {
      // 乗除算式を返す
      return left ;
   }
}

// MUL,DIVだけの式
//   DIGITに相当する構文木の処理は、組み込んでしまう。
int exp_MUL_DIV( const char* pc , const char** endp ) {
   int left = BRACKETED(pc, endp);
   *endp = *endp + 1;
   if ( **endp == '*' ) {
      // right=乗除算式
      int right = exp_MUL_DIV( *endp + 1 , endp ) ;
      return left * right ;
   } else if ( **endp == '/' ) {
      // right=乗除算式
      int right = exp_MUL_DIV( *endp + 1 , endp ) ;
      return left / right ;
   }
   // 数値を返す
   return left ;
}

int BRACKETED( const char* pc, const char** endp) {
   if(*pc == '(') {
      *endp = pc;
      int left = exp_PLUS_MINUS(pc + 1, endp);
      if(**endp == ')')
         return left;
      printf("Error\n");
      return 0;
   }
   return DIGIT(pc, endp);
}

int DIGIT( const char* pc, const char** endp) {
   if ( isdigit( *pc ) ){
      *endp = pc;
      return *pc - '0';
   }

   printf( "Error\n" ) ;
   return 0 ;
}


int main() {
   const char* e = NULL ;
   printf( "%d\n" , exp_PLUS_MINUS( "(1+2)*3" , &e ) ) ;
   printf( "%d\n" , exp_PLUS_MINUS( "1*(2+3)" , &e ) ) ;
   printf( "%d\n" , exp_PLUS_MINUS( "(1+2)*(3+4)" , &e ) ) ;
   return 0 ;
}