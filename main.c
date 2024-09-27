#include <stdio.h>
#include "functions.h"
#include "preprocess.h"
#include "firstmove.h"
#include "secondmove.h"

void processor(int argc, char** argv);

int main(int argc, char** argv) {
    processor(argc, argv);
    return 0;
}

void processor(int argc, char** argv) {
    symbol *sym = NULL;
    binary *bin = NULL;
    int i = 0;

    for (i = 1; i < argc; i++) {/*Runs loop as long as there is files to process.*/
        processfile(argv[i]);
        firstmove(argv[i], &sym, &bin);
        secondmove(&bin, &sym, argv[i]);


        sym = NULL;
        bin = NULL;
    }
}/*Brains of whole project.*/

