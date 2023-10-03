#include <stdio.h>
#include "erro.h"

void info_erro(char* str){
    fprintf(stderr,"%s\n",str);
}