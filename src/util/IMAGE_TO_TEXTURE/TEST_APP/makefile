SOURCES = main.c ../imgToTex.c
EXE = IMG_TEST
CC = gcc
CFLAGS = -I ../
LFLAGS =
OBJECTS = $(SOURCES:.c=.o)


all: BUILD
	
BUILD: $(EXE) $(SOURCES)
	
$(EXE): $(OBJECTS)
	$(CC) $(LFLAGS) -o $@ $^
	rm -f $^

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

clean: 
	rm -f $(OBJECTS) $(EXE)