#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <ctype.h> 
#include "tree.h"

noLex * addNo(char * id,int col,int line){
    noLex * no = (noLex *)malloc(sizeof(noLex));
    if(id!=NULL){
        no->id=(char*)malloc((strlen(id)+1)*sizeof(char));
        strcpy(no->id,id);
    }
    else{
        no->id=NULL;
    }
    no->col=col;
    no->line=line;
    return no;
}