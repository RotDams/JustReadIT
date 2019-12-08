rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SOURCE      = $(call rwildcard, src/, *.c)
OBJS        = $(SOURCE:%.c=_build/%.o)
HEADER      = $(SOURCE:.c=.h)
OUT         = JustReadIT.exe
CC          = gcc
FLAGS       = -O0

CFLAGS      =  -g `pkg-config --cflags gtk+-3.0` #-Wall -Wextra -Werror -O3
SDL_FLAGS   = `sdl-config --cflags --libs` -lSDL_image -lSDL_gfx `pkg-config --libs gtk+-3.0`
CPPFLAGS= `pkg-config --cflags sdl` -MMD `pkg-config --libs gtk+-3.0`
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lSDL_gfx `pkg-config --libs gtk+-3.0`


CPPFLAGS= `pkg-config --cflags sdl` -MMD
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: $(OBJS)
	$(CC) $(FLAGS) -g $(OBJS) -o $(OUT) ${SDL_FLAGS} -lm -ldl

_build/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $(^:_build=)

clean:
	rm -fr $(OBJS) $(OUT) _build/ *.jpg
