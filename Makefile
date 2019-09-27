OBJS	= main.o
SOURCE	= main.c
HEADER	= main.h
OUT	= output.exe
CC	 = gcc
FLAGS	 = -g -c -Wall
SDL_FLAGS	 = `sdl-config --cflags --libs` 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(SDL_FLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 


clean:
	rm -f $(OBJS) $(OUT)
