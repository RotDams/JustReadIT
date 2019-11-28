#include <SDL/SDL.h>
#include <unistd.h>
#include "libs/image_segmentation/index.h"
#include "libs/neural_network/neuralnetwork.h"
//#include "libs/neural_network/index.h"


int main() {//int argc, char *argv[]) {

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

    size_t nb_layer = 3;
    size_t nb_input = 3;
    size_t nb_output = 3;
    size_t hidden = (nb_input*nb_input) / (3*nb_input*nb_output) +1 ;
    size_t nb_neurons_per_layer[] = {nb_input,10 , nb_output};
    NeuralNetwork n;
    //init(&n, nb_layer, nb_neurons_per_layer);

    //print_info(&n);
    //  print_info(&n);
//
//    print_info(&n);
//    run(&n,k,10);

    double entry[3];
    double expected[3];

    for (int j = 0; j <3 ; j++) {
        entry[j]=0;
    }
    for (int j = 0; j <3 ; j++) {
        expected[j]=0;
    }


//    List l =(List) n.layers->first->next->value;
//    Neuron* ne = (Neuron*) l->first->value;
//    *(double*)ne->links->first->value = 0.5;
//    *(double*)ne->links->first->next->value = 1.5;
//    ne = (Neuron*) l->first->next->value;
//    *(double*)ne->links->first->value = -1;
//    *(double*)ne->links->first->next->value = -2;
//
//     l =(List) n.layers->first->next->next->value;
//     ne = (Neuron*) l->first->value;
//    *(double*)ne->links->first->value = 1;
//    *(double*)ne->links->first->next->value = 3;
//    ne = (Neuron*) l->first->next->value;
//    *(double*)ne->links->first->value = -1;
//    *(double*)ne->links->first->next->value = -4;
//
//    l =(List) n.layers->first->next->next->next->value;
//    ne = (Neuron*) l->first->value;
//    *(double*)ne->links->first->value = 1;
//    *(double*)ne->links->first->next->value = -3;
//    printf("\n=====================================\n\n");
    load_neural_network(&n);
    for (int i = 0; i < 10000; i++) {
        entry[0] = random() %2;
        entry[1] = random() %2;
        printf("\n\n\n\n\n\n");
        printf("%f\n",entry[0]);
        printf("%f\n",entry[1]);
        printf("Expected : %d\n", entry[0] != entry[1]);
        printf("Expected : %d\n", entry[0] == entry[1]);
        expected[0] =  entry[0] != entry[1];
        expected[1] =  entry[0] == entry[1];
        learn(&n, entry, expected);
        printf("\n");

    }
    save_neural_network(&n);
//
//    return 0;

}
