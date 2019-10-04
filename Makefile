rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SOURCE      = $(call rwildcard, src/, *.c)
#SOURCE 		= main.c
OBJS        = $(SOURCE:%.c=_build/%.o)
HEADER      = $(SOURCE:.c=.h)
OUT         = output.exe
CC          = gcc
FLAGS       = -O3
CFLAGS      = -Wall -Wextra -Werror -O3 -g
#SDL_FLAGS   = `sdl-config --cflags --libs`		# For windows and linux
SDL_FLAGS   = -l SDL2-2.0.0               		#FOR MAC OS


all: $(OBJS)
	$(CC) $(FLAGS) -g $(OBJS) -o $(OUT) $(SDL_FLAGS)

_build/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $(^:_build=)

clean:
	rm -fr $(OBJS) $(OUT) _build/