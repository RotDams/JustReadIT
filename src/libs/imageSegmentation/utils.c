#include "utils.h"


Uint32 GetPixel(SDL_Surface *surface,int x,int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}


SDL_Surface* CutImage(SDL_Surface *Source, int startX, int startY, int W, int H) {
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

    return screen;
}

int IsBlankLine(SDL_Surface *image, int height) {
    int imageHeight = image->h;
    int imageWidth  = image->w;

    // If the height in parameter is superior than the size of the image
    if (height > imageHeight) {
        return 0;
    }

    // Else, check for all pixel
    Uint8 r, g, b;
    for (int i = 0; i < imageWidth; i++) {
        // Get RGB color
        SDL_GetRGB(GetPixel(image, i, height), image->format, &r, &g, &b);

        // Check if one of the RGB color is inferior than 250 (we aspect to have blank pixel)
        if (r < 250 || g < 250 || b < 250) {
            return 0;
        }
    }

    // This is a white line
    return 1;
}

int is_blank_column(SDL_Surface *image, int width) {
    int imageHeight = image->h;
    int imageWidth  = image->w;

    if (width > imageWidth) {
        return 0;
    }

    Uint8 r, g, b;
    for (int i = 0; i < imageHeight; i++) {
        SDL_GetRGB(GetPixel(image, width, i), image->format, &r, &g, &b);

        if (r < 250 || g < 250 || b < 250) {
            return 0;

        }
    }

    return 1;
}