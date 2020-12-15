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

void addErros(int line,int col,char * erro,int noOrder){
	erros * aux = errorsHead;
	erros * create;
	/*while (aux->noIrmao!=NULL && aux->noIrmao->line<line){
		aux=aux->noIrmao;
	}
    while (aux->noIrmao!=NULL && aux->noIrmao->col<col && aux->noIrmao->line==line){
		aux=aux->noIrmao;
	}*/
    while (aux->noIrmao!=NULL && aux->noIrmao->noOrder<noOrder){
		aux=aux->noIrmao;
	}
	if(aux->noIrmao==NULL){
        if(aux->line==line && aux->col==col){
            aux->line=line;
            aux->col=col;
            aux->noOrder=noOrder;
            free(aux->erro);
            aux->erro=(char *)malloc((strlen(erro)+1)*sizeof(char));
            strcpy(aux->erro,erro);
		}
        else{
            aux->noIrmao = (erros *) malloc(sizeof(erros));
            aux->noIrmao->line=line;
            aux->noIrmao->col=col;
            aux->noIrmao->noOrder=noOrder;
            aux->noIrmao->erro=(char *)malloc((strlen(erro)+1)*sizeof(char));
            strcpy(aux->noIrmao->erro,erro);
       }
	}
	else{
		if(aux->noIrmao->line==line && aux->noIrmao->col==col){
			aux->noIrmao->line=line;
            aux->noIrmao->col=col;
            aux->noIrmao->noOrder=noOrder;
            free(aux->noIrmao->erro);
            aux->noIrmao->erro=(char *)malloc((strlen(erro)+1)*sizeof(char));
            strcpy(aux->noIrmao->erro,erro);
		}
        else{
            create = (erros *) malloc(sizeof(erros));
            create->line=line;
            create->col=col;
            create->noOrder=noOrder;
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
                addErros(auxNode->noFilho->noIrmao->line,auxNode->noFilho->noIrmao->col-strlen(auxNode->noFilho->noIrmao->id),error,auxNode->noFilho->noIrmao->noCount);
            }
            auxPar=auxPar->next;
        }
        if(auxPar->param!=NULL && auxNode->noFilho->noIrmao!=NULL && strcmp(auxPar->param,auxNode->noFilho->noIrmao->id)==0){
            char error[100];
            sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",auxNode->noFilho->noIrmao->line,auxNode->noFilho->noIrmao->col-strlen(auxNode->noFilho->noIrmao->id),auxNode->noFilho->noIrmao->id);
            addErros(auxNode->noFilho->noIrmao->line,auxNode->noFilho->noIrmao->col-strlen(auxNode->noFilho->noIrmao->id),error,auxNode->noFilho->noIrmao->noCount);
        }
        if(auxNode->noFilho->noIrmao!=NULL){
            auxPar->next = (struct params *) malloc(sizeof(struct params));
            auxPar->next->param = (char *) malloc(strlen(auxNode->noFilho->noIrmao->id)*sizeof(char));
            strcpy(auxPar->next->param,auxNode->noFilho->noIrmao->id);
        }
        auxNode = auxNode->noIrmao;
	}
}

void checkFuncDefinitionError(no * atual){ 
    no * params = atual->noFilho->noIrmao->noIrmao;
	int counter = 0;
	int flagErrorVoid = 0;
    int flagBrother = 0;
    while (params!=NULL && strcmp(params->type,"ParamDeclaration")==0){
		if(strcmp(params->noFilho->type,"Void")!=0){
			counter+=1; 
		} 
        else if(flagBrother==0 && strcmp(params->noFilho->type,"Void")==0 && params->noFilho->noIrmao==NULL && params->noIrmao==NULL){
			counter+=0;             
		}
		else{
			flagErrorVoid=1;
			char error[100];
			sprintf(error,"Line %d, col %d: Invalid use of void type in declaration\n", params->noFilho->line,params->noFilho->col-4);
			addErros(params->noFilho->line,params->noFilho->col-4,error,params->noFilho->noCount);
            break;
		}
        flagBrother = 1;
        params=params->noIrmao;
	}
    if(flagErrorVoid==0){
	    checkParamsError(atual->noFilho->noIrmao->noIrmao);
    }
}

void checkFuncDeclarationError(no * atual){
    no * params = atual->noFilho->noIrmao->noIrmao->noFilho;
	int counter = 0;
	int flagErrorVoid = 0;
    int flagBrother = 0;
    while (params!=NULL && strcmp(params->type,"ParamDeclaration")==0){
		if(params->noFilho!=NULL && strcmp(params->noFilho->type,"Void")!=0){
			counter+=1; 
		} 
        else if(flagBrother==0 && strcmp(params->noFilho->type,"Void")==0 && params->noFilho->noIrmao==NULL && params->noIrmao==NULL){
			counter+=0;             
		}
		else{

			flagErrorVoid=1;
			char error[100];
			sprintf(error,"Line %d, col %d: Invalid use of void type in declaration\n", params->noFilho->line,params->noFilho->col-4);
			addErros(params->noFilho->line,params->noFilho->col-4,error,params->noFilho->noCount);
            break;
		}
        flagBrother = 1;
        params=params->noIrmao;
	}
    if(flagErrorVoid==0){
	    checkParamsError(atual->noFilho->noIrmao->noIrmao);
    }
}

void checkDeclarationError(no * atual){
    char * type = (char *) strdup(atual->noFilho->type); 
    toLowerCase(type);
	if(strcmp(type,"void")==0){
		char error[100];
		sprintf(error,"Line %d, col %ld: Invalid use of void type in declaration\n", atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col-strlen(atual->noFilho->noIrmao->id));
		addErros(atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col-strlen(atual->noFilho->noIrmao->id),error,atual->noFilho->noIrmao->noCount);
	}
    checkDeclarationBodyErrors(atual->noFilho,type);
    
    
}

void checkBodyError(no * atual,char * pai){
	no * aux = atual;
	if(aux->type!=NULL){
		if(strcmp(aux->type,"Store")==0){
            if(strcmp(aux->noFilho->type,"IntLit")==0 || strcmp(aux->noFilho->type,"ChrLit")==0 || strcmp(aux->noFilho->type,"RealLit")==0){
                char error[100];
		        sprintf(error,"Line %d, col %ld: Lvalue required\n", aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id));
		        addErros(aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),error,aux->noFilho->noCount);
            }
            else if (strcmp(aux->noFilho->type,"Call")==0){
                char error[100];
		        sprintf(error,"Line %d, col %d: Lvalue required\n", aux->noFilho->noFilho->line,aux->noFilho->noFilho->col);
		        addErros(aux->noFilho->noFilho->line,aux->noFilho->noFilho->col,error,aux->noFilho->noFilho->noCount);
            }
            else if(aux->exprType==NULL){
                 char error[100];
		        sprintf(error,"Line %d, col %d: Lvalue required\n", aux->noFilho->noFilho->line,aux->noFilho->noFilho->col);
		        addErros(aux->noFilho->noFilho->line,aux->noFilho->noFilho->col,error,aux->noFilho->noFilho->noCount);
            }
            else if(strcmp(aux->noFilho->type,"Lt")==0 || strcmp(aux->noFilho->type,"Le")==0 || strcmp(aux->noFilho->type,"Gt")==0 || strcmp(aux->noFilho->type,"Ge")==0 || strcmp(aux->noFilho->type,"Ne")==0 || strcmp(aux->noFilho->type,"Eq")==0 || strcmp(aux->noFilho->type, "Add") == 0 || strcmp(aux->noFilho->type, "Sub") == 0 ||  strcmp(aux->noFilho->type, "Mul") == 0 || strcmp(aux->noFilho->type, "Div") == 0 || strcmp(aux->noFilho->type, "Or") == 0 || strcmp(aux->noFilho->type, "And") == 0 ||  strcmp(aux->noFilho->type, "BitWiseAnd") == 0 || strcmp(aux->noFilho->type, "BitWiseOr") == 0 || strcmp(aux->noFilho->type, "BitWiseXor") == 0 || strcmp(aux->noFilho->type, "Mod") == 0 || strcmp(aux->noFilho->type,"Not")==0 || strcmp(aux->noFilho->type,"Plus")==0 || strcmp(aux->noFilho->type,"Minus")==0){
                 char error[100];
		        sprintf(error,"Line %d, col %d: Lvalue required\n", aux->noFilho->noFilho->line,aux->noFilho->noFilho->col);
		        addErros(aux->noFilho->noFilho->line,aux->noFilho->noFilho->col,error,aux->noFilho->noFilho->noCount);
            }
            if(aux->noFilho->id!=NULL && checkFunc(aux->noFilho->id)){
                char error[100];
                char * token = (char *) malloc(strlen(aux->noFilho->exprType)*sizeof(char));
                strcpy(token,aux->noFilho->exprType);
                token=strtok(token,"- ");
                char * token1 = (char *) malloc(strlen(aux->noFilho->noIrmao->exprType)*sizeof(char));
                strcpy(token1,aux->noFilho->noIrmao->exprType);
                token1=strtok(token1,"- ");
                sprintf(error,"Line %d, col %d: Operator = cannot be applied to types %s, %s\n", aux->line,aux->col-1,token,token1);
                addErros(aux->line,aux->col-1,error,aux->noCount);
            }
            else if( strcmp(aux->exprType,"- double")!=0 && strcmp(aux->noFilho->noIrmao->exprType,"- double")==0 ){
                char error[100];
                char* rest = (char*) malloc(strlen(aux->exprType)*sizeof(char)); 
                strcpy(rest,aux->exprType);
                char * token = strtok_r(rest,"- ",&rest);   
		        sprintf(error,"Line %d, col %d: Operator = cannot be applied to types %s, double\n", aux->line,aux->col-1,token);
		        addErros(aux->line,aux->col-1,error,aux->noCount);
            }
            else if(strcmp(aux->exprType,"- undef")!=0 && strcmp(aux->noFilho->noIrmao->exprType,"- undef")==0 ){
                char error[100];
                char * rest = (char*) malloc(strlen(aux->exprType)*sizeof(char)); 
                strcpy(rest,aux->exprType);
                char * token = strtok_r(rest,"- ",&rest);
		        sprintf(error,"Line %d, col %d: Operator = cannot be applied to types %s, undef\n", aux->line,aux->col-1,token);
		        addErros(aux->line,aux->col-1,error,aux->noCount);
            }
        }
        else if(strcmp(aux->type,"Comma")==0){ //pode causar merda
            if((strstr(aux->noFilho->exprType,"(")!=NULL || strstr(aux->noFilho->noIrmao->exprType,"(")!=NULL) && strcmp(aux->noFilho->exprType,aux->noFilho->noIrmao->exprType)!=0){
                char error[100];
                char * token = (char *) malloc(strlen(aux->noFilho->exprType)*sizeof(char));
                strcpy(token,aux->noFilho->exprType);
                token=strtok(token,"- ");
                char * token1 = (char *) malloc(strlen(aux->noFilho->noIrmao->exprType)*sizeof(char));
                strcpy(token1,aux->noFilho->noIrmao->exprType);
                token1=strtok(token1,"- ");
                sprintf(error,"Line %d, col %d: Operator , cannot be applied to types %s, %s\n", aux->line,aux->col-1,token,token1);
                addErros(aux->line,aux->col-1,error,aux->noCount);
            }
            else if(strcmp(aux->noFilho->exprType,"- undef")==0 || strcmp(aux->noFilho->noIrmao->exprType,"- undef")==0){
                char error[100];
                char * token = (char *) malloc(strlen(aux->noFilho->exprType)*sizeof(char));
                strcpy(token,aux->noFilho->exprType);
                token=strtok(token,"- ");
                char * token1 = (char *) malloc(strlen(aux->noFilho->noIrmao->exprType)*sizeof(char));
                strcpy(token1,aux->noFilho->noIrmao->exprType);
                token1=strtok(token1,"- ");
                sprintf(error,"Line %d, col %d: Operator , cannot be applied to types %s, %s\n", aux->line,aux->col-1,token,token1);
                addErros(aux->line,aux->col-1,error,aux->noCount);
            }
        }
        else if(strcmp(aux->type,"Call")==0){
            int n = checkFunc(aux->noFilho->id);
            int countParams =0;
            if(n==1){
                int count = 0;
                countParams = nParams(aux->noFilho->id);
                no * irmaos = atual->noFilho;
                while(irmaos->noIrmao!=NULL){
                    count+=1;
                    irmaos=irmaos->noIrmao;
                }
                if(count!=countParams){
                    char error[100];
		            sprintf(error,"Line %d, col %ld: Wrong number of arguments to function %s (got %d, required %d)\n", aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),aux->noFilho->id,count,countParams);
		            addErros(aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),error,aux->noFilho->noCount);
                }
            }
            else{
                char error[100];
		        sprintf(error,"Line %d, col %d: Symbol %s is not a function\n", aux->noFilho->line,aux->noFilho->col,aux->noFilho->id);
		        addErros(aux->noFilho->line,aux->noFilho->col,error,aux->noFilho->noCount);
            }
            if(countParams!=0){
                checkParamsTypeError(aux->noFilho->id,funcName,aux->noFilho->noIrmao,aux);
            }
        }
        else if(strcmp(aux->type,"If")==0 || strcmp(aux->type,"While")==0){
            if(strcmp(aux->noFilho->exprType,"- double")==0){
                if(strcmp(aux->noFilho->type,"Plus")==0 || strcmp(aux->noFilho->type,"Minus")==0 || strcmp(aux->noFilho->type,"Not")==0){
                    char error[100];
                    sprintf(error,"Line %d, col %d: Conflicting types (got double, expected int)\n", aux->noFilho->line,aux->noFilho->col-1);
                    addErros(aux->noFilho->line,aux->noFilho->col-1,error,aux->noFilho->noCount);
                }
                else if(strcmp(aux->noFilho->type,"Call")==0){
                    char error  [100];
                    sprintf(error,"Line %d, col %d: Conflicting types (got double, expected int)\n", aux->noFilho->line,aux->noFilho->col-1);
                    addErros(aux->noFilho->line,aux->noFilho->col-1,error,aux->noFilho->noCount);
                }
                else{
                    char error[100];
                    sprintf(error,"Line %d, col %d: Conflicting types (got double, expected int)\n", aux->noFilho->line,aux->noFilho->col-1);
                    addErros(aux->noFilho->line,aux->noFilho->col-1,error,aux->noFilho->noCount);
                }
            }
        }
        else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
			char * type, *brotherType;
			type = aux->noFilho->exprType;
			brotherType = aux->noFilho->noIrmao->exprType;
			char operator[3];
			if(strstr(type,"(")!=NULL || strstr(brotherType,"(")!=NULL || strcmp(type,"- double")==0 || strcmp(brotherType,"- double")==0 || strcmp(type,"- undef")==0 || strcmp(brotherType,"- undef")==0 || strcmp(aux->noFilho->type,"Call")==0 || strcmp(aux->noFilho->noIrmao->type,"Call")==0){
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
                    rest = (char*) malloc(strlen(aux->noFilho->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->noFilho->exprType);
                    token1 = strtok_r(rest,"- (",&rest);
                }
                else{
                    rest = (char*) malloc(strlen(aux->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->exprType);
                    token1 = strtok_r(rest,"- ",&rest);
                }
                if(strcmp(aux->noFilho->noIrmao->type,"Call")==0){
                    rest2 = (char*) malloc(strlen(aux->noFilho->noIrmao->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest2,aux->noFilho->noIrmao->noFilho->exprType);
                    token2 =strtok_r(rest,"- (",&rest2);
                }
                else{
                    rest2 = (char*) malloc(strlen(aux->noFilho->noIrmao->exprType)*sizeof(char)); 
                    strcpy(rest2,aux->noFilho->noIrmao->exprType);
                    token2 =strtok_r(rest2,"- ",&rest2);
                }
				sprintf(error,"Line %d, col %ld: Operator %s cannot be applied to types %s, %s\n", aux->line,aux->col-strlen(operator), operator,token1,token2);
				addErros(aux->line,aux->col-strlen(operator),error,aux->noCount);
			}
		}
        else if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type, "Add") == 0 || strcmp(aux->type, "Sub") == 0 ||  strcmp(aux->type, "Mul") == 0 || strcmp(aux->type, "Div") == 0){
			if((strcmp(aux->noFilho->exprType,"- int")!=0 && /*strcmp(aux->noFilho->type,"Call")!=0 && strcmp(aux->noFilho->exprType,"- void")!=0 &&*/ strcmp(aux->noFilho->exprType,"- char")!=0 && strcmp(aux->noFilho->exprType,"- short")!=0 && strcmp(aux->noFilho->exprType,"- double")!=0) || (strcmp(aux->noFilho->noIrmao->exprType,"- int")!=0  && /*strcmp(aux->noFilho->noIrmao->type,"Call")!=0  && strcmp(aux->noFilho->noIrmao->exprType,"- void")!=0  &&*/ strcmp(aux->noFilho->noIrmao->exprType,"- char")!=0  && strcmp(aux->noFilho->noIrmao->exprType,"- short")!=0  && strcmp(aux->noFilho->noIrmao->exprType,"- double")!=0)){
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
                    rest = (char*) malloc(strlen(aux->noFilho->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->noFilho->exprType);
                    token1 = strtok_r(rest,"- (",&rest);
                }
                else{
                    rest = (char*) malloc(strlen(aux->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->exprType);
                    token1 = strtok_r(rest,"- ",&rest);
                }
                if(strcmp(aux->noFilho->noIrmao->type,"Call")==0){
                    rest = (char*) malloc(strlen(aux->noFilho->noIrmao->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->noIrmao->noFilho->exprType);
                    token2 =strtok_r(rest  ,"- (",&rest2);
                }
                else{
                    rest = (char*) malloc(strlen(aux->noFilho->noIrmao->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->noIrmao->exprType);
                    token2 =strtok_r(rest,"- ",&rest2);
                }
                sprintf(error,"Line %d, col %ld: Operator %s cannot be applied to types %s, %s\n", aux->line,aux->col-strlen(operator), operator,token1,token2);
				addErros(aux->line,aux->col-strlen(operator),error,aux->noCount);
            }
		}
        else if( strcmp(aux->type,"Plus")==0 || strcmp(aux->type,"Minus")==0){
            char error[100];
            char operator[3];
            if(strcmp(aux->type, "Plus") == 0 ){
                strcpy(operator,"+");
            }
            else if(strcmp(aux->type, "Minus") == 0){
                strcpy(operator,"-");
            }
            if(strcmp(aux->noFilho->type,"Call")==0 ){  
                char* rest; 
                char * token1;
                rest = (char*) malloc(strlen(aux->noFilho->noFilho->exprType)*sizeof(char)); 
                strcpy(rest,aux->noFilho->noFilho->exprType);
                token1 = strtok_r(rest,"- (",&rest);
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to type %s\n", aux->line,aux->col-1, operator,token1);
				addErros(aux->line,aux->col,error-1,aux->noCount);
            }
            if(strcmp(aux->noFilho->exprType,"- double")!=0 && strcmp(aux->noFilho->exprType,"- int")!=0 && strcmp(aux->noFilho->exprType,"- char")!=0 && strcmp(aux->noFilho->exprType,"- short")!=0){
                char * token1;
                char * rest = (char*) malloc(strlen(aux->noFilho->exprType)*sizeof(char)); 
                strcpy(rest,aux->noFilho->exprType);
                //token1 = strtok_r(rest,"- (",&rest);
                token1 = strtok_r(rest,"- ",&rest);
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to type %s\n", aux->line,aux->col-1, operator,token1);
				addErros(aux->line,aux->col-1,error,aux->noCount);
            }
        }
        else if(strcmp(aux->type,"Not")==0){
            char error[100];
            char operator[]="!";
             if(strcmp(aux->noFilho->type,"Call")==0 ){  
                char* rest; 
                char * token1;
                rest = (char*) malloc(strlen(aux->noFilho->noFilho->exprType)*sizeof(char)); 
                strcpy(rest,aux->noFilho->noFilho->exprType);
                token1 = strtok_r(rest,"- (",&rest);
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to type %s\n", aux->line,aux->col-1, operator,token1);
				addErros(aux->line,aux->col,error-1,aux->noCount);
            }
            if(strcmp(aux->noFilho->exprType,"- int")!=0 && strcmp(aux->noFilho->exprType,"- char")!=0 && strcmp(aux->noFilho->exprType,"- short")!=0){
                char * token1;
                char * rest = (char*) malloc(strlen(aux->noFilho->exprType)*sizeof(char)); 
                strcpy(rest,aux->noFilho->exprType);
                //token1 = strtok_r(rest,"- (",&rest);
                token1 = strtok_r(rest,"- ",&rest);
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to type %s\n", aux->line,aux->col-1, operator,token1);
				addErros(aux->line,aux->col-1,error,aux->noCount);
            }
        }
        else if(aux->id!=NULL){
            if(aux->exprType!=NULL){
                if(strcmp(aux->exprType,"- undef")==0){
                    char error[100];
                    sprintf(error,"Line %d, col %ld: Unknown symbol %s\n", aux->line,aux->col-strlen(aux->id),aux->id);
                    addErros(aux->line,aux->col-strlen(aux->id),error,aux->noCount);
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

void checkDeclarationBodyErrors(no * atual, char * type){
    no * aux = atual;
	if(aux->type!=NULL){
        if(strcmp(aux->type,"Call")==0){
            int n = checkFunc(aux->noFilho->id);
            int countParams = 0;
            if(n==1){
                int count = 0;
                countParams = nParams(aux->noFilho->id);
                no * irmaos = atual->noFilho;
                while(irmaos->noIrmao!=NULL){
                    count+=1;
                    irmaos=irmaos->noIrmao;
                }
                if(count!=countParams){
                    char error[100];
		            sprintf(error,"Line %d, col %ld: Wrong number of arguments to function %s (got %d, required %d)\n", aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),aux->noFilho->id,count,countParams);
		            addErros(aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),error,aux->noFilho->noCount);
                }
            }
            else{
                char error[100];
		        sprintf(error,"Line %d, col %d: Symbol %s is not a function\n", aux->noFilho->line,aux->noFilho->col,aux->noFilho->id);
		        addErros(aux->noFilho->line,aux->noFilho->col,error,aux->noFilho->noCount);
            }
            if(countParams!=0){
                checkParamsTypeError(aux->noFilho->id,funcName,aux->noFilho->noIrmao,aux);
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
                    rest = (char*) malloc(strlen(aux->noFilho->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->noFilho->exprType);
                    token1 = strtok_r(rest,"- (",&rest);
                }
                else{
                    rest = (char*) malloc(strlen(aux->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->exprType);
                    token1 = strtok_r(rest,"- ",&rest);
                }
                if(strcmp(aux->noFilho->noIrmao->type,"Call")==0){
                    rest2 = (char*) malloc(strlen(aux->noFilho->noIrmao->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->noIrmao->noFilho->exprType);
                    token2 =strtok_r(rest2,"- (",&rest2);
                }
                else{
                    rest2 = (char*) malloc(strlen(aux->noFilho->noIrmao->exprType)*sizeof(char)); 
                    strcpy(rest2,aux->noFilho->noIrmao->exprType);
                    token2 =strtok_r(rest2,"- ",&rest2);
                }
				sprintf(error,"Line %d, col %ld: Operator %s cannot be applied to types %s, %s\n", aux->line,aux->col-strlen(operator), operator,token1,token2);
				addErros(aux->line,aux->col-strlen(operator),error,aux->noCount);
			}
		}
        else if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type, "Add") == 0 || strcmp(aux->type, "Sub") == 0 ||  strcmp(aux->type, "Mul") == 0 || strcmp(aux->type, "Div") == 0){
			char operator[3];
            char error[100];
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
            if((strcmp(aux->noFilho->exprType,"- int")!=0 && strcmp(aux->noFilho->exprType,"- char")!=0 && strcmp(aux->noFilho->exprType,"- short")!=0 && strcmp(aux->noFilho->exprType,"- double")!=0) || (strcmp(aux->noFilho->noIrmao->exprType,"- int")!=0  && strcmp(aux->noFilho->noIrmao->exprType,"- char")!=0  && strcmp(aux->noFilho->noIrmao->exprType,"- short")!=0  && strcmp(aux->noFilho->noIrmao->exprType,"- double")!=0)){
                
                char* rest, *rest2; 
                char * token1, *token2;
                if( strcmp(aux->noFilho->type,"Call")==0 ){
                    rest = (char*) malloc(strlen(aux->noFilho->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->noFilho->exprType);
                    token1 = strtok_r(rest,"- ",&rest);
                }
                else{
                    rest = (char*) malloc(strlen(aux->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest,aux->noFilho->exprType);
                    token1 = strtok_r(rest,"- ",&rest);
                }
                if(strcmp(aux->noFilho->noIrmao->type,"Call")==0){
                    rest2 = (char*) malloc(strlen(aux->noFilho->noIrmao->noFilho->exprType)*sizeof(char)); 
                    strcpy(rest2,aux->noFilho->noIrmao->noFilho->exprType);
                    token2 =strtok_r(rest2,"- ",&rest2);
                }
                else{
                    rest2 = (char*) malloc(strlen(aux->noFilho->noIrmao->exprType)*sizeof(char)); 
                    strcpy(rest2,aux->noFilho->noIrmao->exprType);
                    token2 =strtok_r(rest2,"- ",&rest2);
                }
                sprintf(error,"Line %d, col %ld: Operator %s cannot be applied to types %s, %s\n", aux->line,aux->col-strlen(operator), operator,token1,token2);
				addErros(aux->line,aux->col-strlen(operator),error,aux->noCount);
            }
            else if((strcmp(type,"int")==0 || strcmp(type,"char")==0 || strcmp(type,"short")==0) && strcmp(aux->exprType,"- double")==0){
                char * token = (char *)malloc(sizeof(char)*strlen(aux->exprType));
                strcpy(token,aux->exprType);
                token = strtok(token,"- ");
                sprintf(error,"Line %d, col %ld: Operator %s cannot be applied to types %s, %s\n", aux->line,aux->col-strlen(operator), operator,type,token);
				addErros(aux->line,aux->col-strlen(operator),error,aux->noCount);
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
            if(strcmp(aux->noFilho->type,"Call")==0 ){  
                char* rest; 
                char * token1;
                rest = (char*) malloc(strlen(aux->noFilho->noFilho->exprType)*sizeof(char)); 
                strcpy(rest,aux->noFilho->noFilho->exprType);
                token1 = strtok_r(rest,"- (",&rest);
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to type %s\n", aux->line,aux->col-1, operator,token1);
				addErros(aux->line,aux->col,error-1,aux->noCount);
            }
            else if(strcmp(aux->noFilho->exprType,"- int")!=0 && /*strcmp(aux->noFilho->type,"Call")!=0 && strcmp(aux->noFilho->exprType,"- void")!=0 &&*/ strcmp(aux->noFilho->exprType,"- char")!=0 && strcmp(aux->noFilho->exprType,"- short")!=0 && strcmp(aux->noFilho->exprType,"- double")!=0){
                char * token1;
                char * rest = (char*) malloc(strlen(aux->noFilho->noFilho->exprType)*sizeof(char)); 
                strcpy(rest,aux->noFilho->noFilho->exprType);
                token1 = strtok_r(rest,"- (",&rest);
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to type %s\n", aux->line,aux->col-1, operator,token1);
				addErros(aux->line,aux->col-1,error,aux->noCount);
            }    
        }
        else if(aux->id!=NULL){
            if(aux->exprType!=NULL){
                if(strcmp(aux->exprType,"- undef")==0){
                    char error[100];
                    sprintf(error,"Line %d, col %ld: Unknown symbol %s\n", aux->line,aux->col-strlen(aux->id),aux->id);
                    addErros(aux->line,aux->col-strlen(aux->id),error,aux->noCount);
                }
            }
        }
        if(aux->noIrmao!=NULL){
            checkDeclarationBodyErrors(aux->noIrmao,type);
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
            if(tipoF!=NULL && tipoF->tabelaAtual!=NULL && strcmp(tipoF->tabelaAtual->tipo,type)==0 && n==1){
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
