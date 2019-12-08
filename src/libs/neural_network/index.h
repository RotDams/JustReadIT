#ifndef JUSTREADIT_XOR_INDEX_H
#define JUSTREADIT_XOR_INDEX_H

#include "SDL/SDL.h"
/* xor_neural_network
 * Input:
 *      - nb_iteration: int (the number of iteration)
 *      - must_load:    int (if we load data)
 *      - must_save:    int (if we save data at the end of the iterations)
 *      - must_save:    int (if we save data at the end of the iterations)
 *
 * - Description:
 *      - Do the xor function for x iteration in the neural network
 * */
void training(size_t len , size_t nb_layer,size_t hidden,int load, int save);

char get_letter_by_image(SDL_Surface *image);
#endif //JUSTREADIT_XOR_INDEX_H
