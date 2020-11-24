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
void checkBody(no * atual);
void printTreeAnotada(no * auxNode, int pontos);

#endif 