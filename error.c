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
		aux->noIrmao->erro=(char *)malloc((strlen(erro)+1)*sizeof(char));
		strcpy(aux->noIrmao->erro,erro);
	}
	else{
		if(aux->noIrmao->line==line && aux->noIrmao->col==col && strcmp(aux->noIrmao->erro,erro)==0){
			return;
		}
		create = (erros *) malloc(sizeof(erros));
		create->line=line;
		create->col=col;
		create->erro=(char *)malloc((strlen(erro)+1)*sizeof(char));
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
		        sprintf(error,"Line %d, col %d: Operator = cannot be applied to types %s, double\n", aux->noFilho->line,aux->noFilho->col,token);
		        addErros(aux->noFilho->line,aux->noFilho->col,error);
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
			if(strcmp(type,"- double")==0 || strcmp(brotherType,"- double")==0){
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
                 char* rest = aux->noFilho->exprType, *rest2 = aux->noFilho->noIrmao->exprType; 
                char * token1 = strtok_r(type,"- ",&rest), *token2 =strtok_r(brotherType,"- ",&rest2);
                
				sprintf(error,"Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", aux->noFilho->line,aux->noFilho->col, operator,token1,token2);
				addErros(aux->noFilho->line,aux->noFilho->col,error);
			}
		}
        else if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type, "Add") == 0 || strcmp(aux->type, "Sub") == 0 ||  strcmp(aux->type, "Mul") == 0 || strcmp(aux->type, "Div") == 0){
			if(strcmp(aux->noFilho->exprType,"- undef")==0 || strcmp(aux->noFilho->noIrmao->exprType,"- undef")==0){
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
                char* rest = aux->noFilho->exprType, *rest2 = aux->noFilho->noIrmao->exprType; 
                char * token1 = strtok_r(aux->noFilho->exprType,"- ",&rest), *token2 =strtok_r(aux->noFilho->noIrmao->exprType,"- ",&rest2);
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", aux->noFilho->line,aux->noFilho->col, operator,token1,token2);
				addErros(aux->noFilho->line,aux->noFilho->col,error);
            }
		}
        else if(strcmp(aux->type,"Not")==0 || strcmp(aux->type,"Plus")==0 || strcmp(aux->type,"Minus")==0){
            if(strcmp(aux->noFilho->exprType,"- undef")==0){
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
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to type undef\n", aux->noFilho->line,aux->noFilho->col, operator);
				addErros(aux->noFilho->line,aux->noFilho->col,error);
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
			funcName = (char *) malloc((strlen(atual->noFilho->noIrmao->id)+1)*sizeof(char));
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
