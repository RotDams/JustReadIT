#ifndef JUSTREADIT_SEGMENTATION_H
#define JUSTREADIT_SEGMENTATION_H

#include "utils.h"
#include "paragraph/paragraph_manipulation.h"


/* extract_text
 * Input:
 *      - *source_location  => "string" location of the source file
 *      - threshold         => threshold of tolerance
 *
 * Return:
 *      *image  => SLD_SURFACE
 *
 * Description:
 *      - correct the picture, remove all images and just keep the text, and then scale the image
 * */
SDL_Surface *extract_text(char *source_location, int threshold);


/* get_all_text
 * Input:
 *      - *image  => SLD_SURFACE
 *
 * Description:
 *      - Return a a new scale image with the text in border
 * */
SDL_Surface *get_all_text(SDL_Surface *image, int Seuil);

#endif
