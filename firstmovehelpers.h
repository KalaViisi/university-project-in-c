#ifndef FIRSTMOVEHELPERS_H
#define FIRSTMOVEHELPERS_H 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "functions.h"
#include "firstmove.h"

void processfirstreg(symbol** sym, binary** bin, char* line, char* firstvar, char* firstname, int* are, int* regnum, int* IC, int* DC, int* L);
void processsecondreg(symbol** sym, binary** bin, char* line, char* firstvar, char* firstname, int* are, int* firstare, int* regnum, int* IC, int* DC, int* L);
void processother(symbol** sym, binary** bin, char* line, char* firstvar, char* firstname, int* are, int* IC, int* DC, int* L);
void processarray(symbol** sym, binary** bin, char* line, char* firstvar, char* firstname, int* are, int *IC, int *DC, int *L);
void processhashtag(symbol** sym, binary** bin, char* line, char* firstvar, char* firstname, int* are, int *IC, int *DC, int *L);
void processdefine(symbol** sym, char* line);
void processdata(symbol** sym, binary** bin, char* line, char* name, int *IC, int *DC, int *L);
void processstring(symbol** sym, binary** bin, char* line, char* name, int *IC, int *DC, int *L);
void processextern(symbol** sym, char* line, char* name);
void processentry(symbol** sym, char* line, char* name);
void processcode(symbol** sym, binary** bin, char* line, char opcodearray[OPCODE_ARRAY_LENGTH][STRING_LENGTH], char* actionname, int *IC, int *DC, int *L);
void parcetwice(symbol** sym, binary **bin, char *line,int* sourceop, int* destop, int *IC, int *DC, int *L, int opcode);
void parceonce(symbol** sym, binary **bin, char *line, int* destop, int *IC, int *DC, int *L, int opcode);
void parcezero(binary** bin, int *IC, int *DC, int *L, int opcode);

#endif 
