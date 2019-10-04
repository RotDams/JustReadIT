#include <SDL2/SDL.h>

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
    return *(Uint32 *)p;
}

SDL_Surface CutImage(SDL_Surface *Source,int startX,int startY,int W,int H){
    SDL_Rect decoupe, pos;
    SDL_Surface *ecran;

    //Init of the new image
    ecran = SDL_CreateRGBSurface(0,W,H,32,
                                 0xff000000,0x00ff0000,
                                 0x0000ff00,0x000000ff);

    //init of the start of the édition
    pos.x = 0;
    pos.y = 0;

    decoupe.x = startX; // X start
    decoupe.y = startY; // Y start
    decoupe.w = W;  // width
    decoupe.h = H;  // height

    SDL_BlitSurface(Source, &decoupe, ecran, &pos);
    SDL_SaveBMP(ecran, "ImageTest.bmp");//TODO Debug mode

    return *ecran;
}

void takeAllPixelColor(SDL_Surface *image) {
    Uint8 r, g, b;
    for (int y = 0; y < image->h; ++y) {
        for (int x = 0; x < image->w; ++x) {
            printf("%i %i \n", x, y);
            SDL_GetRGB(getpixel(image, x, y), image->format, &r, &g, &b);
            printf("pixel: r(%u) g(%u) b(%u) \n ", r, g, b);
        }
    }
}

//void TakeParagraphe(SDL_Surface *image) {}

//oid TakeWords(SDL_Surface *Paragraphe) {}

//void TakeLetters(SDL_Surface *Words) {}


int main() {
    SDL_Surface *image;
    image = SDL_LoadBMP("src/assets/chat.bmp");
    //image = SDL_LoadBMP("src/assets/test.bmp");
    //takeAllPixelColor(image);
    CutImage(image,100,100,400,200);
    takeAllPixelColor(image);
    return 0;
}

