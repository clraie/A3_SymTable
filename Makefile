# Macros
CC = gcc217
CFLAGS =

# Dependency rules for non-file targets
all: testsymtablelist testsymtablehash

clean:
	rm -f testsymtablelist testsymtablehash *.o

# Dependency rules for file targets
testsymtablelist: testsymtablelist.o symtablelist.o
	$(CC) $(CFLAGS) testsymtablelist.o symtablelist.o -o testsymtablelist

testsymtablehash: testsymtablehash.o symtablehash.o
	$(CC) $(CFLAGS) testsymtablehash.o symtablehash.o -o testsymtablehash

testsymtablelist.o: testsymtablelist.c symtable.h
	$(CC) $(CFLAGS) -c testsymtablelist.c

symtablelist.o: symtablelist.c symtable.h
	$(CC) $(CFLAGS) -c symtablelist.c

testsymtablehash.o: testsymtablehash.c symtable.h
	$(CC) $(CFLAGS) -c testsymtablehash.c

symtablehash.o: symtablehash.c symtable.h
	$(CC) $(CFLAGS) -c symtablehash.c
