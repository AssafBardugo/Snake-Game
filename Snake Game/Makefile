CC = gcc
CFLAGS = -Wall -Wextra

INCLUDES = -Isrc/Include -Lsrc/lib
LIBS = -lSDL2main -lSDL2

SRCS = snake.c
OBJS = $(SRCS:.c=.o)

MAIN = snake


all: clean $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)

snake.o: snake.c
	$(CC) $(CFLAGS) $(INCLUDES) -c snake.c -o snake.o

clean: 
	rm -f *.o *~ $(MAIN)