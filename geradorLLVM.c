#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geradorLLVM.h"
#include "symtab.h"

char* tipoLLVM(char* type){ // retorna o tipo da variavel
	type = strtok(type,"- ()");
	if(strcmp(type, "Int")==0 || strcmp(type, "int")==0 || strcmp(type, "Short")==0 || strcmp(type, "short")==0 || strcmp(type, "Char")==0 || strcmp(type, "char")==0) {
		return strdup("i32");
	} 
	else if(strcmp(type, "Double")==0 || strcmp(type, "double")==0) {
		return strdup("double");
	} 
	else if(strcmp(type, "Void")==0) {
		return strdup("void");
	}
	return "null";
}

void geracaoLLVM(no *node){
	if(0==strcmp(node->type, "Program")) {
		program_gen(node);
	} 
	else if(0 == strcmp(node->type, "Declaration")) {
		gerDec(node);
	} 
	else if(0 == strcmp(node->type, "FuncDefinition")) {
		gerFuncDef(node);
	} 
	else if(0 == strcmp(node->type, "ParamList")) {
		gerParams(node);
	} 
	else if(0 == strcmp(node->type, "FuncBody")) {
		gerFuncBody(node);
	} 
	else if(0 == strcmp(node->type, "ChrLit") || 0 == strcmp(node->type, "IntLit") || 0 == strcmp(node->type, "RealLit")) {
		gerLits(node);
	} 
	else if(0 == strcmp(node->type, "Id")){
		gerId(node);
	} 
	else if(0 == strcmp(node->type, "Store")) {
		gerStore(node);
	}
	/*} else if(0 == strcmp(node->type, "Plus")) {
		geracaoLLVM(node->noFilho);
	} */
	else if(0 == strcmp(node->type, "Minus")) {
		gerMinus(node);
	} 
	else if(0 == strcmp(node->type, "Return")) {
		gerReturn(node);
	} 
	else if(0 == strcmp(node->type, "Call")) {
		gerCall(node);
	} 
	/*else if(0 == strcmp(node->type, "Add")) {
		gerAdd(node);
	} else if(0 == strcmp(node->type, "Sub")) {
		gerSub(node);
	} else if(0 == strcmp(node->type, "Mul")) {
		gerMul(node);
	} else if(0 == strcmp(node->type, "Div")) {
		gerDiv(node);
	} else if(0 == strcmp(node->type, "Mod")) {
		gerMod(node);
	} else if(0 == strcmp(node->type, "Or")) {
		gerOr(node);
	} else if(0 == strcmp(node->type, "And")) {
		gerAnd(node);
	} else if(0 == strcmp(node->type, "BitWiseAnd")) {
		gerBitWiseAnd(node);
	} else if(0 == strcmp(node->type, "BitWiseOr")) {
		gerBitWiseOr(node);
	} else if(0 == strcmp(node->type, "BitWiseXor")) {
		gerBitWiseXor(node);
	} else if(0 == strcmp(node->type, "Eq")) {
		gerEq(node);
	} else if(0 == strcmp(node->type, "Ne")) {
		gerNe(node);
	} else if(0 == strcmp(node->type, "Le")) {
		gerLe(node);
	} else if(0 == strcmp(node->type, "Ge")) {
		gerGe(node);
	} else if(0 == strcmp(node->type, "Lt")) {
		gerLt(node);
	} else if(0 == strcmp(node->type, "Gt")) {
		gerGt(node);
	} else if(0 == strcmp(node->type, "Not")) {
		gerNot(node);
	}*/
}

void program_gen(no *node){
	no *aux = node->noFilho;
	while(aux != NULL){
		geracaoLLVM(aux);
		if(aux->noIrmao == NULL)
			break;
		aux = aux->noIrmao;
	}
	printf("declare i32 @putchar(i32)\n");
	printf("declare i32 @getchar()\n\n");
}

void gerDec(no *node){
	char *type = tipoLLVM(node->noFilho->type);
	if(node->noFilho->noIrmao->noIrmao == NULL){
		if(0 == strcmp(type, "double")) { //só é feito inicializacao da variavel
			printf("@%s = common global double 0.000000e+00\n", node->noFilho->noIrmao->id);
		} else { /* Int, Short, Char */
			printf("@%s = common global %s 0\n", node->noFilho->noIrmao->id, type);
		}
	} else { //está a ser adicionado um valor na inicializacao
		printf("@%s = global %s ", node->noFilho->noIrmao->id, type);
		geracaoLLVM(node->noFilho->noIrmao->noIrmao);
		printf("\n");
	}
}

void gerLocalDec(no *node){//trata das declaracoes locais
	char *type = tipoLLVM(node->noFilho->type);
	char *name = node->noFilho->noIrmao->id;
	printf("%%%s = alloca %s\n", name, type);
	if(node->noFilho->noIrmao->noIrmao != NULL){
		if(0 == strcmp(node->noFilho->noIrmao->noIrmao->type, "Id")){
			printf("%%%d = load %s, %s* ", temporary_var, type, type);
			geracaoLLVM(node->noFilho->noIrmao->noIrmao); // dar print a variavel que esta no lado direito do igual
			printf("\n");
			temporary_var++;
			printf("store %s %%%d, %s* %%%s\n", type, temporary_var, type, name);
		} else if(0 == strcmp(node->noFilho->noIrmao->noIrmao->type, "IntLit") || 0 == strcmp(node->noFilho->noIrmao->noIrmao->type, "RealLit") || 0 == strcmp(node->noFilho->noIrmao->noIrmao->type, "ChrLit")){ /* Other Terminal */
			printf("store %s ", type);
			geracaoLLVM(node->noFilho->noIrmao->noIrmao);
			printf(", %s* %%%s\n", type, name);
		} else if(0 == strcmp(node->noFilho->noIrmao->noIrmao->type, "Minus") || 0 == strcmp(node->noFilho->noIrmao->noIrmao->type, "Plus")){
			if(0 == strcmp(node->noFilho->noIrmao->noIrmao->noFilho->type, "IntLit") || 0 == strcmp(node->noFilho->noIrmao->noIrmao->noFilho->type, "RealLit") || 0 == strcmp(node->noFilho->noIrmao->noIrmao->noFilho->type, "ChrLit")){
				printf("store %s ", type);
				geracaoLLVM(node->noFilho->noIrmao->noIrmao);
				printf(", %s* %%%s\n", type, name);
			} else {
				geracaoLLVM(node->noFilho->noIrmao->noIrmao); // vai correr para ir a parte do gerMinus e por o mul -1
				printf("store %s %%%d, %s* %%%s\n", type, temporary_var-1, type, name);
			}
		} else { //expressoes 
			geracaoLLVM(node->noFilho->noIrmao->noIrmao);
			printf("store %s %%%d, %s* %%%s\n", type, temporary_var-1, type, name);
		}
	}
}

void gerReturn(no *node){
	char * type;
	if(strcmp(node->noFilho->type,"Id")==0){
			type = tipoLLVM(node->noFilho->exprType);		
	}
	else{
			type = tipoLLVM(node->noFilho->type);
	}
	if(0 == strcmp(node->noFilho->type, "Id")){
		printf("%%%d = load %s, %s* ", temporary_var, type, type);
		geracaoLLVM(node->noFilho);
		printf("\n");
		printf("ret %s %%%d\n", type, temporary_var);
		temporary_var++;
	} else {
		printf("ret %s ", type);
		geracaoLLVM(node->noFilho);
		printf("\n");
	}
}

void gerFuncDef(no *node){ //falta saber para q serve as flags and,or
	temporary_var = 1; // numero da variavel
	//and_cmp = 1;
	//or_cmp = 1;
	char *type = tipoLLVM(node->noFilho->type);
	char *name = node->noFilho->noIrmao->id; // nome da funcao
	printf("define %s @%s", type, name);
	no *aux = node->noFilho->noIrmao->noIrmao; // passa a param list
	gerParams(aux->noFilho);
	printf("{\n");
	aux = aux->noIrmao;
	geracaoLLVM(aux); // corre até final da funcao
	if(0 == strcmp(type, "void")){
		printf("ret void\n");
	}
	printf("ret i32 0\n"); //adiciona sempre um return 0 pois nao causa problemas e toma conta de funções sem return definido
	printf("}\n\n");//fecha funcao
}

void gerParams(no *node){ // gere os argumentos da funcao
	no *aux = node;
	printf("(");
	while(aux != NULL){ 
		char *type = tipoLLVM(aux->noFilho->type);
		if(0 == strcmp(type, "void")){
			printf(") ");
			return;
		} else {
			if(aux->noIrmao != NULL){ // tem mais argumentos
				printf("%s %%%s,", type, aux->noFilho->noIrmao->id); 
			} else { //ultimo argumento
				printf("%s %%%s) ", type, aux->noFilho->noIrmao->id); 
			}
		}
		if(aux->noIrmao == NULL)
			break;
		aux = aux->noIrmao;
	}
}

void gerFuncBody(no *node){
	no *aux = node->noFilho;
	while(aux != NULL){
		if(0 == strcmp(aux->type, "Declaration")){
			gerLocalDec(aux);
		} else {
			geracaoLLVM(aux);
		}
		if(aux->noIrmao == NULL)
			break;
		aux = aux->noIrmao;
	}
}

void gerLits(no *node){//da print ao id do Lit(ChrList,RealLit e IntLit)
	if(strcmp(node->type,"ChrLit")==0){
		if(node->id[1]=='\\'){
			if(node->id[2] == 'n'){
				char tmp = '\n';
				printf("%d", tmp);
			}
			else if(node->id[2] == 't'){
				char tmp = '\t';
				printf("%d", tmp);
			} 
			else if(node->id[2] == 'r'){
				char tmp = '\r';
				printf("%d", tmp);
			} 
			else if(node->id[2] == '\''){
				char tmp = '\'';
				printf("%d", tmp);
			} 
			else if(node->id[2] == '\\'){
				char tmp = '\\';
				printf("%d", tmp);
			} 
			else if(node->id[2] == '\"'){
				char tmp = '\"';
				printf("%d", tmp);
			}
			else{
				printf("%d", node->id[1]);
			}
		}
		else{
			printf("%d", node->id[1]);
		}
	}
	else{
		printf("%s", node->id);
	}
}

void gerId(no *node){//verifica se é global, se não é local
	int n = isGlobal(node->id);
	if(n==1){	
		printf("@%s", node->id);
	}
	else{
		printf("%%%s", node->id);
	}

}

void gerStore(no *node){
	char *type = tipoLLVM(node->noFilho->type); //tipo do filho
	char *name = node->noFilho->id;
	int global = isGlobal(name); // ve se a variavel é global, pois se for é preciso alterar
	if(0 == strcmp(node->noFilho->noIrmao->type, "IntLit") || 0 == strcmp(node->noFilho->noIrmao->type, "RealLit") || 0 == strcmp(node->noFilho->noIrmao->type, "ChrLit") || 0 == strcmp(node->noFilho->noIrmao->type, "Id")){
		printf("store %s ", type);
		geracaoLLVM(node->noFilho->noIrmao); // obter o valor que vai estar no lado direito do Store
		if(global){ // se for global e preciso adicionar o @ 
			printf(", %s* @%s\n", type, name);
		} else {
			printf(", %s* %%%s\n", type, name);
		}
	} else if(0 == strcmp(node->noFilho->noIrmao->type, "Minus") || 0 == strcmp(node->noFilho->noIrmao->type, "Plus")){
			if(0 == strcmp(node->noFilho->noIrmao->noFilho->type, "IntLit") || 0 == strcmp(node->noFilho->noIrmao->noFilho->type, "RealLit") || 0 == strcmp(node->noFilho->noIrmao->noFilho->type, "ChrLit")){
				printf("store %s ", type);
				geracaoLLVM(node->noFilho->noIrmao);
				if(global) {
					printf(", %s* @%s\n", type, name);
				} else {
					printf(", %s* %%%s\n", type, name);
				}
			} else { //id
				geracaoLLVM(node->noFilho->noIrmao);
				if(global){
					printf("store %s %%%d, %s* @%s\n", type, temporary_var-1, type, name);
				} else {
					printf("store %s %%%d, %s* %%%s\n", type, temporary_var-1, type, name);
				}
			}
	} else { //id
		geracaoLLVM(node->noFilho->noIrmao);
		if(global){
			printf("store %s %%%d, %s* @%s\n", type, temporary_var-1, type, name);
		} else {
			printf("store %s %%%d, %s* %%%s\n", type, temporary_var-1, type, name);
		}
		
	}
}

void gerMinus(no *node){//gera codigo para quando do Minus
	if(0 == strcmp(node->noFilho->type, "IntLit") || 0 == strcmp(node->noFilho->type, "RealLit") || 0 == strcmp(node->noFilho->type, "ChrLit")){
			printf("-");
			geracaoLLVM(node->noFilho);
	} 
	else if(0 == strcmp(node->noFilho->type, "Id")){ //ids
		if(0 == strcmp(node->noFilho->type, "double")){
			printf("%%%d = load double, double* ", temporary_var);
			geracaoLLVM(node->noFilho);
			printf("\n");
			temporary_var++;
			printf("%%%d = fmul double -1.0, %%%d\n", temporary_var, temporary_var-1); //fmul é para doubles
		} else {
			printf("%%%d = load %s, %s* ", temporary_var, tipoLLVM(node->noFilho->exprType), tipoLLVM(node->noFilho->exprType));
			geracaoLLVM(node->noFilho);
			printf("\n");
			temporary_var++;
			printf("%%%d = mul %s -1, %%%d\n", temporary_var, tipoLLVM(node->noFilho->exprType), temporary_var-1); // para obter o resultado negativo multplica-se por -1
		}
		temporary_var++;
	} else {  //funcoes
		if(0 == strcmp(node->noFilho->type, "double")){
			printf("%%%d = fmul double -1.0, %%%d\n", temporary_var, temporary_var-1);
		} else {
			printf("%%%d = mul %s -1, %%%d\n", temporary_var, tipoLLVM(node->noFilho->type), temporary_var-1);
		}
		temporary_var++;
	}
}

void gerCall(no *node){
	char *name = node->noFilho->id;
	char * type;
	if(strcmp(node->noFilho->type,"Id")==0){
		type = tipoLLVM(node->noFilho->exprType);
	}
	else{
		type = tipoLLVM(node->noFilho->type);
	}
	
	noTabela *symbol = searchFunc(node->noFilho->id);
	int params = 0;
	int param_array[999]; //vai guardar os varios parametros para fazer a Call da funcao 
	no *aux = node->noFilho->noIrmao; //primeiro parametro
	char *param = (char*)malloc(strlen(symbol->tipo)*sizeof(char));
	strcpy(param,symbol->tipo);
	char * token=strtok(param,"(");
	token = strtok(NULL,")");

	char *param_type, *expected_type;
	while(aux != NULL){ //vai dar load dos argumentos da funcao
		if(strcmp(aux->type,"ChrLit")==0 || strcmp(aux->type,"IntLit")==0 || strcmp(aux->type,"RealLit")==0 || strcmp(aux->type,"Id")==0){
			param_type = tipoLLVM(aux->exprType);	//tipo do argumento
		}
		else{
			param_type = tipoLLVM(aux->type);//tipo do argumento
		}
		expected_type = tipoLLVM(token); // tipo esperado pela funcao
		if(strcmp(param_type, expected_type)!=0){ //se forem de tipos diferentes ve se pode converter
			if(0 == strcmp(aux->type, "Id")){
				printf("%%%d = load %s, %s* ", temporary_var, param_type, param_type);
				geracaoLLVM(aux);
				printf("\n");
				temporary_var++;
				typeConversion(aux, expected_type);
				param_array[params] = temporary_var-1; 
			} else if(0 == strcmp(aux->type, "IntLit") || 0 == strcmp(aux->type, "ChrLit") || 0 == strcmp(aux->type, "RealLit")){
				typeConversion(aux, expected_type);
				param_array[params] = temporary_var-1;
			} else { //expressao(ADD,OR,...)
				geracaoLLVM(aux);
				typeConversion(aux, expected_type);
				param_array[params] = temporary_var-1;
			}
		} 
		else { // tipo correto
			if(0 == strcmp(aux->type, "Id")){
				printf("%%%d = load %s, %s* ", temporary_var, param_type, param_type);
				geracaoLLVM(aux); // vai buscar a variavel correta a dar load 
				printf("\n");
				param_array[params] = temporary_var;
				temporary_var++;
			} else if(is_expression(aux)){
				geracaoLLVM(aux);
				param_array[params] = temporary_var-1;
			} 
		}
		params++;
		token = strtok(NULL,",");
		aux = aux->noIrmao;
	}
	printf("%%%d = call %s @%s(", temporary_var, type, name); //vai por os diferentes argumentos na funcao
	temporary_var++;
	aux = node->noFilho->noIrmao;
	free(param);
	param = (char*)malloc(strlen(symbol->params)*sizeof(char));
	strcpy(param,symbol->tipo);
	token=strtok(param,"(");
	token = strtok(NULL,")");
	int temp = 0;
	if(aux==NULL){ //caso seja void
		printf(")\n");
	}
	while(aux != NULL){
		if(strcmp(aux->type,"ChrLit")==0 || strcmp(aux->type,"IntLit")==0 || strcmp(aux->type,"RealLit")==0 || strcmp(aux->type,"Id")==0){
			param_type = tipoLLVM(aux->exprType);	
		}
		else{
			param_type = tipoLLVM(aux->type);
		}
		
		expected_type = tipoLLVM(token);
		if(strcmp(param_type, expected_type)!=0){ 
			if(aux->noIrmao == NULL){
				printf("%s %%%d)\n", expected_type, param_array[temp]);
			} else {
				printf("%s %%%d,", expected_type, param_array[temp]);		
			}
		} else {
			if(aux->noIrmao == NULL){
				if(0 == strcmp(aux->type, "IntLit") || 0 == strcmp(aux->type, "ChrLit") || 0 == strcmp(aux->type, "RealLit")){
					printf("%s ", expected_type);
					geracaoLLVM(aux); //para dar print ao aux(variavel)
					printf(")\n");
				} else {
					printf("%s %%%d)\n", expected_type, param_array[temp]);
				}
			} else {
				if(0 == strcmp(aux->type, "IntLit") || 0 == strcmp(aux->type, "ChrLit") || 0 == strcmp(aux->type, "RealLit")){
					printf("%s ", expected_type);
					geracaoLLVM(aux); //para dar print ao aux(variavel)
					printf(",\n");
				} else {
					printf("%s %%%d,\n", expected_type, param_array[temp]);
				}
			}
		}
		temp++;
		token = strtok(NULL,",");
		aux = aux->noIrmao;
	}
}

void typeConversion(no *received, char *expected){
	if((0 == strcmp(received->type, "char") || 0 == strcmp(received->type, "short") || 0 == strcmp(received->type, "int")) && 0 == strcmp(expected, "double")){
		if(strcmp(received->type, "int")){
			if(0 == strcmp(received->type, "IntLit") || 0 == strcmp(received->type, "ChrLit") || 0 == strcmp(received->type, "RealLit")){ //fazer conversao para double
				printf("%%%d = sext ", temporary_var); //estende o numero de bits do valor
				geracaoLLVM(received); // vai buscar o valor do Literal
				printf(" to %s\n", expected);
			} else {
				printf("%%%d = sext %s %%%d to i32\n", temporary_var, tipoLLVM(received->type), temporary_var-1);
			}
			temporary_var++;
		}
		printf("%%%d = sitofp i32 %%%d to %s\n", temporary_var, temporary_var-1, expected); // converte para floating pointer(double)
	} else if(((0 == strcmp(received->type, "int") || 0 == strcmp(received->type, "short")) && 0 == strcmp(expected, "i8")) || (0 == strcmp(received->type, "int") && 0 == strcmp(expected, "i16"))){ //char -> int, short || short -> Got int
		if(0 == strcmp(received->type, "IntLit") || 0 == strcmp(received->type, "ChrLit") || 0 == strcmp(received->type, "RealLit")){
			printf("%%%d = trunc %s ", temporary_var, tipoLLVM(received->type)); // trunca o numero de bits
			geracaoLLVM(received); // buscar o valor do literal
			printf(" to %s\n", expected);
		} else {
			printf("%%%d = trunc %s %%%d to %s\n", temporary_var, tipoLLVM(received->type), temporary_var-1, expected);
		}
	} else if((0 == strcmp(expected, "i8") || 0 == strcmp(expected, "i16") || 0 == strcmp(expected, "i32")) && 0 == strcmp(received->type, "double")){ //char, short, int -> Got double 
		if(0 == strcmp(received->type, "IntLit") || 0 == strcmp(received->type, "ChrLit") || 0 == strcmp(received->type, "RealLit")){
			printf("%%%d = fptosi double ", temporary_var); //floating to double
			geracaoLLVM(received);
			printf(" to %s\n", expected);
		} else {
			printf("%%%d = fptosi double %%%d to %s\n", temporary_var, temporary_var-1, expected);
		}
	} else {
		if(0 == strcmp(received->type, "IntLit") || 0 == strcmp(received->type, "ChrLit") || 0 == strcmp(received->type, "RealLit")){
			printf("%%%d = sext ", temporary_var);
			geracaoLLVM(received);
			printf(" to %s\n", expected);
		} else {
			printf("%%%d = sext %s %%%d to %s\n", temporary_var, tipoLLVM(received->type), temporary_var-1, expected);
		}
	}
	temporary_var++;
}

int is_expression(no *node){//verifica se e uma expressao 
	if(0 == strcmp(node->type, "Add") || 0 == strcmp(node->type, "Sub") || 0 == strcmp(node->type, "Mul") || 0 == strcmp(node->type, "Div") || 0 == strcmp(node->type, "Mod") || 0 == strcmp(node->type, "Eq") || 0 == strcmp(node->type, "Ne") || 0 == strcmp(node->type, "Gt") || 0 == strcmp(node->type, "Lt") || 0 == strcmp(node->type, "Ge") || 0 == strcmp(node->type, "Le") || 0 == strcmp(node->type, "And") || 0 == strcmp(node->type, "Or") || 0 == strcmp(node->type, "BitWiseAnd") || 0 == strcmp(node->type, "BitWiseOr") || 0 == strcmp(node->type, "BitWiseXor")){
		return 1;
	}
	return 0;
}

