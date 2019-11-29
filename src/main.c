#include <SDL/SDL.h>
#include <unistd.h>
#include "libs/image_segmentation/index.h"
#include "libs/neural_network/neuralnetwork.h"
//#include "libs/neural_network/index.h"

double *get_matrix(SDL_Surface *image) {
    int len = image->w * image->h;
    double *array = malloc(sizeof(double) * len);

    Uint8 r, g, b;
    for (int i = 0; i < image->h; i++) {
        for (int j = 0; j < image->w; j++) {
            Uint32 pixel = (get_pixel(image, i, j));
            SDL_GetRGB(pixel, image->format, &r, &b, &g);
            if (r < 15 && b < 15 && g < 15)
                array[i * image->w + j] = 1;
            else
                array[i * image->w + j] = 0;
        }
    }
    return array;
}

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

    SDL_Surface *l_A = SDL_LoadBMP("src/A.bmp");
    SDL_Surface *l_B = SDL_LoadBMP("src/B.bmp");
    SDL_Surface *l_C = SDL_LoadBMP("src/C.bmp");
    SDL_Surface *l_D = SDL_LoadBMP("src/D.bmp");
    SDL_Surface *l_E = SDL_LoadBMP("src/E.bmp");

    size_t nb_layer = 4;
    size_t nb_input = l_A->h * l_A->w;
    size_t nb_output = 5;

    size_t hidden1 = 100;
    size_t nb_neurons_per_layer[] = {nb_input, hidden1, hidden1, nb_output};
    NeuralNetwork n;
    //init(&n, nb_layer, nb_neurons_per_layer);
    load_neural_network(&n);

    double *entry_A = get_matrix(l_A);
    double *entry_B = get_matrix(l_B);
    double *entry_C = get_matrix(l_C);
    double *entry_D = get_matrix(l_D);
    double *entry_E = get_matrix(l_E);


    double latter_A[] = {1, 0, 0, 0, 0};
    double latter_B[] = {0, 1, 0, 0, 0};
    double latter_C[] = {0, 0, 1, 0, 0};
    double latter_D[] = {0, 0, 0, 1, 0};
    double latter_E[] = {0, 0, 0, 0, 1};

    double coef = 0.1;

    int k = 0;
    size_t result = 0;
    for (int i = 1; i < 1000000; i++) {
        for (int j = 0; j <=9 ; j++) {



            k = random() % 5;
            switch (k) {
                case 0:

                    result = learn(&n, entry_A, latter_A, coef,j==9);
                    if(j==9)printf("Expected: A\n");
                    break;
                case 1:

                    result = learn(&n, entry_B, latter_B, coef,j==9);
                    if(j==9)printf("Expected: B\n");
                    break;
                case 2:

                    result = learn(&n, entry_C, latter_C, coef ,j==9);
                    if(j==9) printf("Expected: C\n");
                    break;
                case 3:

                    result = learn(&n, entry_D, latter_D, coef,j==9);
                    if(j==9)printf("Expected: D\n");
                    break;
                default:

                    result = learn(&n, entry_E, latter_E, coef,j==9);
                   if(j==9) printf("Expected: E\n");
                    break;
            }
        }
            backpropagation(&n,coef);

        printf("result : %c \n\n\n",'A'+(char)result);



        if (i % 5000 == 0) {
            print_info(&n);
            save_neural_network(&n);
        }
    }
    printf("%c",'A'+ (char)run(&n,entry_A));
    //print_info(&n);
    save_neural_network(&n);

}
