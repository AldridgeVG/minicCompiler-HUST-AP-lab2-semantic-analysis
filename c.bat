chcp 65001
flex lex.l
bison -v -d parser.y
gcc -o e lex.yy.c parser.tab.c ast.c Analysis.c
e test1.c