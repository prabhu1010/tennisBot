CC=gcc
CFLAGS=-I.

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
    TARGET := tennisBot
else
    TARGET := tennisBot.exe
endif

%.o: %.c 
	$(CC) -c -g -o $@ $< $(CFLAGS)

tennisBot: tensim_Main.o tensim_Validations.o tensim_Routines.o 
	gcc -o $(TARGET) tensim_Main.o tensim_Validations.o tensim_Routines.o -I.

clean: 
	rm *.o $(TARGET)

