#ifndef JUSTREADIT_MAIN_H
#define JUSTREADIT_MAIN_H

#include "segmentation.h"
#include "utils.h"

void ExecAndSAve(char *input, char *output);

/* CutImage
 * Input:
 *      - *source_location  => "string" location of the source file
 *      - Seuil             => threshold of tolerance
 *
 * Return:
 *      *image  => SLD_SURFACE
 *
 * Description:
 *      - corect the picture, remove all images and just keep the text, and then scale the image
 * */
SDL_Surface* extract_text(char* source_location, int Seuil);

#endif //JUSTREADIT_MAIN_H
