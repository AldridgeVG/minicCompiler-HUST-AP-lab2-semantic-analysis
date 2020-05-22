chcp 65001
win_flex lex.l
win_bison -d -v parser.y
gcc -o e lex.yy.c parser.tab.c ast.c Analysis.c
e test1.c