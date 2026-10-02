CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = shellforge

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -Iinclude src/lexer.c src/main.c src/token.c src/parser.c src/expand.c src/executor.c src/builtin.c src/jobs.c src/jobcontrol.c src/history.c -lreadline -o $(TARGET)

clean:
	rm -f $(TARGET)
