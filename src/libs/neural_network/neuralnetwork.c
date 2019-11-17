#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//#include <gmpxx.h>
#include "neuralnetwork.h"

double sigmoide(double value) {
    return 1 / (1 + exp(-value));
}

double derivative(double value) {
    return (value) * (1 - (value));
}

double *get_link() {
    double *link = malloc(sizeof(double));
    *link = drand48();
    return link;
}

Neuron *get_neuron(size_t nb_neurons_per_layer[], size_t index) {
    Neuron *n = malloc(sizeof(Neuron));
    n->bias = drand48();
    n->value = drand48();
    n->links = create_list();
    if (index != 1) {
        for (size_t i = 0; i < nb_neurons_per_layer[index - 2]; i++) {
            n->links = push_back_list(n->links, (double *) get_link(), LinkType);
        }
    }
    return n;
}

List get_neurons_list(size_t nb_neurons_per_layer[], size_t index) {
    List neurons = create_list();

    for (size_t i = 0; i < nb_neurons_per_layer[index - 1]; i++) {
        neurons = push_back_list(neurons, get_neuron(nb_neurons_per_layer, index), NeuronType);
    }
    return neurons;
}


// Initialize the network from scratch
void init(NeuralNetwork *net, size_t nb_layer, size_t nb_neurons_per_layer[]) {

    // Definition of variables
    net->layers = create_list();


    // Declaration of variables
    for (size_t i = 1; i <= nb_layer; i++) {
        net->layers = push_back_list(net->layers, get_neurons_list(nb_neurons_per_layer, i), LayerType);
    }
}

void print_info(NeuralNetwork *net) {
    List layers = net->layers;
    printf("number of layers : %lu\n", layers->length);

    Node neurons = *(Node *) (layers->first);
    List list_neuron = (List) (neurons.value);

    Node link_node;
    double link_value;

    Neuron n;

    for (unsigned long i = 1; i <= layers->length; i++) {
        printf("\n===============layer: %lu===============(with %lu neurons)\n", i, ((List) (neurons.value))->length);
        Node current_n = *(Node *) (list_neuron->first);
        n = *(Neuron *) (current_n.value);

        if (n.links)
            for (unsigned long k = 1; k <= list_neuron->length; k++) {
                link_node = *(Node *) (n.links->first);
                link_value = *(double *) link_node.value;

                printf("Current Neuron: %lu : value = %lf ( with %lu links)\n ----links:------\n", k, n.value,
                       n.links->length);

                for (unsigned long j = 1; j <= n.links->length; j++) {
                    printf("  (%lu) = %f   ", j, link_value);
                    if (j != n.links->length ) {
                        link_node = *(Node *) link_node.next;
                        link_value = *(double *) link_node.value;
                    }

                }
                printf("\n");
                if (current_n.next) {
                    current_n = *(Node *) (current_n.next);
                    n = *(Neuron *) (current_n.value);
                }
            }
        else
            for (unsigned long k = 1; k <= list_neuron->length; k++) {
                printf("Current Neuron: %lu : value = %lf  ( with %lu links)\n", k, n.value, (unsigned long) 0);
                if (current_n.next) {
                    current_n = *(Node *) (current_n.next);
                    n = *(Neuron *) (current_n.value);
                }
            }

        if (neurons.next) {
            neurons = *(Node *) neurons.next;
            list_neuron = (List) (neurons.value);
        }

    }
}


void propagation(NeuralNetwork *network, double entry[], size_t len) {

    size_t nb_inputs = ((List) (network->layers->first->value))->length;


    List layers = network->layers;

    //------------Layer1--------------------
    Node *previous_neurons = (Node *) (layers->first);
    List previous_list_neuron = (List) (previous_neurons->value);

    Node *previous_neuron = (Node *) (previous_list_neuron->first);
    Neuron *previous_n = (Neuron *) (previous_neuron->value);

    //-----------Layer2---------------------
    Node *neurons = (Node *) (layers->first->next);
    List list_neuron = (List) (neurons->value);

    Node *current_neuron = (Node *) (list_neuron->first);
    Neuron *n = (Neuron *) (current_neuron->value);

    Node *link_node = (Node *) (n->links->first);
    double link_value = *(double *) link_node->value;
    //------------------------------------

    // Check if there is enough entry or neuron
    if (len == nb_inputs) {
        Node previous_neuron_init = *(Node *) (((List) (network->layers->first->value))->first);
        Neuron *previous_n_init = (Neuron *) (previous_neuron_init.value);
        // Place the entry data on the first layer
        for (size_t i = 0; i < nb_inputs; i++) {
            previous_n_init->value = entry[i];
            if ((previous_neuron_init.next)) {
                previous_neuron_init = *(Node *) previous_neuron_init.next;
                previous_n_init = (Neuron *) (previous_neuron_init.value);
            }

        }
        double val_neuron = 0;
        // Propagation throught the neural network
        for (size_t j = 1; j < network->layers->length; j++) {
//
            for (size_t k = 0; k < list_neuron->length; k++) {
                val_neuron = 0;
                for (size_t l = 0; l < n->links->length; l++) {
                    val_neuron += previous_n->value * link_value;

                    //next links in current layer
                    if (link_node->next) {
                        link_node = (Node *) link_node->next;
                        link_value = *(double *) link_node->value;
                    }

                    //next neuron in previous layer
                    if (previous_neuron->next) {
                        previous_neuron = (Node *) previous_neuron->next;
                        previous_n = (Neuron *) (previous_neuron->value);
                    }
                }
                val_neuron += n->bias;
                // Activation function
                n->value = sigmoide(val_neuron);

                if (current_neuron->next) {
                    current_neuron = (Node *) current_neuron->next;
                    n = (Neuron *) (current_neuron->value);
                }

                link_node = (Node *) n->links->first;
                link_value = *(double *) link_node->value;

                previous_neuron = (Node *) (previous_list_neuron->first);
                previous_n = (Neuron *) (previous_neuron->value);
            }

            //------------Next Layer (1)--------------------
            previous_neurons = (Node *) (previous_neurons->next);
            previous_list_neuron = (List) (previous_neurons->value);

            previous_neuron = (Node *) (previous_list_neuron->first);
            previous_n = (Neuron *) (previous_neuron->value);

            //-----------Next Layer (2)---------------------
            if (neurons->next) {
                neurons = (Node *) (neurons->next);
                list_neuron = (List) (neurons->value);
            }

            current_neuron = (Node *) (list_neuron->first);
            n = (Neuron *) (current_neuron->value);

            link_node = (Node *) (n->links->first);
            link_value = *(double *) link_node->value;
            //------------------------------------
        }
    } else {
        printf("Error in propagation\n");
    }
}
/*
void backpropagation(NeuralNetwork *network, int expected[], size_t len) {

    // Declaration of variables
    List *layer = network->layers->last;
    Node *neuron = layer->first;
    List *link = neuron->links->first;
    Node *previous_neuron = network->layers->last->previous;
    Node *link_to_output;
    Node *neuron_to_output;
    double error_weight = 0, error_val = 0, derivative_neuron_val = 0, error_tot = 0;

    // Compute weight to output layer
    for (unsigned long i = 0; i < layer->length; i++) {
        for (unsigned long j = 0; j < layer->previous->length; j++) {
            error_val = (-(expected - (neuron->value)));
            derivative_neuron_val = (neuron->value) * (1 - (neuron->value));

            error_weight = error_val * previous_neuron * *derivative_neuron_val;
            link = link - (0.42 * error_weight);

            error_tot += (error_val * derivative_neuron_val) * link;

            // Change the link to compute
            link = link->next;
            previous_neuron = previous_neuron->next;
        }
        // Change the neuron to compute links
        neuron = neuron->next;
    }

    // Compute weigth to hidden layer
    // Set variables
    neuron_to_output = layer->first;
    link_to_output = neuron_to_output->links->first;
    layer = layer->previous;

    for (int i = 0; i < layer->length; ++i) {
        for (int j = 0; j < layer->previous->length; j++) {
            derivative_neuron_val = (neuron->value) * (1 - (neuron->value));
            error_weight = error_tot * previous_neuron * derivative_neuron_val;
            link = link - (0.42 * error_weight);

            // Change the link to compute
            link = link->next;
            previous_neuron = previous_neuron->next;
        }
        // Change the neuron to compute links
        neuron = neuron->next;
        neuron_to_output = neuron_to_output->next;
        links_to_output = neuron_to_output->links->first;
    }
}

// Machine learning function
void learn(NeuralNetwork *network, int entry[], size_t len, int expected) {
    propagation(network, entry, len);
    backpropagation(network, expected);
    double max_proba = 0;
    size_t letter = 0
    Node *output_neurons = network->layers->last->first;
    for (int i = 0; i < network->layers->last->length; ++i) {
        if (max_proba < output_neurons->value) {
            max_proba = output_neurons->value;
            letter = i;
        }
        output_neurons = output_neurons->next;
    }
    printf("Output : %d\n", letter);
}

// Pass through the neural network and return the output
double run(NeuralNetwork *network, int entry[], size_t len) {
    propagation(network, entry, len);
    double max_proba = 0;
    size_t letter = 0
    Node *output_neurons = network->layers->last->first;
    for (int i = 0; i < network->layers->last->length; ++i) {
        if (max_proba < output_neurons->value) {
            max_proba = output_neurons->value;
            letter = i;
        }
        output_neurons = output_neurons->next;
    }
    printf("Output : %d\n", letter);
}

// Save network into save_network.txt
void save_neural_network(NeuralNetwork *network) {
    FILE *file = NULL;
    // Set the file where to save data
    file = fopen("src/libs/neural_network/save_network.txt", "w");

    Node *layer = network->layers->first;
    Node *neuron;
    Node *link;
    // Write all info
    for (int i = 0; i < network->layers->length; i++) {
        neuron = layer->first;
        link = = neuron->link->first
        fprintf(file, "===============Layer %i===============\n\n", i);
        for (int j = 0; j < layer->length; j++) {
            fprintf(file, "Value :\n%f\n", neuron->value);
            fprintf(file, "Bias :\n%f\n", neuron->bias);
            fprintf(file, "Links :\n");
            for (int k = 0; k < count; k++) {
                fprintf(file, "%lf / ", link->value);
                link = link->next;
            }
            fprintf(file, "\n\n");
            neuron = neuron->next;
        }
        layer = layer->next
    }
    fprintf(file, "===============Credit===============\n\n");
    fprintf(file, "JustReadIt was developed by damien.champeyroux,
    quentin.briolant, adrien.barens
    and
    sami.beyhum.\n
    ");
    // Close the file
    fclose(file);
}

// Get information from one link of one neuron in save_network.txt
double load_link(FILE *file) {
    // Check if the pointer have been initialized
    if (neuron == NULL || file == NULL) {
        printf("Error: fail to load link\n");
        exit(EXIT_FAILURE);
    } else {
        // Get link data of a neuron
        double stock_link;
        fscanf(file, "%lf / ", stock_link);
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
        if (num_layer != 0) {
            for (size_t i = 0; i < nb_neurons_per_layer[num_layer - 1]; ++i) {
                stock_link = load_link(file);
                links = push_back_list(links, (void *) stock_link, LinkType);
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
        // Creation of the layer with all data in save_network.txt
        for (size_t i = 0; i < nb_neurons_per_layer[num_layer]; ++i) {
            n = load_neuron(file, i, nb_neurons_per_layer);
            layer = push_back_list(layer, (void *) n, NeuronType);
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
            stock = load_layer(file, i, nb_neurons_per_layer);
            net.layers = push_back_list(net.layer, (void *) stock, LayerType);
        }
        // Close file
        fclose(file);
        return net;
    }
    // Close file
    fclose(file);
}*/