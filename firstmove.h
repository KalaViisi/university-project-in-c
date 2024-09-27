#ifndef FIRSTMOVE_H
#define FIRSTMOVE_H 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "functions.h"
#include "firstmovehelpers.h"

typedef enum {
    MOV,
    CMP,
    ADD,
    SUB,
    NOT,
    CLR,
    LEA,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    HLT
} Opcode;

void firstmove(char *filename, symbol **sym, binary **bin);
void linemanager(char *inputfilename, symbol **sym, binary **bin, char *line);
void secondoperand(symbol* sym, binary** bin, char *line, int *IC, int *DC, int *L, int *destop, int are, int regnum);
void firstoperand(symbol* sym, binary* *bin, char *line,int *IC, int *DC, int *L, int *sourceop, int *regnum);
void updatebin(binary* bin, int valueid, int sourceop, int destop);
void addbin(binary** bin, char *name, char *data, char *are, char *destinationoperand, char *sourcenoperand, char *opcode, char *notused, int value);
void addsym(symbol** sym, char *name, char *definition, int value);
void opcodemanager(symbol** sym, binary **bin, char *line, Opcode opcode, int *IC, int *L, int *DC);
void parsestring(binary** bin, char *name, char *line, int *IC, int *L, int *DC);
void parsedata(symbol* sym, binary** bin, char* name, char* line, int *IC, int *L, int *DC);
int countstr(char* line);
int countdata(char* line);
int islabel(char* line, char *label, int skipchar);
int issymbol(char* line);
int existlabel(symbol** sym, char *name);
int findopcode( char opcodearray[OPCODE_ARRAY_LENGTH][STRING_LENGTH], char opcodename[STRING_LENGTH]);
int findsymvalue(symbol* sym,  char *name,  char *definition);
int isempty( char *line);
char* intobinary(int n, int size);


#endif 
