myprog: dcode.o stable.o macro.o firstcheck.o secondcheck.o prog.o
	gcc -g -ansi prog.o dcode.o stable.o macro.o firstcheck.o secondcheck.o -o myprog -lm
prog.o: prog.c prog.h
	gcc -c -Wall -ansi -pedantic prog.c -o prog.o -lm
dcode.o: dcode.c dcode.h
	gcc -c -Wall -ansi -pedantic dcode.c -o dcode.o -lm
stable.o: stable.c stable.h
	gcc -c -Wall -ansi -pedantic stable.c -o stable.o -lm
macro.o: macro.c macro.h
	gcc -c -Wall -ansi -pedantic macro.c -o macro.o -lm
firstcheck.o: firstcheck.c firstcheck.h stable.h dcode.h macro.h
	gcc -c -Wall -ansi -pedantic firstcheck.c -o firstcheck.o -lm
secondcheck.o: secondcheck.c secondcheck.h firstcheck.h 
	gcc -c -Wall -ansi -pedantic secondcheck.c -o secondcheck.o -lm