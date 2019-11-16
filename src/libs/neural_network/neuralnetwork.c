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
    double* link = malloc(sizeof(double));
    *link = 0.2;
    return link;
}

Neuron *get_neuron(size_t nb_neurons_per_layer[], size_t index) {
    Neuron *n = malloc(sizeof(Neuron));
    n->bias = 0.5;
    n->value = 0.1;
    n->links = create_list();
    if (index!=1) {
        for (size_t i = 0; i < nb_neurons_per_layer[index - 1]; i++) {
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


//    for (size_t i = 1; i <= nb_layer; i++) {
//        // Reboot the lists
//        List neurons = create_list();
//        links = create_list();
//        // Creation of a list of neurons
//        for (size_t j = 0; j < nb_neurons_per_layer[i - 1]; j++) {
//            Neuron n;
//            n.value = 0;
//            n.bias = (double) rand() / (double) 1;
//            if (i != 0) {
//                for (size_t k = 0; k < nb_neurons_per_layer[i - 1]; k++) {
//                    links = push_back_list(links, (double *) &r, LinkType);
//                }
//                n.links = links;
//                links = create_list();
//            }
//            neurons = push_back_list(neurons, &n, NeuronType);
//        }
//        layers = push_back_list(layers, &neurons, LayerType); //todo pas sur "&"
//    }
//    net->layers = layers;
}

void print_info(NeuralNetwork *net) {
    printf("\n=================Network Information=================\n\n");

    NeuralNetwork network = *(NeuralNetwork *) (&net);
    // Init all variables
    List l = network.layers;
    printf("%lu", l->length);

    List current_list_neurons;

    Node current_neuron_in_list;
    Neuron current_neuron;

    Node current_link_in_list;
    double current_link;

    // ====== Init the first layer in current network=======

    Node current_layer = *(Node *) (network.layers->first);
    //----------------- All layers:-----------------------
    for (unsigned long current_layer_index = 0; current_layer_index < network.layers->length; current_layer_index++) {

        //  >print<

        printf("\n\nLayer %lu:", current_layer_index);


        // ====== Init the first neuron in current layer=======
        current_list_neurons = *(List *) current_layer.value;
        current_neuron_in_list = *(Node *) current_list_neurons->first;
        current_neuron = *(Neuron *) (current_neuron_in_list.value);

        //----------------- All neurons:-----------------------
        for (unsigned long current_neuron_index = 0;
             current_neuron_index < current_list_neurons->length; current_neuron_index++) {

            //  >print<
            printf("\nneuron %lu:", current_neuron_index);

            // ====== Init the first link in current neuron======
            current_link_in_list = *(Node *) current_neuron.links->first;
            current_link = *(double *) (current_link_in_list.value);

            //----------------- All links:-----------------------
            for (unsigned long current_link_index = 0;
                 current_link_index < current_neuron.links->length; current_link_index++) {

                //  >print<
                printf("\nlink %lu: %f", current_link_index, current_link);

                // <<<<swap to the next link>>>>
                current_link_in_list = *(Node *) current_link_in_list.next;
                current_link = *(double *) (current_link_in_list.value);

            }
            // <<<<swap to the next Neuron>>>>
            current_neuron_in_list = *(Node *) current_neuron_in_list.next;
            current_neuron = *(Neuron *) (current_neuron_in_list.value);
        }
        // <<<<swap to the next Layer>>>>
        current_layer = *(Node *) (current_layer.next);
    }

    printf("\n=====================================================\n\n");
//    Node *layer_node = network.layers->first;
//    List layer = *(List*) layer_node->value;
//
//
//    Node *neuron_node = layer->first;
//    Neuron neuron = *(Neuron* ) (neuron_node->value);
//    Node *link_node = NULL;
//    double link = 0;
//    if (neuron.links) {
//        link_node = neuron.links->first;
//        link = *(double *) link_node->value;
//    }

    //for (unsigned long i = 0; i < network->layers->length; i++) {
//    int i = 0;
//    int j = 0;
//    while (layer_node) {
//
//        // Print information of layers
//        printf("Layer %d :\n\n", i);//todo debug mode layer
//        i++;
//        j = 0;
//
//        while (neuron_node) {
//
//            // Print information of neurons
//            printf("Neuron %d :\n", j);
//            j++;
//            printf("Links :\n ");
//            // Print links of neurons
//
//            //for (int k = 0; k < neuron->value->links->length; k++) {
//            link_node = neuron.links->first->value;
//            for (unsigned long k = 1; k < neuron.links->length; k++) {
//                printf("%lf / \n", link);
//                if (neuron.links) {
//                    link_node = link_node->next;
//                    if (link_node)
//                        link = *(double *) link_node->value;
//                }
//            }
//            printf("\n");
//            //next neuron
//            neuron_node = neuron_node->next;
//            if (neuron_node)
//                neuron = *(Neuron *) neuron_node->value;
//
//
//        }
//        printf("\n");
//        // next layer
//        layer_node = layer_node->next;
//        if (layer_node) {
//            layer = (List) layer_node->value;
//            neuron_node = layer->first;
//            if (neuron_node) {
//                neuron = *(Neuron *) (neuron_node->value);
//                if (neuron.links) {
//                    link_node = neuron.links->first;
//                    link = *(double *) link_node->value;
//                }
//            }
//        }
//
//    }



}
/*
void propagation(NeuralNetwork *network, int entry[], size_t len) {

    int nb_inputs = network->layers->first->length;
    Node *layer = network->layers->first;
    Neuron *neuron = layer->value;
    Node *link = neuron->links->first;
    Node *previous_neuron = neuron;
    // Check if there is enough entry or neuron
    if (len == nb_inputs) {
        // Place the entry data on the first layer
        for (int i = 0; i < nb_inputs; i++) {
            neuron->value = entry[i];
            neuron = neuron->next;
        }
        double val_neuron = 0;
        // Propagation throught the neural network
        for (unsigned long j = 1; j < network->layers->length; j++) {
            previous_neuron = layer->first;
            layer = layer->next;
            neuron = layer->first;
            link = neuron->value->links->first;
            for (int k = 0; k < layer->length; k++) {
                val_neuron = 0;
                for (int l = 0; l < neuron->links->length; l++) {
                    val_neuron += previous_neuron->value * link;
                    link = link->next;
                    previous_neuron = previous_neuron->next;
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