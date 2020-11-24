#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <ctype.h> 
#include "semantics.h"
#include "symtab.h"
#include "tree.h"

char name[20];
int bodys = 0;

void checkProgram(no * atual){ 
	if (atual == NULL)
		return;
	if (atual->type != NULL){
		if (strcmp(atual->type, "Program")==0){ // quando começa a analise sintatica tem de inicializar as vars globais
			initTabela();
		}
		if (strcmp(atual->type, "FuncDefinition")==0){
			checkFuncDefinition(atual);
		}
		if (strcmp(atual->type, "FuncDeclaration")==0){
			checkFuncDeclaration(atual);
		}
		if (strcmp(atual->type, "Declaration")==0){
			checkDeclaration(atual);
		}
		if(strcmp(atual->type,"FuncBody")==0){
			checkBody(atual);
		}
	}
	no * auxNode = atual->noFilho;

    // chechar o programa para os varios irmaos
	while(auxNode != NULL){
		if (atual->type != NULL && strcmp(atual->type, "Program")==0)
			strcpy(name,"Global");
		checkProgram(auxNode);
		auxNode = auxNode->noIrmao;	
	}
}


void toLowerCase(char* str){
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

char * checkParams(no * atual){
	char params[1000] = "(";

	no * auxNode = atual->noFilho;

	while(auxNode != NULL){
		char * type =	(char *) strdup(auxNode->noFilho->type);	
		toLowerCase(type);
		strcat(params,type);
		auxNode = auxNode->noIrmao;
		if (auxNode != NULL)
			strcat(params,",");
	}
	strcat(params,")");
	return strdup(params);
}

void addParamsFunction(no * atual, char * nameTable){
	no * auxNode = atual->noFilho;

	while(auxNode != NULL){
		if (auxNode->noFilho != NULL){
			char * type =	strdup(auxNode->noFilho->type);	
            toLowerCase(type);
			if(strcmp(type,"void")!=0){
				insert(auxNode->noFilho->noIrmao->id, type, "\tparam", nameTable);
			}
		}
		auxNode = auxNode->noIrmao;
	}
}


void checkFuncDefinition(no * atual){ //pode haver aqui erros
	char * type = (char *) strdup(atual->noFilho->type); 
    toLowerCase(type);
	char * id = (char *) strdup(atual->noFilho->noIrmao->id);
	char * params = checkParams(atual->noFilho->noIrmao->noIrmao);

	strcpy(name, id);

	insert(id, type, params, "Global");
	initFunctionTabela(id, 1);
	insert("return", type, "", id);
	addParamsFunction(atual->noFilho->noIrmao->noIrmao, id);
}

void checkFuncDeclaration(no * atual){
	char * type = (char *) strdup(atual->noFilho->type); 
    toLowerCase(type);
	char * id = (char *) strdup(atual->noFilho->noIrmao->id);
	char * params = checkParams(atual->noFilho->noIrmao->noIrmao);

	insert(id, type, params, "Global");
	initFunctionTabela(id, 0);
}


void checkDeclaration(no * atual){
    char * type = (char *) strdup(atual->noFilho->type); 
    toLowerCase(type);
    char * id = (char *) strdup(atual->noFilho->noIrmao->id);
	atual->noFilho->noIrmao->noIrmao->exprType = (char *) malloc((strlen(type)+4)*sizeof(char));
	sprintf(atual->noFilho->noIrmao->noIrmao->exprType, " - %s",type);
    insert(id, type, "", name);
}

void printTreeAnotada(no * auxNode, int pontos){
	 int i=0;
        if(auxNode!=NULL){
                for(i=0;i<pontos*2;i++){
                        printf(".");
                }
				if(auxNode->id!=NULL){
					if(auxNode->exprType==NULL){
						printf("%s(%s)\n", auxNode->type,auxNode->id);
					}
					else{
						printf("%s(%s) %s\n", auxNode->type,auxNode->id,auxNode->exprType);
					}
					}
				else{
					if(auxNode->exprType==NULL){
						printf("%s\n", auxNode->type);
					}
					else{
						printf("%s %s\n", auxNode->type, auxNode->exprType);
					}
				}
                if(auxNode->noFilho!=NULL){
                        printTreeAnotada(auxNode->noFilho,pontos+1);
                }
                if(auxNode->noIrmao!=NULL){
                        printTreeAnotada(auxNode->noIrmao,pontos);
                }

        }
}

void checkBody(no * atual){
	no * aux = atual;
	if(aux!=NULL){
		if(strcmp(aux->type,"ChrLit")==0 || strcmp(aux->type,"IntLit")==0){
			aux->exprType= (char *) strdup("- int");
		}
		else if(strcmp(aux->type,"RealLit")==0){
			aux->exprType= (char *) strdup("- double");
		}
		else if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type,"Not")==0){
			aux->exprType= (char *) strdup("- int");
		}
		else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
			aux->exprType= (char *) strdup("- int");
		}
		if(aux->noFilho!=NULL){
            checkBody(aux->noFilho);
        }
        if(aux->noIrmao!=NULL){
			checkBody(aux->noIrmao);
        }
	}
}


