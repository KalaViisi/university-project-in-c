#ifndef FUNCTIONS_H
#define FUNCTIONS_H 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 81
#define STRING_LENGTH 4
#define OPCODE_ARRAY_LENGTH 16
#define BINARY_CODE_LENGTH 14
#define BINARY_DATA_LENGTH 12
#define BINARY_SHORT_LENGTH 2
#define BINARY_LENGTH_OF_3 3
#define LENGTH_OF_BASE_4 8
#define NOT_FOUND -999
#define FIRST_ARE 0
#define SECOND_ARE 1
#define THIRD_ARE 2
#define FOURTH_ARE 3
#define MAX_LINE_LENGTH 81
#define LENGTH_OF_MCR 4
#define MAX_NAME_LENGTH 16

typedef struct symbol {
    char *name;
    char *definition;
    int value;
    struct symbol *next;
} symbol;
typedef struct binary {
    char *name;
    char data[BINARY_CODE_LENGTH+1];
    char are[BINARY_LENGTH_OF_3];
    char destinationoperand[BINARY_LENGTH_OF_3];
    char sourcenoperand[BINARY_LENGTH_OF_3];
    char opcode[STRING_LENGTH+1];
    char notused[STRING_LENGTH+1];
    int value;
    struct binary *next;
} binary;

char* cutspace(char* str);
char* copystr(char* s);



#endif 
