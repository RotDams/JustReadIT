#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "neuralnetwork.h"
#include <SDL/SDL.h>
#include "../image_segmentation/index.h"
#include "math.h"

int nb_fonts = 4;
char *path[] = {
        "font-01", "font-02", "font-03", "font-04",
};

int nb_results = 74;
char result_elements[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
        'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
        'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
        'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '.', ';', ':', '!', '?',
        '\'', '(', ')', '[', ']', '-', '%'
};

double *get_matrix(SDL_Surface *image) {
    // Create the new array
    int length = 32;
    double *array = calloc(32 * 32, sizeof(double));

    // convert the image into matrix
    Uint8 r, g, b;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (i >= image->w || j >= image->h) {
                array[j * length + i] = 0;
            } else {
                Uint32 pixel = (get_pixel(image, i, j));
                SDL_GetRGB(pixel, image->format, &r, &b, &g);

                // if the pixel is black ->1 else -> 0
                if (r < 150 && g < 150 && b < 150)
                    array[j * length + i] = 1;
                else
                    array[j * length + i] = 0;
            }
        }
    }
    return array;
}

char get_letter(SDL_Surface *image) {
    // Load or init the NeuralNetwork
    NeuralNetwork network;
    NeuralNetwork *n = &network;
    load_neural_network(n);
    double *yes = get_matrix(image);
    //yes[32 * 32 - 10] = 1;
    //SDL_SaveBMP(image, "pb.jpg");
//    for (int i = 0; i < 32; i++) {
//        for (int j = 0; j < 32; j++) {
//            printf("%.0f.", yes[i * 32 + j]);
//        }
//        printf("\n");
//    }
//    printf("\n\n\n\n");
    size_t result = run(n, yes);
    return result_elements[result];
}

void training(size_t len, size_t nb_layer, size_t hidden, int load) {


    // Save all images into an array
    double **models = malloc(sizeof(char *) * nb_fonts * nb_results);

    char paths[] = "src/assets/training/font-01/letter000.bmp";

    int index_in = 0;
    for (size_t m = 0; m < nb_fonts; m++) {
        paths[36] = '0';
        paths[35] = '0';

        for (size_t m = 0; m < nb_results; m++) {


            SDL_Surface *image = SDL_LoadBMP(paths);
            models[index_in] = get_matrix(image);
            index_in++;
            if (paths[36] == '9') {
                paths[36] = '0';
                paths[35]++;
            } else
                paths[36]++;
        }

        if (paths[26] == '9') {
            paths[26] = '0';
            paths[27]++;
        } else
            paths[26]++;
    }

    // Array of 0
    double *expected = calloc(len, sizeof(double));

    // Load or init the NeuralNetwork
    NeuralNetwork network;
    NeuralNetwork *n = &network;
    if (load)
        load_neural_network(n);
    else {
        size_t nb_input = 32 * 32;
        size_t nb_output = len;
        size_t nb_neurons_per_layer[] = {nb_input, hidden, hidden, nb_output};

        init(n, nb_layer, nb_neurons_per_layer);
        save_neural_network(n);
    }

    // learning rate
    double coef = 0.1;

    // Principal learn function
    int k = 0;
    size_t result = 0;
    for (size_t i = 1; i < 100000; i++) {
        for (size_t j = 0; j <= 10; j++) {
            // Random letter
            k = random() % nb_results * nb_fonts;

            // Expected response
            expected[k % nb_results] = (double) 1;

            // Update all errors and return result
            result = learn(n, models[k % nb_results], expected);

            // Reset expected to 0
            expected[k % nb_results] = (double) 0;
        }
        // Update all links with the errors
        backpropagation(n, coef);
//
//        for (size_t l = 0; l < nb_results * nb_fonts; ++l) {
//
           // printf("(%zu)  : %c  \n",l, result_elements[l % nb_results]);
//            for (int i = 0; i < 32; i++) {
//                for (int j = 0; j < 32; j++) {
//                    printf("%.0f.", models[k][i * 32 + j]);
//                }
//                printf("\n");
//            }
//        }
//        printf("\n\n\n\n");
        // Print th result of the last test
        printf("(%zu) Expected : %c  ", i, result_elements[k % nb_results]);
        printf("result : %c \n", result_elements[result]);

        // All 1000 call, save th network into a file
        if (i % 1000 == 0)
            save_neural_network(n);
    }
    save_neural_network(n);
}

