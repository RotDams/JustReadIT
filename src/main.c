#include <SDL/SDL.h>
#include <unistd.h>
#include "libs/image_segmentation/index.h"
#include "libs/neural_network/neuralnetwork.h"
#include "libs/neural_network/index.h"
//#include "libs/neural_network/index.h"


int main() {//int argc, char *argv[]) {


    int nb_output = 26+26;
    char *paths[] = {
            "src/assets/training/up_letter/A.bmp",
            "src/assets/training/up_letter/B.bmp",
            "src/assets/training/up_letter/C.bmp",
            "src/assets/training/up_letter/D.bmp",
            "src/assets/training/up_letter/E.bmp",
            "src/assets/training/up_letter/F.bmp",
            "src/assets/training/up_letter/G.bmp",
            "src/assets/training/up_letter/H.bmp",
            "src/assets/training/up_letter/I.bmp",
            "src/assets/training/up_letter/J.bmp",
            "src/assets/training/up_letter/K.bmp",
            "src/assets/training/up_letter/L.bmp",
            "src/assets/training/up_letter/M.bmp",
            "src/assets/training/up_letter/N.bmp",
            "src/assets/training/up_letter/O.bmp",
            "src/assets/training/up_letter/P.bmp",
            "src/assets/training/up_letter/Q.bmp",
            "src/assets/training/up_letter/R.bmp",
            "src/assets/training/up_letter/S.bmp",
            "src/assets/training/up_letter/T.bmp",
            "src/assets/training/up_letter/U.bmp",
            "src/assets/training/up_letter/V.bmp",
            "src/assets/training/up_letter/W.bmp",
            "src/assets/training/up_letter/X.bmp",
            "src/assets/training/up_letter/Y.bmp",
            "src/assets/training/up_letter/Z.bmp",
            "src/assets/training/down_letter/a.bmp",
            "src/assets/training/down_letter/b.bmp",
            "src/assets/training/down_letter/c.bmp",
            "src/assets/training/down_letter/d.bmp",
            "src/assets/training/down_letter/e.bmp",
            "src/assets/training/down_letter/f.bmp",
            "src/assets/training/down_letter/g.bmp",
            "src/assets/training/down_letter/h.bmp",
            "src/assets/training/down_letter/i.bmp",
            "src/assets/training/down_letter/j.bmp",
            "src/assets/training/down_letter/k.bmp",
            "src/assets/training/down_letter/l.bmp",
            "src/assets/training/down_letter/m.bmp",
            "src/assets/training/down_letter/n.bmp",
            "src/assets/training/down_letter/o.bmp",
            "src/assets/training/down_letter/p.bmp",
            "src/assets/training/down_letter/q.bmp",
            "src/assets/training/down_letter/r.bmp",
            "src/assets/training/down_letter/s.bmp",
            "src/assets/training/down_letter/t.bmp",
            "src/assets/training/down_letter/u.bmp",
            "src/assets/training/down_letter/v.bmp",
            "src/assets/training/down_letter/w.bmp",
            "src/assets/training/down_letter/x.bmp",
            "src/assets/training/down_letter/y.bmp",
            "src/assets/training/down_letter/z.bmp",
    };
    char result_elements[] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
            'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
            'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
            'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
            's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };

    training(paths, nb_output, 4, 100, result_elements, 1);
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
    return 0;
}
