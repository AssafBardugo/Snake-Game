CC = gcc
CFLAGS = -Wall -Wextra

INCLUDES = -Isrc/Include -Lsrc/lib
LIBS = -lSDL2main -lSDL2

SRCS = main.c snake.c
OBJS = $(SRCS:.c=.o)


snake: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o snake $(OBJS) $(LIBS)

main.o: main.c snake.h
	$(CC) $(CFLAGS) $(INCLUDES) -c main.c $(LIBS)

snake.o: snake.c snake.h
	$(CC) $(CFLAGS) $(INCLUDES) -c snake.c $(LIBS)

clean: 
	rm -f $(OBJS)