#ifndef PREPROCESS_H
#define PREPROCESS_H 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

typedef struct macro {
    char* name;
    char* body;
    struct macro* next;
} macro;

macro* findmacro(macro* start, char* name);
void addmacro(macro** start, char* name, char* macrobody);
void processfile(char* inputFileName);




#endif 
