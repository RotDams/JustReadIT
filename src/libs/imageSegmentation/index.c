#include "index.h"

void ExecAndSAve(char *input, char *output) {
    SDL_Surface *image;
    image = SDL_LoadBMP(input);
    GetParagraph(image);
    SDL_SaveBMP (image, output);
}

SDL_Surface* extract_text(char* source_location, int Seuil){
    SDL_Surface *image;
    Uint8 r, g, b;
    image = SDL_LoadBMP(source_location);
    if (image== NULL) {
        printf("Fail with loading image");
        return NULL;
    }

    for (int x = 0; x < image->w; ++x) {
        for (int y = 0; y < image-> h; ++y) {
            SDL_GetRGB(GetPixel(image, x, y), image->format, &r, &g, &b);
            if (!(r<=Seuil && g <=Seuil && b <=Seuil)){
                put_pixel(image,x,y,SDL_MapRGBA(image->format,255,255,255,255));
            }
        }
    }
    image = correct_image(image,Seuil+2);
    return GetAllText(image,Seuil);
}


//void takeAllPixelColor(SDL_Surface *image) {
//    Uint8 r, g, b;
//    for (int y = 0; y < image->h; ++y) {
//        for (int x = 0; x < image->w; ++x) {
//            printf("%i %i \n", x, y);
//            SDL_GetRGB(GetPixel(image, x, y), image->format, &r, &g, &b);
//            printf("pixel: r(%u) g(%u) b(%u) \n ", r, g, b);
//        }
//    }
//}

