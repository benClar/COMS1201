#	maze.c is main().
#	I have implemented the basic and SDL options fully, with extra functionality around storing the correct exit routecoordinates in a linked list.  I have attempted the random maze generator, but have not achieved full functionality. 
# 	To run: 1) make all
#  			2) .compiled/maze .text/maze[N].txt
#  			3) .compiled/maze .text/maze[N].txt SDL
#  			4) .compiled/maze RANDOM


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
