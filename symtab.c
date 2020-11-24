#include "symtab.h"

tabela * tabelaSimbolos;

void initTabela(){
    tabelaSimbolos = (tabela *)malloc(sizeof(tabela));

	tabelaSimbolos->type = (char *)malloc((strlen("Global") + 1)*sizeof(char));
	strcpy(tabelaSimbolos->type, "Global");

	tabelaSimbolos->name = "";
	tabelaSimbolos->flag = 1;

	insert("putchar","int(int)","","Global");
	insert("getchar","int(void)","","Global");
}


void insert(char * id, char * tipo, char * params, char * nomeTabela){
    noTabela * tab = (noTabela*)malloc(sizeof(noTabela));
    tab->id=(char *)malloc((strlen(id)+1)*sizeof(char));
    tab->tipo=(char *)malloc((strlen(tipo)+1)*sizeof(char));
    tab->params=(char *)malloc((strlen(params)+1)*sizeof(char));
    strcpy(tab->id,id);
    strcpy(tab->tipo,tipo);
    strcpy(tab->params,params);
    tabela * aux = searchTabela(nomeTabela);
    if (aux->tabelaAtual == NULL)
		aux->tabelaAtual = tab;
	else{
		noTabela * auxNoTabela = aux->tabelaAtual;
		while (auxNoTabela->next != NULL){
			if (strcmp(auxNoTabela->id, id)==0){ // se ja estiver o id na tabela nao precisamos do adicionar novamente
				free(tab);
				return;
			}
			auxNoTabela = auxNoTabela->next;
		}
		if (strcmp(auxNoTabela->id, id)==0){ // se ja estiver o id na tabela nao precisamos do adicionar novamente
			free(tab);
			return;
		}
		auxNoTabela->next = tab;
	}
}

tabela * searchTabela(char * nome){
    tabela * aux;
    if (strcmp(nome, "Global")==0)
		return tabelaSimbolos;
	else{
		aux = tabelaSimbolos->next;
		while (aux != NULL && strcmp(nome,aux->name)!=0)
			aux = aux->next;
        return aux;
	}
}

void initFunctionTabela(char * name, int flag){
	tabela * novaTabela = (tabela *)malloc(sizeof(tabela));

	novaTabela->type = (char *)malloc((strlen("Function") + 1)*sizeof(char));
	strcpy(novaTabela->type, "Function");
    novaTabela->name = (char *)malloc((strlen(name)+1)*sizeof(char));
	strcpy(novaTabela->name,name);
	novaTabela->flag = flag;
	tabela * auxTabela = tabelaSimbolos;

    while (auxTabela->next != NULL){
        if (strcmp(auxTabela->name, name)==0){
            auxTabela->flag = 1;
            free(novaTabela);
            return;
        }
        auxTabela = auxTabela->next;
    }
    if (strcmp(auxTabela->name, name)==0){
        auxTabela->flag = 1;
        free(novaTabela);
        return;
    }
    auxTabela->next = novaTabela;
}

void printTabela(){
	tabela * auxTabela = tabelaSimbolos;
	noTabela * auxNoTabela;

	while (auxTabela != NULL){
		if (auxTabela->flag == 1){
			if (strcmp(auxTabela->type,"Global")==0)
				printf("===== Global Symbol Table =====\n");
			else 
				printf("===== %s %s Symbol Table =====\n", auxTabela->type, auxTabela->name);
			auxNoTabela = auxTabela->tabelaAtual;

			while ( auxNoTabela != NULL){
				printf("%s\t%s%s\n", auxNoTabela->id, auxNoTabela->tipo, auxNoTabela->params);
				auxNoTabela = auxNoTabela->next;
			}
			printf("\n");
		}
		auxTabela = auxTabela->next;
	}
}
