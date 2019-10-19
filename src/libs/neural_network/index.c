#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "neuralnetwork.h"

void xor_neural_network(int nb_iteration, int must_load, int must_save) {
    NeuralNetwork *pointer = NULL, network;

    if (must_load)
        network = Load_Neural_Network();
    else
        network = Init();
    pointer = &network;

    int entry[2];
    entry[0] = 1;
    entry[1] = 0;

    printf("\n=====================================\n\n");
    for (int i = 0; i < nb_iteration; i++) {
        entry[0] = rand() % 2;
        entry[1] = rand() % 2;
        printf("Expected : %d\n", entry[0] != entry[1]);
        Learn(pointer, entry, entry[0] != entry[1]);
        printf("\n");
    }

    entry[0] = 1;
    entry[1] = 0;
    printf("\n");
    Run(pointer, entry, 2);

    if (must_save)
        Save_Neural_Network(pointer);

    Print_Info(pointer);
}