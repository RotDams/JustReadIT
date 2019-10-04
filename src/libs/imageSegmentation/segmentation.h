#ifndef JUSTREADIT_SEGMENTATION_H
#define JUSTREADIT_SEGMENTATION_H

#include "utils.h"

/* GetParagraph
 * Input:
 *      - *image  => SLD_SURFACE
 *
 * Description:
 *      - Return a list of paragraph
 * */
void GetParagraph(SDL_Surface *image);

void GetWords(SDL_Surface *Paragraph);

void GetLetters(SDL_Surface *Words);

#endif
