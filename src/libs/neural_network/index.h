#ifndef JUSTREADIT_XOR_INDEX_H
#define JUSTREADIT_XOR_INDEX_H

/* xor_neural_network
 * Input:
 *      - nb_iteration: int (the number of iteration)
 *      - must_load:    int (if we load data)
 *      - must_save:    int (if we save data at the end of the iterations)
 *
 * - Description:
 *      - Do the xor function for x iteration in the neural network
 * */
void training(char ** path,size_t len , size_t nb_layer,size_t hidden,char* result_elements,int load);
#endif //JUSTREADIT_XOR_INDEX_H
