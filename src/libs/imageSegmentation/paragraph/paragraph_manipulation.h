#ifndef JUSTREADIT_PARAGRAPH_MANIPULATION_H
#define JUSTREADIT_PARAGRAPH_MANIPULATION_H

#include "../../list_manipulation/index.h"

/* GetParagraphSpace
 * Input:
 *      - *image                => SLD_SURFACE (the image)
 *      - *lineJumpSpace        => int (the space between 2 line)
 *      - *paragraphJumpSpace   => int (the space between 2 paragraph)
 *
 * Description:
 *      - Return a a new scale image with the text in border
 * */
int GetParagraphSpace(SDL_Surface *image);

/* GetParagraphSpace
 * Input:
 *      - *image                => SLD_SURFACE (the image)
 *      - *lineJumpSpace        => int (the space between 2 line)
 *      - *paragraphJumpSpace   => int (the space between 2 paragraph)
 *
 * Description:
 *      - Return a a new scale image with the text in border
 * */
List GetParagraphAndLine(SDL_Surface *image, int paragraphJumpSpace);

#endif //JUSTREADIT_PARAGRAPH_MANIPULATION_H
