#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <ctype.h> 
#include "semantics.h"
#include "symtab.h"
#include "tree.h"
#include "error.h"

erros * errorsHead;
char name[20];
char * funcName;
extern int linha;
extern int coluna;

void initErrors(){
	errorsHead = (erros *) malloc(sizeof(erros));
	errorsHead->col=0;
	errorsHead->line=0;
	errorsHead->erro=(char *)malloc((strlen("header")+1)*sizeof(char));
	strcpy(errorsHead->erro,"header");
}

void addErros(int line,int col,char * erro){
	erros * aux = errorsHead;
	erros * create;
	while (aux->noIrmao!=NULL && aux->noIrmao->line<line){
		aux=aux->noIrmao;
	}
	while (aux->noIrmao!=NULL && aux->noIrmao->col<col && aux->noIrmao->line==line){
		aux=aux->noIrmao;
	}
	if(aux->noIrmao==NULL){
		aux->noIrmao = (erros *) malloc(sizeof(erros));
		aux->noIrmao->line=line;
		aux->noIrmao->col=col;
		aux->noIrmao->erro=(char *)malloc(strlen(erro)*sizeof(char));
		strcpy(aux->noIrmao->erro,erro);
	}
	else{
		if(aux->noIrmao->line==line && aux->noIrmao->col==col && strcmp(aux->noIrmao->erro,erro)==0){
			return;
		}
		create = (erros *) malloc(sizeof(erros));
		create->line=line;
		create->col=col;
		create->erro=(char *)malloc(strlen(erro)*sizeof(char));
		strcpy(create->erro,erro);
		create->noIrmao=aux->noIrmao;
		aux->noIrmao=create;
	}
}

void printErros(){
	erros * aux = errorsHead;
	if(aux->noIrmao!=NULL){
		aux=aux->noIrmao;
		while(aux!=NULL){
			char * ptr = strchr(aux->erro, '\n');
			*ptr='\0';
			printf("%s\n",aux->erro);
			aux=aux->noIrmao;
		}
	}
}
// Search for errors

void checkProgramError(no * atual){ 
	if (atual == NULL)
		return;
	if (atual->type != NULL){
		if (strcmp(atual->type, "FuncDefinition")==0){
			funcName = (char *) malloc(strlen(atual->noFilho->noIrmao->id)*sizeof(char));
			strcpy(funcName,atual->noFilho->noIrmao->id);
			checkFuncDefinitionError(atual);
		}
		if (strcmp(atual->type, "FuncDeclaration")==0){
			checkFuncDeclarationError(atual);
		}
		if (strcmp(atual->type, "Declaration")==0){
			if(funcName==NULL){
				funcName = (char *) malloc(strlen("Global")*sizeof(char));
				strcpy(funcName,"Global");
			}
			checkDeclarationError(atual);
		}
	}
	no * auxNode = atual->noFilho;

    // chechar o programa para os varios irmaos
	while(auxNode != NULL){
		if (atual->type != NULL && strcmp(atual->type, "Program")==0)
			strcpy(name,"Global");
		checkProgramError(auxNode);
		auxNode = auxNode->noIrmao;	
	}
}

void checkParamsError(no * atual){
	no * auxNode = atual->noFilho;
	while(auxNode != NULL){
		char * type =	(char *) strdup(auxNode->noFilho->type);	
		toLowerCase(type);
		if(strcmp(type,"void")==0 && auxNode->noFilho->noIrmao!=NULL && strcmp(auxNode->noFilho->noIrmao->type,"Id")==0){
			char error[100];
			sprintf(error,"Line %d, col %d: Invalid use of void type in declaration\n", atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col);
			addErros(auxNode->noFilho->noIrmao->line,auxNode->noFilho->noIrmao->col,error);
		}
	}
}

void checkFuncDefinitionError(no * atual){ 
	checkParams(atual->noFilho->noIrmao->noIrmao);
}

void checkFuncDeclarationError(no * atual){
    checkParams(atual->noFilho->noIrmao->noIrmao);
}

void checkDeclarationError(no * atual){
    char * type = (char *) strdup(atual->noFilho->type); 
    toLowerCase(type);
	if(strcmp(type,"void")==0){
		char error[100];
		sprintf(error,"Line %d, col %d: Invalid use of void type in declaration\n", atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col);
		addErros(atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col,error);
	}
}

void checkBodyError(no * atual,char * pai){
	no * aux = atual;
	if(aux->type!=NULL){
		
	}
}

void anotateBodyError(no * atual){
	if (atual == NULL)
		return;
	if (atual->type != NULL){
		if(strcmp(atual->type, "FuncDefinition")==0){
			funcName = (char *) malloc(strlen(atual->noFilho->noIrmao->id)*sizeof(char));
			strcpy(funcName,atual->noFilho->noIrmao->id);
		}
		else if(strcmp(atual->type,"FuncBody")==0){
			checkBodyError(atual,"FuncBody");
		}
	}
	no * auxNode = atual->noFilho;

    // chechar o programa para os varios irmaos
	while(auxNode != NULL){
		anotateBodyError(auxNode);
		auxNode = auxNode->noIrmao;	
	}
}
