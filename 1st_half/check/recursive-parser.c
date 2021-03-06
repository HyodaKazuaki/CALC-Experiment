// 再帰下読みの基本の一番短いサンプル
//
//   字句解析は面倒なので、
//   演算子は1文字のみ、定数は数字1桁だけとする。

#include <stdio.h>
#include <ctype.h>

// 括弧の無い +,-,*,/ だけの式の処理
//
//   今回は以下の構文だけとする。
//   以下のような文法の書き方をバッカス記法(BNF)と言う。
//
// exp_PLUS_MINUS ::= exp_MUL_DIV '+' exp_PLUS_MINUS
//                  | exp_MUL_DIV '-' exp_PLUS_MINUS
//                  | exp_MUL_DIV
//                  ;
// exp_MUL_DIV    ::= DIGIT '*' exp_MUL_DIV
//                  | DIGIT '/' exp_MUL_DIV
//                  | DIGIT
//                  ;
// DIGIT          ::= [0-9] ;

// 構文解析に使う関数のプロトタイプ宣言
int exp_PLUS_MINUS( const char* , const char** ) ;
int exp_MUL_DIV( const char* , const char** ) ;

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
   if ( isdigit( *pc ) ) {
      // left=数値
      int left = *pc - '0' ;
      *endp = pc + 1 ;
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
   } else {
      printf( "Error\n" ) ;
      return 0 ;
   }
}

// 課題(1): () を含む処理にしたかったら、
//          BNF の構文木は、どう直すべきか？
// 課題(2): () を含む処理、空白を無視する処理、
//          定数式が複数桁を使える処理。

int main() {
   const char* e = NULL ;
   printf( "%d\n" , exp_PLUS_MINUS( "1+2*3" , &e ) ) ;
   printf( "%d\n" , exp_PLUS_MINUS( "1*2+3" , &e ) ) ;
   return 0 ;
}