# CALC-Experiment
字句解析、構文解析を含む計算プログラムの実装実験


## 構成
* 字句解析
* 構文解析


## 字句解析


## 構文解析
### 再起下降パーサ
```
<ADDSUM> ::= <MULDIV> '+' <ADDSUM>
        | <MULDIV> '-' <ADDSUM>
        | <MULDIV>
        ;
<MULDIV> ::= <DIGIT> '*' <MULDIV>
        | <DIGIT> '/' <MULDIV>
        | <DIGIT>
        ;
<DIGIT> ::= [0-9]
        ;
```
### LR構文解析法
