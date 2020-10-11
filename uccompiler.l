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
plica   "'"
ID                  [a-zA-Z_][a-zA-Z0-9_]*
INTLIT              [0-9]+
CHARLIT             {plica}[a-zA-z]|"\n"|"\t"|"\\"|"\’"|"\"|"\"{plica}
LPAR                "("
LT                  "<"
MINUS               "-"
MOD                 "%"
NE                  "!="
NOT                 "!"
OR                  "||"
PLUS                "+"
RBRACE              "}"
RPAR                ")"
SEMI                ";"
ELSE                "else"
CHAR                "char"
WHILE      "while"
INT         "int"
SHORT   "short"
DOUBLE  "double"
RETURN  "return"
VOID    "void"
BAND "&"
BOR  "|"
BXOR "^"
AND "&&"
ASSIGN  "="
MUL "*"
COMMA   ","
DIV "/"
EQ  "=="
GE  ">="
GT  ">"
LBRACE  "{"
LE  "<="
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
{CHAR}  {if(flag)printf("CHAR\n");}
{ELSE} {if(flag)printf("ELSE\n");}
{WHILE} {if(flag)printf("WHILE\n");}
{INT}   {if(flag)printf("INT\n");}
{SHORT} {if(flag)printf("SHORT\n");}
{DOUBLE} {if(flag)printf("DOUBLE\n");}
{RETURN}   {if(flag)printf("RETURN\n");}
{VOID} {if(flag)printf("VOID\n");}
{BAND} {if(flag)printf("BITWISEAND\n");}
{BOR} {if(flag)printf("BITWISEOR\n");}
{BXOR} {if(flag)printf("BITWISEXOR\n");}
{AND}    {if(flag)printf("AND\n");}
{ASSIGN} {if(flag)printf("ASSIGN\n");}
{MUL}   {if(flag)printf("MUL\n");}
{COMMA} {if(flag)printf("COMMA\n");}
{DIV} {if(flag)printf("DIV\n");}
{EQ}   {if(flag)printf("EQ\n");}
{GE}    {if(flag)printf("GE\n");}
{GT} {if(flag)printf("GT\n");}
{LBRACE} {if(flag)printf("LBRACE\n");}
{LE} {if(flag)printf("LE\n");}
{INTLIT}  {if(flag)printf("INTLIT(%s)\n",yytext);}  
{ID}    {if(flag)printf("ID(%s)\n",yytext);}
{CHARLIT}    {if(flag)printf("CHARLIT(%s)\n",yytext);}
{LPAR}  {if(flag)printf("LPAR\n");}
{LT}                 {if(flag)printf("LT\n");}
{MINUS}             {if(flag)printf("MINUS\n");}
{MOD}                {if(flag)printf("MOD\n");}
{NE}                  {if(flag)printf("NE\n");}
{NOT}                 {if(flag)printf("NOT\n");}
{OR}                  {if(flag)printf("OR\n");}
{PLUS}               {if(flag)printf("PLUS\n");}
{RBRACE}             {if(flag)printf("RBRACE\n");}
{RPAR}                {if(flag)printf("RPAR\n");}
{SEMI}  {if(flag)printf("SEMI\n");}
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
