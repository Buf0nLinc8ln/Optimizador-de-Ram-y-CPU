CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lm

SRCS = main.c #direccion de los .c
OBJS = $(SRCS:.c=.o)
EXEC = proyecto

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(EXEC) $(OBJS)