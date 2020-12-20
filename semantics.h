#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "symtab.h"
#include "tree.h"

void checkProgram(no * atual);
void toLowerCase(char* str);
char * checkParams(no * atual);
void addParamsFunction(no * atual, char * nameTable);
void checkFuncDefinition(no * atual);
void checkFuncDeclaration(no * atual);
void checkDeclaration(no * atual);
void checkBody(no * atual, char* pai);
void printTreeAnotada(no * auxNode, int pontos);
void anotateBody(no * atual);
char * prioridade(no * atual);
void addCounter(no * atual);
void clearTreeAnotada(no* auxNode);
#endif 