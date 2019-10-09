#include "segmentation.h"

SDL_Surface* GetAllText(SDL_Surface *image,int Seuil) {

    // default values
    int left = image->w;
    int top = image->h;
    int bottom = 0;
    int right = 0;

    //Find the borders of the text in the image
    Uint8 r, g, b;
    for (int y = 0; y < image->h; ++y) {
        for (int x = 0; x < image->w; ++x) {
            SDL_GetRGB(GetPixel(image, x, y), image->format, &r, &g, &b);
            if (r <=Seuil && x < left)
                left = x;
            if (r <=Seuil  && x > right)
                right = x;
            if (r <=Seuil && y >bottom )
                bottom = y;
            if (r <=Seuil  && y < top)
                top = y;
        }
    }
    //Create a new image with the good scale
    SDL_Surface* newImage = CutImage(image, left, top, right - left, bottom - top);
    return  newImage;
}
/*
void GetParagraph(SDL_Surface *image) {

}

void GetWords(SDL_Surface *Paragraph) {

}

void GetLetters(SDL_Surface *Words) {

}*/
