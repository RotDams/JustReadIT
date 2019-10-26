#include <SDL/SDL.h>

#ifndef JUSTREADIT_UTILS_H
#define JUSTREADIT_UTILS_H

/* get_pixel
 * Input:
 *      - *surface  => SLD_SURFACE
 *      - x         => position x of the pixel
 *      - y         => position y of the pixel
 *
 * Return:
 *      - Return the uint8* format of the pixel
 *
 * Description:
 *      - Return a the ref of the pixel of an SDL_SURFACE
 * */
Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);

/* get_pixel
 * Input:
 *      - *surface  => SLD_SURFACE
 *      - x         => position x of the pixel
 *      - y         => position y of the pixel
 *
 * Return:
 *      - Return the uint32 format of the pixel
 *
 * Description:
 *      - Return a pixel (with Uint32 format) of an SDL_SURFACE
 * */
Uint32 get_pixel(SDL_Surface *surface, int x, int y);


/* cut_image
 * Input:
 *      - *Source   => SDL_SURFACE (source image)
 *      - startX    => int (start x index)
 *      - startY    => int (start y index)
 *      - W         => int (the weight of the new image)
 *      - H         => int (the height of the new image)
 *
 * Return:
 *      - Pointer on the new image
 *
 * Description:
 *      - Return a new image, which is a part of an other image
 * */
SDL_Surface *cut_image(SDL_Surface *Source, int startX, int startY, int W, int H);


/* is_blank_line
 * Input:
 *      - *image    => SDL_SURFACE (image source)
 *      - height    => int (the line height)
 *
 * Return:
 *      - int (0 or 1)
 *
 * Description:
 *      - Check if an image contain a blank line in the height line, return 1 if true, else 0
 * */
int is_blank_line(SDL_Surface *image, int height);


/* is_blank_line
 * Input:
 *      - *image    => SDL_SURFACE (image source)
 *      - width    => int (the line width)
 *
 * Return:
 *      - int (0 or 1)
 *
 * Description:
 *      - Check if an image contain a blank column in the width line, return 1 if true, else 0
 * */
int is_blank_column(SDL_Surface *image, int width);


/* cut_image
 * Input:
 *      - *surface  => SDL_SURFACE (source image)
 *      - x         => int (start x index)
 *      - y         => int (start y index)
 *      - pixel     => Uint32 used to set the new color
 *
 * Return:
 *      void
 *
 * Description:
 *      -put new pixels colors on the image
 * */
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

/* cut_image
 * Input:
 *      - *image        => SDL_SURFACE (source image)
 *      - Seuil         => threshold of tolerance
 *
 * Return:
 *      The image with correction
 *
 * Description:
 *      - Is use to correct the black pixels that are only one on the image
 * */
SDL_Surface *correct_image(SDL_Surface *image, int Seuil);


/* show_image
 * Input:
 *      - *image        => SDL_SURFACE (source image)
 *      - id            => size_t (the id for the type of element to show)
 *
 * Description:
 *      - If dev mode is set to true, show image
 * */
void show_image(SDL_Surface *image, size_t id);


/* wait_for_keypressed
 *
 * Description:
 *      - wait for a key to be pressed
 * */
void wait_for_keypressed();



/* display_image
 *
 * Input:
 *      - *image        => SDL_SURFACE (source image)
 *
 * Description:
 *      - show an image in the screen
 * */
SDL_Surface *display_image(SDL_Surface *img);

#endif
