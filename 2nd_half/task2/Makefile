# lex+yacc を用いた式の計算の Makefile

# 最終ターゲット
calc:	y.tab.o lex.yy.o
	gcc -o calc y.tab.o lex.yy.o

# 構文解析処理
y.tab.o: calc.y
	bison -dy calc.y	# -dy : yacc互換
	gcc -c y.tab.c

# 字句解析処理
lex.yy.o: calc.l calc.y
	flex -l calc.l
	gcc -c lex.yy.c

# 不要なファイルを消す、仮想ターゲット(make clean せよ)
clean:; rm calc y.tab.c y.tab.h lex.yy.c *.o

### Local Variables: ###
### mode: makefile ###
### End: ###
