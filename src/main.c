#include <SDL/SDL.h>
#include <unistd.h>
#include "libs/image_segmentation/index.h"
#include "libs/neural_network/neuralnetwork.h"
#include "libs/neural_network/index.h"
//#include "libs/neural_network/index.h"


int main() {//int argc, char *argv[]) {


    int nb_output = 26;
    char* paths[] = {
            "src/assets/training/A.bmp",
            "src/assets/training/B.bmp",
            "src/assets/training/C.bmp",
            "src/assets/training/D.bmp",
            "src/assets/training/E.bmp",
            "src/assets/training/F.bmp",
            "src/assets/training/G.bmp",
            "src/assets/training/H.bmp",
            "src/assets/training/I.bmp",
            "src/assets/training/J.bmp",
            "src/assets/training/K.bmp",
            "src/assets/training/L.bmp",
            "src/assets/training/M.bmp",
            "src/assets/training/N.bmp",
            "src/assets/training/O.bmp",
            "src/assets/training/P.bmp",
            "src/assets/training/Q.bmp",
            "src/assets/training/R.bmp",
            "src/assets/training/S.bmp",
            "src/assets/training/T.bmp",
            "src/assets/training/U.bmp",
            "src/assets/training/V.bmp",
            "src/assets/training/W.bmp",
            "src/assets/training/X.bmp",
            "src/assets/training/Y.bmp",
            "src/assets/training/Z.bmp",
    };

    training(paths,nb_output,4,100);
    /*if (argc == 1) {
        printf("Error: Please specify the path of the image\nEx: ./output.exe -p \"my_image.bmp\"\n");
        return EXIT_FAILURE;
    }
    // List of all possible arguments
    char *path = "";
    int is_xor = 0;
    int xor_load_data = 0;
    int xor_save_data = 0;
    int xor_nb_iteration = 1000000;

    // for each arg
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--xor") == 0) {
            is_xor = 1;

        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--load") == 0) {
            xor_load_data = 1;

        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--save") == 0) {
            xor_save_data = 1;

        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--path") == 0) {
            i++;
            if (i >= argc) {
                printf("Error: Please specify the path of the image\nEx: ./output.exe -p \"my_image.bmp\"\n");
                return EXIT_FAILURE;
            }
            path = argv[i];

        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iteration") == 0) {
            i++;
            if (i >= argc) {
                printf("Error: Please specify the number of iteration \nEx: ./output.exe -x -l -s -i 50\n");
                return EXIT_FAILURE;
            }
            xor_nb_iteration = strtol(argv[i], (char **) NULL, 10);
        }
        i++;
    }

    if (!is_xor) {
        if (access(path, F_OK) == -1) {
            printf("Error: The file does not exist.\n");
            return EXIT_FAILURE;
        }

        List paragraphs = image_segmentation(path);
        print_list(paragraphs);
    } else {
        xor_neural_network(xor_nb_iteration, xor_load_data, xor_save_data);
    }*/
//    size_t nb_layer = 3;
//    size_t nb_neurons_per_layer[] = {2, 2, 2};
//    NeuralNetwork n;
//    init(&n, nb_layer, nb_neurons_per_layer);
    //  print_info(&n);
//    double k[] = {1, 0};
//
//    print_info(&n);
//    run(&n,k,10);
//    print_info(&n);
//    propagation(&n, k, 2);
//    print_info(&n);
//    size_t d[]= {0, 1};
//    backpropagation(&n,d);
//    save_neural_network(&n);
//    for (int i = 0; i <100000 ; i++) {
//        learn(&n,k,10,d);
//    }
//    print_info(&n);
    // print_info(&n);

//
//    print_info(&n);
//    save_neural_network(&n);
//    NeuralNetwork s;
//    load_neural_network(&s);
//    print_info(&s);

}
