#CFLAGS = `sdl2-config --cflags` -O4 -Wall -pedantic -std=c99 -lm
CFLAGS = -O4 -Wall -pedantic -std=c99 -lm
INCS = 
TARGET = main
SOURCES = boardModule.c  $(TARGET).c 
#LIBS =  `sdl2-config --libs`
CC = gcc


all: $(TARGET)

$(TARGET) $(SOURCES):  $(INCS)
	$(CC) $(SOURCES) -o pOut  $(CFLAGS) $(LIBS)


clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
