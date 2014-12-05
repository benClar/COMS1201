CFLAGS = `sdl2-config --cflags` -O4 -Wall -pedantic -std=c99 -lm
INCS = neillsdl2.h graphics.h boardModule.h moveModule.h testingModule.h main.h 
TARGET = main
SOURCES = neillsdl2.c graphics.c boardModule.c moveModule.c testingModule.c $(TARGET).c 
LIBS =  `sdl2-config --libs`
CC = gcc


all: $(TARGET)

$(TARGET) $(SOURCES):  $(INCS)
	$(CC) $(SOURCES) -o pOut  $(CFLAGS) $(LIBS)


clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
