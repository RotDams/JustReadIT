#include <SDL2/SDL.h>
#include "libs/imageSegmentation/segmentation.h"


int main() {
    SDL_Surface *image;
    SDL_Surface *image2;
    image = SDL_LoadBMP("src/assets/chat.bmp");
    image2 = GetAllText(image);
    SDL_SaveBMP (image2, "image2.bmp");
//    image = SDL_LoadBMP("src/assets/test.bmp");
//    //takeAllPixelColor(image);
//    CutImage(image,100,100,400,200);
//    takeAllPixelColor(image);
    return 0;
}

