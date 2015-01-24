CFLAGS = `sdl2-config --cflags` -O4 -Wall -pedantic -std=c99 -lm
INCS = debug.h libraryFunctions.h testLib.h neillsdl2.h turtle.h interpreter.h parser.h main.h
TARGET = main
SOURCES = libraryFunctions.c testLib.c neillsdl2.c turtle.c interpreter.c parser.c $(TARGET).c 
CC = gcc
all: $(TARGET)
LIBS =  `sdl2-config --libs`
$(TARGET) $(SOURCES):  $(INCS)
	$(CC) $(SOURCES) -o pOut  $(CFLAGS) $(LIBS)


clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
