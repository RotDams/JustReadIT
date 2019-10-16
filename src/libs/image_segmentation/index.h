#ifndef JUSTREADIT_MAIN_H
#define JUSTREADIT_MAIN_H

#include "segmentation.h"
#include "utils.h"


/* image_segmentation
 * Input:
 *      - image_path  => char* (the path of the image)
 *
 * Return:
 *      - List        => the list of paragraphs
 *
 * Description:
 *      - Do the image segmentation an return the list of paragraphs
 * */
List image_segmentation(char* image_path);

#endif //JUSTREADIT_MAIN_H
