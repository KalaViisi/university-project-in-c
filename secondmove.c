#include "secondmove.h"

/*Secondmove goal is to complete firstmove, by searching for assembly language firstmove couldnt code, and code to secret base 4 code.*/


void secondmove(binary** bin, symbol** sym, char* name){
    char* binaryare = NULL, *binaryData = NULL;
    char fullBinary[BINARY_CODE_LENGTH] = {0};
    int symbolValue = NOT_FOUND;
    binary* current = *bin;

    while (current != NULL) {
        if (strchr(current->data, '?') != NULL) { /*Search for ? struct*/
            binaryare = NULL;
            symbolValue = findsymvalue(*sym, current->name, "code");
            if (symbolValue != NOT_FOUND) {/*Apply approperiate are*/
                binaryare = "10";
            } else {
                symbolValue = findsymvalue(*sym, current->name, "data");
                if (symbolValue != NOT_FOUND) {
                    binaryare = "01";
                }
            }

            if (binaryare != NULL) {/*If found symbol*/
                binaryData = intobinary(symbolValue, BINARY_DATA_LENGTH);
                if (binaryData) {
                    memset(fullBinary, '\0', sizeof(fullBinary));
    
                    strcat(fullBinary, binaryData);/*copy to struct binary code*/
                    strcat(fullBinary, binaryare);
                    strncpy(current->data, fullBinary, BINARY_CODE_LENGTH);
    
                    free(binaryData);
                }
            }
        }
        current = current->next;
    }

    
    printentries(*sym, name);
    printexterns(*sym, name);
    printcode(*bin, name);
    
}/*Brains of secondmove*/

void printentries(symbol *sym, char* name) {
    symbol *current = sym;
    int hasEntry = 0, value = 0;
    FILE *file = NULL;
    char filename[MAX_NAME_LENGTH];
    sprintf(filename, "%s.ent", name);

    
    while (current != NULL) {/*Check for entries*/
        if (strcmp(current->definition, "entry") == 0) {
            hasEntry = 1;
            break;
        }
        current = current->next;
    }

    if (!hasEntry){/*If no entries found*/
        return;
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open ps.ent");
        return;
    }

    current = sym;
    while (current != NULL) {
        if (strcmp(current->definition, "entry") == 0) {/*Struct of entries*/
            value = findsymvalue(sym, current->name, "code");
            if (value == NOT_FOUND) {
                value = findsymvalue(sym, current->name, "data");
            }
            fprintf(file, "%s %d\n", current->name, value);
        }
        current = current->next;
    }

    fclose(file);
}/*Print to file all entries*/

void printexterns(symbol *sym, char* name) {
    symbol *current = sym;
    int hasExtern = 0;
    FILE *file = NULL;
    char filename[MAX_NAME_LENGTH];
    sprintf(filename, "%s.ext", name);

    
    while (current != NULL) {/*Check for entries*/
        if (strcmp(current->definition, "external") == 0) {
            hasExtern = 1;
            break;
        }
        current = current->next;
    }

    if (!hasExtern){/*If no entries found*/
        return;
    } 

    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open ps.ext");
        return;
    }

    while (current != NULL) {
        if (strcmp(current->definition, "external") == 0) {/*Struct of external*/
            fprintf(file, "%s\n", current->name);
        }
        current = current->next;
    }

    fclose(file);
}/*Print to file all externs*/

void codetosecretbase(char* str, FILE* file) {
    char modifiedStr[LENGTH_OF_BASE_4] = {0};
    int i, j;
    for (i = 0, j = 0; i < BINARY_CODE_LENGTH && str[i] != '\0'; i += 2, ++j) { /*Code to secret base 4*/
        if (str[i] == '0' && str[i+1] == '0') {/*If is 1, 2, 3, 0 in base 4*/
            modifiedStr[j] = '*';
        } else if (str[i] == '0' && str[i+1] == '1') {
            modifiedStr[j] = '#';
        } else if (str[i] == '1' && str[i+1] == '0') {
            modifiedStr[j] = '%';
        } else if (str[i] == '1' && str[i+1] == '1') {
            modifiedStr[j] = '!';
        }
    }

    fprintf(file, "%s", modifiedStr);
}/*Changes code from base 2, to secret base 4*/

void printcode(binary *bin, char* name) {
    binary *current = bin;
    char filename[MAX_NAME_LENGTH];
    FILE* file = NULL;
    sprintf(filename, "%s.ob", name);
    file = fopen(filename, "w");



    while (current != NULL) {
        if (strcmp(current->data, "\0") != 0) {/*Print data section if not null, else print other things*/
            fprintf(file, "%d\t", current->value);
            codetosecretbase(current->data, file);
            fprintf(file, "\n");
        } else {
            fprintf(file, "%d\t", current->value);
            codetosecretbase(current->notused, file);
            codetosecretbase(current->opcode, file);
            codetosecretbase(current->sourcenoperand, file);
            codetosecretbase(current->destinationoperand, file);
            codetosecretbase(current->are, file);
            fprintf(file, "\n");
        }
        current = current->next; 
    }

    fclose(file);
}/*Print to file all code*/
