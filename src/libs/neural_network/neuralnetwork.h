#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

// ================================
// Type definition
// ================================

/* Neuron
 * Definition of a neuron
 * */
typedef struct Neuron
{
	double value;
	double bias;
	double link[2];
	size_t nb_link;
} Neuron;

/* NeuralNetwork
 * Definition of a neural network
 * */
typedef struct NeuralNetwork
{
    float error;
    Neuron neurons[5];
    size_t nb_neurons;
} NeuralNetwork;

// ================================
// Functions prototypes
// ================================

/* sigmoide
 * Input:
 *      - double => value to compute with sigmoide function
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
 * Return:
 *      - double => value*(1-value)
 *
 * Description:
 *      - Return the result of the function applicate to input
 * */
double derivative(double value);


/* Init
 * Return:
 *      - NeuralNetwork => new neural network from scratch
 *
 * Description:
 *      - Return a new neural network from scratch with default value
 * */
struct NeuralNetwork Init();


/* Print_Info
 * Input:
 *      - *network => pointer to a neural network
 *
 * Description:
 *      - Write all neural network information to the console
 * */
void Print_Info(NeuralNetwork *network);


/* Propagation
 * Input:
 *      - *network => pointer to a neural network
 *      - entry    => entry values of the neural network
 *      - len      => size of the entry array
 *
 * Description:
 *      - Compute the output of the neural network with entry values
 * */
void Propagation(NeuralNetwork *network, int entry[],size_t len);


/* Backpropagation
 * Input:
 *      - *network => pointer to a neural network
 *      - expected => output expected from neural network
 *
 * Description:
 *      - Compute all new weights
 * */
void Backpropagation(NeuralNetwork *network, int expected);


/* Learn
 * Input:
 *      - *network => pointer to a neural network
 *      - entry[]  => entry values of the neural network
 *      - expected => expected output after propagation
 *
 * Description:
 *      - Function that combine propagation and backpropagation
 * */
void Learn(NeuralNetwork *network, int entry[], int expected);


/* Run
 * Input:
 *      - *network => pointer to a neural network
 *      - entry[]  => entry values of the neural network
 *      - len => size of the entry array
 * Return:
 *      - double => the output of the neural network
 *
 * Description:
 *      - Display in the console the result of the neural network
 *          and returns this value
 * */
double Run(NeuralNetwork *network, int entry[], size_t len);


/* Save_Neural_Network
 * Input:
 *      - *network => pointer to a neural network
 *
 * Description:
 *      - Save all neural network information into save_network.txt
 * */
void Save_Neural_Network(NeuralNetwork *network);


/* Load_Neuron
 * Input:
 *      - *file => pointer to a file
 * Return:
 *      - Neuron => neuron create with information from save_network.txt
 *
 * Description:
 *      - Read in save_network.txt information about one neuron
 *          and return a neuron instanciated with this information
 * */
Neuron Load_Neuron(FILE *file);


/* Load_Neuron
 * Input:
 *      - *file => pointer to a file
 * Return:
 *      - NeuralNetwork => neural network create with information
 *          from save_network.txt
 *
 * Description:
 *      - Read in save_network.txt all information about neurons
 *          thanks to Load_Neuron function and return a neural network
 *          instanciated
 * */
NeuralNetwork Load_Neural_Network();


#endif
