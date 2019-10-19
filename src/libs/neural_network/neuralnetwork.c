#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "neuralnetwork.h"

double sigmoide(double value) {
    return 1 / (1 + exp(-value));
}

double derivative(double value) {
    return (value) * (1 - (value));
}

// Initialize the network from scratch
NeuralNetwork init() {
    Neuron n, n1, n2, n3, n4;
    n.value = 0;
    n.value = 1;
    n.bias = 0.2;
    n.link[0] = 1;
    n.link[1] = 1;
    n.nb_link = 2;
    n1 = n2 = n3 = n4 = n;
    n1.bias = 0.1;
    n2.bias = 0.7;
    n3.bias = 0.9;
    n4.bias = 0.1;

    NeuralNetwork net;
    net.error = 0.05;
    net.neurons[0] = n;
    net.neurons[1] = n1;
    net.neurons[2] = n2;
    net.neurons[3] = n3;
    net.neurons[4] = n4;
    net.nb_neurons = 5;
    return net;
}

void print_info(NeuralNetwork *network) {
    printf("=================Network Information=================\n");
    // Print all links information
    printf("Link :\n");
    printf("%f, %f\n", network->neurons[2].link[0], network->neurons[2].link[1]);
    printf("%f, %f\n", network->neurons[3].link[0], network->neurons[3].link[1]);
    printf("%f, %f\n", network->neurons[4].link[0], network->neurons[4].link[1]);

    // Print all neurons values
    printf("Values :\n");
    printf("%f, %f \n", network->neurons[0].value, network->neurons[1].value);
    printf("%f, %f \n", network->neurons[2].value, network->neurons[3].value);
    printf("%f \n", network->neurons[4].value);

    printf("=====================================================\n\n");
}

void propagation(NeuralNetwork *network, int entry[], size_t len) {
    // Check if there is enough entry or neuron
    if (len == 2) {
        // Place the entry data on the first layer
        for (int i = 0; i < 2; i++) {
            network->neurons[i].value = entry[i];
        }
        double val_neuron = 0;
        // propagation throught the neural network
        for (int j = 2; j < 5; j++) {
            val_neuron = 0;
            for (int k = 0; k < 2; k++) {
                val_neuron += network->neurons[j - 1 - k - (j % 2)].value * network->neurons[j].link[(k+1)%2];
            }
            val_neuron += network->neurons[j].bias;
            // Activation function
            network->neurons[j].value = sigmoide(val_neuron);
        }
    } else {
        printf("Error in propagation\n");
    }
}

void backpropagation(NeuralNetwork *network, int expected)
{
	// Compute weight to output layer
	// Compute new weight between neuron 2 and 4
	double derivative_error = -(expected-network->neurons[4].value);
	double derivative_output = derivative(network->neurons[4].value);
	double value_previous_neuron = network->neurons[2].value;

	double derivative_weight = derivative_error*derivative_output*value_previous_neuron;

	double new_weight = network->neurons[4].link[0] - (0.5*derivative_weight);
	network->neurons[4].link[0] = new_weight;

	// Compute new weight between neuron 3 and 4
	value_previous_neuron = network->neurons[3].value;

	derivative_weight = derivative_error*derivative_output*value_previous_neuron;
	
	new_weight = network->neurons[4].link[1] - (0.5*derivative_weight);
	network->neurons[4].link[1] = new_weight;

	// Compute weight to hidden layer
	// Compute new weight between neuron 0 and 2
	double layer_error = (derivative_error * derivative_output) * network->neurons[4].link[0];
	double derivative_neuron = derivative(network->neurons[2].value);
	value_previous_neuron = network->neurons[0].value;

	derivative_weight = layer_error * derivative_neuron * value_previous_neuron;

	new_weight = network->neurons[2].link[0] - (0.5*derivative_weight);
	network->neurons[2].link[0] = new_weight;

	// Compute new weight between neuron 1 and 2
	layer_error = (derivative_error * derivative_output) * network->neurons[4].link[0];
	derivative_neuron = derivative(network->neurons[2].value);
	value_previous_neuron = network->neurons[1].value;

	derivative_weight = layer_error * derivative_neuron * value_previous_neuron;

	new_weight = network->neurons[2].link[1] - (0.5*derivative_weight);
	network->neurons[2].link[1] = new_weight;

	// Compute new weight between neuron 0 and 3
	layer_error = (derivative_error * derivative_output) * network->neurons[4].link[1];
	derivative_neuron = derivative(network->neurons[3].value);
	value_previous_neuron = network->neurons[0].value;

	derivative_weight = layer_error * derivative_neuron * value_previous_neuron;

	new_weight = network->neurons[3].link[0] - (0.5*derivative_weight);
	network->neurons[3].link[0] = new_weight;

	// Compute new weight between neuron 1 and 3
	layer_error = (derivative_error * derivative_output) * network->neurons[4].link[1];
	derivative_neuron = derivative(network->neurons[3].value);
	value_previous_neuron = network->neurons[1].value;

	derivative_weight = layer_error * derivative_neuron * value_previous_neuron;

	new_weight = network->neurons[3].link[1] - (0.5*derivative_weight);
	network->neurons[3].link[1] = new_weight;
}

// Machine learning function
void learn(NeuralNetwork *network, int entry[], int expected) {
    propagation(network, entry, 2);
    backpropagation(network, expected);
    printf("Output : %f\n", network->neurons[4].value);
}

// Pass through the neural network and return the output
double run(NeuralNetwork *network, int entry[], size_t len) {
    propagation(network, entry, len);
    printf("Output : %f\n", network->neurons[4].value);
    return network->neurons[4].value;
}

// Save network into save_network.txt
void save_neural_network(NeuralNetwork *network) {
    FILE *file = NULL;
    // Set the file where to save data
    file = fopen("src/libs/neural_network/save_network.txt", "w");

    // Write all info
    for (size_t i = 0; i < network->nb_neurons; i++) {
        fprintf(file, "Value :\n%f\n", network->neurons[i].value);
        fprintf(file, "Links :\n%f; %f\n", network->neurons[i].link[0], network->neurons[i].link[1]);
        fprintf(file, "Bias :\n%f\n\n", network->neurons[i].bias);
    }

    // Close the file
    fclose(file);
}

// Get information from one neuron in save_network.txt
Neuron load_neuron(FILE *file) {
    double neuron_data[4];
    // Check if the file exist
    if (file == NULL) {
        printf("Error: fail to load neuron\n");
        exit(EXIT_FAILURE);
    } else {
        // Get neuron data through thanks to a pattern
        fscanf(file, "Value :\n%lf\nLinks :\n%lf; %lf\nBias :\n%lf\n\n", &neuron_data[0], &neuron_data[1], &neuron_data[2], &neuron_data[3]);

        // Creation of the neuron with data from save_network information
        Neuron n;
        n.value = neuron_data[0];
        n.link[0] = neuron_data[1];
        n.link[1] = neuron_data[2];
        n.bias = neuron_data[3];

        return n;
    }
}

// Create a network with data from save_network.txt
NeuralNetwork load_neural_network() {
    FILE *file = NULL;
    // Get the file where network data is saved
    file = fopen("src/libs/neural_network/save_network.txt", "r");
    // Check if the file exist
    if (file == NULL) {
        printf("Error: fail to load neural network\n");
        exit(EXIT_FAILURE);
    } else {
        // Creation of the network with all data in save_network.txt
        NeuralNetwork net;
        // Init all neurons
        for (int i = 0; i < 5; i++) {
            net.neurons[i] = load_neuron(file);
        }
        net.nb_neurons = 5;
		// Close file
		fclose(file);        
        return net;
    }
    // Close file
    fclose(file);
}