#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "neuralnetwork.h"
#include <SDL/SDL.h>
#include "../image_segmentation/index.h"
#include "math.h"

double *get_matrix(SDL_Surface *image) {
    // Create the new array
    int length = 32;
    double *array = calloc(32 * 32, sizeof(double));

    // convert the image into matrix
    Uint8 r, g, b;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (i >= image->h || j >= image->h) {
                array[i * length + j] = 0;
            } else {
                Uint32 pixel = (get_pixel(image, i, j));
                SDL_GetRGB(pixel, image->format, &r, &b, &g);

                // if the pixel is black ->1 else -> 0
                if (r < 15 && g < 15 && b < 15)
                    array[i * length + j] = 1;
                else
                    array[i * length + j] = 0;
            }
        }
    }
    return array;
}

void training( size_t len, size_t nb_layer, size_t hidden,char* result_elements, int load) {

    // All paths for tests images
    char *path[] = {
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

    // Save all images into an array
    double **models = malloc(sizeof(char *) * len);
    for (size_t m = 0; m < len; m++) {
        SDL_Surface *image = SDL_LoadBMP(path[m]);
        models[m] = get_matrix(image);
    }

    // Array of 0
    double *expected = calloc(len, sizeof(double));

    // Load or init the NeuralNetwork
    NeuralNetwork network;
    NeuralNetwork *n = &network;
    if (load)
        load_neural_network(n);
    else{
        size_t nb_input = 32*32;
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
            k = random() % len;

            // Expected response
            expected[k] = (double) 1;

            // Update all errors and return result
            result = learn(n, models[k], expected);

            // Reset expected to 0
            expected[k] = (double) 0;
        }
        // Update all links with the errors
        backpropagation(n, coef);

        // Print th result of the last test
        printf("(%zu) Expected : %c  ", i, result_elements[k]);
        printf("result : %c \n", result_elements[result]);

        // All 1000 call, save th network into a file
        if (i % 1000 == 0)
            save_neural_network(n);
    }
    save_neural_network(n);
}