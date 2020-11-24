// 再帰下読みの基本の一番短いサンプル
//
//   字句解析は面倒なので、
//   演算子は1文字のみ、定数は数字1桁だけとする。

#include <math.h>
#include <stdio.h>
#include <ctype.h>

// 課題(2): () を含む処理、空白を無視する処理、定数式が+-を含む複数桁を使える処理に対応
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
//                  | FULL_NUM_VALUE
//                  ;
// FULL_NUM_VALUE ::= '+' NUM_VALUE
//                  | '-' NUM_VALUE
//                  | NUM_VALUE
//                  ;
// NUM_VALUE      ::= DIGITS '.' DIGITS
//                  | DIGITS
//                  ;
// DIGITS         ::= DIGIT DIGITS
//                  | DIGIT
//                  ;
// DIGIT          ::= [0-9]
//                  ;

// 構文解析に使う関数のプロトタイプ宣言
float exp_PLUS_MINUS( const char* , const char** ) ;
float exp_MUL_DIV( const char* , const char** ) ;
float BRACKETED( const char* , const char** ) ;
float FULL_NUM_VALUE(const char* , const char**);
float NUM_VALUE(const char* , const char**);
float DIGITS(const char* , const char**, int*, int);
float DIGIT( const char* , const char** ) ;

const char* ignore_space(const char* ptr) {
   while(*ptr == ' ') {
      ptr += 1;
   }
   return ptr;
}

// PLUS,MINUSだけの式
//   構文解析関数の引数
//   pc:   解析する文字列の先頭
//   endp: 解析が終わった場所
float exp_PLUS_MINUS( const char* pc , const char** endp ) {
   // left=乗除算式
   float left = exp_MUL_DIV( pc , endp ) ;
   pc = ignore_space(*endp + 1);

   if ( *pc == '+' ) {
      // right=加減算式
      *endp = pc;
      pc = *endp + 1;
      float right = exp_PLUS_MINUS( pc , endp ) ;
      return left + right ;
   } else if ( *pc == '-' ) {
      // right=加減算式
      *endp = pc;
      pc = *endp + 1;
      float right = exp_PLUS_MINUS( pc , endp ) ;
      return left - right ;
   } else {
      // 乗除算式を返す
      return left ;
   }
}

// MUL,DIVだけの式
//   DIGITに相当する構文木の処理は、組み込んでしまう。
float exp_MUL_DIV( const char* pc , const char** endp ) {
   float left = BRACKETED(pc, endp);
   pc = ignore_space(*endp + 1);
   if ( *pc == '*' ) {
      // right=乗除算式
      *endp = pc;
      pc = *endp + 1;
      float right = exp_MUL_DIV(pc , endp ) ;
      return left * right ;
   } else if ( *pc == '/' ) {
      // right=乗除算式
      *endp = pc;
      pc = *endp + 1;
      float right = exp_MUL_DIV(pc , endp ) ;
      return left / right ;
   }
   // 数値を返す
   return left ;
}

float BRACKETED( const char* pc, const char** endp) {
   pc = ignore_space(pc);
   if(*pc == '(') {
      *endp = pc;
      pc = *endp + 1;
      float left = exp_PLUS_MINUS(pc, endp);
      pc = ignore_space(*endp + 1);
      if(*pc == ')'){
         *endp = pc;
         return left;
      }
      printf("bracket Error\n");
      return 0;
   }
   return FULL_NUM_VALUE(pc, endp);
}

float FULL_NUM_VALUE(const char* pc, const char** endp){
   pc = ignore_space(pc);
   if(*pc == '+'){
      *endp = pc;
      pc = *endp + 1;
      float left = NUM_VALUE(pc, endp);
      return left;
   }
   if(*pc == '-'){
      *endp = pc;
      pc = *endp + 1;
      float left = NUM_VALUE(pc, endp);
      return -left;
   }
   return NUM_VALUE(pc, endp);
}

float NUM_VALUE(const char* pc, const char** endp){
   int counter = 0;
   float left = DIGITS(pc, endp, &counter, 0);
   pc = *endp + 1;
   if(*pc == '.'){
      // 小数を計算
      *endp = pc;
      pc = *endp + 1;
      int counter = 0;
      float right = DIGITS(pc, endp, &counter, 1);
      return left + right;
   }
   return left;
}

float DIGITS(const char* pc, const char** endp, int* counter, int inverse_flag) {
   float left = DIGIT(pc, endp);
   pc = *endp + 1;
   pc = ignore_space(pc);
   if(isdigit( *pc )){
      // 複数桁(下に桁がある)
      float right = DIGITS(pc, endp, counter, inverse_flag);
      *counter += inverse_flag ? -1 : 1;
      // *counter += 1;
      return left * powf(10.0, *counter) + right;
   }
   *counter = 0;
   return left;
}

float DIGIT( const char* pc, const char** endp) {
   pc = ignore_space(pc);
   if ( isdigit( *pc ) ){
      *endp = pc;
      return (float)(*pc - '0');
   }

   printf( "digit Error\n" ) ;
   return 0 ;
}

// 課題(2): () を含む処理、空白を無視する処理、
//          定数式が複数桁を使える処理。

int main() {
   const char* e = NULL ;
   printf( "%f\n" , exp_PLUS_MINUS( " ( 1 + 2 ) * 3 " , &e ) ) ;
   printf( "%f\n" , exp_PLUS_MINUS( " 1 * ( 2 + 3 ) " , &e ) ) ;
   printf( "%f\n" , exp_PLUS_MINUS( " ( 1 + 2 ) * ( 3 + 4 ) " , &e ) ) ;
   printf( "%f\n" , exp_PLUS_MINUS( "  + 1042.0  /  - 2  " , &e ) ) ;
   printf( "%f\n" , exp_PLUS_MINUS( "  - 1.0  /  - 2  " , &e ) ) ;
   return 0 ;
}