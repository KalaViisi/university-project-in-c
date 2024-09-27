#include "preprocess.h"

/*Preprocess goal is to get input file and process macros.*/

macro* findmacro(macro* start, char* name) {
    while (start != NULL) {
        if (strcmp(start->name, name) == 0) {
            return start;
        }
        start = start->next;
    }
    return NULL;
}/* Function to find a macro by name */

void addmacro(macro** start, char* name, char* macrobody) {
    macro* newMacro = (macro *)malloc(sizeof(macro));

    newMacro->name = copystr(name); 
    newMacro->body = copystr(macrobody);
    newMacro->next = *start;
    *start = newMacro;
}/* Function to add a new macro */

void processfile(char *filename) {
    char inputpath[MAX_NAME_LENGTH] = {0};
    char outputpath[MAX_NAME_LENGTH] = {0};
    char line[MAX_LINE_LENGTH];
    char processedLine[MAX_LINE_LENGTH];
    char macroname[LENGTH_OF_MCR] = {0};
    char macrobody[MAX_LINE_LENGTH] = {0};
    int inmacro = 0;
    macro* current = NULL;
    macro* macros = NULL;
    macro* next = NULL;
    FILE* inputfile, *outputfile;
    sprintf(inputpath, "%s.as", filename);
    sprintf(outputpath, "%s.am", filename);
    inputfile = fopen(inputpath, "r");
    outputfile = fopen(outputpath, "w");


    if (!inputfile || !outputfile) {
        perror("Error opening file");
        exit(1);
    }

    while (fgets(line, MAX_LINE_LENGTH, inputfile) != NULL) {
        strcpy(processedLine, cutspace(line));

        if (strncmp(processedLine, "mcr", 3) == 0) {/*Is start of mcr*/
            inmacro = 1;
            sscanf(processedLine, "mcr %s", macroname);
            macrobody[0] = '\0';
        } else if (strncmp(processedLine, "endmcr", 6) == 0 && inmacro) {/*Is end of mcr*/
            addmacro(&macros, macroname, macrobody);
            inmacro = 0;
        } else if (inmacro) {/*Is inside mcr*/
            strcat(macrobody, processedLine);
            strcat(macrobody, "\n"); 
        } else {/*Normal code*/
            current = findmacro(macros, processedLine);
            if (current) {
                fprintf(outputfile, "%s", current->body);
            } else {
                fprintf(outputfile, "%s\n", processedLine);
            }
        }
    }

    fclose(inputfile);
    fclose(outputfile);

    current = macros;
    while (current != NULL) {/*free memory*/
        next = current->next;
        free(current->name);
        free(current->body);
        free(current);
            current = next;

    }
}/*Brains of preprocess program.*/
