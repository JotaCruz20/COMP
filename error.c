#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <ctype.h> 
#include "semantics.h"
#include "symtab.h"
#include "tree.h"
#include "error.h"

erros * errorsHead;

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