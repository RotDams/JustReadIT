#include <SDL/SDL.h>

#ifndef JUSTREADIT_UTILS_H
#define JUSTREADIT_UTILS_H

/* GetPixel
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
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);

/* GetPixel
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
Uint32 GetPixel(SDL_Surface *surface, int x, int y);


/* CutImage
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
SDL_Surface* CutImage(SDL_Surface *Source, int startX, int startY, int W, int H);

/* CutImage
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

/* CutImage
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
SDL_Surface* correct_image(SDL_Surface *image,int Seuil);

#endif
