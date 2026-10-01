CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11
LIBS = -lreadline

SOURCES = main.c lexer.c token.c parser.c expand.c executor.c builtin.c history.c
TARGET = shellforge

all:
	$(CC) $(CFLAGS) $(SOURCES) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)
