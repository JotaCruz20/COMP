#ifndef ERROR_H
#define ERROR_H

typedef struct erros{
        int line;
        int col;
        char * erro;
        struct erros * noIrmao; // facilita, pois so temos de ter 1 pointer para o filho
} erros; 


void initErrors();
void addErros(int line,int col,char * erro);
void printErros();
void checkProgramError(no * atual);
void checkParamsError(no * atual);
void checkFuncDefinitionError(no * atual);
void checkFuncDeclarationError(no * atual);
void checkDeclarationError(no * atual);
void checkBodyError(no * atual,char * pai);
void anotateBodyError(no * atual);
void addParamsFunctionError(no * atual, char * nameTable);
#endif 