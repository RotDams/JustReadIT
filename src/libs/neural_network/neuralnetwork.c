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

    size_t nb_inputs = ((List) (network->layers->first->value))->length;


    List layers = network->layers;

    //------------Layer1--------------------
    Node *previous_neurons = (Node *) (layers->first);
    List previous_list_neuron = (List) (previous_neurons->value);

    Node *previous_neuron = (Node *) (previous_list_neuron->first);
    Neuron *previous_n = (Neuron *) (previous_neuron->value);

    //-----------Layer2---------------------
    Node *neurons = layers->first->next;
    List list_neuron = (List) (neurons->value);

    Node *current_neuron = list_neuron->first;
    Neuron *n = (Neuron *) (current_neuron->value);

    Node *link_node = n->links->first;
    double *link_value = (double *) link_node->value;
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
                    val_neuron += previous_n->value * *link_value;

                    //next links in current layer
                    if (link_node->next) {
                        link_node = (Node *) link_node->next;
                        link_value = (double *) link_node->value;
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
                link_value = (double *) link_node->value;

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
            link_value = (double *) link_node->value;
            //------------------------------------
        }
    } else {
        printf("Error in propagation\n");
    }
}


void backpropagation(NeuralNetwork *network, size_t expected[]) {

    // Declaration of variables

    // Init list des layers
    List layers = network->layers;

    //------------Layer1--------------------
    // layer end-1 :
    Node *previous_neurons = (layers->last->previous);
    List previous_list_neurons = (List) (previous_neurons->value);

    // Neuron 0 of layer end-1:
    Node *previous_neuron = (previous_list_neurons->first);
    Neuron *previous_n;

    //-----------Layer2---------------------

    // layer end :
    Node *neurons = (layers->last);
    List list_neuron = (List) (neurons->value);

    // Neuron 0 of layer end
    Node *current_neuron = (Node *) (list_neuron->first);
    Neuron *n;

    // link value of neuron in last layer
    Node *link_node;
    double *link_value;
    //------------------------------------

    // error init to 0
    double error_weight = 0, error_val = 0, derivative_neuron_val = 0, error_tot = 0;

    // range : all neurons il last layer
    for (unsigned long i = 0; i < ((List) (layers->last->value))->length; i++) {

        // Take the current Neuron in the layer
        n = (Neuron *) (current_neuron->value);
        // Take the first link of the Neuron
        link_node = n->links->first;


        // range : all links in the current neuron
        for (unsigned long j = 0; j < n->links->length; j++) {

            // Init the value of the current link
            link_value = (double *) link_node->value;
            // Init the value of the previous neurons
            previous_n = (Neuron *) (previous_neuron->value);



            // Calcul of the error
            error_val = (-(expected[i] - (n->value)));
            derivative_neuron_val = (n->value) * (1 - (n->value));

            error_weight = error_val * previous_n->value * derivative_neuron_val;
            *link_value = *link_value - (0.42 * error_weight);
            error_tot += (error_val * derivative_neuron_val) * *link_value;


            // Next link and next previous neuron
            link_node = link_node->next;
            previous_neuron = previous_neuron->next;

        }

        // Take next neuron and reset previous neuron to first
        current_neuron = current_neuron->next;
        previous_neuron = (Node *) (previous_list_neurons->first);

    }

    //-----------Layer2---------------------
    // Take the layer end-1
    neurons = (Node *) (layers->last->previous);
    list_neuron = (List) (neurons->value);

    // Take the first neuron in this layer
    current_neuron = (Node *) (list_neuron->first);
    n = (Neuron *) (current_neuron->value);

    // take the first link of the neuron
    link_node = (Node *) (n->links->first);

    //------------Layer1--------------------
    // Take the layer end-2
    previous_neurons = (Node *) (neurons->previous);
    previous_list_neurons = (List) (previous_neurons->value);

    // Take the first neuron of the layer end-2
    previous_neuron = (Node *) (previous_list_neurons->first);

    // variable error of 1 layer
    double layer_error;

    // range: all layers
    for (size_t i = 0; i < layers->length - 2; i++) {

        // init the layer of this layer to 0
        layer_error = 0;

        // Take the current list of neurons
        previous_list_neurons = (List) (previous_neurons->value);

        // Take the first neuron in the list
        list_neuron = (List) (neurons->value);

        // range : all neurons in the current layer
        for (size_t k = 0; k < list_neuron->length; k++) {

            // take the value of the current neuron
            n = (Neuron *) (current_neuron->value);

            // range : all links in the neuron
            for (size_t j = 0; j < n->links->length; j++) {

                // Take the value of the neuron
                previous_n = (Neuron *) (previous_neuron->value);
                // Take the value of the current link
                link_value = (double *) link_node->value;

                // Calul
                derivative_neuron_val = (n->value) * (1 - (n->value));

                error_weight = layer_error * previous_n->value * derivative_neuron_val;
                *link_value = *link_value - (0.42 * error_weight);


                // next link of the current neuron
                link_node = link_node->next;

                // Take the next neuron of the layer end - 2
                previous_neuron = previous_neuron->next;

            }

            // Take the next neuron of layer end -1
            current_neuron = current_neuron->next;
            // init to the first link
            link_node = (Node *) n->links->first;
            // init to the first neuron of the layer end - 2
            previous_neuron = (Node *) (previous_list_neurons->first);
        }

        // Swap to the layer before (end -2 (-1))
        previous_neurons = previous_neurons->previous;
        // Init to the first neuron
        previous_neuron = (previous_list_neurons->first);

        // Swap to the layer before (end -1 (-1))
        neurons = neurons->previous;
        // Init to the first neuron
        current_neuron = (list_neuron->first);
        // Init to the first link
        link_node = n->links->first;
    }
}


// Machine learning function
void learn(NeuralNetwork *network, double entry[], size_t len, size_t expected[]) {
    propagation(network, entry, len);
    backpropagation(network, expected);

    double max_proba = 0;
    Node *output_neurons = ((List) (network->layers->last->value))->first;
    Neuron *n;

    for (unsigned long i = 0; i < ((List) (network->layers->last->value))->length; i++) {
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