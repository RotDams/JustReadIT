SOURCE  	= $(wildcard *.c)
OBJS     	= $(SOURCE:.c=.o)
HEADER   	= $(SOURCE:.c=.h)
OUT       	= output.exe
CC        	= gcc
FLAGS      	= -O3
CFLAGS     	= -Wall -Wextra -Werror -O3
SDL_FLAGS 	= `sdl-config --cflags --libs`

all: $(OBJS)
	$(CC) $(FLAGS) -g $(OBJS) -o $(OUT) $(SDL_FLAGS)

clean:
	rm -f $(OBJS) $(OUT)
