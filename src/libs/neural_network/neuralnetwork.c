
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
                    printf("  (%lu) = %.30f   ", j, link_value);
                    if (j != n.links->length) {
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

    // number of neuron in the first layer
    size_t nb_inputs = ((List) (network->layers->first->value))->length;
    //------------------------------------

    // Check if there is enough entry or neuron
    if (len != nb_inputs) {
        printf("Error in propagation\n");
        return;
    }
    // take the first neuron of the first layer
    Node previous_neuron_init = *(((List) (network->layers->first->value))->first);
    Neuron *previous_n_init = (Neuron *) (previous_neuron_init.value);

    // Place the entry data on the first layer

    for (size_t i = 0; i < nb_inputs; i++) {
        previous_n_init->value = entry[i];
        if ((previous_neuron_init.next)) {
            previous_neuron_init = *(Node *) previous_neuron_init.next;
            previous_n_init = (Neuron *) (previous_neuron_init.value);
        }
    }


    print_info(network);
    Node *current_layer_1 = network->layers->first;
    Node *current_layer_2 = network->layers->first->next;

    // range : layers 2 -> n
    for (size_t i_layer_2 = 1; i_layer_2 < network->layers->length; i_layer_2++) {

        //Init current layer
        List layer_1 = (List) current_layer_1->value;
        List layer_2 = (List) current_layer_2->value;

        Node *current_neuron_layer2 = layer_2->first;

        // range : all neurons of the layers 2
        for (size_t i_neurons_2 = 0; i_neurons_2 < layer_2->length; i_neurons_2++) {

            Node *current_neuron_layer1 = layer_1->first;

            Neuron *neuron_layer2 = (Neuron *) current_neuron_layer2->value;

            double y = 0;

            Node* current_link = neuron_layer2->links->first;

            // range : all links to neurons 'i_neurons_2'
            for (size_t i_neurons_1 = 0; i_neurons_1 < layer_1->length; i_neurons_1++) {
                Neuron *neuron_layer1 = (Neuron *) current_neuron_layer1->value;
                double* link = (double*)current_link->value;
                y += neuron_layer1->value * *link;

                // next
                current_neuron_layer1 = current_neuron_layer1->next;
                current_link = current_link->next;
            }
            // next
            neuron_layer2->value = sigmoide(y);
            current_neuron_layer2 = current_neuron_layer2->next;

        }
        // next
        current_layer_1 = current_layer_1->next;
        current_layer_2 = current_layer_2->next;
    }
}


void backpropagation1(NeuralNetwork *network, size_t expected[]) {

    List last_layer = (List) (network->layers->last->value);
    Node *current_neuron_last = last_layer->first;
    List layer_end_2 = (List) network->layers->last->previous->value;


    double error_neuron, derivate_neuron;
    for (size_t i_neurons = 0; i_neurons < last_layer->length; i_neurons++) {
        Neuron *neuron_last = (Neuron *) (current_neuron_last->value);

        error_neuron = -(expected[i_neurons] - neuron_last->value);

        derivate_neuron = neuron_last->value * (1 - neuron_last->value);

        Node *current_link = neuron_last->links->first;

        Node *current_neuron_end_2 = layer_end_2->first;
        for (size_t i_links = 0; i_links < neuron_last->links->length; i_links++) {
            double *link_last = (double *) current_link->value;

            Neuron *neuron_end_2 = (Neuron *) current_neuron_end_2->value;

            *link_last = *link_last - (error_neuron * derivate_neuron * neuron_end_2->value);

            current_neuron_end_2 = current_neuron_end_2->next;
            current_link = current_link->next;
        }
        current_neuron_last = current_neuron_last->next;
    }

}

void backpropagation2(NeuralNetwork *network, size_t expected[]) {
    List layer1 = (List) network->layers->first->value;

    List layer_middle = (List) network->layers->first->next->value;
    List layer_end = (List) network->layers->last->value;

    Node *current_neuron_mid = layer_middle->first;
    for (size_t i_neurons_mid = 0; i_neurons_mid < layer_middle->length; i_neurons_mid++) {
        Neuron *neuron_mid = (Neuron *) current_neuron_mid->value;


        for (size_t i_links = 0; i_links < neuron_mid->links->length; i_links++) {
            double layer_error = 0;
            Node *current_neuron_end = layer_end->first;
            Node *current_link_mid = neuron_mid->links->first;

            double *link = (double *) current_link_mid->value;

            for (size_t i_layer_end = 0; i_layer_end < layer_end->length; i_layer_end++) {

                Neuron *neuron_end = (Neuron *) current_neuron_end->value;


                double w = *(double *) (get_element_by_index(neuron_end->links, i_neurons_mid)->value);
                double v = neuron_end->value;

                layer_error += w * (-(expected[i_layer_end] - v)) * (v * (1 - v));


                current_link_mid = current_link_mid->next;
                current_neuron_end = current_neuron_end->next;
            }

            Neuron* neu_l =(Neuron*) get_element_by_index(layer1,i_links)->value;
            double v1 =neu_l->value;

            *link = *link - (3 *((neuron_mid->value * (1 - neuron_mid->value)) * v1 * layer_error));

        }

        current_neuron_mid = current_neuron_mid->next;
    }

}


// Machine learning function
void learn(NeuralNetwork *network, double entry[], size_t len, size_t expected[]) {
   // print_info(network);
   // printf("\n\n\n\n\n");
   // print_info(network);
    printf("\n\n\n\n\n");

    propagation(network, entry, len);
    backpropagation1(network, expected);
    backpropagation2(network, expected);

    double max_proba = 0;
    Node *output_neurons = ((List) (network->layers->last->value))->first;
    Neuron *n;

    for (unsigned long i = 0; i < ((List) (network->layers->last->value))->length; i++) {
        //print_info(network);
        n = (Neuron *) (output_neurons->value);
        if (max_proba < n->value) {
            max_proba = n->value;
        }
        output_neurons = output_neurons->next;
    }
    printf("Output : %.30lf\n", max_proba);

}

// Pass through the neural network and return the output

size_t run(NeuralNetwork *network, double entry[], size_t len) {
    propagation(network, entry, len);
    double max_proba = 0;
    size_t letter = 0;
    Node *output_neurons = ((List) (network->layers->last->value))->first;
    Neuron n = *(Neuron *) (output_neurons->value);

    for (unsigned long i = 0; i < ((List) (network->layers->last->value))->length; ++i) {
        if (max_proba < n.value) {
            max_proba = n.value;
            letter = i;
        }
        output_neurons = output_neurons->next;
    }
    printf("Output : %zu\n", letter);
    return letter;
}

// Save network into save_network.txt

void save_neural_network(NeuralNetwork *net) {
    List layers = net->layers;

    Node neurons = *(Node *) (layers->first);
    List list_neuron = (List) (neurons.value);

    Node link_node;
    double link_value;

    Neuron n;
    FILE *file = fopen("src/libs/neural_network/save_network.txt", "w");
    fprintf(file, "layers: %lu\n", net->layers->length);
    // List current_layers = net->layers;

    Node current_neurons = *(Node *) (layers->first);
    fprintf(file, "neurons:");
    for (unsigned long i = 1; i <= layers->length; i++) {
        fprintf(file, " %lu", ((List) (current_neurons.value))->length);
        if (current_neurons.next)
            current_neurons = *(Node *) current_neurons.next;
    }
    for (unsigned long i = 1; i <= layers->length; i++) {
        fprintf(file, "\n= Layer %lu =\n\n", i);
        Node current_n = *(Node *) (list_neuron->first);
        n = *(Neuron *) (current_n.value);
        if (n.links)
            for (unsigned long k = 1; k <= list_neuron->length; k++) {

                link_node = *(Node *) (n.links->first);
                link_value = *(double *) link_node.value;


                fprintf(file, "Value :\n%.30f\n", n.value);
                fprintf(file, "Bias :\n%.30f\n", n.bias);
                fprintf(file, "Links :\n");

                for (unsigned long j = 1; j <= n.links->length; j++) {


                    fprintf(file, "%.30f / ", link_value);
                    if (j != n.links->length) {
                        link_node = *(Node *) link_node.next;
                        link_value = *(double *) link_node.value;
                    }

                }
                fprintf(file, "\n\n");
                if (current_n.next) {
                    current_n = *(Node *) (current_n.next);
                    n = *(Neuron *) (current_n.value);
                }
            }
        else
            for (unsigned long k = 1; k <= list_neuron->length; k++) {
                fprintf(file, "Value :\n%.30f\n", n.value);
                fprintf(file, "Bias :\n%.30f\n", n.bias);
                // fprintf(file, "Links :\n");

                if (current_n.next) {
                    current_n = *(Node *) (current_n.next);
                    n = *(Neuron *) (current_n.value);
                }
                fprintf(file, "\n");
            }
        if (neurons.next) {
            neurons = *(Node *) neurons.next;
            list_neuron = (List) (neurons.value);
        }

    }
    fprintf(file, "===============Credit===============\n\n");
    fprintf(file,
            "JustReadIt was developed by damien.champeyroux, quentin.briolant, adrien.barens and sami.beyhum.\n\n");
    // Close the file
    fclose(file);
}


double *set_new_link(FILE *file) {
    double *link = malloc(sizeof(double));
    fscanf(file, "%lf / ", link);
    return link;
}

Neuron *set_new_neuron(size_t nb_neurons_per_layer[], size_t index, FILE *file) {
    Neuron *n = malloc(sizeof(Neuron));
    fscanf(file, "Value :\n%lf\nBias :\n%lf\n", &n->value, &n->bias);
    n->links = create_list();
    if (index != 1) {
        fscanf(file, "Links :\n");
        for (size_t i = 0; i < nb_neurons_per_layer[index - 2]; i++) {
            n->links = push_back_list(n->links, (double *) set_new_link(file), LinkType);
        }
    }
    fscanf(file, "\n");
    return n;
}

List set_new_neurons_list(size_t nb_neurons_per_layer[], size_t index, FILE *file) {
    List neurons = create_list();

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
    fscanf(file, "layers: %zi\n", &nb_layer);
    size_t nb_neurons_per_layer[3];


    fscanf(file, "neurons:");
    int k = 0;
    while (fscanf(file, " %zi", &nb_neurons_per_layer[k]))
        k++;


    int trash;

    // Declaration of variables
    for (size_t i = 1; i <= nb_layer; i++) {
        fscanf(file, "= Layer %d =\n\n", &trash);
        net->layers = push_back_list(net->layers, set_new_neurons_list(nb_neurons_per_layer, i, file), LayerType);
    }
}