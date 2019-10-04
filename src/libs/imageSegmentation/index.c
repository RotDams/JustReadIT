#include "index.h"

void ExecAndSAve(char* nameFile){
    SDL_Surface *image;
    SDL_Surface *image2;
    image = SDL_LoadBMP("src/assets/test-image-0.bmp");
    image2 = GetAllText(image);
    SDL_SaveBMP (image2, nameFile);
}



//void takeAllPixelColor(SDL_Surface *image) {
//    Uint8 r, g, b;
//    for (int y = 0; y < image->h; ++y) {
////        for (int x = 0; x < image->w; ++x) {
////            printf("%i %i \n", x, y);
////            SDL_GetRGB(GetPixel(image, x, y), image->format, &r, &g, &b);
////            printf("pixel: r(%u) g(%u) b(%u) \n ", r, g, b);
////        }
////    }
//}