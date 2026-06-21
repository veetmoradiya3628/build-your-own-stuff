%{
#include <stdio.h>

void yyerror(const char *s);
int yylex();
%}

%union{
    char *str;
}

%token SELECT
%token FROM
%token SEMICOLON
%token <str> IDENTIFIER

%%

query

    : SELECT IDENTIFIER FROM IDENTIFIER SEMICOLON
      {
          printf("SQL is Valid!\n");
          printf("Column = %s\n", $2);
          printf("Table = %s\n", $4);
      }

    ;

%%

void yyerror(const char *s)
{
    printf("Syntax Error!\n");
}

int main()
{
    yyparse();
    return 0;
}