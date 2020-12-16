#ifndef GERADORLLVM_H
#define GERADORLLVM_H

#include "tree.h"
int temporary_var;
int and_cmp;
int or_cmp;
char* tipoLLVM(char* type);
void geracaoLLVM(no *node);
void program_gen(no *node);
void gerDec(no *node);
void gerLocalDec(no *node);
void gerFuncDef(no *node);
void gerParams(no *node);
void gerFuncBody(no *node);
void gerLits(no *node);
int isGlobal(char *name);
void gerId(no *node);
void gerStore(no *node);
void gerMinus(no *node);
void gerReturn(no *node);
void gerCall(no *node);
void typeConversion(no *received, char *expected);
int is_expression(no *node);
void gerArithemetic(no *node,char * op,char * opD);
void gerMod(no *node);
#endif //CODE_GEN_H