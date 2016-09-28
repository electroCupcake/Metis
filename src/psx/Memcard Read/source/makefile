SOURCES = main.c
PSX_EXEC = memcardr.exe
PSX_CC = CCPSX.EXE
PSX_CPE2X = CPE2X.EXE
PSX_CFLAGS = -O3 -Dpsx -c
PSX_ADDRESS = 0x80010000
PSX_LDFLAGS =  -l libpad -l libmcrd -Xo$(PSX_ADDRESS)
PSX_OBJECTS = $(SOURCES:.c=.obj)
CPE = $(PSX_EXEC:.exe=.cpe)
SYM = $(PSX_EXEC:.exe=.sym)
MAP = $(PSX_EXEC:.exe=.map)
WINE = wine
DOSEMU = echo 'D:\r cd "$(CURDIR:/%=%)/"\r $(PSX_CPE2X) $(CPE)\r exitemu\r' | dosemu -dumb

all: PSX_CC := $(WINE) $(PSX_CC)
all: PSX_CPE2X := $(DOSEMU)
all: PSX_BUILD
	
PSX_BUILD: $(SOURCES) $(PSX_EXEC)

$(PSX_EXEC): $(CPE)
	$(PSX_CPE2X)
	rm -rf $(PSX_OBJECTS) $(CPE) $(SYM) $(MAP)

$(CPE): $(PSX_OBJECTS)
	$(PSX_CC) $(PSX_OBJECTS) $(PSX_LDFLAGS) -o$(CPE),$(SYM),$(MAP)
	
%.obj: %.c
	$(PSX_CC) $< $(PSX_CFLAGS) -o $@

clean:
	rm -f $(EXEC) $(PSX_EXEC) $(CPE) $(SYM) $(MAP) $(OBJECTS) $(PSX_OBJECTS)E) $(SYM) $(MAP) $(OBJECTS) $(PSX_OBJECTS)