
%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "types.h"
#include "rpcalc.hxx"
%}

%%
"_"    {yylval.operetor = strdup("_"); return UNDER;}
"="    {yylval.operetor = strdup("="); return EQ; }
">="   {yylval.operetor = strdup(">=");return GRETEREQ;}
"<="   {yylval.operetor = strdup("<="); return LESSEQ;}
">"    {yylval.operetor = strdup(">"); return GRETER;}
"<"    {yylval.operetor = strdup("<"); return LESS;}
"=="   {yylval.operetor = strdup("=="); return EQEQ;}
"&"    {yylval.operetor = strdup("&"); return AND;}
"|"    {yylval.operetor = strdup("|"); return OR;}
"+"    {yylval.operetor = strdup("+"); return PLUS;}
"-"    {yylval.operetor = strdup("-"); return MINUS;}
"*"    {yylval.operetor = strdup("*"); return MULT;}
"/"    {yylval.operetor = strdup("/"); return DIV; }   
"!="   {yylval.operetor = strdup("!="); return NOTEQ; }
[0-9]+ { yylval.constant = atoi(yytext) ; return NUM ; }
[a-zA-Z] { yylval.literal = yytext[0] ; return CHAR ; }
%%

void string_scan(char* buffer)
{
	yy_scan_string(buffer) ;
}
