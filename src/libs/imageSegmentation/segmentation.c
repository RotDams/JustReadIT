#include "segmentation.h"
#include "../list_manipulation/index.h"

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
    SDL_Surface *newImage = CutImage(image, left, top, right - left + 1, bottom - top + 1);
    return newImage;
}

List GetParagraph(SDL_Surface *image) {
    // Get the space between 2 paragraphs
    int paragraphJumpSpace = GetParagraphSpace(image);

    // Get a list of paragraphs (a paragraphs is a list of lines)
    List paragraphs = GetParagraphAndLine(image, paragraphJumpSpace);

    return paragraphs;
}

/*
void GetWords(SDL_Surface *Paragraph) {

}

void GetLetters(SDL_Surface *Words) {

}*/
