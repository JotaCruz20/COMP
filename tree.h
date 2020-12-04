#ifndef TREE_H
#define TREE_H
   
typedef struct no{
    char * id;
    char * type;
    int line;
    int col;
    char * exprType;
    struct no * noFilho; // cada no tem 1 filho e 1 irmao(podem ser null)
    struct no * noIrmao; // facilita, pois so temos de ter 1 pointer para o filho
} no;                    // e o filho terá pointer para os irmaos (nao e necessario array de ponteiros)

 typedef struct noLex{
    char * id;
    int line;
    int col;
} noLex;  

void printTree(no * auxNode, int pontos);
noLex* addNo(char * id,int col,int line);
#endif