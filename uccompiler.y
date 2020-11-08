%{
    /*
        João Alexandre Santos Cruz 2018288464
        André Cristóvão Ferreira da Silva  2018277921
    */

    #include <stdio.h>
    #include <stdlib.h>  
    #include <string.h>  

%}
%token CHRLIT 
%token ID
%token REALLIT
%token INTLIT


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

Programa: FunctionsAndDeclarations 
	    ;

FunctionsAndDeclarations:	  FunctionDefinition 
        | FunctionDeclaration 
        | Declaration 
        | FunctionsAndDeclarations FunctionDefinition 
        | FunctionsAndDeclarations FunctionDeclaration 
        | FunctionsAndDeclarations Declaration
        ;
	
FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody
			        ;

FunctionBody:   LBRACE RBRACE
        | LBRACE DeclarationsAndStatements RBRACE
        ;

DeclarationsAndStatements: DeclarationsAndStatements  Statement 
        | DeclarationsAndStatements  Declaration 
        | Statement 
        | Declaration
        ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI
		;

FunctionDeclarator:ID LPAR ParameterList RPAR
        ;

ParameterList: ParameterDeclaration 
        | ParameterList COMMA ParameterDeclaration 
        ;


ParameterDeclaration: TypeSpec 
		| TypeSpec ID
          ;
                           
Declaration:TypeSpec DeclarationCD SEMI
             ;

DeclarationCD: Declarator 
        | DeclarationCD COMMA Declarator
        ;
                                     

TypeSpec: CHAR 
        | INT 
        | VOID 
        | SHORT 
        | DOUBLE
        ;

Declarator: ID 
       | ID ASSIGN Expr
       ;

Statement: Expr SEMI
        | SEMI
        |  LBRACE Statementlbrace
        | IF LPAR Expr RPAR Statement 
        | IF LPAR Expr RPAR Statement ELSE Statement
        | WHILE LPAR Expr RPAR Statement
        | RETURN StatementReturn
        ;

Statementlbrace: Statement RBRACE
        |  RBRACE
        ;

StatementReturn: SEMI
        |  Expr SEMI
        ;

Expr:   Expr ASSIGN Expr 
        | Expr COMMA Expr   
        | Expr PLUS Expr
        | Expr MINUS Expr
        | Expr MUL Expr
        | Expr DIV Expr
        | Expr MOD Expr
        | Expr OR Expr
        | Expr AND Expr
        | Expr BAND Expr
        | Expr BOR Expr
        | Expr BXOR Expr
        | Expr EQ Expr
        | Expr NE Expr
        | Expr LE Expr
        | Expr GE Expr
        | Expr LT Expr
        | Expr GT Expr
        | PLUS Expr
        | MINUS Expr
        | NOT Expr
        | ID LPAR RPAR
        | ID LPAR ExprCE RPAR
        | ID 
        | INTLIT 
        | CHRLIT 
        | REALLIT 
        | LPAR Expr RPAR
        ;

ExprCE: Expr
        | ExprCE COMMA Expr
        ;