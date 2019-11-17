#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include "../list_manipulation/index.h"

// ================================
// Type definition
// ================================

/* Neuron
 * Definition of a neuron
 * */
typedef struct Neuron {
    double value;
    double bias;
    List links;
} Neuron;

/* Layers
 * Definition of layer
 * */
/*typedef struct Layer {
	List neurons;
} Layers;*/

/* NeuralNetwork
 * Definition of a neural network
 * */
typedef struct NeuralNetwork {
    List layers;
} NeuralNetwork;

// ================================
// Functions prototypes
// ================================

/* sigmoide
 * Input:
 *      - double => value to compute with sigmoide function
 *
 * Return:
 *      - double => sigmoide(input)
 *
 * Description:
 *      - Return the result of the function sigmoide applicate to input
 * */
double sigmmoide(double value);


/* derivative
 * Input:
 *      - double => value to compute
 *
 * Return:
 *      - double => value*(1-value)
 *
 * Description:
 *      - Return the result of the function applicate to input
 * */
double derivative(double value);


/* Init
 * Input:
 *      - nb_layer 			   => number of layers wanted in the neural network
 *		- nb_neurons_per_layer => array containing the number of neuron
 *			for each layer
 *
 * Return:
 *      - NeuralNetwork => new neural network from scratch
 *
 * Description:
 *      - Return a new neural network from scratch with default value
 * */
void init(NeuralNetwork *net, size_t nb_layer, size_t nb_neurons_per_layer[]);


/* print_info
 * Input:
 *      - *network => pointer to a neural network
 *
 * Description:
 *      - Write all neural network information to the console
 * */
void print_info(NeuralNetwork *network);


/* propagation
 * Input:
 *      - *network => pointer to a neural network
 *      - entry    => entry values of the neural network
 *      - len      => size of the entry array
 *
 * Description:
 *      - Compute the output of the neural network with entry values
 * */
void propagation(NeuralNetwork *network, double entry[], size_t len);


/* backpropagation
 * Input:
 *      - *network => pointer to a neural network
 *      - expected => output expected from neural network
 *
 * Description:
 *      - Compute all new weights
 * */
void backpropagation(NeuralNetwork *network, int expected);


/* learn
 * Input:
 *      - *network => pointer to a neural network
 *      - entry[]  => entry values of the neural network
 *      - expected => expected output after propagation
 *
 * Description:
 *      - Function that combine propagation and backpropagation
 * */
void learn(NeuralNetwork *network, int entry[], int expected);


/* run
 * Input:
 *      - *network => pointer to a neural network
 *      - entry[]  => entry values of the neural network
 *      - len => size of the entry array
 *
 * Return:
 *      - double => the output of the neural network
 *
 * Description:
 *      - Display in the console the result of the neural network
 *          and returns this value
 * */
double run(NeuralNetwork *network, int entry[], size_t len);


/* save_neural_network
 * Input:
 *      - *network => pointer to a neural network
 *
 * Description:
 *      - Save all neural network information into save_network.txt
 * */
void save_neural_network(NeuralNetwork *network);


/* load_link
 * Input:
 *      - *file => pointer to a file
 *
 * Return:
 *      - double => neuron create with information from save_network.txt
 *
 * Description:
 *      - Read in save_network.txt information about one link from 
 *			one neuron and return a neuron instanciated with this information
 * */
double load_link(FILE *file);


/* load_neuron
 * Input:
 *      - *file 			   => pointer to a file
 *		- num_layer 		   => number of the current loading layer
 *		- nb_neurons_per_layer => array of number of neurons per layer
 *
 * Return:
 *      - Neuron => neuron create with information from save_network.txt
 *
 * Description:
 *      - Read in save_network.txt information about one neuron
 *          and return a neuron instanciated with this information
 * */
Neuron load_neuron(FILE *file, size_t num_layer, size_t nb_neurons_per_layer[]);

List load_layer(FILE *file, int num_layer, size_t nb_neurons_per_layer[]);

/* load_neural_network
 * Input:
 *      - nb_layer 			   => number of layers in the neural network
 *		- nb_neurons_per_layer => array of number of neurons per layer
 *
 * Return:
 *      - NeuralNetwork => neural network create with information
 *          from save_network.txt
 *
 * Description:
 *      - Read in save_network.txt all information about neurons
 *          thanks to load_neuron function and return a neural network
 *          instanciated
 * */
NeuralNetwork load_neural_network(size_t nb_layer, size_t nb_neurons_per_layer[]);


#endif
