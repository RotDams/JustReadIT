#include <SDL2/SDL.h>

int main() {
	SDL_Surface *image;
	image = SDL_LoadBMP("src/assets/chat.bmp");
	printf("Height: %d", image->h);
	return 0;
}
