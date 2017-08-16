CC=gcc
CFLAGS=-I.

%.o: %.c 
	$(CC) -c -g -o $@ $< $(CFLAGS)

tennisBot: tensim_Main.o tensim_Validations.o tensim_Routines.o 
	gcc -o tennisBot tensim_Main.o tensim_Validations.o tensim_Routines.o -I.

clean: 
	rm *.o tennisBot
