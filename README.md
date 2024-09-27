# university-project-in-c


Welcome to Daniel's project in Systems Programming Lab.

I have created a project that receives assembly language code,
which will be converted to binary code, and then to base 4 and lastly to cryptic code.

TABLE_OF_CONTENTS:
	HOW TO USE PROJECT.
	HOW TO SEE RESULTS.
	FILES IN PROJECT.
	
HOW TO USE PROJECT:
	Write "make" in ubuntu.
	Write "./assembler assembly1 assembly2" for precoded files. - or create own "name.as" files.

HOW TO SEE RESULTS:
	Results will be put in files, .am for after processing mcr,
		.ent for entries, .ext for externs and .ob for cryptic code.
	If code doesnt hace entries or externs, files will not be made.


FILES IN PROJECT:
	assembly1.as, assembly2.as - contain assembly language code.
	main.c - contains code which brings together all files.
	preprocess.c, preprocess.h - contains code for processing mcr.
	firstmove.c, firstmove.h - contain code for first move of project.
	firstmovehelpers.c firstmovehelpers.h - contain functions for processing in firstmove.c.
	secondmove.c, secondmove.h - contains code for second move of project.
	functions.c, functions.h - contain structs and all defines, for easier control.
	pictures - screenshots of output from files assembly1 and assembly2.


	
