#include <math.h>
#include <stdio.h>
#include <ctype.h>

// 課題(5): () を含む処理、空白を無視する処理、
// 定数式が+-を含む複数桁を使える処理、余り演算、
// 指数演算処理、科学的表記に対応
//
// exp_PLUS_MINUS ::= exp_MUL_DIV '+' exp_PLUS_MINUS
//                  | exp_MUL_DIV '-' exp_PLUS_MINUS
//                  | exp_MUL_DIV
//                  ;
// exp_MUL_DIV    ::= exp_MULTIPLIER '*' exp_MUL_DIV
//                  | exp_MULTIPLIER '/' exp_MUL_DIV
//                  | exp_MULTIPLIER '%' exp_MUL_DIV
//                  | exp_MULTIPLIER
//                  ;
// exp_MULTIPLIER ::= FULL_NUM_VALUE '^' FULL_NUM_VALUE
//                  | FULL_NUM_VALUE
//                  ;
// FULL_NUM_VALUE ::= '+' BRACKETED
//                  | '-' BRACKETED
//                  | BRACKETED
//                  ;
// BRACKETED      ::= '(' exp_PLUS_MINUS ')'
//                  | SCI_NOTATION
//                  ;
// SCI_NOTATION   ::= NUM_VALUE 'E' FULL_NUM_VALUE
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
float exp_MULTIPLIER( const char* , const char** ) ;
float FULL_NUM_VALUE(const char* , const char**);
float BRACKETED( const char* , const char** ) ;
float SCI_NOTATION( const char* , const char** ) ;
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
   pc = ignore_space(pc);
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
   float left = exp_MULTIPLIER(pc, endp);
   pc = ignore_space(*endp + 1);
   if ( *pc == '*' ) {
      // right=乗除算式
      *endp = pc;
      pc = ignore_space(*endp + 1);
      float right = exp_MUL_DIV(pc , endp ) ;
      return left * right ;
   } else if ( *pc == '/' ) {
      // right=乗除算式
      *endp = pc;
      pc = ignore_space(*endp + 1);
      float right = exp_MUL_DIV(pc , endp ) ;
      return left / right ;
   } else if ( *pc == '%' ) {
      // right=乗除算式
      *endp = pc;
      pc = *endp + 1;
      pc = ignore_space(*endp + 1);
      float right = exp_MUL_DIV(pc , endp ) ;
      return (float)((int)left % (int)right);
   }
   // 数値を返す
   return left ;
}

float exp_MULTIPLIER( const char* pc , const char** endp ) {
   float left = FULL_NUM_VALUE(pc, endp);
   pc = ignore_space(*endp + 1);
   if(*pc == '^'){
      // 指数を計算
      *endp = pc;
      pc = ignore_space(*endp + 1);
      int counter = 0;
      float right = FULL_NUM_VALUE(pc, endp);
      return powf(left,right);
   }
   return left;
}

float FULL_NUM_VALUE(const char* pc, const char** endp){
   // pc = ignore_space(pc);
   if(*pc == '-'){
      *endp = pc;
      pc = *endp + 1;
      float left = BRACKETED(pc, endp);
      return -left;
   }
   if(*pc == '+'){
      *endp = pc;
      pc = *endp + 1;
   }
   return BRACKETED(pc, endp);
}

float BRACKETED( const char* pc, const char** endp) {
   // pc = ignore_space(pc);
   if(*pc == '(') {
      *endp = pc;
      pc = *endp + 1;
      float left = exp_PLUS_MINUS(pc, endp);
      pc = ignore_space(*endp + 1);
      if(*pc == ')'){
         *endp = pc;
         return left;
      }
      printf("bracket or sign Error\n");
      return 0;
   }
   return SCI_NOTATION(pc, endp);
}

float SCI_NOTATION( const char* pc, const char** endp) {
   float left = NUM_VALUE(pc, endp);
   pc = *endp + 1;
   if(*pc == 'E') {
      *endp = pc;
      pc = *endp + 1;
      float right = FULL_NUM_VALUE(pc, endp);
      return left * powf(10, right);
   }
   return left;
}

float NUM_VALUE( const char* pc, const char** endp) {
   int counter = 0;
   float left = DIGITS(pc, endp, &counter, 0);
   pc = *endp + 1;
   // pc = ignore_space(pc);
   if(*pc == '.'){
      // 小数を計算
      *endp = pc;
      pc = *endp + 1;
      counter = 0;
      float right = DIGITS(pc, endp, &counter, 1);
      return left + right * 0.1;
   }
   return left;
}

float DIGITS(const char* pc, const char** endp, int* counter, int inverse_flag) {
   float left = DIGIT(pc, endp);
   pc = *endp + 1;
   // pc = ignore_space(pc);
   if(isdigit( *pc )){
      // 複数桁(下に桁がある)
      float right = DIGITS(pc, endp, counter, inverse_flag);
      *counter += 1;
      // right = inverse_flag ? left + 0.1 * right : left * 10.0 + right;
      if(inverse_flag)
         right = left + 0.1 * right;
      else {
         right = left * powf(10.0, *counter) + right;
      }
      return right;
   }

   return left;
}

float DIGIT( const char* pc, const char** endp) {
   // pc = ignore_space(pc);
   if ( isdigit( *pc ) ){
      *endp = pc;
      return (float)(*pc - '0');
   }

   printf( "digit Error\n" ) ;
   return 0 ;
}

int assert_equal(const char* str, float true_value, float* result){
   const char* e = NULL;
   *result = exp_PLUS_MINUS(str, &e);
   return true_value == *result;
}

// struct task{

// }

int test(){
   int i;
   float result;
   int length = 12;
   const char str[][100] = {
      "13 + 2",
      " ( 1 + 2 ) * 3 ",
      "( 1 + 2 ) * ( 3 + 4 )",
      "+1042.0  /  -2",
      "  -1.0  /  -2  ",
      "  38 % 5  ",
      " -(5 ^ -2) ",
      " +(31)^3",
      " -1.302",
      " -10E-4+2",
      " -10E-(4+2)",
      " -1.302E+5"
   };
   const float true_value[] = {
      13 + 2,
       ( 1 + 2 ) * 3 ,
      ( 1 + 2 ) * ( 3 + 4 ),
      + 1042.0  /  - 2,
        - 1.0  /  - 2  ,
        38 % 5  ,
       -powf(5, -2),
       +powf(31, 3),
       -1.302,
       -10E-4+2,
       -10E-6,
       -1.302E+5,
   };

   for(i = 0; i < length; i++){
      printf("test %d: ", i + 1);
      if(assert_equal(str[i], true_value[i], &result)) {
         printf("passed.\n");
      }else{
         printf("failed. expected: %f, result: %f\n", true_value[i], result);
      }
   }

   return 0;
}

int main() {
   test();
   const char* e = NULL ;
   // printf( "%f\n" , exp_PLUS_MINUS( " ( 1 + 2 ) * 3 " , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( " 1 * ( 2 + 3 ) " , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( " ( 1 + 2 ) * ( 3 + 4 ) " , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( "  +1042.0  /  -2  " , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( "  -1.0  /  -2  " , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( "  38 % 5  " , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( " -(5 ^ -2) " , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( " +(31)^3" , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( " -1.302" , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( " -10E-4+2" , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( " -10E-(4+2)" , &e ) ) ;
   // printf( "%f\n" , exp_PLUS_MINUS( " -1.302E+5" , &e ) ) ;
   // printf("%f\n", exp_PLUS_MINUS("-10E-5", &e));
   return 0 ;
}