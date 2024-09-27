#include "functions.h"


char* cutspace(char* str) {
    char* end = NULL;

    while(isspace((unsigned char)*str)){/*Remove space from start*/
        str++;
    } 

    if(*str == 0){/*Only spaces*/
        return str;
    }

    end = str + strlen(str) - 1;
    
    while(end > str && isspace((unsigned char)*end)){/*Remove space from end*/
        end--;
    }

    *(end+1) = 0;

    return str;
}/*Trim space in start and end of line*/

char* copystr(char* s) {
    char* result = (char *)malloc(strlen(s) + 1);

    if (s == NULL){
        return NULL;
    }  

    strcpy(result, s);
    return result;
    free(result);
}/*Copy string */

