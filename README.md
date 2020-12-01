# CALC-Experiment
字句解析、構文解析を含む計算プログラムの実装実験


## 構成
* 字句解析
* 構文解析


## 字句解析と構文解析
### 再起下降パーサ
```
<exp_PLUS_MINUS> ::= <exp_MUL_DIV> '+' <exp_PLUS_MINUS>
                 | <exp_MUL_DIV> '-' <exp_PLUS_MINUS>
                 | <exp_MUL_DIV>
                 ;
<exp_MUL_DIV>    ::= <exp_MULTIPLIER> '*' <exp_MUL_DIV>
                 | <exp_MULTIPLIER> '/' <exp_MUL_DIV>
                 | <exp_MULTIPLIER> '%' <exp_MUL_DIV>
                 | <exp_MULTIPLIER>
                 ;
<exp_MULTIPLIER> ::= <FULL_NUM_VALUE> '^' <FULL_NUM_VALUE>
                 | <FULL_NUM_VALUE>
                 ;
<FULL_NUM_VALUE> ::= '+' <BRACKETED>
                 | '-' <BRACKETED>
                 | <BRACKETED>
                 ;
<BRACKETED>      ::= '(' <exp_PLUS_MINUS> ')'
                 | <SCI_NOTATION>
                 ;
<SCI_NOTATION>   ::= <NUM_VALUE> 'E' <FULL_NUM_VALUE>
                 | <NUM_VALUE>
                 ;
<NUM_VALUE>      ::= <DIGITS> '.' <DIGITS>
                 | <DIGITS>
                 ;
<DIGITS>         ::= <DIGIT> <DIGITS>
                 | <DIGIT>
                 ;
<DIGIT>          ::= [0-9]
                 ;
```


### LR構文解析法
