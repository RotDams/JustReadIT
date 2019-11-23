
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
    n->bias = 2;
    n->value = 0;
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

            Node *current_link = neuron_layer2->links->first;

            // range : all links to neurons 'i_neurons_2'
            for (size_t i_neurons_1 = 0; i_neurons_1 < layer_1->length; i_neurons_1++) {
                Neuron *neuron_layer1 = (Neuron *) current_neuron_layer1->value;
                double *link = (double *) current_link->value;
                y += neuron_layer1->value * *link;//+ neuron_layer2->bias;

                // next
                current_neuron_layer1 = current_neuron_layer1->next;
                current_link = current_link->next;
            }
            y += neuron_layer2->bias;
            // next
            neuron_layer2->value = sigmoide(y);
            current_neuron_layer2 = current_neuron_layer2->next;

        }
        // next
        current_layer_1 = current_layer_1->next;
        current_layer_2 = current_layer_2->next;
    }
}


void backpropagation1(NeuralNetwork *network, double expected[]) {

    // print_info(network);
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


        //print_info(network);
        for (size_t i_links = 0; i_links < neuron_last->links->length; i_links++) {

            double *link_last = (double *) current_link->value;

            double value_n_2 = ((Neuron *) get_element_by_index(layer_end_2, i_links)->value)->value;

            //neuron_end_2
            *link_last = *link_last - (3.4 * error_neuron * derivate_neuron * value_n_2);

            current_neuron_end_2 = current_neuron_end_2->next;
            current_link = current_link->next;
        }

        current_neuron_last = current_neuron_last->next;
    }
}

void backpropagation2(NeuralNetwork *network, double expected[]) {
    List layer1 = (List) network->layers->first->value;
    List layer2 = (List) network->layers->first->next->value;
    List layer3 = (List) network->layers->first->next->next->value;

    Node *current_neuron_layer_2 = layer2->first;

    double derivate_error=0;

    Node* current_last_neuron = layer3->first;
    for (int i_last = 0; i_last < layer3->length; i_last++) {
        double last_n = ((Neuron*) current_last_neuron->value)->value;
        derivate_error += - (expected[i_last] - last_n) *(expected[i_last] - last_n) ;

        current_last_neuron= current_last_neuron->next;
    }

    //todo a check
    double derivate_out = derivative(((Neuron*)(layer3->first->value))->value);



    for (int i_layer_2 = 0; i_layer_2 < layer2->length; i_layer_2++) {
        Neuron *neuron_layer_2 = (Neuron *) current_neuron_layer_2->value;

        // print_info(network);
        double derivat_neuron = neuron_layer_2->value * (1 - neuron_layer_2->value);

        Node *current_link_2 = neuron_layer_2->links->first;
        for (int i_links_n = 0; i_links_n < neuron_layer_2->links->length; i_links_n++) {
            double *link_2 = (double *) current_link_2->value;

            double  derivative_error = derivate_error;

            double derivate_output = derivate_out;

            double derivativ_neuron =derivat_neuron;

            double previous_neuron =((Neuron*) get_element_by_index(layer1,i_links_n)->value)->value;

            double last_link =*(double*)(get_element_by_index(((Neuron*) (layer3->first->value))->links,i_layer_2)->value);

            double layer_error =(derivative_error * derivate_output)* last_link;

            double derivate_weight = layer_error *derivativ_neuron * previous_neuron;

            double new_weight =*link_2 - (3.4 * derivate_weight);

            *link_2 = new_weight;

           // *link_2 -= 0.4 * derivate_weight;

            current_link_2 = current_link_2->next;
        }
        current_neuron_layer_2 = current_neuron_layer_2->next;
    }

}


// Machine learning function
void learn(NeuralNetwork *network, double entry[], size_t len, double expected[]) {
    // print_info(network);


    // print_info(network);

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