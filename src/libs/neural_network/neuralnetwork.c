
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

    List layers = network->layers;

    //------------Layer1--------------------
    // Take the First layer
    Node *first_layer = layers->first;
    List first_layer_list = (List) (first_layer->value);

    // Take the first neuron of this layer
    Node *previous_neuron = first_layer_list->first;
    Neuron *previous_n;

    //-----------Layer2---------------------
    // Take the second layer
    Node *second_layer = layers->first->next;
    List second_layer_list = (List) (second_layer->value);

    // Take the first neuron of this layer
    Node *current_neuron = second_layer_list->first;
    Neuron *n = (Neuron *) (current_neuron->value);

    // Take the first link of this neuron
    Node *link_node = n->links->first;
    double *link_value;
    //------------------------------------

    // Check if there is enough entry or neuron
    if (len == nb_inputs) {
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
        double val_neuron = 0;
        // Propagation throught the neural network

        //todo ca marche jusque la
        for (size_t j = 1; j < network->layers->length; j++) {

            //Take the value of neurons in previous layer
            first_layer_list = (List) (first_layer->value);
            //Take the value of neurons in this layer
            second_layer_list = (List) (second_layer->value);
            // take the first neuron of this layer
            current_neuron = second_layer_list->first;
            // take the first link of this neuron
            link_node = n->links->first;

            for (size_t k = 0; k < second_layer_list->length; k++) {
                //init
                val_neuron = 0;
                //Take the value of the current neuron
                n = (Neuron *) (current_neuron->value);


                for (size_t l = 0; l < n->links->length; l++) {
                    //Take the value of the current link of this neuron
                    link_value = (double *) link_node->value;
                    //Take the value of neuton of the previous layer
                    previous_n = (Neuron *) (previous_neuron->value);

                    //Calcul
                    val_neuron += previous_n->value * *link_value;

                    //next links in current layer
                    link_node = link_node->next;
                    //next neuron in previous layer
                    previous_neuron = previous_neuron->next;
                }

                val_neuron += n->bias;
                // Change neuron value
                n->value = sigmoide(val_neuron);

                //Take the next neuron
                current_neuron = current_neuron->next;
                // pick the first link of this neuron
                link_node = n->links->first;

                //Take the first neuron in the previous layer
                previous_neuron = first_layer_list->first;
            }

            // Swap to the next layers (previous)
            first_layer = first_layer->next;
            // Swap to the next layers (current)
            second_layer = second_layer->next;


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
    Node *layer_end_1 = (layers->last->previous);
    List layer_end_1_list = (List) (layer_end_1->value);

    // Neuron 0 of layer end-1:
    Node *previous_neuron = (layer_end_1_list->first);
    Neuron *previous_n;

    //-----------Layer2---------------------

    // layer end :
    Node *layer_end = (layers->last);
    List layer_end_list = (List) (layer_end->value);

    // Neuron 0 of layer end
    Node *current_neuron = (Node *) (layer_end_list->first);
    Neuron *n;

    // link value of neuron in last layer
    Node *link_node;
    double *link_value;
    //------------------------------------

    // error init to 0
    double error_weight = 0, error_val = 0, derivative_neuron_val = 0, error_tot = 0;

    // range : all layer_end il last layer
    for (unsigned long i = 0; i < ((List) (layers->last->value))->length; i++) {

        // Take the current Neuron in the layer
        n = (Neuron *) (current_neuron->value);
        // Take the first link of the Neuron
        link_node = n->links->first;


        // range : all links in the current neuron
        for (unsigned long j = 0; j < n->links->length; j++) {

            // Init the value of the current link
            link_value = (double *) link_node->value;
            // Init the value of the previous layer_end
            previous_n = (Neuron *) (previous_neuron->value);



            // Calcul of the error
            error_val = (-(expected[i] - (n->value)));
            derivative_neuron_val = (n->value) * (1 - (n->value));

            error_weight = error_val * previous_n->value * derivative_neuron_val;
            *link_value = *link_value - (0.42 * error_weight);


            // Next link and next previous neuron
            link_node = link_node->next;
            previous_neuron = previous_neuron->next;

        }

        // Take next neuron and reset previous neuron to first
        current_neuron = current_neuron->next;
        previous_neuron = (layer_end_1_list->first);

    }

    //-----------Layer2---------------------
    // Take the layer end-1
    layer_end = (Node *) (layers->last->previous);
    layer_end_list = (List) (layer_end->value);

    // Take the first neuron in this layer
    current_neuron = (Node *) (layer_end_list->first);
    n = (Neuron *) (current_neuron->value);

    // take the first link of the neuron
    link_node = (Node *) (n->links->first);

    //------------Layer1--------------------
    // Take the layer end-2
    layer_end_1 = (layer_end->previous);
    layer_end_1_list = (List) (layer_end_1->value);

    // Take the first neuron of the layer end-2
    previous_neuron = (layer_end_1_list->first);

    // variable error of 1 layer

    // range: all layers
    for (size_t i = 0; i < layers->length - 2; i++) {

        // init the layer of this layer to 0

        // Take the current list of layer_end
        layer_end_1_list = (List) (layer_end_1->value);

        // Take the first neuron in the list
        layer_end_list = (List) (layer_end->value);

        // Init to the first neuron
        previous_neuron = (layer_end_1_list->first);
        // Init to the first neuron
        current_neuron = (layer_end_list->first);
        // Init to the first link
        link_node = n->links->first;

        // range : all layer_end in the current layer
        for (size_t k = 0; k < layer_end_list->length; k++) {

            // take the value of the current neuron
            n = (Neuron *) (current_neuron->value);
            // init to the first link
            link_node = n->links->first;
            // init to the first neuron of the layer end - 2
            previous_neuron = (Node *) (layer_end_1_list->first);

            // range : all links in the neuron
            for (size_t j = 0; j < n->links->length; j++) {

                // Take the value of the neuron
                previous_n = (Neuron *) (previous_neuron->value);
                // Take the value of the current link
                link_value = (double *) link_node->value;

                // Calul
                derivative_neuron_val = (n->value) * (1 - (n->value));

                // Calcul of the error tot

                error_tot = 0;
                Node *temp_neuron = layer_end_list->first;
                Neuron *temp_neuron_value;
                for (size_t k = 0; k < layer_end_list->length; k++) {
                    temp_neuron_value = (Neuron *) (temp_neuron->value);

                    Node *temp_next_neuron = layer_end_list->first;
                    Neuron *temp_next_neuron_value;
                    double link_er =0;
                    for (size_t l = 0; l < layer_end_list->length; l++) {
                        temp_next_neuron_value = (Neuron *) temp_next_neuron->value;

                        link_er += *(double*) (get_element_by_index(temp_next_neuron_value->links, j).value);

                        temp_next_neuron = temp_next_neuron->next;
                    }
                    error_tot += (temp_neuron_value->value * (1 - temp_neuron_value->value)) *
                                 (-(expected[k] - temp_neuron_value->value) * link_er);

                    temp_neuron = temp_neuron->next;
                }


                error_weight = error_tot * previous_n->value * derivative_neuron_val;
                *link_value = *link_value - (0.42 * error_weight);


                // next link of the current neuron
                link_node = link_node->next;
                // Take the next neuron of the layer end - 2
                previous_neuron = previous_neuron->next;

            }

            // Take the next neuron of layer end -1
            current_neuron = current_neuron->next;

        }

        // Swap to the layer before (end -2 (-1))
        layer_end_1 = layer_end_1->previous;
        // Swap to the layer before (end -1 (-1))
        layer_end = layer_end->previous;

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