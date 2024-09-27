#include "firstmovehelpers.h"

void processfirstreg(symbol** sym, binary** bin, char* line, char* firstvar, char* firstname, int* are, int* regnum, int* IC, int* DC, int* L) {
    *are = FOURTH_ARE; /*are value*/
    *regnum = firstvar[1] - '0'; /*register value*/
    
    sscanf(line, "%*[^,],%s", firstvar);
    
    if (firstvar[0] == 'r' && isdigit(firstvar[1])) {
        /* Both operands are registers */
    } else {
        addbin(bin, firstname, strcat(intobinary(*regnum, 9), "00000"), "", "", "", "", "", *IC + *DC + *L);
        (*L)++;
    }
} /* Process first register section of line. */

void processsecondreg(symbol** sym, binary** bin, char* line, char* firstvar, char* firstname, int* are, int* firstare, int* regnum, int* IC, int* DC, int* L) {
    *are = FOURTH_ARE; /*are value*/
    
    if (*firstare == FOURTH_ARE) { /*Is register*/
        strcat(firstname, "000000");
        strcat(firstname, intobinary(*regnum, BINARY_LENGTH_OF_3));
        strcat(firstname, intobinary(firstvar[1] - '0', BINARY_LENGTH_OF_3));
        strcat(firstname, "00");
        addbin(bin, "", firstname, "", "", "", "", "", *IC + *DC + *L);
    } else { 
        addbin(bin, "", strcat(intobinary(firstvar[1] - '0', BINARY_DATA_LENGTH), "00"), "", "", "", "", "", *IC + *DC + *L);
    }
    (*L)++;
} /* Process second register section of line. */

void processother(symbol** sym, binary** bin, char* line, char* firstvar, char* firstname, int* are, int* IC, int* DC, int* L) {
    int val = 0;
    *are = SECOND_ARE; /*are value*/
    
    sscanf(firstvar, "%s", firstname);

    if ((val = findsymvalue(*sym, firstname, "data")) != NOT_FOUND) {/*Find if data or external*/
        addbin(bin, firstname, intobinary(val, BINARY_CODE_LENGTH), "", "", "", "", "", *IC + *DC + *L);
        (*L)++;
    } else if ((val = findsymvalue(*sym, firstname, "external")) != NOT_FOUND) {
        addbin(bin, firstname, intobinary(1, BINARY_CODE_LENGTH), "", "", "", "", "", *IC + *DC + *L);
        (*L)++;
    } else {
        addbin(bin, firstname, "?", "", "", "", "", "", *IC + *DC + *L);
        (*L)++;
    }
} /* process other code section of line. */

void processarray(symbol** sym, binary** bin, char* line, char* firstvar, char* firstname, int* are, int *IC, int *DC, int *L) {
    int num = 0;
    char bracketsymbol[MAX_LINE_LENGTH] = {0};
    *are = THIRD_ARE;

    if (sscanf(firstvar, "%[^[] [%d]", firstname, &num) == 2) { 
        
        if (existlabel(sym, firstname) && findsymvalue(*sym, firstname, "data") != NOT_FOUND) { /*Find if known array*/
            addbin(bin, firstname, "", "", "", "", "", "", *IC + *DC + *L);
            (*L)++;
        } else {
            addbin(bin, firstname, "?", "", "", "", "", "", *IC + *DC + *L);
            (*L)++;
        }
        addbin(bin, "", strcat(intobinary(num, BINARY_DATA_LENGTH), "00"), "", "", "", "", "", *IC + *DC + *L);
        (*L)++;
    } else {
        sscanf(firstvar, "%[^[] [%[^]]]", firstname, bracketsymbol);
        
        if (existlabel(sym, firstname) && findsymvalue(*sym, firstname, "data") != NOT_FOUND) {/*If inside [] is define*/
            addbin(bin, firstname, intobinary(findsymvalue(*sym, firstname, "data"), BINARY_CODE_LENGTH), "", "", "", "", "", *IC + *DC + *L);
            (*L)++;
        } else {
            addbin(bin, firstname, "?", "", "", "", "", "", *IC + *DC + *L);
            (*L)++;
        }

        if (existlabel(sym, bracketsymbol) && findsymvalue(*sym, bracketsymbol, "mdefine") != NOT_FOUND) {
            addbin(bin, "", strcat(intobinary(findsymvalue(*sym, bracketsymbol, "mdefine"), BINARY_DATA_LENGTH), "00"), "", "", "", "", "", *IC + *DC + *L);
            (*L)++;
        } else {
            addbin(bin, firstname, "?", "", "", "", "", "", *IC + *DC + *L);
            (*L)++;
        }
    }
} /* process array section of line. */

void processhashtag(symbol** sym, binary** bin, char* line, char* firstvar, char* firstname, int* are, int *IC, int *DC, int *L) {
    int num = 0;
    *are = FIRST_ARE;
    if (sscanf(firstvar + 1, "%d", &num) == 1) {/*Is num or define*/
        addbin(bin, firstname, strcat(intobinary(num, BINARY_DATA_LENGTH), "00"), "", "", "", "", "", *IC + *DC + *L);
        (*L)++;
    } else {
        sscanf(firstvar + 1, "%s", firstname);
        if ((num = findsymvalue(*sym, firstname, "mdefine")) == NOT_FOUND) {
            fprintf(stderr, "ERROR! Line: %d, illegal define\n", *IC + *DC + *L);
            exit(1);
        } else {
            addbin(bin, firstname, strcat(intobinary(num, BINARY_DATA_LENGTH), "00"), "", "", "", "", "", *IC + *DC + *L);
            (*L)++;
        }
    }
} /* process hashtag section of line. */

void processdefine(symbol** sym, char* line) {
    char name[MAX_LINE_LENGTH];
    int value = 0;

    sscanf(line, ".define %s = %d", name, &value);
    if (existlabel(sym, name)) { /*Is mdefine already intialized*/
            fprintf(stderr, "ERROR! define already intialized\n");
            exit(1);
    } else {
        addsym(sym, name, "mdefine", value); 
    }
} /* process define section of line. */

void processdata(symbol** sym, binary** bin, char* line, char* name, int *IC, int *DC, int *L) {
    if (existlabel(sym, name) && (findsymvalue(*sym, name, "data") != NOT_FOUND)) { /*Is data array already intialized*/
            fprintf(stderr, "ERROR! Line: %d, array data already intialized\n", *IC + *DC + *L);
            exit(1);
    } else {
        addsym(sym, name, "data", *IC + *DC + *L);
        parsedata(*sym, bin, name, line, IC, L, DC);
        *DC = countdata(line);
    }
} /* process data section of line. */

void processstring(symbol** sym, binary** bin, char* line, char* name, int *IC, int *DC, int *L) {
    if (existlabel(sym, name) && (findsymvalue(*sym, name, "data") != NOT_FOUND)) {/*Is data string already intialized*/
            fprintf(stderr, "ERROR! Line: %d, array string already intialized\n", *IC + *DC + *L);
            exit(1);
    } else {
        addsym(sym, name, "data", *DC + *IC + *L);
        parsestring(bin, name, line, IC, L, DC);
        *DC = 1 + countstr(line);
    }
} /* process string data section of line. */

void processextern(symbol** sym, char* line, char* name) {
    sscanf(line, ".extern %s", name);
    addsym(sym, name, "external", 0);
} /* process extern section of line. */

void processentry(symbol** sym, char* line, char* name){
    sscanf(line, ".entry %s", name);
    addsym(sym, name, "entry", 0);
}/*process entry section of line.*/

void processcode(symbol** sym, binary** bin, char* line, char opcodearray[OPCODE_ARRAY_LENGTH][STRING_LENGTH], char* actionname, int *IC, int *DC, int *L){
    int opcodevalue = findopcode(opcodearray, actionname);
    if(opcodevalue == -1){ /*If operand not found*/
        fprintf(stderr, "ERROR! Line: %d didn't find opcode: %s\n", *IC + *DC + *L, actionname);
        exit(1);
    } else {
        opcodemanager(sym, bin, line, opcodevalue, IC, L, DC);
    }
}/*process code section of line.*/

void parcetwice(symbol** sym, binary **bin, char *line,int* sourceop, int* destop, int *IC, int *DC, int *L, int opcode){
    int regnum = 0;
    int place = *IC + *L + *DC;
    
    addbin(bin, "", "", "00", intobinary(*destop, BINARY_SHORT_LENGTH), intobinary(*sourceop, BINARY_SHORT_LENGTH), intobinary(opcode, STRING_LENGTH), "0000", *IC + *L + *DC);
    (*L)++;
    firstoperand(*sym, bin, line, IC, DC, L, sourceop, &regnum);
    secondoperand(*sym, bin, line, IC, DC, L, destop, *sourceop, regnum);
    updatebin(*bin, place, *sourceop, *destop);
}/*Parce opcode with both sourceop and destop.*/

void parceonce(symbol** sym, binary **bin, char *line, int* destop, int *IC, int *DC, int *L, int opcode) {
    int place = *IC + *L + *DC;
    
    addbin(bin, "", "", "00", intobinary(*destop, BINARY_SHORT_LENGTH), "00", intobinary(opcode, STRING_LENGTH), "0000", *IC + *L + *DC);
    (*L)++;
    secondoperand(*sym, bin, line, IC, DC, L, destop, 0, 0);
    updatebin(*bin, place, 0, *destop);
}/*Parce opcode with only destop.*/

void parcezero(binary** bin, int *IC, int *DC, int *L, int opcode){
    addbin(bin, "", "", "00", "00", "00", intobinary(opcode, STRING_LENGTH), "0000", *IC + *L + *DC);
    (*L)++;
}/*Parce opcode with no sourceop or destop.*/

