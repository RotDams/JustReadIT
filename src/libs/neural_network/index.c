#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "neuralnetwork.h"
#include <SDL/SDL.h>
#include "../image_segmentation/index.h"

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

void training(char ** path,size_t len , size_t nb_layer,size_t hidden) {

    SDL_Surface *l =SDL_LoadBMP(path[0]);
    double ** models= malloc(sizeof(char*)*len);
    for (int m = 0; m < len; m++) {
        SDL_Surface* image = SDL_LoadBMP(path[m]);
        models[m] = get_matrix(image);
    }

    double *expected = calloc(len, sizeof(double));

    NeuralNetwork network;
    NeuralNetwork* n = &network;

    load_neural_network(n);

    size_t nb_input = l->h * l->w;
    size_t nb_output = len;
    size_t nb_neurons_per_layer[] = {nb_input, hidden, hidden, nb_output};
    //init(n, nb_layer, nb_neurons_per_layer);



    double coef = 0.1;

    int k = 0;
    size_t result = 0;
    for (size_t i = 1; i < 1000000; i++) {

        for (size_t j = 0; j <= len; j++) {


            k = random() % len;

            expected[k]=1;

            result = learn(n,models[k],expected,coef,j==len);
            if (j==len)
                printf("Expected : %c\n",(char)('A'+k));
            expected[k]=0;
        }
        backpropagation(n, coef);

        printf("result : %c \n\n\n", 'A' + (char) result);

        if (i % 5000 == 0) {
            save_neural_network(n);
        }
    }
    save_neural_network(n);

}