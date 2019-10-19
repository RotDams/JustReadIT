#ifndef JUSTREADIT_PARAGRAPH_MANIPULATION_H
#define JUSTREADIT_PARAGRAPH_MANIPULATION_H

#include "../../list_manipulation/index.h"


/* get_paragraph_space
 * Input:
 *      - *image                => SLD_SURFACE (the image)
 *      - *lineJumpSpace        => int (the space between 2 line)
 *      - *paragraphJumpSpace   => int (the space between 2 paragraph)
 *
 * Description:
 *      - Return a a new scale image with the text in border
 * */
int get_paragraph_space(SDL_Surface *image);


/* get_paragraph_space
 * Input:
 *      - *image                => SLD_SURFACE (the image)
 *      - *lineJumpSpace        => int (the space between 2 line)
 *      - *paragraphJumpSpace   => int (the space between 2 paragraph)
 *
 * Description:
 *      - Return a a new scale image with the text in border
 * */
List get_paragraphs_and_lines(SDL_Surface *image, int paragraphJumpSpace);


/* get_paragraphs
 * Input:
 *      - *image  => SLD_SURFACE
 *
 * Return:
 *      - List (The list of paragraphs)
 * Description:
 *      - Return a list of paragraph
 * */
List get_paragraphs(SDL_Surface *image);


#endif //JUSTREADIT_PARAGRAPH_MANIPULATION_H
