CC = clang

CFILES = MD4.c

STRING = ../CStringUtils

LFLAGS = -lCStringUtils -L$(STRING)
IFLAGS = -I$(STRING)

compile: $(CFILES)
	$(CC) -c $(CFILES) $(IFLAGS)
	ar -cvq libMD4.a MD4.o 
