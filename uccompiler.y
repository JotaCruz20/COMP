%{
    /*
        João Alexandre Santos Cruz 2018288464
        André Cristóvão Ferreira da Silva  2018277921
    */

    #include <stdio.h>
    #include <stdlib.h>  
    #include <string.h>  
    #include "tree.h"
    int yylex(void);
    void yyerror (char * s);

    extern int errorFlag;
    extern struct no * noAST;
    int counter = 0;
    extern int flag;
    extern int linha;
    extern int coluna;

    char * auxType;
    no * noAuxiliar;

    no * inserirNo(char * id, char * type, no * noFilho,int line,int col){
        no * noAtual = (no *)malloc(sizeof(no));
        if(type!=NULL){
                noAtual->type =(char *)malloc(sizeof(char)*(strlen(type)+1));
                strcpy(noAtual->type,type);
        }
        else{
               noAtual->type=NULL; 
        }
        if(id!=NULL){
                noAtual->id =(char *)malloc(sizeof(char)*(strlen(id)+1));
                strcpy(noAtual->id,id);
        }
        else{
                noAtual->id = NULL;
        }
        noAtual->line = line;
        noAtual->col= col;
        noAtual->noFilho = noFilho;
        noAtual->noIrmao = NULL;
    return noAtual;
    }

    void addIrmao(no * no1, no * no2)
    {
        no * noAuxiliar  = no1;
        while(noAuxiliar->noIrmao!=NULL){
                noAuxiliar=noAuxiliar->noIrmao;
        }
        noAuxiliar->noIrmao = no2;
    }

    void printTree(no * auxNode, int pontos) /* mudar isto */
    {
        int i=0;
        if(auxNode!=NULL){
                for(i=0;i<pontos*2;i++){
                        printf(".");
                }
                if(auxNode->id!=NULL){
                        printf("%s(%s)\n", auxNode->type,auxNode->id);
                }
                else{
                         printf("%s\n", auxNode->type);
                }
                if(auxNode->noFilho!=NULL){
                        printTree(auxNode->noFilho,pontos+1);
                }
                if(auxNode->noIrmao!=NULL){
                        printTree(auxNode->noIrmao,pontos);
                }

        }
        //free(auxNode);
    }

%}

%union{
    struct no * no;
    struct noLex * noCol;
}

%type <no> Programa
%type <no> FunctionsAndDeclarations
%type <no> FunctionDefinition
%type <no> FunctionBody
%type <no> DeclarationsAndStatements
%type <no> FunctionDeclaration
%type <no> FunctionDeclarator
%type <no> ParameterList
%type <no> ParameterDeclaration
%type <no> Declaration
%type <no> DeclarationCD
%type <no> TypeSpec
%type <no> Declarator
%type <no> Statement
%type <no> Statementlbrace
%type <no> StatementError
%type <no> StatementReturn
%type <no> StatementIrmao
%type <no> Expr
%type <no> CommaExpr
%type <no> ExprFunctions
%type <no> ExprMath

%token <noCol> CHRLIT 
%token <noCol> ID
%token <noCol> REALLIT
%token <noCol> INTLIT
%token CHAR 
%token ELSE
%token WHILE
%token IF
%token INT 
%token SHORT
%token DOUBLE
%token RETURN
%token VOID
%token BAND
%token BOR
%token BXOR
%token AND
%token ASSIGN
%token MUL
%token COMMA
%token DIV
%token EQ
%token GE
%token GT
%token LBRACE
%token LE
%token LPAR
%token LT
%token MINUS
%token MOD
%token NE
%token NOT
%token OR
%token PLUS
%token RBRACE
%token RPAR
%token SEMI
%token RESERVED


%left COMMA
%right ASSIGN
%left OR
%left AND
%left BOR
%left BXOR
%left BAND
%left EQ NE
%left LE LT GT GE
%left PLUS MINUS
%left DIV MUL MOD
%right NOT
%left RPAR LPAR
%nonassoc ELSE

%%

Programa: FunctionsAndDeclarations {noAST=inserirNo(NULL,"Program",$1,linha,coluna);}
	    ;

FunctionsAndDeclarations: FunctionDefinition  {$$ = $1;}
        | FunctionDeclaration  {$$ = $1;}
        | Declaration  {$$ = $1;}
        | FunctionsAndDeclarations FunctionDefinition   {$$ = $1;
                                                        noAuxiliar = $1;
                                                        while(noAuxiliar->noIrmao!=NULL){
                                                                noAuxiliar=noAuxiliar->noIrmao;
                                                        }
                                                        addIrmao(noAuxiliar,$2);
                                                        }
        | FunctionsAndDeclarations FunctionDeclaration  {$$ = $1;
                                                        noAuxiliar = $1;
                                                        while(noAuxiliar->noIrmao!=NULL){
                                                                noAuxiliar=noAuxiliar->noIrmao;
                                                        }
                                                        addIrmao(noAuxiliar,$2);
                                                        }
        | FunctionsAndDeclarations Declaration          {$$ = $1;
                                                        noAuxiliar = $1;
                                                        while(noAuxiliar->noIrmao!=NULL){
                                                                noAuxiliar=noAuxiliar->noIrmao;
                                                        }
                                                        addIrmao(noAuxiliar,$2);
                                                        }
        ;
	
FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody  { $$ = inserirNo(NULL,"FuncDefinition",$1,linha,coluna);
                                                                addIrmao($1,$2);
                                                                noAuxiliar = $2;
                                                                while (noAuxiliar->noIrmao!=NULL)
                                                                        noAuxiliar = noAuxiliar->noIrmao;
                                                                noAuxiliar->noIrmao = $3;
                                                                }
		     ;

FunctionBody:   LBRACE RBRACE {$$ = inserirNo(NULL,"FuncBody",NULL,linha,coluna);} 
        | LBRACE DeclarationsAndStatements RBRACE {$$ = inserirNo(NULL,"FuncBody",$2,linha,coluna);} 
        ;

DeclarationsAndStatements: DeclarationsAndStatements  Statement {if($1!=NULL){
                                                                        $$=$1;
                                                                        if ($2!=NULL){
                                                                                noAuxiliar = $$;
                                                                                while (noAuxiliar->noIrmao!=NULL)
                                                                                        noAuxiliar = noAuxiliar->noIrmao;
                                                                                noAuxiliar->noIrmao = $2;
                                                                        }
                                                                        }else{
                                                                                $$=$2;
                                                                        }
                                                                }
                | DeclarationsAndStatements  Declaration        {if($1!=NULL){
                                                                        $$=$1;
                                                                        if ($2!=NULL){
                                                                                noAuxiliar = $$;
                                                                                while (noAuxiliar->noIrmao!=NULL)
                                                                                        noAuxiliar = noAuxiliar->noIrmao;
                                                                                noAuxiliar->noIrmao = $2;
                                                                        }
                                                                }else{
                                                                        $$=$2;
                                                                }}
        | Statement {$$=$1;}
        | Declaration {$$=$1;}
        ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI { addIrmao($1,$2); 
                                                        $$ = inserirNo(NULL,"FuncDeclaration",$1,linha,coluna);}
		;

FunctionDeclarator:ID LPAR ParameterList RPAR  {$$=inserirNo($1->id,"Id",NULL,$1->line,$1->col);
                                                addIrmao($$,inserirNo(NULL,"ParamList",$3,linha,coluna));
                                                } 
        ;

ParameterList: ParameterDeclaration {$$ = $1;}
        | ParameterList COMMA ParameterDeclaration      {$$ = $1;
                                                        noAuxiliar = $1;
                                                        while(noAuxiliar->noIrmao!=NULL){
                                                                noAuxiliar=noAuxiliar->noIrmao;
                                                        }
                                                        addIrmao(noAuxiliar,$3);
                                                                }
        ;


ParameterDeclaration: TypeSpec  {$$ = inserirNo(NULL,"ParamDeclaration",$1,linha,coluna);}
		| TypeSpec ID   {addIrmao($1,inserirNo($2->id,"Id",NULL,$2->line,$2->col));
                                $$ = inserirNo(NULL,"ParamDeclaration",$1,linha,coluna);}
                ;
                           
Declaration:TypeSpec DeclarationCD SEMI  {      no * noDeclarate;
                                                /*addIrmao($1,$2);*/
                                                noAuxiliar=$2;
                                                while(noAuxiliar!=NULL){
                                                        noDeclarate=noAuxiliar->noFilho;
                                                        noAuxiliar->noFilho=inserirNo($1->id,$1->type,$1->noFilho,linha,coluna);
                                                        addIrmao(noAuxiliar->noFilho,noDeclarate);
                                                        noAuxiliar=noAuxiliar->noIrmao;
                                                }
                                                $$ = $2;}
            | error SEMI                 {$$ = inserirNo(NULL,NULL,NULL,linha,coluna);errorFlag=1;}
            ;

DeclarationCD: Declarator       {$$=inserirNo(NULL,"Declaration",$1,linha,coluna);}
             | DeclarationCD COMMA Declarator { $$ = $1;
                                                noAuxiliar = $$;
                                                while(noAuxiliar->noIrmao!=NULL){
                                                        noAuxiliar=noAuxiliar->noIrmao;
                                                }
                                                addIrmao(noAuxiliar,inserirNo(NULL,"Declaration",$3,linha,coluna));
                                                }
                          

TypeSpec: CHAR  {$$ = inserirNo(NULL,"Char",NULL,linha,coluna);}
        | INT   {$$ = inserirNo(NULL,"Int",NULL,linha,coluna);}
        | VOID  {$$ = inserirNo(NULL,"Void",NULL,linha,coluna);}
        | SHORT {$$ = inserirNo(NULL,"Short",NULL,linha,coluna);}
        | DOUBLE {$$ = inserirNo(NULL,"Double",NULL,linha,coluna);}
        ;

Declarator: ID {$$ = inserirNo($1->id,"Id",NULL,$1->line,$1->col);}
       | ID ASSIGN CommaExpr { $$ = inserirNo($1->id,"Id",NULL,$1->line,$1->col); addIrmao($$,$3);}
       ;

Statement: CommaExpr SEMI {$$ = $1;}
        | SEMI       {$$ = NULL;} 
        | LBRACE Statementlbrace  {$$ = $2;
                                   if ($2!=NULL && $2->noIrmao!=NULL){ $$ = inserirNo(NULL,"StatList",$2,linha,coluna); }
                                }
        | IF LPAR CommaExpr RPAR StatementError { if($5==NULL) $5=inserirNo(NULL,"Null",NULL,linha,coluna);
                                             addIrmao($3,$5);
                                             addIrmao($5,inserirNo(NULL,"Null",NULL,linha,coluna));
                                             $$ = inserirNo(NULL,"If",$3,linha,coluna);  }
        | IF LPAR CommaExpr RPAR StatementError ELSE StatementError {if($5==NULL) $5=inserirNo(NULL,"Null",NULL,linha,coluna);
                                                                if($7==NULL) $7=inserirNo(NULL,"Null",NULL,linha,coluna);
                                                                addIrmao($3,$5);
                                                                addIrmao($5,$7);
                                                                $$ = inserirNo(NULL,"If",$3,linha,coluna);
                                                                ;}
        | WHILE LPAR CommaExpr RPAR StatementError { if($5==NULL) $5=inserirNo(NULL,"Null",NULL,linha,coluna);
                                                addIrmao($3,$5);                                                
                                                $$ = inserirNo(NULL,"While",$3,linha,coluna);
                                                }
        | RETURN StatementReturn {$$=$2;}
        ;

Statementlbrace: StatementIrmao RBRACE  {$$=$1;}
        | error RBRACE  {$$ = inserirNo(NULL,NULL,NULL,linha,coluna);errorFlag=1;}
        |  RBRACE  {$$ = NULL;}
        ;

StatementError: Statement {$$=$1;}
        | error SEMI {$$ = inserirNo(NULL,NULL,NULL,linha,coluna);errorFlag=1;}
        ;

StatementIrmao : StatementIrmao StatementError {
                                                if($1!=NULL){
                                                        $$ = $1;
                                                        if ($2!=NULL){
                                                                noAuxiliar = $1;
                                                                while (noAuxiliar->noIrmao!=NULL)
                                                                        noAuxiliar = noAuxiliar->noIrmao;
                                                                noAuxiliar->noIrmao = $2;
                                                        }
                                                    }else
                                                        $$ = $2;
                                                    }    
                | StatementError {$$=$1;}

StatementReturn: SEMI  {$$ = inserirNo(NULL,"Return",inserirNo(NULL,"Null",NULL,linha,coluna),linha,coluna);}
        |  CommaExpr SEMI {$$ = inserirNo(NULL,"Return",$1,linha,coluna);}
        ;

Expr:   Expr ASSIGN Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Store",$1,linha,coluna);}
        | Expr COMMA Expr {addIrmao($1,$3);$$=$1;}
        | Expr PLUS Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Add",$1,linha,coluna);}
        | Expr MINUS Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Sub",$1,linha,coluna);}
        | Expr MUL Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Mul",$1,linha,coluna);}
        | Expr DIV Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Div",$1,linha,coluna);}
        | Expr MOD Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Mod",$1,linha,coluna);}
        | Expr OR Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Or",$1,linha,coluna);}
        | Expr AND Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"And",$1,linha,coluna);}
        | Expr BAND Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseAnd",$1,linha,coluna);}
        | Expr BOR Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseOr",$1,linha,coluna);}
        | Expr BXOR Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseXor",$1,linha,coluna);}
        | Expr EQ Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Eq",$1,linha,coluna);}
        | Expr NE Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Ne",$1,linha,coluna);}
        | Expr LE Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Le",$1,linha,coluna);}
        | Expr GE Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Ge",$1,linha,coluna);}
        | Expr LT Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Lt",$1,linha,coluna);}
        | Expr GT Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Gt",$1,linha,coluna);}
        | ExprMath          
        | ExprFunctions {$$ = inserirNo(NULL,"Call",$1,linha,coluna);}
        | ID LPAR error RPAR   {$$ = inserirNo(NULL,NULL,NULL,linha,coluna);errorFlag=1;}
        | ID {$$=inserirNo($1->id,"Id",NULL,$1->line,$1->col);}
        | INTLIT {$$=inserirNo($1->id,"IntLit",NULL,$1->line,$1->col);}
        | CHRLIT  {$$=inserirNo($1->id,"ChrLit",NULL,$1->line,$1->col);}
        | REALLIT    {$$=inserirNo($1->id,"RealLit",NULL,$1->line,$1->col);}
        | LPAR CommaExpr RPAR      {$$ = $2;} 
        | LPAR error RPAR {$$ = inserirNo(NULL,NULL,NULL,linha,coluna);errorFlag=1;}
        ;

CommaExpr :CommaExpr ASSIGN CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Store",$1,linha,coluna);}
        | CommaExpr COMMA CommaExpr     {noAuxiliar=$1;
                                        while(noAuxiliar->noIrmao!=NULL){
                                             noAuxiliar=noAuxiliar->noIrmao;
                                        } 
                                        noAuxiliar->noIrmao=$3;
                                        $$ = inserirNo(NULL,"Comma",$1,linha,coluna);}
        | CommaExpr PLUS CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Add",$1,linha,coluna);}
        | CommaExpr MINUS CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"Sub",$1,linha,coluna);}
        | CommaExpr MUL CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"Mul",$1,linha,coluna);}
        | CommaExpr DIV CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"Div",$1,linha,coluna);}
        | CommaExpr MOD CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"Mod",$1,linha,coluna);}
        | CommaExpr OR CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Or",$1,linha,coluna);}
        | CommaExpr AND CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"And",$1,linha,coluna);}
        | CommaExpr BAND CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseAnd",$1,linha,coluna);}
        | CommaExpr BOR CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseOr",$1,linha,coluna);}
        | CommaExpr BXOR CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseXor",$1,linha,coluna);}
        | CommaExpr EQ CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Eq",$1,linha,coluna);}
        | CommaExpr NE CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Ne",$1,linha,coluna);}
        | CommaExpr LE CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Le",$1,linha,coluna);}
        | CommaExpr GE CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Ge",$1,linha,coluna);}
        | CommaExpr LT CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Lt",$1,linha,coluna);}
        | CommaExpr GT CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Gt",$1,linha,coluna);}
        | ExprMath          
        | ExprFunctions {$$ = inserirNo(NULL,"Call",$1,linha,coluna);}
        | ID LPAR error RPAR   {$$ = inserirNo(NULL,NULL,NULL,linha,coluna);errorFlag=1;}
        | ID {$$=inserirNo($1->id,"Id",NULL,$1->line,$1->col);}
        | INTLIT {$$=inserirNo($1->id,"IntLit",NULL,$1->line,$1->col);}
        | CHRLIT  {$$=inserirNo($1->id,"ChrLit",NULL,$1->line,$1->col);}
        | REALLIT    {$$=inserirNo($1->id,"RealLit",NULL,$1->line,$1->col);}
        | LPAR CommaExpr RPAR      {$$ = $2;} 
        | LPAR error RPAR {$$ = inserirNo(NULL,NULL,NULL,linha,coluna);errorFlag=1;}
        ;

ExprFunctions:    ID LPAR RPAR  {$$ = inserirNo($1->id,"Id",NULL,$1->line,$1->col);}  
                | ID LPAR Expr RPAR    {$$ = inserirNo($1->id,"Id",NULL,$1->line,$1->col);
                                        addIrmao($$,$3);} 

ExprMath:  PLUS CommaExpr  %prec NOT   {$$ = inserirNo(NULL,"Plus",$2,linha,coluna);}
        | MINUS CommaExpr  %prec NOT  {$$ = inserirNo(NULL,"Minus",$2,linha,coluna);}
        | NOT CommaExpr      {$$ = inserirNo(NULL,"Not",$2,linha,coluna);}
