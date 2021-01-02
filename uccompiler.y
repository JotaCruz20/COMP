%{
    /*
        João Alexandre Santos Cruz 2018288464
        André Cristóvão Ferreira da Silva  2018277921
    */

    #include <stdio.h>
    #include <stdlib.h>  
    #include <string.h>  
    #include "tree.h"
    #include "error.h"
    int yylex(void);
    void yyerror (char * s);

    extern int errorFlag;
    extern struct no * noAST;
    extern int flag;
    extern int linha;
    int counter = 0;
    extern int coluna;

    char * auxType;
    no * noAuxiliar;

    no * inserirNo(char * id, char * type, no * noFilho,int line,int col){ //insere o no na AST 
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

    void addIrmao(no * no1, no * no2){ //adiciona o irmao no final da lista
        no * noAuxiliar  = no1;
        while(noAuxiliar->noIrmao!=NULL){
                noAuxiliar=noAuxiliar->noIrmao;
        }
        noAuxiliar->noIrmao = no2;
    }

    void printTree(no * auxNode, int pontos){ //print da AST
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

    /* A passagem pelo codigo é feita bottom-up, ou seja, vai ser começado no final até ao inicio, e vao ser
    passados para o topo da pilha o no com o filho é adicionado na AST com os seus irmaos */
    /* Na union é posto o no * e o noLex que tem a linha e coluna do token, assim como o seu id*/

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
%token <noCol> CHAR 
%token <noCol>  ELSE
%token <noCol> WHILE
%token <noCol> IF
%token <noCol> INT 
%token <noCol> SHORT
%token <noCol> DOUBLE
%token <noCol> RETURN
%token <noCol> VOID
%token <noCol> BAND
%token <noCol> BOR
%token <noCol> BXOR
%token <noCol> AND
%token <noCol> ASSIGN
%token <noCol> MUL
%token <noCol> COMMA
%token <noCol> DIV
%token <noCol> EQ
%token <noCol> GE
%token <noCol> GT
%token <noCol> LBRACE
%token <noCol> LE
%token <noCol> LPAR
%token <noCol> LT
%token <noCol> MINUS
%token <noCol> MOD
%token <noCol> NE
%token <noCol> NOT
%token <noCol> OR
%token <noCol> PLUS
%token <noCol> RBRACE
%token <noCol> RPAR
%token <noCol> SEMI
%token <noCol> RESERVED


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

Programa: FunctionsAndDeclarations {noAST=inserirNo(NULL,"Program",$1,linha,coluna);counter+=1;}
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

FunctionBody:   LBRACE RBRACE {$$ = inserirNo(NULL,"FuncBody",NULL,$1->line,$1->col);} 
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
                                                noAuxiliar=$2;
                                                while(noAuxiliar!=NULL){
                                                        noDeclarate=noAuxiliar->noFilho;
                                                        noAuxiliar->noFilho=inserirNo($1->id,$1->type,$1->noFilho,$1->line,$1->col);
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
                          

TypeSpec: CHAR  {$$ = inserirNo(NULL,"Char",NULL,$1->line,$1->col);}
        | INT   {$$ = inserirNo(NULL,"Int",NULL,$1->line,$1->col);}
        | VOID  {$$ = inserirNo(NULL,"Void",NULL,$1->line,$1->col);}
        | SHORT {$$ = inserirNo(NULL,"Short",NULL,$1->line,$1->col);}
        | DOUBLE {$$ = inserirNo(NULL,"Double",NULL,$1->line,$1->col);}
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
                                             $$ = inserirNo(NULL,"If",$3,$1->line,$1->col);  }
        | IF LPAR CommaExpr RPAR StatementError ELSE StatementError {if($5==NULL) $5=inserirNo(NULL,"Null",NULL,linha,coluna);
                                                                if($7==NULL) $7=inserirNo(NULL,"Null",NULL,linha,coluna);
                                                                addIrmao($3,$5);
                                                                addIrmao($5,$7);
                                                                $$ = inserirNo(NULL,"If",$3,$1->line,$1->col);
                                                                ;}
        | WHILE LPAR CommaExpr RPAR StatementError { if($5==NULL) $5=inserirNo(NULL,"Null",NULL,linha,coluna);
                                                addIrmao($3,$5);                                                
                                                $$ = inserirNo(NULL,"While",$3,$1->line,$1->col);
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

Expr:   Expr ASSIGN Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Store",$1,$2->line,$2->col);}
        | Expr COMMA Expr {addIrmao($1,$3);$$=$1;}
        | Expr PLUS Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Add",$1,$2->line,$2->col);}
        | Expr MINUS Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Sub",$1,$2->line,$2->col);}
        | Expr MUL Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Mul",$1,$2->line,$2->col);}
        | Expr DIV Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Div",$1,$2->line,$2->col);}
        | Expr MOD Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Mod",$1,$2->line,$2->col);}
        | Expr OR Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Or",$1,$2->line,$2->col);}
        | Expr AND Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"And",$1,$2->line,$2->col);}
        | Expr BAND Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseAnd",$1,$2->line,$2->col);}
        | Expr BOR Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseOr",$1,$2->line,$2->col);}
        | Expr BXOR Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseXor",$1,$2->line,$2->col);}
        | Expr EQ Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Eq",$1,$2->line,$2->col);}
        | Expr NE Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Ne",$1,$2->line,$2->col);}
        | Expr LE Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Le",$1,$2->line,$2->col);}
        | Expr GE Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Ge",$1,$2->line,$2->col);}
        | Expr LT Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Lt",$1,$2->line,$2->col);}
        | Expr GT Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Gt",$1,$2->line,$2->col);}
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

CommaExpr :CommaExpr ASSIGN CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Store",$1,$2->line,$2->col);}
        | CommaExpr COMMA CommaExpr     {noAuxiliar=$1;
                                        while(noAuxiliar->noIrmao!=NULL){
                                             noAuxiliar=noAuxiliar->noIrmao;
                                        } 
                                        noAuxiliar->noIrmao=$3;
                                        $$ = inserirNo(NULL,"Comma",$1,$2->line,$2->col);}
        | CommaExpr PLUS CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Add",$1,$2->line,$2->col);}
        | CommaExpr MINUS CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"Sub",$1,$2->line,$2->col);}
        | CommaExpr MUL CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"Mul",$1,$2->line,$2->col);}
        | CommaExpr DIV CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"Div",$1,$2->line,$2->col);}
        | CommaExpr MOD CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"Mod",$1,$2->line,$2->col);}
        | CommaExpr OR CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Or",$1,$2->line,$2->col);}
        | CommaExpr AND CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"And",$1,$2->line,$2->col);}
        | CommaExpr BAND CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseAnd",$1,$2->line,$2->col);}
        | CommaExpr BOR CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseOr",$1,$2->line,$2->col);}
        | CommaExpr BXOR CommaExpr {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseXor",$1,$2->line,$2->col);}
        | CommaExpr EQ CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Eq",$1,$2->line,$2->col);}
        | CommaExpr NE CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Ne",$1,$2->line,$2->col);}
        | CommaExpr LE CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Le",$1,$2->line,$2->col);}
        | CommaExpr GE CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Ge",$1,$2->line,$2->col);}
        | CommaExpr LT CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Lt",$1,$2->line,$2->col);}
        | CommaExpr GT CommaExpr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Gt",$1,$2->line,$2->col);}
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

ExprMath:  PLUS CommaExpr  %prec NOT   {$$ = inserirNo(NULL,"Plus",$2,$1->line,$1->col);}
        | MINUS CommaExpr  %prec NOT  {$$ = inserirNo(NULL,"Minus",$2,$1->line,$1->col);}
        | NOT CommaExpr      {$$ = inserirNo(NULL,"Not",$2,$1->line,$1->col);}
