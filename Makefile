rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SOURCE      = $(call rwildcard, src/, *.c)
OBJS        = $(SOURCE:%.c=_build/%.o)
HEADER      = $(SOURCE:.c=.h)
OUT         = output.exe
CC          = gcc
FLAGS       = -O0
CFLAGS      = -Wall -Wextra -Werror -O3 -g `pkg-config --cflags gtk+-3.0`
SDL_FLAGS   = `sdl-config --cflags --libs` `pkg-config --libs gtk+-3.0`


all: $(OBJS)
	$(CC) $(FLAGS) -g $(OBJS) -o $(OUT) $(SDL_FLAGS)

_build/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $(^:_build=)

clean:
	rm -fr $(OBJS) $(OUT) _build/ *.jpg
