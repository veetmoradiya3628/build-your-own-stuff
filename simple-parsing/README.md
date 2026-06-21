```
bison -d parser.y
flex lexer.l
gcc parser.tab.c lex.yy.c -lfl
./a.out
```