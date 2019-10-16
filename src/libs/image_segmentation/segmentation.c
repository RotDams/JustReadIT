#include "segmentation.h"


SDL_Surface *extract_text(char *source_location, int threshold) {
    SDL_Surface *image;
    Uint8 r, g, b;

    image = SDL_LoadBMP(source_location);

    if (image == NULL) {
        printf("Fail with loading image");
        return NULL;
    }

    for (int x = 0; x < image->w; ++x) {

        for (int y = 0; y < image->h; ++y) {

            SDL_GetRGB(get_pixel(image, x, y), image->format, &r, &g, &b);

            if (!(r <= threshold && g <= threshold && b <= threshold)) {
                put_pixel(image, x, y, SDL_MapRGBA(image->format, 255, 255, 255, 255));
            }
        }
    }
    image = correct_image(image, threshold + 2);

    return get_all_text(image, threshold);
}

SDL_Surface* get_all_text(SDL_Surface *image,int Seuil) {
    // default values
    int left = image->w;
    int top = image->h;
    int bottom = 0;
    int right = 0;

    //Find the borders of the text in the image
    Uint8 r, g, b;

    for (int y = 0; y < image->h; ++y) {

        for (int x = 0; x < image->w; ++x) {
            SDL_GetRGB(get_pixel(image, x, y), image->format, &r, &g, &b);

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
    SDL_Surface *newImage = cut_image(image, left, top, right - left + 1, bottom - top + 1);
    return newImage;
}

