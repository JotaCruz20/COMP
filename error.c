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
int flagCheckError = 1;

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
        if(aux->line==line && aux->col==col){
            aux->line=line;
            aux->col=col;
            free(aux->erro);
            aux->erro=(char *)malloc((strlen(erro)+1)*sizeof(char));
            strcpy(aux->noIrmao->erro,erro);
		}
        else{
            aux->noIrmao = (erros *) malloc(sizeof(erros));
            aux->noIrmao->line=line;
            aux->noIrmao->col=col;
            aux->noIrmao->erro=(char *)malloc((strlen(erro)+1)*sizeof(char));
            strcpy(aux->noIrmao->erro,erro);
       }
	}
	else{
		if(aux->noIrmao->line==line && aux->noIrmao->col==col){
			aux->noIrmao->line=line;
            aux->noIrmao->col=col;
            free(aux->noIrmao->erro);
            aux->noIrmao->erro=(char *)malloc((strlen(erro)+1)*sizeof(char));
            strcpy(aux->noIrmao->erro,erro);
		}
        else{
            create = (erros *) malloc(sizeof(erros));
            create->line=line;
            create->col=col;
            create->erro=(char *)malloc((strlen(erro)+1)*sizeof(char));
            strcpy(create->erro,erro);
            create->noIrmao=aux->noIrmao;
            aux->noIrmao=create;
        }
	}
}

void printErros(){
	erros * aux = errorsHead;
	if(aux->noIrmao!=NULL){
		aux=aux->noIrmao;
		while(aux!=NULL){
			printf("%s",aux->erro);
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
            //free(funcName);
			funcName = (char *) malloc((strlen(atual->noFilho->noIrmao->id)+1)*sizeof(char));
			strcpy(funcName,atual->noFilho->noIrmao->id);
			checkFuncDefinitionError(atual);
		}
		if (strcmp(atual->type, "FuncDeclaration")==0){
			checkFuncDeclarationError(atual);
		}
		if (strcmp(atual->type, "Declaration")==0){
			if(funcName==NULL){
				funcName = (char *) malloc((strlen("Global")+1)*sizeof(char));
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
    struct params{
        char * param;
        struct params * next;
    };
    struct params * parametros = (struct params *) malloc(sizeof(struct params));
	while(auxNode!= NULL){
		char * type = (char *) malloc(strlen(auxNode->noFilho->type)*sizeof(char));
        strcpy(type,auxNode->noFilho->type); 
		toLowerCase(type);
        struct params * auxPar = parametros;
        while(auxPar->next!=NULL){
            if(auxPar->param!=NULL && auxNode->noFilho->noIrmao!=NULL &&  strcmp(auxPar->param,auxNode->noFilho->noIrmao->id)==0){
                char error[100];
                sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",auxNode->noFilho->noIrmao->line,auxNode->noFilho->noIrmao->col-strlen(auxNode->noFilho->noIrmao->id),auxNode->noFilho->noIrmao->id);
                addErros(auxNode->noFilho->noIrmao->line,auxNode->noFilho->noIrmao->col-strlen(auxNode->noFilho->noIrmao->id),error);
            }
            auxPar=auxPar->next;
        }
        if(auxPar->param!=NULL && auxNode->noFilho->noIrmao!=NULL && strcmp(auxPar->param,auxNode->noFilho->noIrmao->id)==0){
            char error[100];
            sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",auxNode->noFilho->noIrmao->line,auxNode->noFilho->noIrmao->col-strlen(auxNode->noFilho->noIrmao->id),auxNode->noFilho->noIrmao->id);
            addErros(auxNode->noFilho->noIrmao->line,auxNode->noFilho->noIrmao->col-strlen(auxNode->noFilho->noIrmao->id),error);
        }
        if(auxNode->noFilho->noIrmao!=NULL){
            auxPar->next = (struct params *) malloc(sizeof(struct params));
            auxPar->next->param = (char *) malloc(strlen(auxNode->noFilho->noIrmao->id)*sizeof(char));
            strcpy(auxPar->next->param,auxNode->noFilho->noIrmao->id);
        }

		if(strcmp(type,"void")==0 && auxNode->noFilho->noIrmao!=NULL && strcmp(auxNode->noFilho->noIrmao->type,"Id")==0){
			char error[100];
			sprintf(error,"Line %d, col %d: Invalid use of void type in declaration\n", auxNode->noFilho->line,auxNode->noFilho->col);
			addErros(auxNode->noFilho->line,auxNode->noFilho->col,error);
		}
        auxNode = auxNode->noIrmao;
	}
}

void checkFuncDefinitionError(no * atual){ 
	checkParamsError(atual->noFilho->noIrmao->noIrmao);
}

void checkFuncDeclarationError(no * atual){
    checkParamsError(atual->noFilho->noIrmao->noIrmao);
}

void checkDeclarationError(no * atual){
    char * type = (char *) strdup(atual->noFilho->type); 
    toLowerCase(type);
	if(strcmp(type,"void")==0){
		char error[100];
		sprintf(error,"Line %d, col %d: Invalid use of void type in declaration\n", atual->noFilho->line,atual->noFilho->col-4);
		addErros(atual->noFilho->line,atual->noFilho->col-4,error);
	}
}

void checkBodyError(no * atual,char * pai){
	no * aux = atual;
	if(aux->type!=NULL){
		if(strcmp(aux->type,"Store")==0){
            if(aux->noFilho->id==NULL){
                char error[100];
		        sprintf(error,"Line %d, col %d: Lvalue required\n", aux->noFilho->noFilho->line,aux->noFilho->noFilho->col);
		        addErros(aux->noFilho->noFilho->line,aux->noFilho->noFilho->col,error);
            }
            else if (strcmp(aux->noFilho->type,"Call")==0){
                char error[100];
		        sprintf(error,"Line %d, col %d: Lvalue required\n", aux->noFilho->noFilho->line,aux->noFilho->noFilho->col);
		        addErros(aux->noFilho->noFilho->line,aux->noFilho->noFilho->col,error);
            }
            else if(strcmp(aux->exprType,"- double")!=0 && strcmp(aux->noFilho->noIrmao->exprType,"- double")==0 ){
                char error[100];
                char* rest = aux->exprType; 
                char * token = strtok_r(aux->exprType,"- ",&rest);
		        sprintf(error,"Line %d, col %d: Operator = cannot be applied to types %s, double\n", aux->line,aux->col-1,token);
		        addErros(aux->line,aux->col-1,error);
            }
        }
        else if(strcmp(aux->type,"Call")==0){
            int n = checkFunc(aux->noFilho->id);
            if(n==1){
                int count = 0;
                int countParams = nParams(aux->noFilho->id);
                no * irmaos = atual->noFilho;
                while(irmaos->noIrmao!=NULL){
                    count+=1;
                    irmaos=irmaos->noIrmao;
                }
                if(count!=countParams){
                    char error[100];
		            sprintf(error,"Line %d, col %ld: Wrong number of arguments to function %s (got %d, required %d)\n", aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),aux->noFilho->id,count,countParams);
		            addErros(aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),error);
                }
            }
            else{
                char error[100];
		        sprintf(error,"Line %d, col %d: Symbol %s is not a function\n", aux->noFilho->line,aux->noFilho->col,aux->noFilho->id);
		        addErros(aux->noFilho->line,aux->noFilho->col,error);
            }
        }
        else if(strcmp(aux->type,"If")==0 || strcmp(aux->type,"While")==0){
            if(strcmp(aux->noFilho->exprType,"- double")==0){
                if(strcmp(aux->noFilho->type,"Plus")==0 || strcmp(aux->noFilho->type,"Minus")==0 || strcmp(aux->noFilho->type,"Not")==0){
                    char error[100];
                    sprintf(error,"Line %d, col %d: Conflicting types (got double, expected int)\n", aux->noFilho->line,aux->noFilho->col-1);
                    addErros(aux->noFilho->line,aux->noFilho->col-1,error);
                }
                else if(strcmp(aux->noFilho->type,"Call")==0){
                    char error  [100];
                    sprintf(error,"Line %d, col %d: Conflicting types (got double, expected int)\n", aux->noFilho->line,aux->noFilho->col-1);
                    addErros(aux->noFilho->line,aux->noFilho->col-1,error);
                }
                else{
                    char error[100];
                    sprintf(error,"Line %d, col %d: Conflicting types (got double, expected int)\n", aux->noFilho->noFilho->line,aux->noFilho->noFilho->col);
                    addErros(aux->noFilho->line,aux->noFilho->col,error);
                }
            }
        }
        else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
			char * type, *brotherType;
			type = aux->noFilho->exprType;
			brotherType = aux->noFilho->noIrmao->exprType;
			char operator[3];
			if(strcmp(type,"- double")==0 || strcmp(brotherType,"- double")==0 || strcmp(aux->noFilho->type,"Call")==0 || strcmp(aux->noFilho->noIrmao->type,"Call")==0){
				char error[100];
				if(strcmp(aux->type, "Or") == 0 ){
					strcpy(operator,"||");
				}
				else if(strcmp(aux->type, "And") == 0){
					strcpy(operator,"&&");
				}
				else if(strcmp(aux->type, "BitWiseAnd") == 0){
					strcpy(operator,"&");
				}
				else if(strcmp(aux->type, "BitWiseOr") == 0){
					strcpy(operator,"|");
				}
				else if(strcmp(aux->type, "BitWiseXor") == 0){
					strcpy(operator,"^");
				}
				else if(strcmp(aux->type, "Mod") == 0){
					strcpy(operator,"%");
				}
                char* rest, *rest2; 
                char * token1, *token2;
                if( strcmp(aux->noFilho->type,"Call")==0 ){
                    rest = aux->noFilho->noFilho->exprType;
                    token1 = strtok_r(aux->noFilho->noFilho->exprType,"- ",&rest);
                }
                else{
                    rest = aux->noFilho->exprType;
                    token1 = strtok_r(aux->noFilho->exprType,"- ",&rest);
                }
                if(strcmp(aux->noFilho->noIrmao->type,"Call")==0){
                    rest2 = aux->noFilho->noIrmao->noFilho->exprType;
                    token2 =strtok_r(aux->noFilho->noIrmao->noFilho->exprType,"- ",&rest2);
                }
                else{
                    rest2 = aux->noFilho->noIrmao->exprType;
                    token2 =strtok_r(aux->noFilho->noIrmao->exprType,"- ",&rest2);
                }
				sprintf(error,"Line %d, col %ld: Operator %s cannot be applied to types %s, %s\n", aux->line,aux->col-strlen(operator), operator,token1,token2);
				addErros(aux->line,aux->col-strlen(operator),error);
			}
		}
        else if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type, "Add") == 0 || strcmp(aux->type, "Sub") == 0 ||  strcmp(aux->type, "Mul") == 0 || strcmp(aux->type, "Div") == 0){
			if(strcmp(aux->noFilho->exprType,"- undef")==0 || strcmp(aux->noFilho->noIrmao->exprType,"- undef")==0 || strcmp(aux->noFilho->type,"Call")==0 || strcmp(aux->noFilho->noIrmao->type,"Call")==0 ){
                char error[100];
                char operator[3];
                if(strcmp(aux->type, "Lt") == 0 ){
					strcpy(operator,"<");
				}
				else if(strcmp(aux->type, "Le") == 0){
					strcpy(operator,"<=");
				}
				else if(strcmp(aux->type, "Gt") == 0){
					strcpy(operator,">");
				}
				else if(strcmp(aux->type, "Ge") == 0){
					strcpy(operator,">=");
				}
				else if(strcmp(aux->type, "Ne") == 0){
					strcpy(operator,"!=");
				}
				else if(strcmp(aux->type, "Eq") == 0){
					strcpy(operator,"==");
				}
                else if(strcmp(aux->type, "Not") == 0){
					strcpy(operator,"!");
				}
				else if(strcmp(aux->type, "Add") == 0){
					strcpy(operator,"+");
				}
				else if(strcmp(aux->type, "Sub") == 0){
					strcpy(operator,"-");
				}
				else if(strcmp(aux->type, "Mul") == 0){
					strcpy(operator,"*");
				}
                else if(strcmp(aux->type, "Div") == 0){
					strcpy(operator,"/");
				}
                char* rest, *rest2; 
                char * token1, *token2;
                if( strcmp(aux->noFilho->type,"Call")==0 ){
                    rest = aux->noFilho->noFilho->exprType;
                    token1 = strtok_r(aux->noFilho->noFilho->exprType,"- ",&rest);
                }
                else{
                    rest = aux->noFilho->exprType;
                    token1 = strtok_r(aux->noFilho->exprType,"- ",&rest);
                }
                if(strcmp(aux->noFilho->noIrmao->type,"Call")==0){
                    rest2 = aux->noFilho->noIrmao->noFilho->exprType;
                    token2 =strtok_r(aux->noFilho->noIrmao->noFilho->exprType,"- ",&rest2);
                }
                else{
                    rest2 = aux->noFilho->noIrmao->exprType;
                    token2 =strtok_r(aux->noFilho->noIrmao->exprType,"- ",&rest2);
                }
                sprintf(error,"Line %d, col %ld: Operator %s cannot be applied to types %s, %s\n", aux->line,aux->col-strlen(operator), operator,token1,token2);
				addErros(aux->line,aux->col-strlen(operator),error);
            }
		}
        else if(strcmp(aux->type,"Not")==0 || strcmp(aux->type,"Plus")==0 || strcmp(aux->type,"Minus")==0){
            char error[100];
            char operator[3];
            if(strcmp(aux->type, "Plus") == 0 ){
                strcpy(operator,"+");
            }
            else if(strcmp(aux->type, "Not") == 0){
                strcpy(operator,"!");
            }
            else if(strcmp(aux->type, "Minus") == 0){
                strcpy(operator,"-");
            }
            if(strcmp(aux->noFilho->exprType,"- undef")==0){
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to type undef\n", aux->noFilho->line,aux->noFilho->col-1, operator);
				addErros(aux->noFilho->line,aux->noFilho->col-1,error);
            }
            if(strcmp(aux->noFilho->type,"Call")==0 ){
                char* rest; 
                char * token1;
                rest = aux->noFilho->noFilho->exprType;
                token1 = strtok_r(aux->noFilho->noFilho->exprType,"- ",&rest);
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to types %s\n", aux->line,aux->col-1, operator,token1);
				addErros(aux->line,aux->col,error-1);
            }
        }
        else if(aux->id!=NULL){
            if(aux->exprType!=NULL){
                if(strcmp(aux->exprType,"- undef")==0){
                    char error[100];
                    sprintf(error,"Line %d, col %d: Unknown symbol %s\n", aux->line,aux->col,aux->id);
                    addErros(aux->line,aux->col,error);
                }
            }
        }
        if(aux->noFilho!=NULL){
			checkBodyError(aux->noFilho,aux->type);
        }
        if(aux->noIrmao!=NULL){
			checkBodyError(aux->noIrmao,pai);
        }
	}
}

void anotateBodyError(no * atual){
	if (atual == NULL)
		return;
	if (atual->type != NULL){
		if(strcmp(atual->type, "FuncDefinition")==0){
            //free(funcName);
            tabela * tipoF = searchTabela(atual->noFilho->noIrmao->id);
            int n = checkParamsType(tipoF,atual->noFilho->noIrmao->noIrmao->noFilho);
            char * type = (char *) malloc(strlen(atual->noFilho->type)*sizeof(char));
            strcpy(type,atual->noFilho->type);
            toLowerCase(type);
            if(strcmp(tipoF->tabelaAtual->tipo,type)==0 && n==1){
                funcName = (char *) malloc((strlen(atual->noFilho->noIrmao->id)+1)*sizeof(char));
                strcpy(funcName,atual->noFilho->noIrmao->id);
                flagCheckError=1;
            }
            else
            {
                flagCheckError =0;
            }
            
		}
		else if(strcmp(atual->type,"FuncBody")==0){
            if(flagCheckError==1){
			    checkBodyError(atual,"FuncBody");
            }
		}
	}
	no * auxNode = atual->noFilho;

    // chechar o programa para os varios irmaos
	while(auxNode != NULL){
		anotateBodyError(auxNode);
		auxNode = auxNode->noIrmao;	
	}
}
