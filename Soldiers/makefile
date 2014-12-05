CFLAGS = -O4 -Wall -pedantic -std=c99 -lm -F /usr/local/lib/ -I /usr/local/include -lSDL2 -lSDL2_ttf -lSDL2_image
INCS = neillsdl2.h graphics.h boardModule.h moveModule.h testingModule.h main.h 
TARGET = main
SOURCES = neillsdl2.c graphics.c boardModule.c moveModule.c testingModule.c $(TARGET).c 
CC = gcc

all: $(TARGET)

$(TARGET) $(SOURCES):  $(INCS)
	$(CC) $(SOURCES) -o pOut  $(CFLAGS) $(LIBS)


clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
