CC = cc
CFLAGS = -Wall -Wextra $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)
TARGET = pong

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
