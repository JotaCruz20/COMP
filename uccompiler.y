%{
    /*
        João Alexandre Santos Cruz 2018288464
        André Cristóvão Ferreira da Silva  2018277921
    */

    #include <stdio.h>
    #include <stdlib.h>  
    #include <string.h>  

    int yylex(void);
    void yyerror (char * s);

    typedef struct no{
        char * id;
        char * type;
        struct no * noFilho; // cada no tem 1 filho e 1 irmao(podem ser null)
        struct no * noIrmao; // facilita, pois so temos de ter 1 pointer para o filho
    } no;                    // e o filho terá pointer para os irmaos (nao e necessario array de ponteiros)

    int commaFlag = 0;
    int errorFlag = 0;
    extern int flag;

    char * auxType;
    no * noAtual;

    no * inserirNo(char * id, char * type, no * noFilho){
        node * noAtual = (node *)malloc(sizeof(no));
        noAtual->type = type;
        noAtual->id = id;
        noAtual->noFilho = noFilho;
        noAtual->noIrmao = NULL;
    return noAtual;
    }

    void addIrmao(no * no1, no * no2)
    {
        no1->noIrmao = no2;
    }

%}

%union{
    struct no * no;
    char * id;
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
%type <no> Expr

%token <id> CHRLIT 
%token <id> ID
%token <id> REALLIT
%token <id> INTLIT
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

Programa: FunctionsAndDeclarations {$$=inserirNo(NULL,"Program",$1);}
	    ;

FunctionsAndDeclarations: FunctionDefinition  {$$ = $1;}
        | FunctionDeclaration  {$$ = $1;}
        | Declaration  {$$ = $1;}
        | FunctionsAndDeclarations FunctionDefinition {addIrmao($1,$2);$$ = $1;}
        | FunctionsAndDeclarations FunctionDeclaration  {addIrmao($1,$2);$$ = $1;}
        | FunctionsAndDeclarations Declaration {addIrmao($1,$2);$$ = $1;}
        ;
	
FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody  { $$ = inserirNo(NULL,"FuncDefinition",$1);
                                                                addIrmao($1,$2);
                                                                addIrmao($2,$3);
                                                                }
		     ;

FunctionBody:   LBRACE RBRACE {$$ = inserirNo(NULL,"FuncBody",NULL);} 
        | LBRACE DeclarationsAndStatements RBRACE {$$ = inserirNo(NULL,"FuncBody",$2);} 
        ;

DeclarationsAndStatements: DeclarationsAndStatements  Statement {if($2!=NULL){
                                                                        $$=$2;
                                                                        addIrmao($2,$1);
                                                                }else{
                                                                        $$=$1;
                                                                }
                                                                addIrmao($1,$2);}
        | DeclarationsAndStatements  Declaration  {$$=$2; addIrmao($1,$2);}
        | Statement {$$=$1;}
        | Declaration {$$=$1;}
        ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI { addIrmao($1,$2); $$ = inserirNo(NULL,"FuncDeclaration",$1);}
		;

FunctionDeclarator:ID LPAR ParameterList RPAR  {$$= inserirNo($1,"Id",NULL);
                                                addIrmao($$,inserirNo(NULL,"ParamList",$3))
                                                /* pode causar erro aqui*/;} 
        ;

ParameterList: ParameterDeclaration {$$ = $1;}
        | ParameterList COMMA ParameterDeclaration  {addIrmao($3,$1);$$ = $3;}
        ;


ParameterDeclaration: TypeSpec  {$$ = insertNode(NULL,"ParamDeclaration",$1);}
		| TypeSpec ID   {addIrmao($1,inserirNo($2,"Id",NULL));
                                $$ = inserirNo(NULL,"ParamDeclaration",$1);}
                ;
                           
Declaration:TypeSpec DeclarationCD SEMI  {addIrmao($1,$2);$$ = inserirNo(NULL,"Declaration",$1);}
            | error SEMI                 {$$ = insertNode(NULL,NULL,NULL);}
            ;

DeclarationCD: Declarator       {$$=$1;}
        | DeclarationCD COMMA Declarator { /* pode causar erro */
                                           $$= inserirNo("type","Declaration",$2);
                                           $$->nodeBrother = $1;
                                        }
        ;
                                     

TypeSpec: CHAR  {$$ = inserirNo(NULL,"Char",NULL);}
        | INT   {$$ = inserirNo(NULL,"Int",NULL);}
        | VOID  {$$ = inserirNo(NULL,"Void",NULL);}
        | SHORT {$$ = inserirNo(NULL,"Short",NULL);}
        | DOUBLE {$$ = inserirNo(NULL,"Double",NULL);}
        ;

Declarator: ID {$$ = inserirNo($1,"Id",NULL);}
       | ID ASSIGN Expr { $$ = insertNode($1,"Id",NULL); $$->nodeBrother = $3;}
       ;

Statement: Expr SEMI {$$ = $1;}
        | SEMI       {$$ = NULL;} 
        | LBRACE Statementlbrace  {$$ = $2;}
        | IF LPAR Expr RPAR StatementError { if($5==NULL) $5=insertNode(NULL,"Null",NULL);
                                             addIrmao($3,$5);
                                             addIrmao($5,insertNode(NULL,"Null",NULL));
                                             $$ = insertNode(NULL,"If",$3);  }
        | IF LPAR Expr RPAR StatementError ELSE StatementError
        | WHILE LPAR Expr RPAR StatementError
        | RETURN StatementReturn {$$=$2;}
        ;

Statementlbrace: StatementError RBRACE  {$$=$1;}
        | error RBRACE  {$$ = inserirNo(NULL,NULL,NULL);}
        |  RBRACE  {$$ = NULL;}
        ;

StatementError: Statement {$$=$1;}
        | error SEMI {$$ = inserirNo(NULL,NULL,NULL);}
        ;

StatementReturn: SEMI  {$$ = inserirNo(NULL,"Return",inserirNo(NULL,"Null",NULL));}
        |  Expr SEMI {$$ = inserirNo(NULL,"Return",$1);}
        ;

Expr:   Expr ASSIGN Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Store",$1);}
        | Expr COMMA Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Comma",$1);}
        | Expr PLUS Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Plus",$1);}
        | Expr MINUS Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Minus",$1);}
        | Expr MUL Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Mul",$1);}
        | Expr DIV Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Div",$1);}
        | Expr MOD Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"Mod",$1);}
        | Expr OR Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Or",$1);}
        | Expr AND Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"And",$1);}
        | Expr BAND Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseAnd",$1);}
        | Expr BOR Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseOr",$1);}
        | Expr BXOR Expr {addIrmao($1,$3);$$ = inserirNo(NULL,"BitWiseXor",$1);}
        | Expr EQ Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Eq",$1);}
        | Expr NE Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Ne",$1);}
        | Expr LE Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Le",$1);}
        | Expr GE Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Ge",$1);}
        | Expr LT Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Lt",$1);}
        | Expr GT Expr  {addIrmao($1,$3);$$ = inserirNo(NULL,"Gt",$1);}
        | PLUS Expr     {$$ = inserirNo(NULL,"Plus",$2)}
        | MINUS Expr    {$$ = inserirNo(NULL,"Minus",$2);}
        | NOT Expr      {$$ = inserirNo(NULL,"Not",$2);}
        | ID LPAR RPAR  {$$ = inserirNo($1,"Id",NULL);}
        | ID LPAR Expr RPAR    {%falta isto}                 {%isto poderá estar mal!%}  
        | ID LPAR error RPAR   {%falta isto}
        | ID {$$=inserirNo($1,"Id",NULL);}
        | INTLIT {$$=inserirNo($1,"IntLit",NULL);}
        | CHRLIT  {$$=inserirNo($1,"ChrLit",NULL);}
        | REALLIT    {$$=inserirNo($1,"RealLit",NULL);}
        | LPAR Expr RPAR  {%falta isto}    
        | LPAR error RPAR {$$ = inserirNo(NULL,NULL,NULL);}
        ;
