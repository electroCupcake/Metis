SOURCES = main.c ../objectBuilder.c ../../BUILD_UTIL/buildUtil.c
EXE = OBJ_TEST
CC = gcc
CFLAGS = -I  ../../../../include -I ../ -I ../../YXML_PSYQ_PORT -I ../../BUILD_UTIL
LFLAGS = -L ../../YXML_PSYQ_PORT/ -l yxml
OBJECTS = $(SOURCES:.c=.o)


all: BUILD
	
BUILD: $(EXE) $(SOURCES)
	
$(EXE): $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS)
	rm -f $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJECTS) $(EXE)