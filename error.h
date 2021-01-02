#ifndef ERROR_H
#define ERROR_H

typedef struct erros{ //struct de erros 
        int line;
        int col;
        char * erro;
        int noOrder; // order do no na tree
        struct erros * noIrmao; 
} erros; 


void initErrors();
int returnOrderErros();
void addErros(int line,int col,char * erro,int noOrder);
void printErros();
void checkProgramError(no * atual);
void checkParamsError(no * atual);
void checkFuncDefinitionError(no * atual);
void checkFuncDeclarationError(no * atual);
void checkDeclarationError(no * atual);
void checkBodyError(no * atual,char * pai);
void anotateBodyError(no * atual);
void addParamsFunctionError(no * atual, char * nameTable);
void checkDeclarationBodyErrors(no * atual,char* type);
void clearErros();
#endif