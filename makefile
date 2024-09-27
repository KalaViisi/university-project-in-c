assembler: main.o preprocess.o firstmove.o secondmove.o firstmovehelpers.o functions.o
	gcc -ansi -Wall -pedantic -g main.o preprocess.o firstmove.o secondmove.o firstmovehelpers.o functions.o -o assembler -lm

main.o: main.c
	gcc -ansi -Wall -pedantic -g -c main.c -o main.o

preprocess.o: preprocess.c preprocess.h
	gcc -ansi -Wall -pedantic -g -c preprocess.c -o preprocess.o

firstmove.o: firstmove.c firstmove.h
	gcc -ansi -Wall -pedantic -g -c firstmove.c -o firstmove.o

secondmove.o: secondmove.c secondmove.h
	gcc -ansi -Wall -pedantic -g -c secondmove.c -o secondmove.o

firstmovehelpers.o: firstmovehelpers.c firstmovehelpers.h
	gcc -ansi -Wall -pedantic -g -c firstmovehelpers.c -o firstmovehelpers.o

functions.o: functions.c functions.h
	gcc -ansi -Wall -pedantic -g -c functions.c -o functions.o


