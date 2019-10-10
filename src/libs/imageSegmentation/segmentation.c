#include "segmentation.h"

SDL_Surface *GetAllText(SDL_Surface *image) {

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
            if (r <= 10 && x < left)
                left = x;
            if (r <= 10 && x > right)
                right = x;
            if (r <= 10)
                bottom = y;
            if (r <= 10 && y < top)
                top = y;
        }
    }
    //Create a new image with the good scale
    SDL_Surface *newImage = CutImage(image, left, top, right - left, bottom - top);
    return newImage;
}

void GetParagraph(SDL_Surface *image) {
    // Define variable to save spaces and line height
    int paragraphJumpSpace = 0;
    printf("Is blank line: %d\n", IsBlankLine(image, 22));
    // Get spaces
    GetParagraphSpace(image, &paragraphJumpSpace);

    GetParagraphAndLine(image, paragraphJumpSpace);

    printf("paragraph space: %d\n", paragraphJumpSpace);
}

/*
void GetWords(SDL_Surface *Paragraph) {

}

void GetLetters(SDL_Surface *Words) {

}*/
