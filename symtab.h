#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	int flag;
	struct noTabela * tabelaAtual;
	struct tabela * next;
} tabela;

//ver oq faz a flag
void initTabela();
void insert(char * id, char * tipo, char * params, char * nomeTabela);
tabela * searchTabela(char * nome);
void initFunctionTabela(char * name, int flag);
void printTabela();
char* searchId(char* nTabela,char * id);

#endif