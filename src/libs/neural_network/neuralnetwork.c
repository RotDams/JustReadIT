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
NeuralNetwork init(size_t nb_layer, size_t nb_neurons_per_layer[]) {

	// Definition of variables
	List layers, neurons, links;
    Neuron n;
    NeuralNetwork net;
    float random;

    // Declaration of variables
    layer = create_list();
    n.value = 0;

    for (size_t i = 0; i < nb_layer; i++)
    {
    	// Reboot the lists
    	neurons = create_list();
    	links = create_list();
    	// Creation of a list of neurons
    	for (size_t j = 0; j < nb_neurons_per_layer[i]; j++)
    	{
    		n.bias = (float)rand() / (float)1;
    		if (i != 0)
    		{
    			// Creation of a list of random links
    			for (size_t k = 0; i < nb_neurons_per_layer[i-1]; k++)
    			{
    				random = (float)rand() / (float)1;
    				links = push_back_list(links,(void *)random,LinkType);
    			}
    			n.links = links;
    		}
    		neurons = push_back_list(neurons,(void *)n,NeuronType);
    	}
    	layers = push_back_list(layers,(void *)neurons,LayerType);
    }
    net.layers = layers;
    return net;
}

void print_info(NeuralNetwork *network) {
    printf("\n=================Network Information=================\n\n");
    
    Node* layer = *((Node *)network->layers->first);
    Node* neuron = NULL;
    Node* link = NULL;
    // Print information of layers
    for (int i = 0; i < network->layers->length; i++)
    {
    	neuron = layer->first;
    	link = neurons->links->first;
    	printf("Layer %i :\n\n",i);
    	// Print information of neurons
    	for (int j = 0; j < layer->length; j++)
    	{
    		printf("Neuron %i :\n",j);
    		printf("Links : ");
    		// Print links of neurons
    		for (int k = 0; k < neurons->links->length; k++)
    		{
    			printf("%lf / \n", link);
    			link = link->next;
    		}
    		printf("\n");
    		neurons = neurons->next;
    	}
    	layer = layer->next;
    }

    printf("\n=====================================================\n\n");
}

void propagation(NeuralNetwork *network, int entry[], size_t len) {

	int nb_inputs = network->layers->first->length;
	Node* layer = network->layers->first;
	Node* neuron = layer->first;
	Node* link = neuron->link->first;
	Node* previous_neuron = neuron;
    // Check if there is enough entry or neuron
    if (len == nb_inputs) {
        // Place the entry data on the first layer
        for (int i = 0; i < nb_inputs; i++) {
            neuron->value = entry[i];
            neuron = neuron->next;
        }
        double val_neuron = 0;
        // Propagation throught the neural network
        for (int j = 1; j < network->layers->length; j++) {
        	previous_neuron = layer->first;
        	layer = layer->next;	
        	neuron = layer->first;
        	link = neuron->link->first;
        	for (int k = 0; k < layer->length; k++) {
            	val_neuron = 0;
            	for (int l = 0; l < neuron->link->length; l++) {
                	val_neuron += previous_neuron->value * link;
                	link = link->next;
                	previous_neuron = previous_neuron->next
            	}
            	val_neuron += neuron->bias;
            	// Activation function
            	neuron->value = sigmoide(val_neuron);

            	neuron = neuron->next;
        	}
        }
    } else {
        printf("Error in propagation\n");
    }
}

void backpropagation(NeuralNetwork *network, int expected[], size_t len) {
    // Compute weight to output layer
    // Compute new weight between neuron 2 and 4
    double derivative_error = -(expected - network->neurons[4].value);
    double derivative_output = derivative(network->neurons[4].value);
    double value_previous_neuron = network->neurons[2].value;

    double derivative_weight = derivative_error * derivative_output * value_previous_neuron;

    double new_weight = network->neurons[4].link[0] - (0.5 * derivative_weight);
    network->neurons[4].link[0] = new_weight;

    // Compute new weight between neuron 3 and 4
    value_previous_neuron = network->neurons[3].value;

    derivative_weight = derivative_error * derivative_output * value_previous_neuron;

    new_weight = network->neurons[4].link[1] - (0.5 * derivative_weight);
    network->neurons[4].link[1] = new_weight;

    // Compute weight to hidden layer
    // Compute new weight between neuron 0 and 2
    double layer_error = (derivative_error * derivative_output) * network->neurons[4].link[0];
    double derivative_neuron = derivative(network->neurons[2].value);
    value_previous_neuron = network->neurons[0].value;

    derivative_weight = layer_error * derivative_neuron * value_previous_neuron;

    new_weight = network->neurons[2].link[0] - (0.5 * derivative_weight);
    network->neurons[2].link[0] = new_weight;

    // Compute new weight between neuron 1 and 2
    layer_error = (derivative_error * derivative_output) * network->neurons[4].link[0];
    derivative_neuron = derivative(network->neurons[2].value);
    value_previous_neuron = network->neurons[1].value;

    derivative_weight = layer_error * derivative_neuron * value_previous_neuron;

    new_weight = network->neurons[2].link[1] - (0.5 * derivative_weight);
    network->neurons[2].link[1] = new_weight;

    // Compute new weight between neuron 0 and 3
    layer_error = (derivative_error * derivative_output) * network->neurons[4].link[1];
    derivative_neuron = derivative(network->neurons[3].value);
    value_previous_neuron = network->neurons[0].value;

    derivative_weight = layer_error * derivative_neuron * value_previous_neuron;

    new_weight = network->neurons[3].link[0] - (0.5 * derivative_weight);
    network->neurons[3].link[0] = new_weight;

    // Compute new weight between neuron 1 and 3
    layer_error = (derivative_error * derivative_output) * network->neurons[4].link[1];
    derivative_neuron = derivative(network->neurons[3].value);
    value_previous_neuron = network->neurons[1].value;

    derivative_weight = layer_error * derivative_neuron * value_previous_neuron;

    new_weight = network->neurons[3].link[1] - (0.5 * derivative_weight);
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

    Node* layer = network->layers->first;
	Node* neuron;
	Node* link;
    // Write all info
    for (int i = 0; i < network->layers->length; i++) {
    	neuron = layer->first;
    	link = = neuron->link->first
    	fprintf(file, "===============Layer %i===============\n\n",i);
    	for (int j = 0; j < layer->length; j++)
    	{
    		fprintf(file, "Value :\n%f\n", neuron->value);
    		fprintf(file, "Bias :\n%f\n", neuron->bias);
    		fprintf(file, "Links :\n");
    		for (int k = 0; k < count; k++)
    		{
    			fprintf(file,"%lf / ",link->value);
    			link = link->next;
    		}
    		fprintf(file, "\n\n");
    		neuron = neuron->next;
    	}
        layer = layer->next
    }
    fprintf(file, "===============Credit===============\n\n");
    fprintf(file, "JustReadIt was developed by damien.champeyroux,
    	quentin.briolant, adrien.barens and sami.beyhum.\n");
    // Close the file
    fclose(file);
}

// Get information from one link of one neuron in save_network.txt
double load_link(FILE *file) {
	// Check if the pointer have been initialized
	if (neuron==NULL || file==NULL)
	{
		printf("Error: fail to load link\n");
        exit(EXIT_FAILURE);
	} else {
		// Get link data of a neuron
		double stock_link;
		fscanf(file,"%lf / ",stock_link);
		return stock_link;
	}
}

// Get information from one neuron in save_network.txt
Neuron load_neuron(FILE *file, size_t num_layer, size_t nb_neurons_per_layer[]) {
	double neuron_data[2];
    // Check if the file exist
    if (file == NULL) {
        printf("Error: fail to load neuron\n");
        exit(EXIT_FAILURE);
    } else {
        // Get neuron data through thanks to a pattern
        fscanf(file, "Value :\n%lf\nBias :\n%lf\nLinks :\n", &neuron_data[0], &neuron_data[1]);

        List links;
      	links = create_list();
      	double stock_link;
        if (num_layer!=0)
        {
        	for (size_t i = 0; i < nb_neurons_per_layer[num_layer-1]; ++i)
        	{
        		stock_link = load_link(file);
        		links = push_back_list(links,(void *)stock_link,LinkType);
        	}
        }

        fgetc(file);
        fgetc(file);
        
        // Creation of the neuron with data from save_network information
        Neuron n;
        n.value = neuron_data[0];
        n.bias = neuron_data[1];
        n.link = links;

        return n;
    }
}

// Get information from a layer in save_network.txt
List load_layer(FILE *file, int num_layer, size_t nb_neurons_per_layer[]) {
	// Check if the file exist
	if (file == NULL) {
        printf("Error: fail to load neuron\n");
        exit(EXIT_FAILURE);
    } else {
    	List layer;
    	Neuron n;
    	layer = create_list();
    	for (size_t i = 0; i < nb_neurons_per_layer[num_layer]; ++i)
    	{
    		n = load_neuron(file,i,nb_neurons_per_layer);
    		layer = push_back_list(layer,(void *)n,NeuronType);
    	}
    	return layer;
    }
}

// Create a network with data from save_network.txt
NeuralNetwork load_neural_network(size_t nb_layer, size_t nb_neurons_per_layer[]) {
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
        net.layers = create_list();
        List stock;
        // Init all layer
        for (size_t i = 0; i < nb_layer; i++) {
        	stock = load_layer(file,i, nb_neurons_per_layer);
        	net.layers = push_back_list(net.layer,(void *)stock,LayerType);
        }
        // Close file
        fclose(file);
        return net;
    }
    // Close file
    fclose(file);
}