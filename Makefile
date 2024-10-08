CC = gcc
CFLAGS = -Wall -Wextra

INCLUDES = -Include -Llib
LIBS = -lSDL2main -lSDL2 -lSDL2_ttf

SRCS = main.c snake.c
OBJS = $(SRCS:.c=.o)


snake: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o snake $(OBJS) $(LIBS)

main.o: main.c snake.h
	$(CC) $(CFLAGS) $(INCLUDES) -c main.c $(LIBS)

snake.o: snake.c snake.h
	$(CC) $(CFLAGS) $(INCLUDES) -c snake.c $(LIBS)

clean:
	rm -f $(OBJS) snake