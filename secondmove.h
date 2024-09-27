#ifndef SECONDMOVE_H
#define SECONDMOVE_H 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "functions.h"
#include "firstmove.h"



void secondmove(binary** bin, symbol** sym, char* name);
void printentries(symbol *sym, char* name);
void printexterns(symbol *sym, char* name);
void codetosecretbase(char* str, FILE* file);
void printcode(binary *bin, char* name);

#endif 
