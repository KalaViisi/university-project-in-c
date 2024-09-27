#include "firstmove.h"

/*Firstmove goal is to code assembly language to binary code, functions firstmove, linemanager, first and second operator are the main functions in this file.*/


void firstmove(char *filename, symbol **sym, binary **bin){
    char line[MAX_LINE_LENGTH] = {0};
    char inputpath[MAX_NAME_LENGTH] = {0};
    sprintf(inputpath, "%s.am", filename);
    linemanager(inputpath, sym, bin, line);
}/*Do first move as instructed.*/

void linemanager(char *inputfilename, symbol **sym, binary **bin, char *line){
    char opcodearray[OPCODE_ARRAY_LENGTH][STRING_LENGTH] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc","dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt"};
    int DC = 0, IC = 100, L = 0, lengthofname = 0;
    char name[MAX_LINE_LENGTH] = {0}, actionname[STRING_LENGTH] = {0}; 
    FILE *inputfile = fopen(inputfilename, "r");
    if (!inputfile) {
        perror("Error opening file");
        exit(1);
    }

    while (fgets(line, MAX_LINE_LENGTH, inputfile) != NULL) { 
        memset(name, '\0', sizeof(name));
        memset(actionname, '\0', sizeof(actionname));
        L = 0, DC = 0;
        
        if(isempty(line) || strncmp(line, ";", 1) == 0){ /*if is empty line*/
            continue;
        } else {
        sscanf(line, "%[^:]", name);
            if (islabel(line, ".define", 0)){ /*Is label*/
            processdefine(sym, line);
        } else if(islabel(line, ".extern", 0)){/*Is extern*/
            processextern(sym, line, name);
        } else if(islabel(line, ".entry", 0)){/*Is entry*/
            processentry(sym, line, name);
        } else if(issymbol(line)){ /*Start with label*/
            lengthofname = strlen(name);

            if(islabel(line ,".data", lengthofname+1)){/*Is data*/
                processdata(sym, bin, line, name, &IC, &DC, &L);
            } else if(islabel(line ,".string", lengthofname+1)){/*Is string*/
                processstring(sym, bin, line, name, &IC, &DC, &L);
            } else { 
                if(existlabel(sym , name)){/*Is known symbol*/
                    fprintf(stderr, "ERROR! Line: %d, symbol already exists\n", IC + DC + L);
                    exit(1);
                } else {
                    addsym(sym, name, "code", IC+DC+L);
                    sscanf(line, "%*[^:]%*c%s", actionname);
                    processcode(sym,bin, line, opcodearray, actionname, &IC, &DC, &L);
                }
            }
        } else {/*Is code*/
            sscanf(line, "%s", actionname);
            processcode(sym,bin, line, opcodearray, actionname, &IC, &DC, &L);
        }
        }
        IC += DC + L;
    }
        fclose(inputfile);
}/*Brains of firstmove*/

void secondoperand(symbol* sym, binary** bin, char *line, int *IC, int *DC, int *L, int *destop, int are, int regnum){
    char firstvar[MAX_LINE_LENGTH] = {0};
    char firstname[MAX_LINE_LENGTH] = {0};

    if(countdata(line) > 1){ /*What to scan in line*/
    sscanf(line, "%*[^,],%s", firstvar);
    }else {
        if(issymbol(line)){
            sscanf(line, "%*s %*s %s", firstvar);
        } else {
            sscanf(line, "%*s %s", firstvar);
        }
    }

    if (firstvar[0] == '#') {/*If is define or num*/
        processhashtag(&sym, bin, line, firstvar, firstname, destop, IC, DC, L);
    } else if (firstvar[0] == 'r' && isdigit(firstvar[1])) {/*Is register*/
        processsecondreg(&sym, bin, line, firstvar, firstname, destop, &are, &regnum, IC, DC, L);
    } else if (isalpha(firstvar[0])) {/*Is string*/
        if (strchr(firstvar, '[') != NULL) {/*Is array*/
            processarray(&sym, bin, line, firstvar, firstname, destop, IC, DC, L);
        } else {
            processother(&sym, bin, line, firstvar, firstname, destop, IC, DC, L);
        }
    } else {
        fprintf(stderr, "ERROR! Line: %d, illegal operand used\n", *IC + *DC + *L);
        exit(1);
    }
}/*Manage second operand in line.*/

void firstoperand(symbol* sym, binary* *bin, char *line,int *IC, int *DC, int *L, int *sourceop, int *regnum){
    char firstvar[MAX_LINE_LENGTH] = {0};
    char firstname[MAX_LINE_LENGTH] = {0};

    if(issymbol(line)){/*What to scan in line*/
        sscanf(line, "%*s %*s %[^,]", firstvar);
    } else {
        sscanf(line, "%*s %[^,]", firstvar);
    }

    if (firstvar[0] == '#') {/*If is define or num*/
        processhashtag(&sym, bin, line, firstvar, firstname, sourceop, IC, DC, L);
    } else if (firstvar[0] == 'r' && isdigit(firstvar[1])) {/*Is register*/
        processfirstreg(&sym, bin, line, firstvar, firstname, sourceop, regnum, IC, DC, L);
    } else if (isalpha(firstvar[0])) {/*Is string*/
        if (strchr(firstvar, '[') != NULL) {/*Is array*/
            processarray(&sym, bin, line, firstvar, firstname, sourceop, IC, DC, L);
        } else {
            processother(&sym, bin, line, firstvar, firstname, sourceop, IC, DC, L);
        }
    } else {
        fprintf(stderr, "ERROR! Line: %d, illegal operand used\n", *IC + *DC + *L);
        exit(1);
    }
}/*Manage first operand in line.*/

void updatebin(binary* bin, int valueid, int sourceop, int destop){
    binary *current = bin;

    while (current != NULL) {
        if (current->value == valueid) {
            strcpy(current->sourcenoperand, intobinary(sourceop, BINARY_SHORT_LENGTH));
            strcpy(current->destinationoperand, intobinary(destop,BINARY_SHORT_LENGTH));
            return; 
        }

        current = current->next; 
    }
}/*Update binary link by value.*/

void addbin(binary** bin, char *name, char *data, char *are, char *destinationoperand, char *sourcenoperand, char *opcode, char *notused, int value){
    binary *newbin = (binary *)malloc(sizeof(binary));
    binary *current = *bin;
    
    newbin->name = copystr(name); 
    strncpy(newbin->data, data, BINARY_CODE_LENGTH);
    strncpy(newbin->are, are, BINARY_SHORT_LENGTH);
    strncpy(newbin->destinationoperand, destinationoperand, BINARY_SHORT_LENGTH);
    strncpy(newbin->sourcenoperand, sourcenoperand, BINARY_SHORT_LENGTH);
    strncpy(newbin->opcode, opcode, STRING_LENGTH);
    strncpy(newbin->notused, notused, STRING_LENGTH);
    newbin->value = value;
    newbin->next = NULL;

    if (*bin == NULL) {/*If list is empty*/
        *bin = newbin;
        return;
    }

    while (current->next != NULL) {/*Go to last linkage*/
        current = current->next;
    }
    current->next = newbin;
}/*Add binary link.*/

void addsym(symbol** sym, char *name, char *definition, int value){
    symbol *newsym = (symbol *)malloc(sizeof(symbol));
    
    newsym->name = copystr(name); 
    newsym->definition = copystr(definition); 
    newsym->value = value;
    newsym->next = *sym;
    *sym = newsym;
}/*Add symbol link.*/

void opcodemanager(symbol** sym, binary **bin, char *line, Opcode opcode, int *IC, int *L, int *DC){
    int sourceop = -1, destop = -1;
    
    switch (opcode) {/*parce opcode*/
        case MOV:
        case CMP:
        case ADD:
        case SUB:
        case LEA:
            parcetwice(sym, bin, line, &sourceop, &destop, IC, DC, L, opcode);
            break;

        case NOT:
        case CLR:
        case INC:
        case DEC:
        case JMP:
        case BNE:
        case RED:
        case PRN:
        case JSR:
            parceonce(sym, bin, line, &destop, IC, DC, L, opcode);
            break;

        case RTS:
        case HLT:
            parcezero(bin, IC, DC, L, opcode);
            break;
    }
    
    switch (opcode){/*Check for errors*/
        case MOV:
        case ADD:
        case SUB:
        if(destop == 0){
            fprintf(stderr, "ERROR! Line: %d, illegal operand used\n", *IC + *DC + *L);
            exit(1);
        }
        break;
        
        case CMP:
        break;

        case LEA:
        if(destop == 0 || sourceop == 0 || sourceop == 3){
            fprintf(stderr, "ERROR! Line: %d, illegal operand used\n", *IC + *DC + *L);
            exit(1);
        }
        break;        
        
        case NOT:
        case CLR:
        case INC:
        case DEC:
        case RED:
        if(sourceop != -1 || destop == 0){
            fprintf(stderr, "ERROR! Line: %d, illegal operand used\n", *IC + *DC + *L);
            exit(1);
        }
        break;
        
        case JMP:
        case BNE:
        case JSR:
        if(sourceop != -1 || destop == 0 || destop == 2){
            fprintf(stderr, "ERROR! Line: %d, illegal operand used\n", *IC + *DC + *L);
            exit(1);
        }
        break;
        
        case PRN:
        case RTS:
        case HLT:
        break;

    }
}/*Manage opdcode.*/

void parsestring(binary** bin, char *name, char *line, int *IC, int *L, int *DC){
    char binarystring[BINARY_CODE_LENGTH];
    int i = 0, c = 0;

    while (line[i] != '\0' && line[i] != '\"') { /*skip char until " */
        i++;
    }

    for (i++; line[i] != '\0' && line[i] != '\"'; i++) { /*count amount of char*/
        c = (int)line[i]; 
        strcpy(binarystring, intobinary(c, BINARY_CODE_LENGTH)); 
        addbin(bin, "", binarystring, "", "", "", "", "", *IC+*L+*DC);
        (*DC)++;
    }
    addbin(bin, "", intobinary(0, BINARY_CODE_LENGTH), "", "", "", "", "", *IC+*L+*DC);
    (*DC)++;
}/*Parse string code.*/

void parsedata(symbol* sym, binary** bin, char* name, char* line, int *IC, int *L, int *DC){
    char delimiters[] = " ,\n";
    char* linecopy = copystr(line);
    char* token = strtok(linecopy, delimiters);
    char* binarydata = NULL;
    int num = -1;
    
    strtok(NULL, delimiters); 
    token = strtok(NULL, delimiters);
    while (token != NULL) { /*count amount of num*/
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {/*What code to write to binary*/
            num = atoi(token);
        } else {
            num = findsymvalue(sym, token, "mdefine");
        }
        if (num == NOT_FOUND) {
            binarydata = "?";
        } else {
            binarydata = intobinary(num, BINARY_CODE_LENGTH);
        }
        addbin(bin, "", binarydata, "", "", "", "", "", *IC+*L+*DC);
        (*DC)++;
        token = strtok(NULL, delimiters);
    }
    free(linecopy);

}/*Parse data code.*/

int countstr(char* line){
    int index = 0, dccounter = 0;
    
    while (line[index] != '\"' && line[index] != EOF){/*Skip until "*/
        index++;
    }

    if (line[index] == '\"') { /*Start counting*/
        index++; 
        while (line[index] != '\"' && line[index] != '\n' && line[index] != '\0') {
            dccounter++;
            index++;
        }
        return dccounter; 
    }
    
    return 0; 
}/*Count amount of letters in string.*/

int countdata(char* line){
    int dcamount = 0, i = 0, isdata = 0;


    if (line[i] != '\0' && line[i] != ',') {/*Start count*/
        dcamount = 1;
        isdata = 1;
    }

    while (line[i] != '\0') {/*Now in loop, counting*/
        if (line[i] == ',' && line[i+1] != ',' && line[i+1] != '\0') {
            dcamount++;
            isdata = 1; 
        }
        i++;
    }

    if (!isdata) {
        dcamount = 0;
    }

    return dcamount;
}/*Count amount of numbers in line.*/

int islabel(char* line, char *label, int skipchar){
    char name[MAX_LINE_LENGTH] = {0};
    char *newline = cutspace(line + skipchar);

    sscanf(newline, "%s", name);

    if(strcmp(label, name) == 0) {
        return 1;
    } else {
    return 0;
    }
}/*Check if is a label by the name of *label.*/

int issymbol(char* line){ 
    int i = 0;

    while (line[i] != '\n' && line[i] != '\0') {/*Check if has name: in line*/
        if (line[i] == ':') {
            return 1;
        }
        i++;
    }
    return 0;
}/*Check if is a symbol *name:.*/

int existlabel(symbol** sym, char *name){
    symbol *current = *sym; 

    while (current != NULL) { /*Go through struct to find if label exists*/
        if (strcmp(current->name, name) == 0) {
            return 1; 
        }
        current = current->next; 
    }
    return 0; 
}/*Check if symbol exist.*/

int findopcode( char opcodearray[OPCODE_ARRAY_LENGTH][STRING_LENGTH], char opcodename[STRING_LENGTH]){
    int i;
    for (i = 0; i < OPCODE_ARRAY_LENGTH; i++) {/*Find opcode worth*/
        if (strcmp(opcodearray[i], opcodename) == 0) {
            return i; 
        }
    }
    return -1;
}/*Find opcode in line.*/

int findsymvalue(symbol* sym,  char *name,  char *definition){
    symbol *current = sym;
    
    while (current != NULL) { /*Go through struct to find value of label*/
        if (strcmp(current->name, name) == 0 && strcmp(current->definition, definition) == 0) {
            return current->value; 
        }
        current = current->next; 
    }
    return NOT_FOUND;
}/*Find symbol value from symbol table.*/

int isempty( char *line){
    while (*line != '\0') {
        if (!isspace((unsigned char)*line)) { 
            return 0; 
        }
        line++;
    }
    return 1; 
}/*Check if line is empty.*/

char* intobinary(int n, int size){
    int i = 0;
    unsigned int twocomp = 0;
    char *binaryStr = (char *)malloc((size + 1) * sizeof(char));
    binaryStr[size] = '\0'; 

    if (n < 0) {/*For < 0*/
        twocomp = ~(-n) + 1; 
        twocomp = twocomp & ((1U << size) - 1);
        n = (int)twocomp;
    }

    for (i = size - 1; i >= 0; --i) {
        binaryStr[i] = (n & 1) + '0'; 
        n >>= 1;
    }

    return binaryStr;
}/*Convert number to twos complementary.*/
