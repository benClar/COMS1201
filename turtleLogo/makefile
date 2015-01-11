CFLAGS = -O4 -Wall -pedantic -std=c99 -F /usr/local/lib/ -I /usr/local/include -lSDL2 -lSDL2_ttf -lSDL2_image
INCS = debug.h libraryFunctions.h testLib.h display.h parser.h turtle.h main.h
TARGET = main
SOURCES = libraryFunctions.c testLib.c display.c parser.c turtle.c $(TARGET).c 
CC = gcc
all: $(TARGET)

$(TARGET) $(SOURCES):  $(INCS)
	$(CC) $(SOURCES) -o pOut  $(CFLAGS) $(LIBS)


clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
