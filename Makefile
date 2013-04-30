CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
DIR=src/
CONFIGNAME=StudentVM.ini
SOURCES=$(DIR)main.c $(DIR)core.c $(DIR)exceptionHandler.c $(DIR)interpretator.c $(DIR)IOSystem.c $(DIR)memory.c $(DIR)parser.c $(DIR)stack.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=runVM.exe

all: StudentVM

StudentVM: $(SOURCES) $(EXECUTABLE) clean add-ini

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $< -o $@

clean:
	rm -rf src/*.o

add-ini:
	touch $(CONFIGNAME)
	echo "; This is configuration file for "StudentVM" interpreter, \n; here you can make all necessary settings. \n\n [VM Settings] \n MemorySize = 8" > $(CONFIGNAME)
	echo "Done"
	