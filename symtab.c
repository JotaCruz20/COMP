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


char* searchId(int nTabela,char * id){
	char * type = (char *) calloc(2,sizeof(char));
	int counter = 0;
	tabela * aux = tabelaSimbolos;
	if(aux!=NULL){
			noTabela * auxTab = aux->tabelaAtual;
			//ve se está no global -> se estiver guarda e verifica se n foi definido no atual
			while(auxTab!=NULL){
				if(strcmp(auxTab->id,id)==0){
					if(auxTab->params!=NULL){
						type = (char *) malloc((strlen(auxTab->tipo)+strlen(auxTab->params)+4)*sizeof(char));
						strcpy(type,auxTab->tipo);
						strcat(type,auxTab->params);
					}
					else{
						type = (char *) malloc((strlen(auxTab->tipo)+4)*sizeof(char));
						strcpy(type,auxTab->tipo);
					}
				}
				auxTab=auxTab->next;
			}
			while(counter!= nTabela){
				aux = aux ->next;
				if(aux->tabelaAtual!=NULL){
					counter+=1;
				}
			}
			auxTab = aux->tabelaAtual;
			while(auxTab!=NULL){
				if(strcmp(auxTab->id,id)==0){
					type = (char *) malloc((strlen(auxTab->tipo)+4)*sizeof(char));
					strcpy(type,auxTab->tipo);
				}
				auxTab=auxTab->next;
			}
			if(strcmp(type,"")!=0){
				return strdup(type);
			}
			else{
				return strdup("undef");
			}
			
	}
	return strdup("undef");;
}
