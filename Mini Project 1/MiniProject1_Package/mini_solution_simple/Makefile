CC = gcc
CFLAGS = -g -Wall

SOURCES = lex.c parser.c codeGen.c main.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = calculator

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run
