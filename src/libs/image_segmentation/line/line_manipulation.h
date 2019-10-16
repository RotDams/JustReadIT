#ifndef JUSTREADIT_LINE_MANIPULATION_H
#define JUSTREADIT_LINE_MANIPULATION_H
#include <SDL/SDL.h>
#include "../../list_manipulation/index.h"


/* get_words_space
 * Input:
 *      - *image        => SLD_SURFACE (the image)
 *
 * Description:
 *      - Return the space between 2 words
 * */
int get_words_space(SDL_Surface *image);


/* get_letters
 * Input:
 *      - *image        => SLD_SURFACE (the image)
 *
 * Description:
 *      - Return a list of letters, issues from a letter
 * */
List get_letters(SDL_Surface *image);


/* get_words_and_letters
 * Input:
 *      - *image        => SLD_SURFACE (the image)
 *
 * Description:
 *      - Return a list of list of letters (issues from line)
 * */
List get_words_and_letters(SDL_Surface *image);

#endif //JUSTREADIT_LINE_MANIPULATION_H