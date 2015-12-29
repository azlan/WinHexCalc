CC=g++

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

SRC=$(SRCPATH)/main.cpp \
    $(SRCPATH)/hexcalc.cpp \
    $(SRCPATH)/test.cpp 
OBJ=$(OBJPATH)/main.o \
    $(OBJPATH)/hexcalc.o \
    $(OBJPATH)/test.o 
EXEC=$(BINPATH)/hexcalc

INCLUDES=-I ./$(INCPATH)

default: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ 

$(OBJPATH)/%.o: $(SRCPATH)/%.cpp $(INCPATH)/hexcalc.h
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ 

.PHONY: clean cleanall

clean:
	rm -f $(OBJPATH)/*.o

cleanall: clean
	rm -f $(EXEC)