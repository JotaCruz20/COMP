%{
    /*
        João Alexandre Santos Cruz 2018288464
        André Cristóvão Ferreira da Silva  2018277921
    */
    #include "y.tab.h"
    #include "tree.h"
    #include "error.h"
    #include "symtab.h"
    #include "semantics.h"
    #include "geradorLLVM.h"
    #include <stdio.h>
    #include <stdlib.h>
    int linha=1;
    int coluna=1;
    int length = 0;
    int lenghtSTR=0;
    int flag=0,printM=0;
    int flagError=0;
    int erroC = 0, erroL =0;
    int errorFlag = 0;
    struct no * noAST;

%}
%X COMMENT
INTLIT         [0-9]+
signal        [+-]
exp [eE]
nExp    {exp}{signal}?{INTLIT}
SPACE           (" "|"\t")+
ENTER   \\n 
ASPAS           \\\"
BARRA       \\\\
PLICA           \\\'
SEQUENCIA       \\[0-7]{1,3}
TAB             \\t
REALLIT        (({INTLIT}".")|({INTLIT}"."{INTLIT})|{INTLIT}"."{INTLIT}{nExp}|{INTLIT}"."{nExp}|"."{INTLIT}{nExp}|{INTLIT}{nExp}|"."{INTLIT})
CHRLIT     ([^\n\'\\]|{ENTER}|{ASPAS}|{PLICA}|{SEQUENCIA}|{TAB}|{BARRA})
ENDOFFILE   <<EOF>>
RESERVED        "auto"|"break"|"const"|"continue"|"asm"|"do"|"enum"|"extern"|"float"|"for"|"goto"|"register"|"long"|"signed"|"unsigned"|"sizeof"|"static"|"struct"|"switch"|"case"|"default"|"typedef"|"union"|"volatile"|"break"|"_Bool"|"_Imaginary"|"restrict"|"inline"|"_Complex"|"["|"]"|"++"|"--"
ID                  [a-zA-Z_][a-zA-Z0-9_]*
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
IF      "if"
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
%%
\/\*              {BEGIN COMMENT;erroC=coluna;erroL=linha;coluna+=yyleng;lenghtSTR=(int)yyleng;}
\/\/.*            {coluna+=yyleng;lenghtSTR=(int)yyleng;}         
<COMMENT>.        {coluna+=yyleng;lenghtSTR=(int)yyleng;}
<COMMENT>\n       {linha++;coluna=1;lenghtSTR=(int)yyleng;}
<COMMENT>\*\/     {BEGIN 0;coluna+=yyleng;lenghtSTR=(int)yyleng;}
<COMMENT><<EOF>>   {if(flagError)printf("Line %d, col %d: unterminated comment\n",erroL,erroC); yyterminate();}
<<EOF>>             {coluna+=yyleng;lenghtSTR=(int)yyleng;yyterminate();}
{CHAR}  {if(flag==1)printf("CHAR\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return CHAR;}
{ELSE} {if(flag==1)printf("ELSE\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return ELSE;}
{IF} {if(flag==1)printf("IF\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return IF;}
{WHILE} {if(flag==1)printf("WHILE\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return WHILE;}
{INT}   {if(flag==1)printf("INT\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return INT;}
{DOUBLE}   {if(flag==1)printf("DOUBLE\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return DOUBLE;}
{SHORT} {if(flag==1)printf("SHORT\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return SHORT;}
{RETURN}   {if(flag==1)printf("RETURN\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return RETURN;}
{VOID} {if(flag==1)printf("VOID\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return VOID;}
{BAND} {if(flag==1)printf("BITWISEAND\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return BAND;}
{BOR} {if(flag==1)printf("BITWISEOR\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return BOR;}
{BXOR} {if(flag==1)printf("BITWISEXOR\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return BXOR;}
{AND}    {if(flag==1)printf("AND\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return AND;}
{ASSIGN} {if(flag==1)printf("ASSIGN\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return ASSIGN;}
{MUL}   {if(flag==1)printf("MUL\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return MUL;}
{COMMA} {if(flag==1)printf("COMMA\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return COMMA;}
{DIV} {if(flag==1)printf("DIV\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return DIV;}
{EQ}   {if(flag==1)printf("EQ\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return EQ;}
{GE}    {if(flag==1)printf("GE\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return GE;}
{GT} {if(flag==1)printf("GT\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return GT;}
{LBRACE} {if(flag==1)printf("LBRACE\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return LBRACE;}
{LE} {if(flag==1)printf("LE\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return LE;}
{LPAR}  {if(flag==1)printf("LPAR\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return LPAR;}
{LT}    {if(flag==1)printf("LT\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return LT;}
{MINUS} {if(flag==1)printf("MINUS\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return MINUS;}
{MOD}               {if(flag==1)printf("MOD\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return MOD;}
{NE}                  {if(flag==1)printf("NE\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return NE;}
{NOT}                 {if(flag==1)printf("NOT\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return NOT;}
{OR}                  {if(flag==1)printf("OR\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return OR;}
{PLUS}               {if(flag==1)printf("PLUS\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return PLUS;}
{RBRACE}             {if(flag==1)printf("RBRACE\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return RBRACE;}
{RPAR}                {if(flag==1)printf("RPAR\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return RPAR;}
{SEMI}  {if(flag==1)printf("SEMI\n");coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return SEMI;}
{SPACE} {coluna+=yyleng;}
{RESERVED}  {if(flag==1)printf("RESERVED(%s)\n",yytext);coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);return RESERVED;}
{ID}    {if(flag==1)printf("ID(%s)\n",yytext);coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);lenghtSTR=(int)yyleng;return ID;}
{INTLIT}  {if(flag==1)printf("INTLIT(%s)\n",yytext);coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);lenghtSTR=(int)yyleng;return INTLIT;}
{REALLIT}   {if(flag==1)printf("REALLIT(%s)\n",yytext);coluna+=yyleng; yylval.noCol = addNo(yytext,coluna,linha);lenghtSTR=(int)yyleng;return REALLIT;} 
\'{CHRLIT}\'    {if(flag==1)printf("CHRLIT(%s)\n",yytext);coluna+=yyleng;yylval.noCol = addNo(yytext,coluna,linha);lenghtSTR=(int)yyleng;return CHRLIT;} 
\'([^\n\'\\]|\\.)*\'  {printf("Line %d, col %d: invalid char constant (%s)\n",linha,coluna,yytext);lenghtSTR=(int)yyleng;coluna+=yyleng;}
\'([^\n\'\\]|\\.)*(\\)?    {printf("Line %d, col %d: unterminated char constant\n",linha,coluna);lenghtSTR=(int)yyleng;coluna+=yyleng;}
\n|"\r\n"|"\r"    {linha++;coluna=1;lenghtSTR=(int)yyleng;}
.   {lenghtSTR=(int)yyleng;if(flagError)printf("Line %d, col %d: illegal character (%s)\n",linha,coluna,yytext);coluna+=yyleng;}
%%   

int yywrap(){
    return 1;
}

void yyerror (char *s){
    printf("Line %d, col %d: %s: %s\n",linha,coluna-(int)yyleng,s,yytext);
}


int main(int argc, char **argv){
    if(argc>1){
        if (argv[1][0]=='-' && argv[1][1]=='l'){
            flag=1;
            printM=1;
            flagError=1;
            yylex();
        }
        else if(argv[1][0]=='-' && argv[1][1]=='e' && argv[1][2]=='1'){
            flagError=1;
            flag = 0;
        }
        else if(argv[1][0]=='-' && argv[1][1]=='e' && argv[1][2]=='2'){
            flagError=1;    
            yyparse();
        }
        else if(argv[1][0]=='-' && argv[1][1]=='t'){
            flagError=1;
            flag=2;
            yyparse();
            if (errorFlag==0)
                printTree(noAST,0);    
        }
        else if(argv[1][0]=='-' && argv[1][1]=='s'){
            yyparse();
            initErrors();
            checkProgram(noAST);
            anotateBody(noAST);
            checkProgramError(noAST);
            anotateBodyError(noAST);
            printErros();
            printTabela();
            if (errorFlag==0)
                printTreeAnotada(noAST,0);  
        }
        else if(argv[1][0]=='-' && argv[1][1]=='e' && argv[1][2]=='3'){
            yyparse();
            initErrors();
            checkProgram(noAST);
            anotateBody(noAST);
            checkProgramError(noAST);
            anotateBodyError(noAST);
            printErros();
        }
        else{
            yyparse();
            initErrors();
            checkProgram(noAST);
            anotateBody(noAST);
            checkProgramError(noAST);
            anotateBodyError(noAST);
            printErros();
            printTabela();
            if (errorFlag==0){
                geracaoLLVM(noAST);
            }
        }
    }
    else{
            yyparse();
            initErrors();
            checkProgram(noAST);
            anotateBody(noAST);
            checkProgramError(noAST);
            anotateBodyError(noAST);
            if(errorFlag==0){
                geracaoLLVM(noAST);
            }
        }
    return 0;
}