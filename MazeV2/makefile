CFLAGS = `sdl2-config --cflags` -O4 -Wall -pedantic -std=c99 -lm
INCS = .headers/neillsdl2.h
TARGET =  mazeGridModule
SOURCES =  neillsdl2.c maze.c coordListModule.c $(TARGET).c printingModule.c mazeGeneratorModule.c
LIBS =  `sdl2-config --libs`
CC = gcc


all: $(TARGET)

$(TARGET) $(SOURCES):  $(INCS)
	$(CC) $(SOURCES) -o .compiled/maze $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
