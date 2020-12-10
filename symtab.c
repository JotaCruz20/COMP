#include "symtab.h"
#include "semantics.h"
#include "error.h"
#include "tree.h"

tabela * tabelaSimbolos;

void initTabela(){
    tabelaSimbolos = (tabela *)malloc(sizeof(tabela));

	tabelaSimbolos->type = (char *)malloc((strlen("Global") + 1)*sizeof(char));
	strcpy(tabelaSimbolos->type, "Global");

	tabelaSimbolos->name = "";
	tabelaSimbolos->flag = 1;

	insert("putchar","int(int)","","Global",0,0,0);
	insert("getchar","int(void)","","Global",0,0,0);
}

noTabela *  searchFunc(char* nomeTabela){
	noTabela * aux = tabelaSimbolos->tabelaAtual;
	while(aux!=NULL && strcmp(nomeTabela,aux->id)!=0){
		aux=aux->next;
	}
	return aux;
}

int insert(char * id, char * tipo, char * params, char * nomeTabela, int line, int col, int flag){
    noTabela * tab = (noTabela*)malloc(sizeof(noTabela));
    tab->id=(char *)malloc((strlen(id)+1)*sizeof(char));
    tab->tipo=(char *)malloc((strlen(tipo)+1)*sizeof(char));
    tab->params=(char *)malloc((strlen(params)+1)*sizeof(char));
    strcpy(tab->id,id);
    strcpy(tab->tipo,tipo);
    strcpy(tab->params,params);
    tabela * aux = searchTabela(nomeTabela);
	tabela * auxFunc = searchTabela(id);
	//noTabela * procuraFuncGlobal = searchFunc(nomeTabela);
	if(aux!=NULL){
    if (aux->tabelaAtual == NULL || strcmp("return",id)==0){
		aux->tabelaAtual = tab;
	}
	else{
		noTabela * auxNoTabela = aux->tabelaAtual;
		while (auxNoTabela->next != NULL){
			if (strcmp(auxNoTabela->id, id)==0){ // se ja estiver o id na tabela nao precisamos do adicionar novamente
				if(flag==1){
					if(strcmp(params,"")==0){ //é uma var
						free(tab);
						char error[100];
						sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",line,col-strlen(id),id);
						addErros(line,col-strlen(id),error);
					}
					else{// e uma funcao
						char error[100];
						sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",line,col-strlen(id),id);
						addErros(line,col-strlen(id),error);
					}
				}
				else if(flag==2 || flag==3){
					free(tab);
					if(strcmp(auxNoTabela->tipo,tipo)!=0 && strcmp(params,"")==0){//é uma var
						char error[100];
						sprintf(error, "Line %d, col %d: Conflicting types (got %s, expected %s)\n",line,col,tipo,auxNoTabela->tipo);
						addErros(line,col,error);
						return 0;
					}
					else if(strcmp(auxNoTabela->tipo,tipo)!=0 || strcmp(params,auxNoTabela->params)!=0){//é uma função
						if(auxFunc!=NULL && auxFunc->definida==1 && flag==2){
							char error[100];
							sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",line,col-strlen(id),id);
							addErros(line,col-strlen(id),error);
						}
						else{
							char error[500];
							sprintf(error, "Line %d, col %ld: Conflicting types (got %s%s, expected %s%s)\n",line,col-strlen(id),tipo,params,auxNoTabela->tipo,auxNoTabela->params);
							addErros(line,col-strlen(id),error);
						}
						return 0;
					}
					return 1;
				}
				return 1;
			}
			auxNoTabela = auxNoTabela->next;
		}
		if (strcmp(auxNoTabela->id, id)==0){ // verifica o ultimo elemento -se ja estiver o id na tabela nao precisamos do adicionar novamente
			
			if(flag==1){
				if(strcmp(params,"")==0){ //é uma var
					free(tab);
					char error[100];
					sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",line,col-strlen(id),id);
					addErros(line,col-strlen(id),error);
				}
				else{//e uma funcao
					char error[100];
					sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",line,col-strlen(id),id);
					addErros(line,col-strlen(id),error);
				}
			}
			else if(flag==2 || flag==3){
					free(tab);
					if(strcmp(auxNoTabela->tipo,tipo)!=0 && strcmp(params,"")==0){//é uma var
						char error[100];
						sprintf(error, "Line %d, col %d: Conflicting types (got %s, expected %s)\n",line,col,tipo,auxNoTabela->tipo);
						addErros(line,col,error);
						return 0;
					}
					else if(strcmp(auxNoTabela->tipo,tipo)!=0 || strcmp(params,auxNoTabela->params)!=0){//é uma função
						if(auxFunc!=NULL && auxFunc->definida==1 && flag==2){
							char error[100];
							sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",line,col-strlen(id),id);
							addErros(line,col-strlen(id),error);
						}
						else{
							char error[500];
							sprintf(error, "Line %d, col %ld: Conflicting types (got %s%s, expected %s%s)\n",line,col-strlen(id),tipo,params,auxNoTabela->tipo,auxNoTabela->params);
							addErros(line,col-strlen(id),error);
							return 0;
						}
						return 0;
					}
					return 1;
				}
				return 1;
		}
		auxNoTabela->next = tab;
		return 1;
	}
		return 1;
	}
	return 0;
}

int checkFunc(char * nome){
	tabela * aux = tabelaSimbolos;
	if(aux!=NULL){
		if(strcmp(nome,"putchar")==0 || strcmp(nome,"getchar")==0){
			return 1;
		}
		while(aux!=NULL){
			if(strcmp(aux->name,nome)==0){
				return 1;
			}
			aux=aux->next;
		}
	}
	return 0;
}

int nParams(char * nome){
	tabela * aux = tabelaSimbolos;
	int counter=0;
	if(strcmp(nome,"getchar")==0){
		return 0;
	}
	else if(strcmp(nome,"putchar")==0){
		return 1;
	}
	if(aux!=NULL){
		while (aux!=NULL && strcmp(aux->name,nome)!=0)
		{
			aux=aux->next;
		}
		counter = aux->params;
	}
	return counter;
}

int checkParamsType(tabela * aux, no * atual){
	if(aux!=NULL && aux->tabelaAtual!=NULL){
		noTabela * paramsTabela = aux->tabelaAtual->next;
		while(paramsTabela!=NULL && strcmp(paramsTabela->params,"")!=0){
			if(atual==NULL){
				return 0;
			}
			char * type = (char *) malloc(strlen(atual->noFilho->type)*sizeof(char));
			strcpy(type,atual->noFilho->type);
			toLowerCase(type);
			if(strcmp(type,paramsTabela->tipo)!=0){
				return 0;
			}
			atual=atual->noIrmao;
			paramsTabela=paramsTabela->next;
		}
		if(atual!=NULL){
			if(strcmp(atual->noFilho->type,"Void")==0){
				return 1;
			}
			else{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

void checkParamsTypeError(char * nome,char * nomeFunc, no * atual,no* pai){
	tabela * aux = tabelaSimbolos;
	if(aux!=NULL){
		while (aux!=NULL && strcmp(aux->name,nomeFunc)!=0)
		{
			aux=aux->next;
		}
	}
	if(aux!=NULL && aux->tabelaAtual!=NULL){
		noTabela * paramsTabela = aux->tabelaAtual->next;
		while(paramsTabela!=NULL && strcmp(paramsTabela->params,"")!=0 && atual!=NULL){
			char * type;
			if(atual->exprType!=NULL){
				type= (char *)malloc(strlen(atual->exprType)*sizeof(char));
				strcpy(type,atual->exprType);
				type = strtok(type,"- (");
				if(strcmp(type,"IntLit")==0){
					free(type);
					type = (char *)malloc(strlen("int")*sizeof(char));
					strcpy(type,"int/short");
				}
				else if(strcmp(type,"ChrLit")==0){
					free(type);
					type = (char *)malloc(strlen("char")*sizeof(char));
					strcpy(type,"char");
				}
				else if(strcmp(type,"RealLit")==0){
					free(type);
					type = (char *)malloc(strlen("double")*sizeof(char));
					strcpy(type,"double");
				}
			}
			else{
				type = searchId(nome,atual->id);
			}
			if(strcmp(type,"undef")==0){
				if(strcmp(atual->type,"IntLit")==0){
					free(type);
					type = (char *)malloc(strlen("int")*sizeof(char));
					strcpy(type,"int/short/double");
				}
				else if(strcmp(atual->type,"ChrLit")==0){
					free(type);
					type = (char *)malloc(strlen("char")*sizeof(char));
					strcpy(type,"char");
				}
				else if(strcmp(atual->type,"RealLit")==0){
					free(type);
					type = (char *)malloc(strlen("double")*sizeof(char));
					strcpy(type,"double");
				}
			}
			if(strcmp(type,paramsTabela->tipo)!=0){
				if((strcmp(type,"int/short/double")==0 || strcmp(type,"int")==0) && strcmp(paramsTabela->tipo,"short")==0){
					;
				}
				else if((strcmp(type,"int/short/double")==0 || strcmp(type,"short")==0)  && strcmp(paramsTabela->tipo,"int")==0){
					;
				}
				else if(strcmp(type,"int/short/double")==0 && strcmp(paramsTabela->tipo,"double")==0){
					;
				}
				else{
					char * typeError = (char *)malloc(strlen(atual->exprType)*sizeof(char));
					strcpy(typeError,atual->exprType);
					typeError = strtok(typeError,"- ");
					char error[100];
					sprintf(error,"Line %d, col %d: Conflicting types (got %s, expected %s)\n", atual->line,pai->noFilho->col+1,typeError,paramsTabela->tipo);
					addErros(atual->line,pai->noFilho->col+1,error);
				}
			}
			atual=atual->noIrmao;
			paramsTabela=paramsTabela->next;
		}
		if(atual!=NULL){
			if(strcmp(atual->type,"Void")==0){
				char * typeError = (char *)malloc(strlen(atual->exprType)*sizeof(char));
				strcpy(typeError,atual->exprType);
				typeError = strtok(typeError,"- ");
				char error[100];
				sprintf(error,"Line %d, col %d: Conflicting types (got %s, expected %s)\n", atual->line,atual->col-1,typeError,paramsTabela->tipo);
				addErros(atual->line,atual->col-1,error);
			}
		}
	}
	else{
		aux = tabelaSimbolos;
		noTabela * paramsTabela = aux->tabelaAtual;
		while(strcmp(paramsTabela->id,nome)!=0){
			paramsTabela = paramsTabela->next;
		}
		if(paramsTabela!=NULL){
			char * token, * rest;
			rest = (char * )malloc(strlen(paramsTabela->params)*sizeof(char));
			strcpy(rest,paramsTabela->params);
			while((token=strtok_r(rest,",()",&rest)) && atual!=NULL){
				char * type;
				if(atual->exprType!=NULL){
					type= (char *)malloc(strlen(atual->exprType)*sizeof(char));
					strcpy(type,atual->exprType);
					type = strtok(type,"- ");
					if(strcmp(type,"IntLit")==0){
						free(type);
						type = (char *)malloc(strlen("int")*sizeof(char));
						strcpy(type,"int/short/double");
					}
					else if(strcmp(type,"ChrLit")==0){
						free(type);
						type = (char *)malloc(strlen("char")*sizeof(char));
						strcpy(type,"char");
					}
					else if(strcmp(type,"RealLit")==0){
						free(type);
						type = (char *)malloc(strlen("double")*sizeof(char));
						strcpy(type,"double");
					}
				}
				else{
					type = searchId(nomeFunc,atual->id);
				}
				if(strcmp(type,"undef")==0){
					if(strcmp(atual->type,"IntLit")==0){
						free(type);
						type = (char *)malloc(strlen("int")*sizeof(char));
						strcpy(type,"int/short/double");
					}
					else if(strcmp(atual->type,"ChrLit")==0){
						free(type);
						type = (char *)malloc(strlen("char")*sizeof(char));
						strcpy(type,"char");
					}
					else if(strcmp(atual->type,"RealLit")==0){
						free(type);
						type = (char *)malloc(strlen("double")*sizeof(char));
						strcpy(type,"double");
					}
				}
				if(strcmp(type,token)!=0){
					if(strcmp(type,"int/short/double")==0 && strcmp(token,"short")==0){
						;
					}
					else if(strcmp(type,"int/short/double")==0 && strcmp(token,"int")==0){
						;
					}
					else if(strcmp(type,"int/short/double")==0 && strcmp(token,"double")==0){
						;
					}

					else{
						char * typeError = (char *)malloc(strlen(atual->exprType)*sizeof(char));
						strcpy(typeError,atual->exprType);
						typeError = strtok(typeError,"- ");
						char error[100];
						sprintf(error,"Line %d, col %d: Conflicting types (got %s, expected %s)\n", pai->noFilho->line,pai->noFilho->col-1,paramsTabela->tipo,typeError);
						addErros(pai->noFilho->line,pai->noFilho->col-1,error);
					}
				}
				atual=atual->noIrmao;
				paramsTabela=paramsTabela->next;
			}
		}
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

char * getTypeParams(char * nome){
	tabela * aux = tabelaSimbolos;
	char tipoParams[500]="(";
	if(aux!=NULL){
		while (aux!=NULL && strcmp(aux->name,nome)!=0)
		{
			aux=aux->next;
		}
		if(aux!=NULL){
			noTabela * auxTab = aux->tabelaAtual;
			if(auxTab->next!=NULL){
				auxTab=auxTab->next;
				while(auxTab!=NULL){
					if(strcmp(auxTab->params,"")!=0){
						strcat(tipoParams,auxTab->tipo);
					}
					auxTab=auxTab->next;
					if (auxTab != NULL)
						strcat(tipoParams,",");
				}
			}
		}
	}
	return strdup("");
}

char * typeParams(char * nome,int n,char* type){
	tabela * aux = tabelaSimbolos;
	if(aux!=NULL){
		while (aux!=NULL && strcmp(aux->name,nome)!=0)
		{
			aux=aux->next;
		}
		int counter = 0;
		noTabela * auxTab = aux->tabelaAtual->next;
		while(counter!=n && auxTab!=NULL){
			counter+=1;
			auxTab=auxTab->next;
		}
		if(auxTab!=NULL){
			if(strcmp(auxTab->tipo,type)==0){
				return strdup("");
			}
			else{
				char * returnT = (char *)malloc((strlen(auxTab->tipo)+1)*sizeof(char));
				strcpy(returnT,auxTab->tipo);
				return strdup(returnT);
			}
		}
		else{
			return strdup("");
		}
	}
	return strdup("");
}

void initFunctionTabela(char * name, int flag, int print, int params, int def,int line,int col,int error){
	tabela * novaTabela = (tabela *)malloc(sizeof(tabela));

	novaTabela->type = (char *)malloc((strlen("Function") + 1)*sizeof(char));
	strcpy(novaTabela->type, "Function");
    novaTabela->name = (char *)malloc((strlen(name)+1)*sizeof(char));
	strcpy(novaTabela->name,name);
	novaTabela->flag = flag;
	novaTabela->print = print;
	novaTabela->params=params;
	novaTabela->definida = def;
	tabela * auxTabela = tabelaSimbolos;

    while (auxTabela->next != NULL){
        if (strcmp(auxTabela->name, name)==0){
            auxTabela->flag = 1;
			auxTabela->print=print;
			if(def==1){
				if(auxTabela->definida==1){
					char error[100];
					sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",line,col-strlen(name),name);
					addErros(line,col-strlen(name),error);
				}
				else if(error==1){
					auxTabela->definida=def;
				}
			}
            free(novaTabela);
            return;
        }
        auxTabela = auxTabela->next;
    }
    if (strcmp(auxTabela->name, name)==0){
        auxTabela->flag = 1;
		auxTabela->print=print;
		if(def==1){
			if(auxTabela->definida==1){
				char error[100];
				sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",line,col-strlen(name),name);
				addErros(line,col-strlen(name),error);
			}
			else if(error==1){
				auxTabela->definida=def;
			}
		}
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
			if (strcmp(auxTabela->type,"Global")==0){
				printf("===== Global Symbol Table =====\n");
				auxNoTabela = auxTabela->tabelaAtual;
				while ( auxNoTabela != NULL){
					printf("%s\t%s%s\n", auxNoTabela->id, auxNoTabela->tipo, auxNoTabela->params);
					auxNoTabela = auxNoTabela->next;
				}
				printf("\n");
			}
			else if(auxTabela->print==1 && auxTabela->tabelaAtual!=NULL){
				printf("===== %s %s Symbol Table =====\n", auxTabela->type, auxTabela->name);
				auxNoTabela = auxTabela->tabelaAtual;
				while ( auxNoTabela != NULL){
					printf("%s\t%s%s\n", auxNoTabela->id, auxNoTabela->tipo, auxNoTabela->params);
					auxNoTabela = auxNoTabela->next;
				}
				printf("\n");
			}
			
		}
		auxTabela = auxTabela->next;
	}
}

char* searchId(char* nTabela,char * id){
	char * type = (char *) calloc(2,sizeof(char));
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
			while( strcmp(aux->name,nTabela)!=0 && aux->next!=NULL){
				aux = aux ->next;
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
				strcpy(type,"undef");
				return strdup(type);
			}
			
	}
	return strdup("undef");
}
