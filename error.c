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
char * tipoFunc;
char * funcName;
extern int linha;
extern int coluna;
int flagCheckError = 1;
int colDec= 0;

void initErrors(){ //inicializa lista ligada dos erros
	errorsHead = (erros *) malloc(sizeof(erros));
	errorsHead->col=0;
	errorsHead->line=0;
	errorsHead->erro=(char *)malloc((strlen("header")+1)*sizeof(char));
	strcpy(errorsHead->erro,"header");
}

void addErros(int line,int col,char * erro,int noOrder){ //adiciona erro com base no nó da arvore em que esta
	erros * aux = errorsHead; 
	erros * create;
    while (aux->noIrmao!=NULL && aux->noIrmao->noOrder<noOrder){
		aux=aux->noIrmao;
	}
	if(aux->noIrmao==NULL){
        if(aux->line==line && aux->col==col){ //se houver 2 erros no mesmo sitio da overwrite
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
		if(aux->noIrmao->line==line && aux->noIrmao->col==col){ //se houver 2 erros no mesmo sitio da overwrite
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

void printErros(){ //print dos erros
	erros * aux = errorsHead;
	if(aux->noIrmao!=NULL){
		aux=aux->noIrmao;
		while(aux!=NULL){
			printf("%s",aux->erro);
			aux=aux->noIrmao;
		}
	}
}


// Procura de erros

void checkProgramError(no * atual){ 
	if (atual == NULL)
		return;
	if (atual->type != NULL){ // vai ver os diferentes tipos de nos e analisa-los
		if (strcmp(atual->type, "FuncDefinition")==0){
			funcName = (char *) malloc((strlen(atual->noFilho->noIrmao->id)+1)*sizeof(char)); //guarda o nome da funcao definida
			strcpy(funcName,atual->noFilho->noIrmao->id);
			checkFuncDefinitionError(atual);
		}
		if (strcmp(atual->type, "FuncDeclaration")==0){
			checkFuncDeclarationError(atual);
		}
		if (strcmp(atual->type, "Declaration")==0){
			if(funcName==NULL){ // se for uma declaração guarda o nome da funcao declarada
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

void checkParamsError(no * atual){ //verifica se há 2 parametros com o mesmo id na declaracao/definição de uma funcao
	no * auxNode = atual->noFilho;
    struct params{ //struct auxiliar para guardar o parametro
        char * param;
        struct params * next;
    };
    struct params * parametros = (struct params *) malloc(sizeof(struct params));
	while(auxNode!= NULL){
		char * type = (char *) malloc(strlen(auxNode->noFilho->type)*sizeof(char));
        strcpy(type,auxNode->noFilho->type); 
		toLowerCase(type);
        struct params * auxPar = parametros;
        while(auxPar->next!=NULL){ //vertifica se esta na struct de params auxiliar
            if(auxPar->param!=NULL && auxNode->noFilho->noIrmao!=NULL &&  strcmp(auxPar->param,auxNode->noFilho->noIrmao->id)==0){
                char error[100];
                sprintf(error, "Line %d, col %ld: Symbol %s already defined\n",auxNode->noFilho->noIrmao->line,auxNode->noFilho->noIrmao->col-strlen(auxNode->noFilho->noIrmao->id),auxNode->noFilho->noIrmao->id);
                addErros(auxNode->noFilho->noIrmao->line,auxNode->noFilho->noIrmao->col-strlen(auxNode->noFilho->noIrmao->id),error,auxNode->noFilho->noIrmao->noCount);
            }
            auxPar=auxPar->next;
        } //verifica o ultimo elemento da lista
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

void checkFuncDefinitionError(no * atual){  // verifica erros na definição de uma função
    no * params = atual->noFilho->noIrmao->noIrmao;
	int counter = 0;
	int flagErrorVoid = 0;
    int flagBrother = 0;
    while (params!=NULL && strcmp(params->type,"ParamDeclaration")==0){ //continua até ver todos os parametros da funcao
		if(strcmp(params->noFilho->type,"Void")!=0){//se nao for void soma 1 ao numero de argumentos
			counter+=1; 
		} 
        else if(flagBrother==0 && strcmp(params->noFilho->type,"Void")==0 && params->noFilho->noIrmao==NULL && params->noIrmao==NULL){ //se nao tiver irmaos e for void nao da erro
			counter+=0;             
		}
		else{//erro void 
			flagErrorVoid=1; //flag de erro
			char error[100];
			sprintf(error,"Line %d, col %d: Invalid use of void type in declaration\n", params->noFilho->line,params->noFilho->col-4);
			addErros(params->noFilho->line,params->noFilho->col-4,error,params->noFilho->noCount);
            break;
		}
        flagBrother = 1;
        params=params->noIrmao;
	}
    if(flagErrorVoid==0){ //se nao houver erro verficia os parametros 
	    checkParamsError(atual->noFilho->noIrmao->noIrmao);
    }
}

void checkFuncDeclarationError(no * atual){ //mesma funcao que a anterior mas para declarações 
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

void checkDeclarationError(no * atual){ // verifica a declaracao de uma variavel
    char * type = (char *) strdup(atual->noFilho->type); 
    toLowerCase(type);
	if(strcmp(type,"void")==0){ //verifica se a variavel nao é void
		char error[100];
		sprintf(error,"Line %d, col %ld: Invalid use of void type in declaration\n", atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col-strlen(atual->noFilho->noIrmao->id));
		addErros(atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col-strlen(atual->noFilho->noIrmao->id),error,atual->noFilho->noIrmao->noCount);
	}
    colDec = atual->noFilho->noIrmao->col-strlen(atual->noFilho->noIrmao->id);
    checkDeclarationBodyErrors(atual->noFilho,type); //verifica se nao há erros com o corpo da declaracao
    
    
}

void checkBodyError(no * atual,char * pai){ //verifica o corpo de uma funcao
	no * aux = atual;
	if(aux->type!=NULL){
		if(strcmp(aux->type,"Store")==0){ //funcao que verifica os stores(=)
            if(strcmp(aux->noFilho->type,"IntLit")==0 || strcmp(aux->noFilho->type,"ChrLit")==0 || strcmp(aux->noFilho->type,"RealLit")==0){ //se for um literal no lado esquerdo da erro de lvalue
                char error[100];
		        sprintf(error,"Line %d, col %ld: Lvalue required\n", aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id));
		        addErros(aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),error,aux->noFilho->noCount);
            }
            else if (strcmp(aux->noFilho->type,"Call")==0){ // se for uma funcao no lado esquerdo da lvalue
                char error[100];
		        sprintf(error,"Line %d, col %d: Lvalue required\n", aux->noFilho->noFilho->line,aux->noFilho->noFilho->col);
		        addErros(aux->noFilho->noFilho->line,aux->noFilho->noFilho->col,error,aux->noFilho->noFilho->noCount);
            }
            else if(aux->exprType==NULL){ //se for uma expressao sem tipo no lado esquerdo da lvalue
                char error[100];
		        sprintf(error,"Line %d, col %d: Lvalue required\n", aux->noFilho->noFilho->line,aux->noFilho->noFilho->col);
		        addErros(aux->noFilho->noFilho->line,aux->noFilho->noFilho->col,error,aux->noFilho->noFilho->noCount);
            }
            //se for um operador
            else if(strcmp(aux->noFilho->type,"Lt")==0 || strcmp(aux->noFilho->type,"Le")==0 || strcmp(aux->noFilho->type,"Gt")==0 || strcmp(aux->noFilho->type,"Ge")==0 || strcmp(aux->noFilho->type,"Ne")==0 || strcmp(aux->noFilho->type,"Eq")==0 || strcmp(aux->noFilho->type, "Add") == 0 || strcmp(aux->noFilho->type, "Sub") == 0 ||  strcmp(aux->noFilho->type, "Mul") == 0 || strcmp(aux->noFilho->type, "Div") == 0 || strcmp(aux->noFilho->type, "Or") == 0 || strcmp(aux->noFilho->type, "And") == 0 ||  strcmp(aux->noFilho->type, "BitWiseAnd") == 0 || strcmp(aux->noFilho->type, "BitWiseOr") == 0 || strcmp(aux->noFilho->type, "BitWiseXor") == 0 || strcmp(aux->noFilho->type, "Mod") == 0 || strcmp(aux->noFilho->type,"Not")==0 || strcmp(aux->noFilho->type,"Plus")==0 || strcmp(aux->noFilho->type,"Minus")==0){
                 char error[100];
		        sprintf(error,"Line %d, col %d: Lvalue required\n", aux->noFilho->noFilho->line,aux->noFilho->noFilho->col);
		        addErros(aux->noFilho->noFilho->line,aux->noFilho->noFilho->col,error,aux->noFilho->noFilho->noCount);
            }
            //verifica o tipo a ser stored, doubles so podem ser guardados em doubles, e verifica por undefs
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
        //verifica os returns da funcao
        else if(strcmp(aux->type,"Return")==0){
            //uma funcao void so pode dar return a um ; que nao é inserido na tree, logo é null
            if(strcmp(tipoFunc,"void")==0){
                // se n for null ha erro
                if(aux->noFilho->exprType!=NULL){
                    char * token = (char *)malloc(sizeof(char)*strlen(aux->noFilho->exprType));
                    strcpy(token,aux->noFilho->exprType);
                    token = strtok(token,"- ");
                    if(strcmp(token,tipoFunc)!=0){
                        char error[100];
                        if(aux->noFilho->id!=NULL){
                            sprintf(error,"Line %d, col %ld: Conflicting types (got %s, expected void)\n", aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),token);
                            addErros(aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),error,aux->noFilho->noCount);
                        }
                    }
                }
            }
            else if(strcmp(tipoFunc,"int")==0 || strcmp(tipoFunc,"char")==0 || strcmp(tipoFunc,"short")==0){
                //verifica o tipo que esta a ser retornado, nao pode ser nem void nem double
                 if(aux->noFilho->exprType!=NULL){ 
                    char * token = (char *)malloc(sizeof(char)*strlen(aux->noFilho->exprType));
                    strcpy(token,aux->noFilho->exprType);
                    token = strtok(token,"- ");

                    char * token2 = (char *)malloc(sizeof(char)*strlen(aux->noFilho->exprType));
                    strcpy(token2,aux->noFilho->exprType);
                    token2 = strtok(token2,"-  (");

                    if(strcmp(token2,"int")!=0 && strcmp(token2,"short")!=0 && strcmp(token2,"char")!=0){
                        char error[100];
                        if(aux->noFilho->id!=NULL){
                            sprintf(error,"Line %d, col %ld: Conflicting types (got %s, expected %s)\n", aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),token,tipoFunc);
                            addErros(aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),error,aux->noFilho->noCount);
                        }
                    }
                }
                else{
                    char error[100];
                    sprintf(error,"Line %d, col %ld: Conflicting types (got void, expected %s)\n", aux->noFilho->line,aux->noFilho->col-strlen("return")-1,tipoFunc);
                    addErros(aux->noFilho->col,aux->noFilho->col-strlen("return")-1,error,aux->noFilho->noCount);
                }
            }
            //se for double
            else{
                if(aux->noFilho->exprType!=NULL){
                    char * token = (char *)malloc(sizeof(char)*strlen(aux->noFilho->exprType));
                    strcpy(token,aux->noFilho->exprType);
                    token = strtok(token,"- ");

                    char * token2 = (char *)malloc(sizeof(char)*strlen(aux->noFilho->exprType));
                    strcpy(token2,aux->noFilho->exprType);
                    token2 = strtok(token2,"-  (");
                    //se for diferente de double,int,short ou char entao da erro
                    if(strcmp(token2,"double")!=0 && strcmp(token2,"int")!=0 && strcmp(token2,"short")!=0 && strcmp(token2,"char")!=0){
                        char error[100];
                        sprintf(error,"Line %d, col %ld: Conflicting types (got %s, expected double)\n", aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),token);
                        addErros(aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),error,aux->noFilho->noCount);
                    }
                }
                // é void
                else{
                    char error[100];
                    sprintf(error,"Line %d, col %ld: Conflicting types (got void, expected %s)\n", aux->noFilho->line,aux->noFilho->col-strlen("return")-1,tipoFunc);
                    addErros(aux->noFilho->line,aux->noFilho->col-strlen("return")-1,error,aux->noFilho->noCount);
                
                }
            }
        }
        //comma operatores
        else if(strcmp(aux->type,"Comma")==0){ 
            //os operadores têm de ser do mesmo tipo
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
            //undefs
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
        //call de funçoes 
        else if(strcmp(aux->type,"Call")==0){
            int n = checkFunc(aux->noFilho->id); //verifica se é funcao
            int countParams =0;
            if(n==1){ // se for funcao 
                int count = 0;
                countParams = nParams(aux->noFilho->id); //verifica o numero de parametros esperados
                no * irmaos = atual->noFilho;
                while(irmaos->noIrmao!=NULL){ //conta o nº de parametros passados
                    count+=1;
                    irmaos=irmaos->noIrmao;
                }
                if(count!=countParams){ // se for diferente há erro
                    char error[100];
		            sprintf(error,"Line %d, col %ld: Wrong number of arguments to function %s (got %d, required %d)\n", aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),aux->noFilho->id,count,countParams);
		            addErros(aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),error,aux->noFilho->noCount);
                }
            }
            else{ // nao é funcao
                char error[100];
		        sprintf(error,"Line %d, col %d: Symbol %s is not a function\n", aux->noFilho->line,aux->noFilho->col,aux->noFilho->id);
		        addErros(aux->noFilho->line,aux->noFilho->col,error,aux->noFilho->noCount);
            }
            if(countParams!=0){ //verifica se os parametros passados sao do tipo esperado
                checkParamsTypeError(aux->noFilho->id,funcName,aux->noFilho->noIrmao,aux);
            }
        }
        //ifs e whiles
        else if(strcmp(aux->type,"If")==0 || strcmp(aux->type,"While")==0){
            //se for double há erro
            if(strcmp(aux->noFilho->exprType,"- double")==0){
                //diferentes ifs pcausa do numero da linha e coluna
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
            //funcao
            else if(strstr(aux->noFilho->exprType,"(")!=NULL){
                char * token = (char *)malloc(sizeof(char)*strlen(aux->noFilho->exprType));
                strcpy(token,aux->noFilho->exprType);
                token = strtok(token,"- ");

                char error[100];
                sprintf(error,"Line %d, col %ld: Conflicting types (got %s, expected int)\n", aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),token);
                addErros(aux->noFilho->line,aux->noFilho->col-strlen(aux->noFilho->id),error,aux->noFilho->noCount);
            }
        }
        //operadores logicos
        else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
			char * type, *brotherType;
			type = aux->noFilho->exprType;
			brotherType = aux->noFilho->noIrmao->exprType;
			char operator[3];
            // se for uma funcao, um double ou undef da erro
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
        //operadores matematicos 
        else if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type, "Add") == 0 || strcmp(aux->type, "Sub") == 0 ||  strcmp(aux->type, "Mul") == 0 || strcmp(aux->type, "Div") == 0){
            //so funciona para ints,chars,doubles e ints
            if((strcmp(aux->noFilho->exprType,"- int")!=0 && strcmp(aux->noFilho->exprType,"- char")!=0 && strcmp(aux->noFilho->exprType,"- short")!=0 && strcmp(aux->noFilho->exprType,"- double")!=0) || (strcmp(aux->noFilho->noIrmao->exprType,"- int")!=0  && strcmp(aux->noFilho->noIrmao->exprType,"- char")!=0  && strcmp(aux->noFilho->noIrmao->exprType,"- short")!=0  && strcmp(aux->noFilho->noIrmao->exprType,"- double")!=0)){
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
        //operadores unários
        else if( strcmp(aux->type,"Plus")==0 || strcmp(aux->type,"Minus")==0){
            char error[100];
            char operator[3];
            if(strcmp(aux->type, "Plus") == 0 ){
                strcpy(operator,"+");
            }
            else if(strcmp(aux->type, "Minus") == 0){
                strcpy(operator,"-");
            }
            //se for um funcao da erro
            if(strcmp(aux->noFilho->type,"Call")==0 ){  
                char* rest; 
                char * token1;
                rest = (char*) malloc(strlen(aux->noFilho->noFilho->exprType)*sizeof(char)); 
                strcpy(rest,aux->noFilho->noFilho->exprType);
                token1 = strtok_r(rest,"- (",&rest);
                sprintf(error,"Line %d, col %d: Operator %s cannot be applied to type %s\n", aux->line,aux->col-1, operator,token1);
				addErros(aux->line,aux->col,error-1,aux->noCount);
            }
            //se nao for double,int,short ou char da erro
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
        //unário Not
        //precisa de um caso proprio pois so funciona em char,shorts e ints
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
        //ids
        else if(aux->id!=NULL){
            if(aux->exprType!=NULL){
                //verifica se o id existe
                if(strcmp(aux->exprType,"- undef")==0){
                    char error[100];
                    sprintf(error,"Line %d, col %ld: Unknown symbol %s\n", aux->line,aux->col-strlen(aux->id),aux->id);
                    addErros(aux->line,aux->col-strlen(aux->id),error,aux->noCount);
                }
            }
        }
        //verifica para o resto do corpo
        if(aux->noFilho!=NULL){
			checkBodyError(aux->noFilho,aux->type);
        }
        if(aux->noIrmao!=NULL){
			checkBodyError(aux->noIrmao,pai);
        }
	}
}

void checkDeclarationBodyErrors(no * atual, char * type){ //igual ao checkBodyError eliminando o que nao é necessario
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
                sprintf(error,"Line %d, col %d: Conflicting types (got double, expected %s)\n", aux->line,colDec, type);
				addErros(aux->line,colDec,error,aux->noCount);
            }
		}
        
        else if(strcmp(aux->type,"RealLit")==0){
            char error[100];
            if(strcmp(type,"int")==0 || strcmp(type,"char")==0 || strcmp(type,"short")==0){
                sprintf(error,"Line %d, col %d: Conflicting types (got double, expected %s)\n", aux->line,colDec, type);
				addErros(aux->line,colDec,error,aux->noCount);
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
		if(strcmp(atual->type, "FuncDefinition")==0){ // se for uma definicao verifica o seu body
            tabela * tipoF = searchTabela(atual->noFilho->noIrmao->id); // procura a tabela da funcao
            int n = checkParamsType(tipoF,atual->noFilho->noIrmao->noIrmao->noFilho); //verifica os parametros da funcao
            char * type = (char *) malloc(strlen(atual->noFilho->type)*sizeof(char));
            strcpy(type,atual->noFilho->type);
            toLowerCase(type);
            //verifica se nao ha problemas com a tabela e se o tipo esta certo
            if(tipoF!=NULL && tipoF->tabelaAtual!=NULL && strcmp(tipoF->tabelaAtual->tipo,type)==0 && n==1){
                funcName = (char *) malloc((strlen(atual->noFilho->noIrmao->id)+1)*sizeof(char));
                strcpy(funcName,atual->noFilho->noIrmao->id);
                tipoFunc = (char *) malloc((strlen(type)+1)*sizeof(char));
                strcpy(tipoFunc,type);
                flagCheckError=1;
            }
            else
            {
                flagCheckError =0;
            }
            
		}
		else if(strcmp(atual->type,"FuncBody")==0){
            if(flagCheckError==1){ //se houver erro na definicao nao da check ao body dela
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

void clearErros(){// clear a lista de erros
    erros * clear;

   while (errorsHead != NULL){
       clear = errorsHead;
       errorsHead = errorsHead->noIrmao;
       free(clear);
    }

}