#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "neuralnetwork.h"

void xor_neural_network(int nb_iteration, int must_load, int must_save) {
	NeuralNetwork *pointer = NULL, network;

	if (must_load)
		network = load_neural_network();
	else
		network = init();
	pointer = &network;

	int entry[2];
	entry[0] = 1;
	entry[1] = 0;

	printf("\n=====================================\n\n");
	for (int i = 0; i < nb_iteration; i++) {
		entry[0] = rand() % 2;
		entry[1] = rand() % 2;
		printf("Expected : %d\n", entry[0] != entry[1]);
		learn(pointer, entry, entry[0] != entry[1]);
		printf("\n");
	}

	entry[0] = 1;
	entry[1] = 0;
	printf("\n");
	run(pointer, entry, 2);

	if (must_save)
		save_neural_network(pointer);

	print_info(pointer);
}
