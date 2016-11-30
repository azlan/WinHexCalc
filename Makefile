CC=g++
LIBS=-lgdi32 -lcomctl32

ifeq ($(DEBUG),yes)
	CXXFLAGS=-Wall -W -g
	LDFLAGS=-Wall -W -g
	
else
	CXXFLAGS=-Wall -W
	LDFLAGS=-Wall -W
endif

INCPATH=.
SRCPATH=.
OBJPATH=obj
LIBPATH=lib
BINPATH=.

SRC=$(SRCPATH)/winmain.cpp \
    $(SRCPATH)/hexcalc.cpp \
    $(SRCPATH)/test.cpp
OBJ=$(OBJPATH)/winmain.o \
    $(OBJPATH)/hexcalc.o \
    $(OBJPATH)/test.o
EXEC=$(BINPATH)/hexcalc

INCLUDES=-I ./$(INCPATH)

default: OBJPATH $(EXEC)
	
OBJPATH:
	@mkdir -p $(OBJPATH)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJPATH)/%.o: $(SRCPATH)/%.cpp $(INCPATH)/*.h
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean cleanall

clean:
	rm -rf $(OBJPATH)

cleanall: clean
	rm -rf $(EXEC)