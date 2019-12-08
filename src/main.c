#include "libs/neural_network/index.h"
#include "libs/image_segmentation/rotation.h"
#include "libs/interface/interface.h"
#include "libs/neural_network/neuralnetwork.h"


NeuralNetwork net;
NeuralNetwork *n = &net;
Bool save_letter = false;

int nb_fonts = 19;
int nb_results = 74;

int main(int argc, char *argv[]) {

    load_neural_network(n);

    //training(74, 4, 100, 1);

    init_interface(argc, argv);
    return 0;
}
