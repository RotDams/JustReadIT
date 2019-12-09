#include "libs/neural_network/index.h"
#include "libs/image_segmentation/rotation.h"
#include "libs/interface/interface.h"
#include "libs/neural_network/neuralnetwork.h"
#include "libs/dictionary_correction/dictionary.h"

NeuralNetwork net;
NeuralNetwork *n = &net;
Bool save_letter = false;

int nb_fonts = 22;
int nb_results = 74;

int main(int argc, char *argv[]) {


    load_dictionary("dictionary.txt");

    printf("%i\n",check_in_dictionary("game"));
    printf("%i\n",check_in_dictionary(""));

    return 0;

    load_neural_network(n);

    if (argc == 1) {

        // There is no args
        init_interface(argc, argv);

    } else {
        int i = 1;
        int load = 0;
        int save = 0;
        int train = 0;

        while (i < argc) {
            if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--train") == 0) {
                train = 1;

            } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--load") == 0) {
                load = 1;

            } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--save") == 0) {
                save = 1;
            } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fonts") == 0) {
                i++;
                if (i >= argc) {
                    printf("Error: Please specify the number of fonts \nEx: ./JustReadIT.exe -t -f 5\n");
                    return EXIT_FAILURE;
                }
                int nb = strtol(argv[i], (char **) NULL, 10);

                if (nb > nb_fonts) {
                    printf("Error: There is not enough fonts (⌣_⌣”)");
                    return EXIT_FAILURE;
                } else {
                    nb_fonts = nb;
                }
            }

            i++;
        }

        if (train)
            training(74, 4, 100, load, save);
        else
            printf("Error: Please specify the some arguments:\n -t to do the training\n -l to load the ocr file\n -s to save the ocr\n");
    }

    return 0;
}
