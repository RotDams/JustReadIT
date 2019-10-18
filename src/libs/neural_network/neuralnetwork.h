#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

typedef struct Neuron
{
	double value;
	double bias;
	double link[2];
	size_t nb_link;
} Neuron;

typedef struct NeuralNetwork
{
    float error;
    Neuron neurons[5];
    size_t nb_neurons;
} NeuralNetwork;

double sigmmoide(double value);

struct NeuralNetwork Init();

void Print_Info(NeuralNetwork *network);

void Propagation(NeuralNetwork *network, int entry[],size_t len);

void Backpropagation(NeuralNetwork *network, int expected);

void Learn(NeuralNetwork *network, int entry[], int expected);

double Run(NeuralNetwork *network, int entry[], size_t len);

void Save_Neural_Network(NeuralNetwork *network);

Neuron Load_Neuron(FILE *file);

NeuralNetwork Load_Neural_Network();

#endif
