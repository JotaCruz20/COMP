#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <ctype.h> 
#include "semantics.h"
#include "symtab.h"
#include "tree.h"
#include "error.h"

char name[20];
char * funcName;
extern int linha;
extern int coluna;
int counterAST = 0;

void addCounter(no * atual){ //adiciona o numero dos nos na AST
	if(atual->noFilho!=NULL){
		addCounter(atual->noFilho);
	}
	atual->noCount=counterAST;
	counterAST+=1;
	if(atual->noIrmao!=NULL){
		addCounter(atual->noIrmao);
	}
	

}

void checkProgram(no * atual){ 
	if (atual == NULL)
		return;
	if (atual->type != NULL){
		if (strcmp(atual->type, "Program")==0){ // quando começa a analise sintatica tem de inicializar as vars globais
			initTabela();
		}
		else if (strcmp(atual->type, "FuncDefinition")==0){ //definiciao de funcao
			funcName = (char *) malloc(strlen((atual->noFilho->noIrmao->id)+1)*sizeof(char));
			strcpy(funcName,atual->noFilho->noIrmao->id);
			checkFuncDefinition(atual);
		}
		else if (strcmp(atual->type, "FuncDeclaration")==0){ //declaracao de funcao
			checkFuncDeclaration(atual);
		}
		else if (strcmp(atual->type, "Declaration")==0){ //declaracao de vars
			if(funcName==NULL){
				funcName = (char *) malloc(strlen("Global")*sizeof(char));
				strcpy(funcName,"Global");
			}
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

char * checkParams(no * atual){ //retorna os parametros da funcao entre ()
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

void addParamsFunction(no * atual, char * nameTable){ //adiciona parametros á funcao
	no * auxNode = atual->noFilho;
	//percore o no auxiliar para adicionar parametros a funcao nameTable
	while(auxNode != NULL){
		if (auxNode->noFilho != NULL){
			char * type =	strdup(auxNode->noFilho->type);	
            toLowerCase(type);
			if(strcmp(type,"void")!=0){ // se n for do tipo void adiciona 
				if(auxNode->noFilho->noIrmao!=NULL){ //so adiciona se o parametro tiver id
					insert(auxNode->noFilho->noIrmao->id, type, "\tparam", nameTable, auxNode->line,auxNode->col,0,auxNode->noFilho->noIrmao->noCount);
				}
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
	int counter = 0;
	int flagErrorVoid = 0,flagBrother = 0;
	no * paramsNo = atual->noFilho->noIrmao->noIrmao->noFilho;
	//verifica se nao tem nenhum tipo void nos argumentos
	while (paramsNo!=NULL && strcmp(paramsNo->type,"ParamDeclaration")==0){
		if(paramsNo->noFilho!=NULL && strcmp(paramsNo->noFilho->type,"Void")!=0){
			counter+=1; 
		} 
		else if(flagBrother ==0 && strcmp(paramsNo->noFilho->type,"Void")==0 && paramsNo->noFilho->noIrmao==NULL && paramsNo->noIrmao==NULL){
			counter+=0;             
		}
		else{
			flagErrorVoid=1;
			char error[100];
			sprintf(error,"Line %d, col %d: Invalid use of void type in declaration\n", paramsNo->noFilho->line,paramsNo->noFilho->col-4);
			addErros(paramsNo->noFilho->line,paramsNo->noFilho->col-4,error,paramsNo->noFilho->noCount);
			break;
		}
		flagBrother = 1;
		paramsNo=paramsNo->noIrmao;
	}


	strcpy(name, id);

	//se nao tiver insere a tabela, e se nao houver nenhum problema com a inserçao entao inicia a tabela e adiciona os parametros
	if(flagErrorVoid==0){
		int n= insert(id, type, params, "Global",atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col,2,atual->noFilho->noIrmao->noCount);
		if(n==1){
			initFunctionTabela(id, 1,1,counter,1,atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col,n,atual->noFilho->noIrmao->noCount);
			insert("return", type, "", id,atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col,n,atual->noFilho->noIrmao->noCount);
			addParamsFunction(atual->noFilho->noIrmao->noIrmao, id);
		}
	}
}

void checkFuncDeclaration(no * atual){ //mesmo que para a definição, so que nao inicia a tabela da funcao, so e feito para definições
	char * type = (char *) strdup(atual->noFilho->type); 
    toLowerCase(type);
	char * id = (char *) strdup(atual->noFilho->noIrmao->id);
	char * params = checkParams(atual->noFilho->noIrmao->noIrmao);
	no * paramsNo = atual->noFilho->noIrmao->noIrmao->noFilho;
	int counter = 0;
	int flagErrorVoid = 0, flagBrother=0;
	while (paramsNo!=NULL && strcmp(paramsNo->type,"ParamDeclaration")==0){
		if(paramsNo->noFilho!=NULL && strcmp(paramsNo->noFilho->type,"Void")!=0){
			counter+=1; 
		} 
		else if(flagBrother==0 &&strcmp(paramsNo->noFilho->type,"Void")==0 && paramsNo->noFilho->noIrmao==NULL && paramsNo->noIrmao==NULL){
			counter+=0;             
		}
		else{
			flagErrorVoid=1;
			char error[100];
			sprintf(error,"Line %d, col %d: Invalid use of void type in declaration\n", paramsNo->noFilho->line,paramsNo->noFilho->col-4);
			addErros(paramsNo->noFilho->line,paramsNo->noFilho->col-4,error,paramsNo->noFilho->noCount);
			break;
		}
		flagBrother=1;
		paramsNo=paramsNo->noIrmao;
	}


	if(flagErrorVoid==0){
		int n = insert(id, type, params, "Global",atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col,3,atual->noFilho->noIrmao->noCount);
		initFunctionTabela(id, 0,1, counter,0,atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col,n,atual->noFilho->noIrmao->noCount);
	}
}

void addType(no * atual, char* pai){ //adiciona o tipo em declarações de variaveis globais
	no  * aux =atual;
	if(aux!=NULL){
		//literais
		if(strcmp(aux->type,"ChrLit")==0 || strcmp(aux->type,"IntLit")==0){
			aux->exprType= (char *) strdup("- int");
		}
		else if(strcmp(aux->type,"RealLit")==0){
			aux->exprType= (char *) strdup("- double");
		}
		//operaçoes matematicas
		else if (strcmp(aux->type, "Add") == 0 || strcmp(aux->type, "Sub") == 0 ||  strcmp(aux->type, "Mul") == 0 || strcmp(aux->type, "Div") == 0){
			aux->exprType= prioridade(aux);
		}
		else if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type,"Not")==0){
			aux->exprType= (char *) strdup("- int");
		}
		//operadores logicos
		else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
			aux->exprType= (char *) strdup("- int");
		}
		//unarios e Store
		else if (strcmp(aux->type,"Store") == 0 || strcmp(aux->type,"Minus")==0 || strcmp(aux->type,"Plus")==0){
				char * type = searchId(funcName,aux->noFilho->id);
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
		else if(aux->id!=NULL){ //variaveis
			char * type = searchId(funcName,aux->id);
			if(strcmp(pai,"Call")==0){
				char * params = getTypeParams(funcName);
				strcat(type,params);
			}
			aux->exprType = (char *)malloc((strlen(type)+3)*sizeof(char));
			sprintf(aux->exprType,"- %s",type);
		}
		//declaracao de varias variaveis com virgulas
		if(aux->noFilho!=NULL){
			addType(aux->noFilho,aux->type);
		}
		if(aux->noIrmao!=NULL){
			addType(aux->noIrmao,pai);
		}
	}
}

void checkDeclaration(no * atual){ //adiciona as declarações na tabela
     char * type = (char *) strdup(atual->noFilho->type); 
    toLowerCase(type);
    char * id = (char *) strdup(atual->noFilho->noIrmao->id);
	if(strcmp(name,"Global")==0){
		insert(id, type, "", name,atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col,2,atual->noFilho->noIrmao->noCount);
	}
	else{
		insert(id, type, "", name,atual->noFilho->noIrmao->line,atual->noFilho->noIrmao->col,1,atual->noFilho->noIrmao->noCount);
	}
	if(atual->noFilho->noIrmao->noIrmao!=NULL){
		if(strcmp(atual->noFilho->noIrmao->noIrmao->type,"ChrLit")==0 || strcmp(atual->noFilho->noIrmao->noIrmao->type,"IntLit")==0){
			atual->noFilho->noIrmao->noIrmao->exprType= (char *) strdup("- int");
		}
		else if(strcmp(atual->noFilho->noIrmao->noIrmao->type,"RealLit")==0){
			atual->noFilho->noIrmao->noIrmao->exprType= (char *) strdup("- double");
		}
		else{
			addType(atual->noFilho->noIrmao->noIrmao,atual->type);
		}
	}
}

void printTreeAnotada(no * auxNode, int pontos){ //print da arvore anotada
	 int i=0;
        if(auxNode!=NULL){
                for(i=0;i<pontos*2;i++){ //print dos pontos 
                        printf(".");
                }
				if(auxNode->id!=NULL){ //se tem um id
					if(auxNode->exprType==NULL){ //é anotado ou nao
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
	if(filho1->id==NULL){ //nao é uma variavel, logo é um operador
		if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type,"Not")==0){
			return (char *) strdup("- int");
		}
		else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
			return (char *) strdup("- int");
		}
		else{
			tipo1 = prioridade(filho1); // temos de ir procurar o tipo no seu filho
		}
	}
	else{
		if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type,"Not")==0){
			return (char *) strdup("- int");
		}
		else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
			return (char *) strdup("- int");
		}
		else{
			tipo1 = searchId(funcName,filho1->id); //procura o tipo do id na funcao
			if(strstr(tipo1,"(")!=NULL && strcmp(aux->type,"Call")!=0){
				return strdup("- undef");
			}
		}
	}
	if(filho2!=NULL){ // faz o mesmo para o segundo filho
		if(filho2->id==NULL){
			if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type,"Not")==0){
				return (char *) strdup("- int");
			}
			else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
				return (char *) strdup("- int");
			}
			else{
				tipo2 = prioridade(filho2);
			}
		}
		else{
			if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type,"Not")==0){
				return (char *) strdup("- int");
			}
			else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
				return (char *) strdup("- int");
			}
			else{
			
				tipo2 = searchId(funcName,filho2->id);
				if(strstr(tipo2,"(")!=NULL && strcmp(aux->type,"Call")!=0){
					return strdup("- undef");
				}
			}
		}
	}
	else{
		tipo2 = "";
	}
	//verifica se é literais ou nao
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
	tipo1 = strtok(tipo1,"- (");
	tipo2 = strtok(tipo2,"- ("); 

	if(tipo2==NULL){
		tipo2 = (char *)malloc(sizeof(char));
	}

	//da o tipo ao operador conforme a prioridade, undef>double>int>short>char

	if(strcmp(tipo1,"undef")==0 || strcmp(tipo2,"undef")==0 || strcmp(tipo1,"- undef")==0 || strcmp(tipo2,"- undef")==0 || strcmp(tipo1,"void")==0 || strcmp(tipo2,"void")==0 || strcmp(tipo1,"- void")==0 || strcmp(tipo2,"- void")==0){
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

char * searchComma(no * atual){ // da tipo aos comma operatores, o tipo é o tipo do segundo filho a menos que o primeiro seja undef
	no * filho2 = atual->noFilho->noIrmao;
	if(filho2->id!=NULL){
		char * id = searchId(funcName,filho2->id); // procura o id na tabela
		//verifica se é literal
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
	else{ // é um operador
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
				char * type = searchId(funcName,filho2->noFilho->id);
				char * token;
    			token = strtok(type,"(");
				char * tokenF = (char *) malloc((strlen(token)+3)*sizeof(char));
				strcpy(tokenF,"- ");
				strcat(tokenF,token);
				return strdup(tokenF);

			}
		}
		else if (strcmp(filho2->type,"Store") == 0 || strcmp(filho2->type,"Minus")==0 || strcmp(filho2->type,"Plus")==0){
				char * type = searchId(funcName,filho2->noFilho->id);
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

char * searchCommaFirstSon(no * atual){ //verifica se o primeiro filho do comma não é undef
	no * filho = atual->noFilho;
	if(filho->id!=NULL){
		char * id = searchId(funcName,filho->id);
		if(strcmp(id,"undef")==0){
			if(strcmp(filho->type,"IntLit")==0 || strcmp(filho->type,"ChrLit")==0){
				return strdup("- int");
			}
			else if(strcmp(filho->type,"RealLit")==0){
				return strdup("- double");
			}
		}
		char * tokenF = (char *) malloc((strlen(id)+3)*sizeof(char));
		strcpy(tokenF,"- ");
		strcat(tokenF,id);
		return strdup(tokenF);
	}
	else{
		if(strcmp(filho->type,"ChrLit")==0 || strcmp(filho->type,"IntLit")==0){
			return strdup("- int");
		}
		else if(strcmp(filho->type,"RealLit")==0){
			return strdup("- double");
		}
		else if(strcmp(filho->type,"Lt")==0 || strcmp(filho->type,"Le")==0 || strcmp(filho->type,"Gt")==0 || strcmp(filho->type,"Ge")==0 || strcmp(filho->type,"Ne")==0 || strcmp(filho->type,"Eq")==0 || strcmp(filho->type,"Not")==0){
			return strdup("- int");
		}
		else if(strcmp(filho->type, "Or") == 0 || strcmp(filho->type, "And") == 0 ||  strcmp(filho->type, "BitWiseAnd") == 0 || strcmp(filho->type, "BitWiseOr") == 0 || strcmp(filho->type, "BitWiseXor") == 0 || strcmp(filho->type, "Mod") == 0){
			return strdup("- int");
		}
		else if (strcmp(filho->type, "Add") == 0 || strcmp(filho->type, "Sub") == 0 ||  strcmp(filho->type, "Mul") == 0 || strcmp(filho->type, "Div") == 0){
			return prioridade(filho);
		}
		else if(strcmp(filho->type,"Call") == 0){
			if(strcmp(filho->noFilho->id,"putchar")==0 || strcmp(filho->noFilho->id,"getchar")==0){
				return strdup("- int");
			}	
			else{
				char * type = searchId(funcName,filho->noFilho->id);
				char * token;
    			token = strtok(type,"(");
				char * tokenF = (char *) malloc((strlen(token)+3)*sizeof(char));
				strcpy(tokenF,"- ");
				strcat(tokenF,token);
				return strdup(tokenF);

			}
		}
		else if (strcmp(filho->type,"Store") == 0 || strcmp(filho->type,"Minus")==0 || strcmp(filho->type,"Plus")==0){
				char * type = searchId(funcName,filho->noFilho->id);
				if(strcmp(type,"undef")==0){
					if(strcmp(filho->noFilho->type,"IntLit")==0 || strcmp(filho->noFilho->type,"ChrLit")==0){
						strcpy(type, "- int");
					}
					else if(strcmp(filho->noFilho->type,"RealLit")==0){
						strcpy(type, "- double");
					}
				}
				return strdup(type);
		}
		else if(strcmp(filho->type,"Comma")==0){
			return searchCommaFirstSon(filho);
		}
		else{
			return strdup("- undef");
		}
	}
	return strdup("- undef");
}

void searchStore(no *atual, char* typeBrother){ //adiciona tipo do store
	no * aux = atual;
	if(aux->noFilho->id!=NULL){
		char * type = searchId(funcName,aux->noFilho->id); //procura a variavel
		if(strstr(type,"(")!=NULL){ // verifica se e uma funcao no lado esquerdo
			strcpy(type, "undef");
		}
		//verifica se e um literal
		if(strcmp(type,"undef")==0){
				if(strcmp(aux->noFilho->type,"IntLit")==0 || strcmp(aux->type,"ChrLit")==0){
					strcpy(type, "int");
				}
				else if(strcmp(aux->noFilho->type,"RealLit")==0){
					strcpy(type, "double");
				}
		}
		//o lado direito tem um operador
		if(strcmp(aux->noFilho->noIrmao->type,"Add")==0 ||strcmp(aux->noFilho->noIrmao->type, "Sub") == 0 ||  strcmp(aux->noFilho->noIrmao->type, "Mul") == 0 || strcmp(aux->noFilho->noIrmao->type, "Div") == 0 ){
			typeBrother = prioridade(aux->noFilho->noIrmao);
			typeBrother = strtok(typeBrother,"- ");
		}
		else if(strcmp(aux->noFilho->noIrmao->type,"Call")==0 || strcmp(aux->noFilho->noIrmao->type, "Or") == 0 || strcmp(aux->noFilho->noIrmao->type, "And") == 0 ||  strcmp(aux->noFilho->noIrmao->type, "BitWiseAnd") == 0 || strcmp(aux->noFilho->noIrmao->type, "BitWiseOr") == 0 || strcmp(aux->noFilho->noIrmao->type, "BitWiseXor") == 0 || strcmp(aux->noFilho->noIrmao->type, "Mod") == 0){
			//procura o tipo do lado direito
			if(aux->noFilho->noIrmao->noFilho->id!=NULL){ 
				typeBrother = searchId(funcName,aux->noFilho->noIrmao->noFilho->id);
			}
			else{
				typeBrother = searchId(funcName,aux->noFilho->noIrmao->noFilho->noFilho->id);
			}
			//verifica se o lado direito nao e um literal
			if(strcmp(typeBrother,"undef")==0){
				if(strcmp(aux->noFilho->noIrmao->type,"IntLit")==0 || strcmp(aux->noFilho->noIrmao->type,"ChrLit")==0){
					strcpy(typeBrother, "int");
				}
				else if(strcmp(aux->noFilho->noIrmao->type,"RealLit")==0){
					strcpy(typeBrother, "double");
				}
			}
		}
		//stores dentro de stores 
		else if(strcmp(aux->noFilho->noIrmao->type,"Store")==0){
			searchStore(aux->noFilho->noIrmao,typeBrother);
		}
		//unários 
		else if(strcmp(aux->noFilho->noIrmao->type,"Minus")==0 || strcmp(aux->noFilho->noIrmao->type,"Plus")==0 || strcmp(aux->noFilho->noIrmao->type,"Not")==0 ){
			char * type;
			if(aux->noFilho->noIrmao->noFilho->id==NULL){
				type = searchId(funcName,aux->noFilho->noIrmao->noFilho->noFilho->id);
			}
			else{
				type = searchId(funcName,aux->noFilho->noIrmao->noFilho->id);
			}
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
		//variavel
		else{
			typeBrother = searchId(funcName,aux->noFilho->noIrmao->id);
			if(strcmp(typeBrother,"undef")==0){
				if(strcmp(aux->noFilho->noIrmao->type,"IntLit")==0 || strcmp(aux->noFilho->noIrmao->type,"ChrLit")==0){
					strcpy(typeBrother, "int");
				}
				else if(strcmp(aux->noFilho->noIrmao->type,"RealLit")==0){
					strcpy(typeBrother, "double");
				}
			}
		}
		aux->exprType = (char *)malloc((strlen(type)+3)*sizeof(char));
		sprintf(aux->exprType,"- %s",type);

	}
	else{ //operador matematico
		aux->exprType = prioridade(atual);
	}

}

void checkBody(no * atual,char * pai){ //anota corpo da funcao
	no * aux = atual;
	if(aux->type!=NULL){
		//literais
		if(strcmp(aux->type,"ChrLit")==0 || strcmp(aux->type,"IntLit")==0){
			aux->exprType= (char *) strdup("- int");
		}
		else if(strcmp(aux->type,"RealLit")==0){
			aux->exprType= (char *) strdup("- double");
		}
		//operadores
		else if(strcmp(aux->type,"Lt")==0 || strcmp(aux->type,"Le")==0 || strcmp(aux->type,"Gt")==0 || strcmp(aux->type,"Ge")==0 || strcmp(aux->type,"Ne")==0 || strcmp(aux->type,"Eq")==0 || strcmp(aux->type,"Not")==0){
			aux->exprType= (char *) strdup("- int");
		}
		else if(strcmp(aux->type, "Or") == 0 || strcmp(aux->type, "And") == 0 ||  strcmp(aux->type, "BitWiseAnd") == 0 || strcmp(aux->type, "BitWiseOr") == 0 || strcmp(aux->type, "BitWiseXor") == 0 || strcmp(aux->type, "Mod") == 0){
			aux->exprType= (char *) strdup("- int");
		}
		//matematicos
		else if (strcmp(aux->type, "Add") == 0 || strcmp(aux->type, "Sub") == 0 ||  strcmp(aux->type, "Mul") == 0 || strcmp(aux->type, "Div") == 0){
			aux->exprType= prioridade(aux);
		}
		//comma operator
		else if(strcmp(aux->type,"Comma")==0){	
			char * id2, *id1;
			id2 = searchComma(aux);
			id1 = searchCommaFirstSon(aux);
			//se um dos filhos for undef entao é undef
			if(strcmp(id1,"- undef")==0 || strcmp(id2, "- undef")==0){
				aux->exprType = (char*)malloc(sizeof(char)*strlen("- undef"));
				strcpy(aux->exprType,"- undef");
			}
			//funções 
			else if(strcmp(id1,id2)!=0 && (strstr(id1,"(")!=NULL || strstr(id2,"(")!=NULL)){
				aux->exprType = (char*)malloc(sizeof(char)*strlen("- undef"));
				strcpy(aux->exprType,"- undef");	
			}
			//de outro modo fica com o tipo do segundo filho
			else{ 
				aux->exprType= id2;
			}
		}
		//call de funções
		else if(strcmp(aux->type,"Call") == 0){
			if(strcmp(aux->noFilho->id,"putchar")==0 || strcmp(aux->noFilho->id,"getchar")==0){
				aux->exprType = (char *)malloc((10)*sizeof(char));
				strcpy(aux->exprType, "- int");
			}	
			else{
				char * type = searchId(funcName,aux->noFilho->id); //anota o tipo da funcao
				aux->exprType = (char *)malloc((strlen(type)+3)*sizeof(char));
				char * token;
    			token = strtok(type,"(");
				sprintf(aux->exprType,"- %s",token);

			}
		}
		else if (strcmp(aux->type,"Store") == 0){ //procura o tipo do store
			char * typeBrother = NULL;
			searchStore(aux,typeBrother);
		}
		//unario
		else if(strcmp(aux->type,"Minus")==0 || strcmp(aux->type,"Plus")==0){
			char * type;
			if(aux->noFilho->id==NULL){ //verifica se é uma var ou literal
				type = (char*)malloc(strlen("int")*sizeof(char));
				strcpy(type,"int");
			}
			else{
				type = searchId(funcName,aux->noFilho->id);
			}
			//verifica se é literal 
			if(strcmp(type,"undef")==0){
				if(strcmp(aux->noFilho->type,"IntLit")==0 || strcmp(aux->noFilho->type,"ChrLit")==0){
					strcpy(type, "int");
				}
				else if(strcmp(aux->noFilho->type,"RealLit")==0){
					strcpy(type, "double");
				}
			}
			//retira parenteses e anota
			aux->exprType = (char *)malloc((strlen(type)+3)*sizeof(char));
			strtok(type,"()");
			sprintf(aux->exprType,"- %s",type);
		}
		else if(aux->id!=NULL && strcmp(pai,"Declaration")!=0){ //é uma variavel
			char * type = searchId(funcName,aux->id); // procura o tipo do id 
			if(strcmp(pai,"Call")==0){ // se for a chamada de uma funcao adiciona os parametros ao tipo 
				char * params = getTypeParams(funcName);
				strcat(type,params);
			}
			aux->exprType = (char *)malloc((strlen(type)+3)*sizeof(char));
			sprintf(aux->exprType,"- %s",type);
		}
		//anota o resto do body
		if(aux->noFilho!=NULL){
			checkBody(aux->noFilho,aux->type);
        }
        if(aux->noIrmao!=NULL){
			checkBody(aux->noIrmao,pai);
        }
	}
}

//limpar listas ligadas

void anotateBody(no * atual){
	if (atual == NULL)
		return;
	if (atual->type != NULL){
		if(strcmp(atual->type, "FuncDefinition")==0){
			funcName = (char *) malloc((strlen(atual->noFilho->noIrmao->id)+1)*sizeof(char));
			strcpy(funcName,atual->noFilho->noIrmao->id);
		}
		else if(strcmp(atual->type,"FuncBody")==0){
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


void clearTreeAnotada(no* auxNode){
	if(auxNode!=NULL){
		if(auxNode->noFilho!=NULL){
				clearTreeAnotada(auxNode->noFilho);
		}
		if(auxNode->noIrmao!=NULL){
				clearTreeAnotada(auxNode->noIrmao);
		}
		free(auxNode);
	}
}