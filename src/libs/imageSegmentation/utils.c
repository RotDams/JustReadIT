#include "utils.h"


Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 GetPixel(SDL_Surface *surface,int x,int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = pixel_ref(surface,x,y);
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
    int imageWidth = image->w;

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

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

SDL_Surface* correct_image(SDL_Surface *image,int Seuil) {
    Uint8 r, g, b;
    int bool;
    for (int x = 2; x < image->w-2; ++x) {
        for (int y = 2; y < image-> h-2; ++y) {
            SDL_GetRGB(GetPixel(image, x, y), image->format, &r, &g, &b);
            if ((r<=Seuil && g <=Seuil && b <=Seuil)){
                bool=0;
                for (int i = x-2; i <= x+2 ; ++i) {
                    for (int j = y-2; j <= y+2; ++j) {
                        SDL_GetRGB(GetPixel(image, i, j), image->format, &r, &g, &b);
                        if ((r<=Seuil && g <=Seuil && b <=Seuil)){
                            bool+=1;
                        }
                    }
                }
                if (bool<5)
                    put_pixel(image,x,y,SDL_MapRGBA(image->format,255,255,255,255));
            }
        }
    }
    return image;
}