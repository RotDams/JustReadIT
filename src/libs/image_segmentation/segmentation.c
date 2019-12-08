#include <SDL/SDL_image.h>
#include "segmentation.h"
#include "utils.h"
#include "rotation.h"


SDL_Surface *extract_text(char *source_location, int threshold) {
    SDL_Surface *image;
    Uint8 r, g, b;

    // Get the image
    image = IMG_Load(source_location);

    show_image(image, -1);

    put_in_black_and_white(image);
    show_image(image, -1);

    extern int must_rotate_image;
    if (must_rotate_image) {
        double angle = find_angle(image);
        image = SDL_RotateImage(image, -angle);
        show_image(image, -1);
    }

    if (image == NULL) {
        printf("Fail with loading image");
        return NULL;
    }

    // Browse all pixels of the image
    for (int x = 0; x < image->w; ++x) {
        for (int y = 0; y < image->h; ++y) {

            // Take the color of the current pixel
            SDL_GetRGB(get_pixel(image, x, y), image->format, &r, &g, &b);
             // If the current pixel is not black -> convert white
            if (!(r <= threshold && g <= threshold && b <= threshold)) {
                put_pixel(image, x, y, SDL_MapRGBA(image->format, 255, 255, 255, 255));
            }
        }
    }
    // Corrects the finishes of pixels
    extern int must_remove_bg;

    if (must_remove_bg)
        image = correct_image(image, 20);
    SDL_SaveBMP(image, "kdo.bmp");
    show_image(image, -1);
    return get_all_text(image, threshold);
}


SDL_Surface *get_all_text(SDL_Surface *image, int Seuil) {
    // default values
    int left = image->w;
    int top = image->h;
    int bottom = 0;
    int right = 0;

    //Find the borders of the text in the image
    Uint8 r, g, b;


    // Browse all pixels of the image
    for (int y = 0; y < image->h; ++y) {
        for (int x = 0; x < image->w; ++x) {

            // get the current pixel color
            SDL_GetRGB(get_pixel(image, x, y), image->format, &r, &g, &b);

            // take the border of the text
            if (r <= Seuil && x < left)
                left = x;

            if (r <= Seuil && x > right)
                right = x;

            if (r <= Seuil && y > bottom)
                bottom = y;

            if (r <= Seuil && y < top)
                top = y;
        }
    }

    //Create a new image with the good scale
    SDL_Surface *newImage = cut_image(image, left, top, right - left + 1, bottom - top + 1);
    show_image(newImage, -1);
    return newImage;
}




