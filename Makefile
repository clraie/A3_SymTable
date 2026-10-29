# Macros
CC = gcc217
CFLAGS =

# Dependency rules for non-file targets
all: testsymtablelist testsymtablehash

clobber: clean
	rm -f *~ \#*\#

clean:
	rm -f testsymtablelist testsymtablehash *.o

# Dependency rules for file targets
testsymtablelist: testsymtablelist.o symtablelist.o
	$(CC) $(CFLAGS) testsymtablelist.o symtablelist.o -o testsymtablelist

testsymtablelist: testsymtable.o symtablelist.o
	$(CC) $(CFLAGS) -o testsymtablelist testsymtable.o symtablelist.o

testsymtablehash: testsymtable.o symtablehash.o
	$(CC) $(CFLAGS) -o testsymtablehash testsymtable.o symtablehash.o

testsymtable.o: testsymtable.c symtable.h
	$(CC) $(CFLAGS) -c testsymtable.c

symtablelist.o: symtablelist.c symtable.h
	$(CC) $(CFLAGS) -c symtablelist.c

testsymtablehash.o: testsymtablehash.c symtable.h
	$(CC) $(CFLAGS) -c testsymtablehash.c

symtablehash.o: symtablehash.c symtable.h
	$(CC) $(CFLAGS) -c symtablehash.c
