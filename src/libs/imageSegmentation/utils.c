#include "utils.h"

Uint32 GetPixel(SDL_Surface *surface, int x, int y) {
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * 4;
    return *(Uint32 *) p;
}


SDL_Surface CutImage(SDL_Surface *Source, int startX, int startY, int W, int H) {
    SDL_Rect cut, pos;
    SDL_Surface *screen;

    //Init of the new image
    screen = SDL_CreateRGBSurface(0, W, H, 32,
                                  0xff000000, 0x00ff0000,
                                  0x0000ff00, 0x000000ff);

    //init of the start of the edition
    pos.x = 0;
    pos.y = 0;

    cut.x = startX; // X start
    cut.y = startY; // Y start
    cut.w = W;  // width
    cut.h = H;  // height

    SDL_BlitSurface(Source, &cut, screen, &pos);

    // Save image, to allow user to see the image after manipulation
    // SDL_SaveBMP (screen, "ImageTest.bmp");      //TODO Debug mode

    return *screen;
}