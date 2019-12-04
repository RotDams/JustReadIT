
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//#include <gmpxx.h>
#include "neuralnetwork.h"

// Activation function n°1
double sigmoide(double value) {
	return 1 / (1 + exp(-value));
}

// Return a random value between 5 and -5
double get_link() {
    return (((double) (random() % 100)) / 10) - 5;
}

Neuron *get_neuron(size_t nb_neurons_per_layer[], size_t index) {
    // Creation of the neuron
    Neuron *n = malloc(sizeof(Neuron));
    n->bias = 0;
    n->value = 0;
    n->error = 0;
    n->nb_link = 0;

    // Add links for all layers except the first
    if (index != 1) {
        double *link = malloc(sizeof(double) * nb_neurons_per_layer[index - 2]);
        n->links = link;
        n->nb_link = nb_neurons_per_layer[index - 2];
        // Adding all links
        for (size_t i = 0; i < nb_neurons_per_layer[index - 2]; i++) {
            n->links[i] = get_link();
        }
    }
    return n;
}

List get_neurons_list(size_t nb_neurons_per_layer[], size_t index) {
    // Create a layer
    List neurons = create_list();

    // Add all neurons into it
    for (size_t i = 0; i < nb_neurons_per_layer[index - 1]; i++) {
        neurons = push_back_list(neurons, get_neuron(nb_neurons_per_layer, index), NeuronType);
    }
    return neurons;
}


// Initialize the network from scratch

void init(NeuralNetwork *net, size_t nb_layer, size_t nb_neurons_per_layer[]) {

    // Definition of the list of layers
    net->layers = create_list();

    // Adding all layers
    for (size_t i = 1; i <= nb_layer; i++) {
        net->layers = push_back_list(net->layers, get_neurons_list(nb_neurons_per_layer, i), LayerType);
    }
}

void print_info(NeuralNetwork *net) {
    if (!net)
        return;
    // Init values
    List layers = net->layers;
    printf("number of layers : %lu\n", layers->length);
    Node *neurons = (layers->first);
    List list_neuron;
    Neuron *n;

    // All layers
    for (unsigned long i = 1; i <= layers->length; i++) {
        printf("\n===============layer: %lu===============(with %lu neurons)\n", i, ((List) (neurons->value))->length);
        list_neuron = (List) (neurons->value);
        Node *current_n = list_neuron->first;
        n = (Neuron *) (current_n->value);

        // if it is not the first layer :
        if (n->links)
            // All neurons:
            for (unsigned long k = 1; k <= list_neuron->length; k++) {
                n = (Neuron *) (current_n->value);

                // Print infos of the neuron
                printf("Current Neuron: %lu : value = %.10lf ( with %lu links)\n bias : %.10lf\n ----links:------\n", k,
                       n->value,
                       n->nb_link, n->bias);
                // print all links
                for (unsigned long j = 0; j < n->nb_link; j++) {
                    printf("  (%lu) = %.30f   ", j, n->links[j]);
                }
                printf("\n");
                current_n = current_n->next;
            }
            // it is the first layer :
        else
            for (unsigned long k = 1; k <= list_neuron->length; k++) {
                n = (Neuron *) (current_n->value);
                // Print all infos of the neuron
                printf("Current Neuron: %lu : value = %lf  ( with %lu links)\n", k, n->value, (unsigned long) 0);
                current_n = (current_n->next);
            }

        neurons = neurons->next;
    }
}


void propagation(NeuralNetwork *network, double entry[]) {
    //Init the values of entry
    Node *current_first = ((List) network->layers->first->value)->first;
    for (size_t i_first = 0; i_first < ((List) (network->layers->first->value))->length; i_first++) {
        Neuron *first = (Neuron *) current_first->value;
        first->value = entry[i_first];
        current_first = current_first->next;
    }

    // Propagation of the information into the network
    Node *current_layer1 = network->layers->first;
    Node *current_layer2 = current_layer1->next;

    // All layers:
    for (size_t i_of_layers = 0; i_of_layers < network->layers->length - 1; i_of_layers++) {
        List layer_1 = (List) current_layer1->value;
        List layer_2 = (List) current_layer2->value;


        // For the last layer find the value of the max output (for softmax)
        double max_soft = 0;
        Node *current_n = layer_2->first;
        if (i_of_layers == network->layers->length - 2)
            for (size_t i_neuron = 0; i_neuron < layer_2->length; i_neuron++) {
                Neuron *n = (Neuron *) current_n->value;
                if (n->value > max_soft)
                    max_soft = n->value;
                current_n = current_n->next;
            }

        double total_soft = 0;
        Node *current_neuron_2 = layer_2->first;

        // All neurons in the current layer:
        for (size_t i_layer2 = 0; i_layer2 < layer_2->length; i_layer2++) {
            double value_neuron = 0;
            Neuron *neuron_2 = (Neuron *) current_neuron_2->value;
            Node *current_neuron_1 = layer_1->first;

            // Calcul the weight of the neuron before the activation function
            for (size_t i_links_2 = 0; i_links_2 < neuron_2->nb_link; i_links_2++) {
                Neuron *neuron_1 = (Neuron *) current_neuron_1->value;
                value_neuron += neuron_2->links[i_links_2] * neuron_1->value;
                current_neuron_1 = current_neuron_1->next;
            }

            // if it is the max layer: apply softmax (Part 1)
            if (i_of_layers == network->layers->length - 2) {
                value_neuron = (double) exp(value_neuron - max_soft);
                total_soft += value_neuron;
                neuron_2->value = value_neuron;
            } else
                // Apply sigmoid if it's not
                neuron_2->value = sigmoide(value_neuron + neuron_2->bias);

            current_neuron_2 = current_neuron_2->next;
        }
        // In last layer finich softmax and divide with total (softmax part2)
        if (i_of_layers == network->layers->length - 2) {
            current_neuron_2 = layer_2->first;
            for (size_t i_layer2 = 0; i_layer2 < layer_2->length; i_layer2++) {
                Neuron *neuron_2 = (Neuron *) current_neuron_2->value;

                neuron_2->value = (neuron_2->value ) / total_soft;//+ neuron_2->bias

                current_neuron_2 = current_neuron_2->next;
            }
        }

        // Next layer
        current_layer1 = current_layer1->next;
        current_layer2 = current_layer2->next;
    }
}

void backpropagation(NeuralNetwork *network, double coef) {

    Node *current_layer_2 = network->layers->last;
    Node *current_layer_1 = current_layer_2->previous;

    // All layer :
    for (size_t i_layers = 0; i_layers < network->layers->length - 1; i_layers++) {
        List layer_1 = (List) current_layer_1->value;
        List layer_2 = (List) current_layer_2->value;

        Node *current_neuron_2 = layer_2->first;

        // All neurons in current layer:
        for (size_t i_neuron_2 = 0; i_neuron_2 < layer_2->length; i_neuron_2++) {
            Neuron *neuron_2 = (Neuron *) current_neuron_2->value;

            Node *current_neuron_1 = layer_1->first;

            // All links in current neuron:
            for (size_t i_link_2 = 0; i_link_2 < neuron_2->nb_link; i_link_2++) {
                Neuron *neuron_1 = (Neuron *) current_neuron_1->value;

                // Calcul of the new weight of the link
                double tot = neuron_2->links[i_link_2] + coef * neuron_1->value * neuron_2->error;
                neuron_2->bias += neuron_2->value * neuron_2->error;

                neuron_2->links[i_link_2] = tot;
                current_neuron_1 = current_neuron_1->next;
            }
            // Next neuron
            current_neuron_2 = current_neuron_2->next;
        }
        // Next layer
        current_layer_1 = current_layer_1->previous;
        current_layer_2 = current_layer_2->previous;
    }

}


void update_error(NeuralNetwork *network, double expected[]) {

    Node *current_layer_2 = network->layers->last;
    List layer_2 = (List) current_layer_2->value;
    Node *current_neuron_2 = layer_2->first;

    // Calcul all delta errors of the last layer
    for (size_t i_neuron = 0; i_neuron < layer_2->length; i_neuron++) {
        Neuron *neuron_2 = (Neuron *) current_neuron_2->value;
        neuron_2->error = expected[i_neuron] - neuron_2->value;
        current_neuron_2 = current_neuron_2->next;
    }

    current_layer_2 = network->layers->last;
    Node *current_layer_1 = current_layer_2->previous;

    // All other layer :
    for (size_t i_layers = 0; i_layers < network->layers->length - 2; i_layers++) {
        layer_2 = (List) current_layer_2->value;
        List layer_1 = (List) current_layer_1->value;
        Node *current_neuron_1 = layer_1->first;

        // All neurons of the next layer :
        for (size_t i_neuron_1 = 0; i_neuron_1 < layer_1->length; i_neuron_1++) {
            Neuron *neuron_1 = (Neuron *) current_neuron_1->value;

            // Derivation value
            double derivate_neuron = neuron_1->value * (1 - neuron_1->value);

            double error_propagation = 0;
            Node *current_neuron_2 = layer_2->first;

            // for all neurons in the layer
            for (size_t i_neuron_2 = 0; i_neuron_2 < layer_2->length; i_neuron_2++) {
                Neuron *neuron_2 = (Neuron *) current_neuron_2->value;

                // sum of the error
                double link_2 = neuron_2->links[i_neuron_1];
                error_propagation += link_2 * neuron_2->error;

                current_neuron_2 = current_neuron_2->next;
            }
            // Stock the current error sum into the neuron
            neuron_1->error += derivate_neuron * error_propagation * neuron_1->bias;

            current_neuron_1 = current_neuron_1->next;
        }
        // Next layer
        current_layer_1 = current_layer_1->previous;
        current_layer_2 = current_layer_2->previous;
    }
}

// Machine learning function
size_t learn(NeuralNetwork *network, double entry[], double expected[]) {

    // Propagate and updates errors
    propagation(network, entry);
    update_error(network, expected);

    double max_proba = 0;
    Node *output_neurons = ((List) (network->layers->last->value))->first;
    Neuron *n;

    // Find th index of the max probability of the output
    size_t i_index = 0;
    for (unsigned long i = 0; i < ((List) (network->layers->last->value))->length; i++) {
        n = (Neuron *) (output_neurons->value);
        if (max_proba < n->value) {
            max_proba = n->value;
            i_index = i;
        }
        output_neurons = output_neurons->next;
    }
    return i_index;


}

size_t run(NeuralNetwork *network, double entry[]) {
    // Propagate the info with current letter
    propagation(network, entry);
    double max_proba = 0;
    Node *output_neurons = ((List) (network->layers->last->value))->first;
    Neuron *n;

    // Find th index of the max probability of the output
    size_t i_index = 0;
    for (unsigned long i = 0; i < ((List) (network->layers->last->value))->length; i++) {
        n = (Neuron *) (output_neurons->value);
        printf("%.10f\n",n->value);
        if (max_proba < n->value) {
            max_proba = n->value;
            i_index = i;
        }
        output_neurons = output_neurons->next;
    }
    return i_index;
}

// Save network into save_network.txt
void save_neural_network(NeuralNetwork *net) {
    // Init and open file
    List layers = net->layers;
    Node *neurons = (Node *) (layers->first);
    List list_neuron;
    Neuron *n;
    FILE *file = fopen("src/libs/neural_network/save_network.txt", "w");

    // Print layer number at the top of the file
    fprintf(file, "layers: %lu\n", net->layers->length);
    Node current_neurons = *(Node *) (layers->first);
    fprintf(file, "neurons:");

    // Print all layers length into the file
    for (unsigned long i = 1; i <= layers->length; i++) {
        fprintf(file, " %lu", ((List) (current_neurons.value))->length);
        if (current_neurons.next)
            current_neurons = *(Node *) current_neurons.next;
    }

    // target all layers:
    for (unsigned long i = 1; i <= layers->length; i++) {
        list_neuron = (List) (neurons->value);

        // Print the layer number
        fprintf(file, "\n= Layer %lu =\n\n", i);
        Node *current_n = list_neuron->first;
        n = (Neuron *) (current_n->value);

        // if it is not the first layer:
        if (n->nb_link != 0)
            for (unsigned long k = 1; k <= list_neuron->length; k++) {
                n = (Neuron *) (current_n->value);

                // print all info of the current neuron
                fprintf(file, "Value :\n%.50f\n", n->value);
                fprintf(file, "Bias :\n%.50f\n", n->bias);
                fprintf(file, "Error :\n%.50f\n", n->error);
                fprintf(file, "Links :\n");

                // Print all links
                for (unsigned long j = 0; j < n->nb_link; j++)
                    fprintf(file, "%.50f / ", n->links[j]);

                fprintf(file, "\n\n");
                current_n = current_n->next;

            }
        // It is the first layer:
        else {
            for (unsigned long k = 1; k <= list_neuron->length; k++) {

                // Print all info of the neuron
                n = (Neuron *) (current_n->value);
                fprintf(file, "Value :\n%.50f\n", n->value);
                fprintf(file, "Bias :\n%.50f\n", n->bias);
                fprintf(file, "Error :\n%.50f\n", n->error);
                current_n = (current_n->next);
                fprintf(file, "\n");
            }
        }
        neurons = neurons->next;
    }
    // Signature
    fprintf(file, "===============Credit===============\n\n");
    fprintf(file,
            "JustReadIt was developed by damien.champeyroux, quentin.briolant, adrien.barens and sami.beyhum.\n\n");
    // Close the file
    fclose(file);
}

// Return the value of the current link
double set_new_link(FILE *file) {
    double link;
    fscanf(file, "%lf / ", &link);
    return link;
}

Neuron *set_new_neuron(size_t nb_neurons_per_layer[], size_t index, FILE *file) {

    // Take all info of th current neuron and save it
    Neuron *n = malloc(sizeof(Neuron));
    fscanf(file, "Value :\n%lf\nBias :\n%lf\nError :\n%lf\n", &n->value, &n->bias, &n->error);
    n->nb_link = 0;

    // if it is not the first layer, take all links
    if (index != 1) {
        double *link = malloc(sizeof(double) * nb_neurons_per_layer[index - 2]);
        n->links = link;
        n->nb_link = nb_neurons_per_layer[index - 2];
        fscanf(file, "Links :\n");

        // Read all links
        for (size_t i = 0; i < nb_neurons_per_layer[index - 2]; i++) {
            n->links[i] = set_new_link(file);
        }
    }
    fscanf(file, "\n");
    return n;
}

List set_new_neurons_list(size_t nb_neurons_per_layer[], size_t index, FILE *file) {
    List neurons = create_list();

    // Add all neurons into the current layer
    for (size_t i = 0; i < nb_neurons_per_layer[index - 1]; i++) {
        neurons = push_back_list(neurons, set_new_neuron(nb_neurons_per_layer, index, file), NeuronType);
    }
    return neurons;
}


// Initialize the network from scratch
void load_neural_network(NeuralNetwork *net) {
    FILE *file = NULL;
    // Get the file where network data is saved
    file = fopen("src/libs/neural_network/save_network.txt", "r");
    // Check if the file exist
    if (file == NULL) {
        printf("Error: fail to load neural network\n");
        exit(EXIT_FAILURE);
    }

    // Definition of variables
    net->layers = create_list();
    size_t nb_layer = 0;

    // Init the number of layers
    fscanf(file, "layers: %zi\n", &nb_layer);
    size_t nb_neurons_per_layer[nb_layer];

    // Take the number of neurons into each layers
    fscanf(file, "neurons:");
    int k = 0;
    while (fscanf(file, " %zi", &nb_neurons_per_layer[k]))
        k++;


    int trash;
    // Add all layers into the network :
    for (size_t i = 1; i <= nb_layer; i++) {
        fscanf(file, "= Layer %d =\n\n", &trash);
        net->layers = push_back_list(net->layers, set_new_neurons_list(nb_neurons_per_layer, i, file), LayerType);
    }
    fclose(file);
}