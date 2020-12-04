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
#endif 