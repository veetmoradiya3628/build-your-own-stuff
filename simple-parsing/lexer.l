%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%option noyywrap

%%

"SELECT"      { return SELECT; }
"FROM"        { return FROM; }
";"           { return SEMICOLON; }

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return IDENTIFIER;
}

[ \t\r\n]+    { }

. {
    printf("Unknown character: %s\n", yytext);
}

%%