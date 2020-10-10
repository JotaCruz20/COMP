%{
    /*
        João Alexandre Santos Cruz 2018288464
        André Cristóvão Ferreira da Silva  2018277921
    */

    #include <stdio.h>
    #include <stdlib.h>
    int linha=1;
    int coluna=1;
    int flag=0;
    int flagError=0;
%}
%X COMMENT COMMENT2
ponto           [,.]
numero         [0-9]+
signal         [+-]
exp [eE]
RESERVED            "auto"|"break"|"const"|"continue"|"asm"|"do"|"enum"|"extern"|"float"|"for"|"goto"|"register"|"long"|"signed"|"unsigned"|"sizeof"|"static"|"struct"|"switch"|"case"|"default"|"typedef"|"union"|"volatile"|"break"|"restrict"|"inline"|"["|"]"|"++"|"--"|("__"[a-zA-Z0-9]+)|("_"[A-Z][a-zA-Z0-9]*)
%%
\/\*              { BEGIN COMMENT;}
\/\/              { BEGIN COMMENT2;}
<COMMENT>.        {;}
<COMMENT>\n       {;}
<COMMENT>\*\/     { BEGIN 0;}
<COMMENT2>.        {;}
<COMMENT2>\n       {BEGIN 0;}
{signal}?{numero}{ponto}{numero} {printf("REALLIT\n");}
{signal}?{numero}{exp}{signal}?{numero} {printf("REALLIT\n");}
{signal}?{numero}{ponto}{numero}{exp}{signal}{numero} {printf("REALLIT\n");}
"char"  {if(flag)printf("CHAR\n");}
"else"  {if(flag)printf("ELSE\n");}
"while" {if(flag)printf("WHILE\n");}
"int"   {if(flag)printf("INT\n");}
"short" {if(flag)printf("SHORT\n");}
"double" {if(flag)printf("DOUBLE\n");}
"return"    {if(flag)printf("RETURN\n");}
"void" {if(flag)printf("VOID\n");}
"&" {if(flag)printf("BITWISEAND\n");}
"|" {if(flag)printf("BITWISEOR\n");}
"ˆ" {if(flag)printf("BITWISEXOR\n");}
"&&"    {if(flag)printf("AND\n");}
"=" {if(flag)printf("ASSIGN\n");}
"*" {if(flag)printf("MUL\n");}
"," {if(flag)printf("COMMA\n");}
"/" {if(flag)printf("DIV\n");}
"=="    {if(flag)printf("EQ\n");}
">="    {if(flag)printf("GE\n");}
">" {if(flag)printf("GT\n");}
"{" {if(flag)printf("LBRACE\n");}
"<" {if(flag)printf("LE\n");}
.	;
\n	;
%%
int main(int argc, char **argv)
{
    if(argc>1){
        if (argv[1][0]=='-' && argv[1][1]=='l'){
            flag=1;
        }
    }
    else if(argc>2){
         if (argv[1][0]=='-' && argv[1][1]=='e' && argv[1][1]=='1'){
            flagError=1;
        }
    }
yylex();
return 0;
}

int yywrap()
{
return 1;
}
