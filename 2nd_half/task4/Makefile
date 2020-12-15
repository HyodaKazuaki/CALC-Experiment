# lex+yacc を用いた式の計算の Makefile
LDLIBS = -lm

# 最終ターゲット
calc:	y.tab.o lex.yy.o
	$(CC) -o calc y.tab.o lex.yy.o $(LDLIBS)

# 構文解析処理
y.tab.o: calc.y
	bison -dy calc.y	# -dy : yacc互換
	$(CC) -c y.tab.c

# 字句解析処理
lex.yy.o: calc.l calc.y
	flex -l calc.l
	$(CC) -c lex.yy.c

# 不要なファイルを消す、仮想ターゲット(make clean せよ)
clean:; rm calc y.tab.c y.tab.h lex.yy.c *.o

### Local Variables: ###
### mode: makefile ###
### End: ###
