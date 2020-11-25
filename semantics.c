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


void checkFuncDefinition(no * atual){ 
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

void addType(no * atual){
	no  * aux =atual;
	if(aux!=NULL){
		if(strcmp(aux->type,"ChrLit")==0 || strcmp(aux->type,"IntLit")==0){
			aux->exprType= (char *) strdup("- int");
		}
		else if(strcmp(aux->type,"RealLit")==0){
			aux->exprType= (char *) strdup("- double");
		}
		else if (strcmp(aux->type, "Add") == 0 || strcmp(aux->type, "Sub") == 0 ||  strcmp(aux->type, "Mul") == 0 || strcmp(aux->type, "Div") == 0){
			aux->exprType= prioridade(aux);
		}
		else if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type,"Not")==0){
			aux->exprType= (char *) strdup("- int");
		}
		else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
			aux->exprType= (char *) strdup("- int");
		}
		else if (strcmp(aux->type,"Store") == 0 || strcmp(aux->type,"Minus")==0 || strcmp(aux->type,"Plus")==0){
				char * type = searchId(bodys,aux->noFilho->id);
				if(strcmp(type,"undef")==0){
					if(strcmp(aux->noFilho->type,"IntLit")==0 || strcmp(aux->noFilho->type,"ChrLit")==0){
						strcpy(type, "int");
					}
					else if(strcmp(aux->noFilho->type,"RealLit")==0){
						strcpy(type, "double");
					}
				}
				aux->exprType = (char *)malloc((strlen(type)+3)*sizeof(char));
				sprintf(aux->exprType,"- %s",type);
		}
		else if(aux->id!=NULL){
			char * type = searchId(bodys,aux->id);
			aux->exprType = (char *)malloc((strlen(type)+3)*sizeof(char));
			sprintf(aux->exprType,"- %s",type);
		}
		if(aux->noFilho!=NULL){
			addType(aux->noFilho);
		}
		if(aux->noIrmao!=NULL){
			addType(aux->noIrmao);
		}
	}
}


void checkDeclaration(no * atual){
    char * type = (char *) strdup(atual->noFilho->type); 
    toLowerCase(type);
    char * id = (char *) strdup(atual->noFilho->noIrmao->id);
	insert(id, type, "", name);
	if(atual->noFilho->noIrmao->noIrmao!=NULL){
		if(strcmp(atual->noFilho->noIrmao->noIrmao->type,"ChrLit")==0 || strcmp(atual->noFilho->noIrmao->noIrmao->type,"IntLit")==0){
				atual->noFilho->noIrmao->noIrmao->exprType= (char *) strdup("- int");
		}
		else if(strcmp(atual->noFilho->noIrmao->noIrmao->type,"RealLit")==0){
				atual->noFilho->noIrmao->noIrmao->exprType= (char *) strdup("- double");
		}
		else{
			addType(atual->noFilho->noIrmao->noIrmao);
		}
	}
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

char * prioridade(no * atual){ // vai buscar o tipo para por no Add/Mul/Sub/Mod
	no * aux = atual;
	no * filho1 = aux->noFilho;
	no * filho2 = aux->noFilho->noIrmao;
	char * tipo1, * tipo2;
	if(filho1->id==NULL){
		tipo1 = prioridade(filho1);
	}
	else{
		tipo1 = searchId(bodys,filho1->id);
	}
	if(filho2!=NULL){
		if(filho2->id==NULL){
			tipo2 = prioridade(filho2);
		}
		else{
			tipo2 = searchId(bodys,filho2->id);
		}
	}
	else{
		tipo2 = "";
	}
	if(strcmp(tipo1,"undef")==0){
		if(strcmp(filho1->type,"IntLit")==0 || strcmp(filho1->type,"ChrLit")==0){
			strcpy(tipo1, "int");
		}
		else if(strcmp(filho1->type,"RealLit")==0){
			strcpy(tipo1, "double");
		}
	}

	if(strcmp(tipo2,"undef")==0){
		if(strcmp(filho2->type,"IntLit")==0 || strcmp(filho2->type,"ChrLit")==0){
			strcpy(tipo2, "int");
		}
		else if(strcmp(filho2->type,"RealLit")==0){
			strcpy(tipo2, "double");
		}
	}
	if(strcmp(tipo1,"undef")==0 || strcmp(tipo2,"undef")==0 || strcmp(tipo1,"- undef")==0 || strcmp(tipo2,"- undef")==0){
		return strdup("- undef");
	}
	if(strcmp(tipo1,"double")==0 || strcmp(tipo2,"double")==0 || strcmp(tipo1,"- double")==0 || strcmp(tipo2,"- double")==0){
		return strdup("- double");
	}
	else if(strcmp(tipo1,"int")==0 || strcmp(tipo2,"int")==0 || strcmp(tipo1,"- int")==0 || strcmp(tipo2,"- int")==0){
		return strdup("- int");
	}
	else if(strcmp(tipo1,"short")==0 || strcmp(tipo2,"short")==0 || strcmp(tipo1,"- short")==0 || strcmp(tipo2,"- short")==0){
		return strdup("- short");
	}
	else {
		return strdup("- char");
	}
}


char * searchComma(no * atual){
	no * filho2 = atual->noFilho->noIrmao;
	if(filho2->id!=NULL){
		char * id = searchId(bodys,filho2->id);
		if(strcmp(id,"undef")==0){
			if(strcmp(filho2->type,"IntLit")==0 || strcmp(filho2->type,"ChrLit")==0){
				return strdup("- int");
			}
			else if(strcmp(filho2->type,"RealLit")==0){
				return strdup("- double");
			}
		}
		char * tokenF = (char *) malloc((strlen(id)+3)*sizeof(char));
		strcpy(tokenF,"- ");
		strcat(tokenF,id);
		return strdup(tokenF);
	}
	else{
		if(strcmp(filho2->type,"ChrLit")==0 || strcmp(filho2->type,"IntLit")==0){
			return strdup("- int");
		}
		else if(strcmp(filho2->type,"RealLit")==0){
			return strdup("- double");
		}
		else if(strcmp(filho2->type,"Lt")==0 || strcmp(filho2->type,"Le")==0 || strcmp(filho2->type,"Gt")==0 || strcmp(filho2->type,"Ge")==0 || strcmp(filho2->type,"Ne")==0 || strcmp(filho2->type,"Eq")==0 || strcmp(filho2->type,"Not")==0){
			return strdup("- int");
		}
		else if(strcmp(filho2->type, "Or") == 0 || strcmp(filho2->type, "And") == 0 ||  strcmp(filho2->type, "BitWiseAnd") == 0 || strcmp(filho2->type, "BitWiseOr") == 0 || strcmp(filho2->type, "BitWiseXor") == 0 || strcmp(filho2->type, "Mod") == 0){
			return strdup("- int");
		}
		else if (strcmp(filho2->type, "Add") == 0 || strcmp(filho2->type, "Sub") == 0 ||  strcmp(filho2->type, "Mul") == 0 || strcmp(filho2->type, "Div") == 0){
			return prioridade(filho2);
		}
		else if(strcmp(filho2->type,"Call") == 0){
			if(strcmp(filho2->noFilho->id,"putchar")==0 || strcmp(filho2->noFilho->id,"getchar")==0){
				return strdup("- int");
			}	
			else{
				char * type = searchId(bodys,filho2->noFilho->id);
				char * token;
    			token = strtok(type,"(");
				char * tokenF = (char *) malloc((strlen(token)+3)*sizeof(char));
				strcpy(tokenF,"- ");
				strcat(tokenF,token);
				return strdup(tokenF);

			}
		}
		else if (strcmp(filho2->type,"Store") == 0 || strcmp(filho2->type,"Minus")==0 || strcmp(filho2->type,"Plus")==0){
				char * type = searchId(bodys,filho2->noFilho->id);
				if(strcmp(type,"undef")==0){
					if(strcmp(filho2->noFilho->type,"IntLit")==0 || strcmp(filho2->noFilho->type,"ChrLit")==0){
						strcpy(type, "- int");
					}
					else if(strcmp(filho2->noFilho->type,"RealLit")==0){
						strcpy(type, "- double");
					}
				}
				return strdup(type);
		}
		else if(strcmp(filho2->type,"Comma")==0){
			return searchComma(filho2);
		}
		else{
			return strdup("- undef");
		}
	}
	return strdup("- undef");
}



void checkBody(no * atual,char * pai){
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
		else if (strcmp(aux->type, "Add") == 0 || strcmp(aux->type, "Sub") == 0 ||  strcmp(aux->type, "Mul") == 0 || strcmp(aux->type, "Div") == 0){
			aux->exprType= prioridade(aux);
		}
		else if(strcmp(aux->type,"Comma")==0){	
			char * id;
			id = searchComma(aux);
			aux->exprType= id;
		}
		else if(strcmp(aux->type,"Call") == 0){
			if(strcmp(aux->noFilho->id,"putchar")==0 || strcmp(aux->noFilho->id,"getchar")==0){
				aux->exprType = (char *)malloc((10)*sizeof(char));
				strcpy(aux->exprType, "- int");
			}	
			else{
				char * type = searchId(bodys,aux->noFilho->id);
				aux->exprType = (char *)malloc((strlen(type)+3)*sizeof(char));
				char * token;
    			token = strtok(type,"(");
				sprintf(aux->exprType,"- %s",token);

			}
		}
		else if (strcmp(aux->type,"Store") == 0 || strcmp(aux->type,"Minus")==0 || strcmp(aux->type,"Plus")==0){
				char * type = searchId(bodys,aux->noFilho->id);
				if(strcmp(type,"undef")==0){
					if(strcmp(aux->noFilho->type,"IntLit")==0 || strcmp(aux->noFilho->type,"ChrLit")==0){
						strcpy(type, "int");
					}
					else if(strcmp(aux->noFilho->type,"RealLit")==0){
						strcpy(type, "double");
					}
				}
				aux->exprType = (char *)malloc((strlen(type)+3)*sizeof(char));
				sprintf(aux->exprType,"- %s",type);
		}
		else if(aux->id!=NULL && strcmp(pai,"Declaration")!=0){
			char * type = searchId(bodys,aux->id);
			aux->exprType = (char *)malloc((strlen(type)+3)*sizeof(char));
			sprintf(aux->exprType,"- %s",type);
		}
		if(aux->noFilho!=NULL){
			checkBody(aux->noFilho,aux->type);
        }
        if(aux->noIrmao!=NULL){
			checkBody(aux->noIrmao,pai);
        }
	}
}

void anotateBody(no * atual){
	if (atual == NULL)
		return;
	if (atual->type != NULL){
		if(strcmp(atual->type,"FuncBody")==0){
			bodys+=1;
			checkBody(atual,"FuncBody");
		}
	}
	no * auxNode = atual->noFilho;

    // chechar o programa para os varios irmaos
	while(auxNode != NULL){
		anotateBody(auxNode);
		auxNode = auxNode->noIrmao;	
	}
}

