CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lm -lprocps $(shell pkg-config --cflags --libs gtk4)

SRCS = main.c src/Filtros.c src/interfaz.c src/control.c
OBJS = $(SRCS:.c=.o)
EXEC = programa

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(shell pkg-config --cflags gtk4)

clean:
	rm -f $(EXEC) $(OBJS)
