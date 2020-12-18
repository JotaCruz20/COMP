#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"

typedef struct noTabela
{
	char * id;
	char * tipo;
	char * params;	
	struct noTabela * next;
} noTabela;

typedef struct tabela
{
	char * name;
	char * type;
	int definida;
	int flag;
	int print;
	int params;
	struct noTabela * tabelaAtual;
	struct tabela * next;
} tabela;

//ver oq faz a flag
void initTabela();
int insert(char * id, char * tipo, char * params, char * nomeTabela,int line, int col,int flag,int noCount);
tabela * searchTabela(char * nome);
void initFunctionTabela(char * name, int flag,int print,int params, int def,int line,int col,int error,int noCount);
void printTabela();
char* searchId(char* nTabela,char * id);
int checkFunc(char * nome);
int nParams(char * nome);
int checkParamsType(tabela * aux, no * atual);
void checkParamsTypeError(char * nome, char * nomeFunc, no * atual,no* pai);
char * getTypeParams(char * nome);
void clearSymTableParams(noTabela * aux);
void clearSymTable();
#endif