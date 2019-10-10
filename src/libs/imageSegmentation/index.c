#include "index.h"

void ExecAndSAve(char *input, char *output) {
    SDL_Surface *image;
    image = SDL_LoadBMP(input);
    GetParagraph(image);
    SDL_SaveBMP (image, output);
}