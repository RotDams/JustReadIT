#include "libs/image_segmentation/rotation.h"
#include "libs/interface/interface.h"
#include "libs/neural_network/index.h"
#include "libs/neural_network/neuralnetwork.h"

NeuralNetwork net;
NeuralNetwork* n = &net;
Bool save_letter = true;

int main(int argc, char *argv[]) {

    load_neural_network(n);

    // Check if we want the interface
    // printf("%c",get_letter(SDL_LoadBMP("src/assets/training/font-01-01/letter073.bmp")));
    //training(74,4,100,0);

    init_interface(argc, argv);
    return 0;

}
