
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

double get_link() {
    //double *link = malloc(sizeof(double));
    //*link = ((double) (random() % 10)) / 10;
    //return link;
    return ((double) (random() % 10)) / 10;
}

Neuron *get_neuron(size_t nb_neurons_per_layer[], size_t index) {
    Neuron *n = malloc(sizeof(Neuron));
    n->bias = ((double) (random() % 10)) / 10;
    n->value = ((double) (random() % 10)) / 10;
    n->error = 0;
    double *link = malloc(sizeof(double) * nb_neurons_per_layer[index - 2]);
    n->links = link;
    n->nb_link = 0;
    if (index != 1) {
        n->nb_link = nb_neurons_per_layer[index - 2];
        for (size_t i = 0; i < nb_neurons_per_layer[index - 2]; i++) {
            n->links[i] = get_link();
        }
    }
    return n;
}

List get_neurons_list(size_t nb_neurons_per_layer[], size_t index) {
    List neurons = create_list();

    for (size_t i = 0; i < nb_neurons_per_layer[index - 1]; i++) {
        Neuron *k = get_neuron(nb_neurons_per_layer, index);
        neurons = push_back_list(neurons, k, NeuronType);
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
    if (!net)
        return;
    List layers = net->layers;
    printf("number of layers : %lu\n", layers->length);

    Node *neurons = (layers->first);
    List list_neuron;
    Neuron *n;

    for (unsigned long i = 1; i <= layers->length; i++) {
        printf("\n===============layer: %lu===============(with %lu neurons)\n", i, ((List) (neurons->value))->length);
        list_neuron = (List) (neurons->value);

        Node *current_n = (list_neuron->first);
        n = (Neuron *) (current_n->value);

        if (n->links)
            for (unsigned long k = 1; k <= list_neuron->length; k++) {
                n = (Neuron *) (current_n->value);
                printf("Current Neuron: %lu : value = %lf ( with %lu links)\n ----links:------\n", k, n->value,
                       n->nb_link);

                for (unsigned long j = 0; j < n->nb_link; j++) {
                    printf("  (%lu) = %.30f   ", j, n->links[j]);
                }
                printf("\n");
                current_n = current_n->next;
            }
        else
            for (unsigned long k = 1; k <= list_neuron->length; k++) {
                n = (Neuron *) (current_n->value);
                printf("Current Neuron: %lu : value = %lf  ( with %lu links)\n", k, n->value, (unsigned long) 0);
                current_n = (current_n->next);
            }

        neurons = neurons->next;
    }
}


void propagation(NeuralNetwork *network, double entry[]) {
    //Init of values
    Node *current_first = ((List) network->layers->first->value)->first;
    for (size_t i_first = 0; i_first < ((List) (network->layers->first->value))->length; i_first++) {
        Neuron *first = (Neuron *) current_first->value;
        first->value = entry[i_first];
        current_first = current_first->next;
    }

    // Propagation of the information
    Node *current_layer1 = network->layers->first;
    Node *current_layer2 = current_layer1->next;

    for (size_t i_of_layers = 0; i_of_layers < network->layers->length - 1; i_of_layers++) {
        List layer_1 = (List) current_layer1->value;
        List layer_2 = (List) current_layer2->value;

        Node *current_neuron_2 = layer_2->first;
        for (size_t i_layer2 = 0; i_layer2 < layer_2->length; i_layer2++) {
            double value_neuron = 0;
            Neuron *neuron_2 = (Neuron *) current_neuron_2->value;


            Node *current_neuron_1 = layer_1->first;

            for (size_t i_links_2 = 0; i_links_2 < neuron_2->nb_link; i_links_2++) {
                Neuron *neuron_1 = (Neuron *) current_neuron_1->value;

                value_neuron += neuron_2->links[i_links_2] * neuron_1->value;

                current_neuron_1 = current_neuron_1->next;
            }

            neuron_2->value = sigmoide(value_neuron);
            current_neuron_2 = current_neuron_2->next;
        }

        current_layer1 = current_layer1->next;
        current_layer2 = current_layer2->next;
    }
}

void backpropagation(NeuralNetwork *network) {
    double coef = 0.1;

    Node *current_layer_2 = network->layers->last;
    Node *current_layer_1 = current_layer_2->previous;

    for (size_t i_layers = 0; i_layers < network->layers->length - 1; i_layers++) {
        List layer_1 = (List) current_layer_1->value;
        List layer_2 = (List) current_layer_2->value;

        Node *current_neuron_2 = layer_2->first;
        for (size_t i_neuron_2 = 0; i_neuron_2 < layer_2->length; i_neuron_2++) {
            Neuron *neuron_2 = (Neuron *) current_neuron_2->value;

            Node *current_neuron_1 = layer_1->first;
            for (size_t i_link_2 = 0; i_link_2 < neuron_2->nb_link; i_link_2++) {
                Neuron *neuron_1 = (Neuron *) current_neuron_1->value;

                double tot = neuron_2->links[i_link_2] + coef * neuron_1->value * neuron_2->error;
                neuron_2->links[i_link_2] = tot;

                current_neuron_1 = current_neuron_1->next;
            }


            current_neuron_2 = current_neuron_2->next;
        }

        current_layer_1 = current_layer_1->previous;
        current_layer_2 = current_layer_2->previous;
    }

}

// Pass through the neural network and return the output

void update_error(NeuralNetwork *network, double expected[]) {

    // Delta error of the last layer
    Node *current_layer_2 = network->layers->last;
    List layer_2 = (List) current_layer_2->value;
    Node *current_neuron_2 = layer_2->first;
    for (size_t i_neuron = 0; i_neuron < layer_2->length; i_neuron++) {
        Neuron *neuron_2 = (Neuron *) current_neuron_2->value;
        neuron_2->error = expected[i_neuron] - neuron_2->value;
        // printf("\n%.5f",expected[i_neuron] - neuron_2->value);
        current_neuron_2 = current_neuron_2->next;
    }


    current_layer_2 = network->layers->last;
    Node *current_layer_1 = current_layer_2->previous;

    for (int i_layers = 0; i_layers < network->layers->length - 2; i_layers++) {
        layer_2 = (List) current_layer_2->value;
        List layer_1 = (List) current_layer_1->value;
        Node *current_neuron_1 = layer_1->first;
        for (int i_neuron_1 = 0; i_neuron_1 < layer_1->length; i_neuron_1++) {
            Neuron *neuron_1 = (Neuron *) current_neuron_1->value;
            double derivate_neuron = neuron_1->value * (1 - neuron_1->value);

            double error_propagation = 0;
            Node *current_neuron_2 = layer_2->first;

            for (int i_neuron_2 = 0; i_neuron_2 < layer_2->length; i_neuron_2++) {
                Neuron *neuron_2 = (Neuron *) current_neuron_2->value;

                // double link_2 = *(double *) (get_element_by_index(neuron_2->links, i_neuron_1)->value);
                double link_2 = neuron_2->links[i_neuron_1];

                error_propagation += link_2 * neuron_2->error;
                current_neuron_2 = current_neuron_2->next;
            }
            // printf("\n%.5f",derivate_neuron * error_propagation);
            neuron_1->error = derivate_neuron * error_propagation;

            current_neuron_1 = current_neuron_1->next;
        }
        current_layer_1 = current_layer_1->previous;
        current_layer_2 = current_layer_2->previous;

    }
}

// Machine learning function
size_t learn(NeuralNetwork *network, double entry[], double expected[]) {
    // print_info(network);


    // print_info(network);

    propagation(network, entry);
    update_error(network, expected);
    backpropagation(network);
    // backpropagation1(network, expected);
    //  backpropagation2(network, expected);

    double max_proba = 0;
    Node *output_neurons = ((List) (network->layers->last->value))->first;
    Neuron *n;

    size_t i_index = 0;
    for (unsigned long i = 0; i < ((List) (network->layers->last->value))->length; i++) {
        //print_info(network);
        n = (Neuron *) (output_neurons->value);
        printf("Output : %.30lf\n", n->value);
        if (max_proba < n->value) {
            max_proba = n->value;
            i_index = i;
        }
        output_neurons = output_neurons->next;
    }
    //printf("Output : %.30lf\n", max_proba);
    return i_index;


}

size_t run(NeuralNetwork *network, double entry[], size_t len) {
    propagation(network, entry);
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

    Node *neurons = (Node *) (layers->first);
    List list_neuron;

    Node *link_node;
    double *link_value;

    Neuron *n;
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
        list_neuron = (List) (neurons->value);
        fprintf(file, "\n= Layer %lu =\n\n", i);
        Node *current_n = list_neuron->first;
        n = (Neuron *) (current_n->value);
        if (n->nb_link !=0)
            for (unsigned long k = 1; k <= list_neuron->length; k++) {
                n = (Neuron *) (current_n->value);


                fprintf(file, "Value :\n%.30f\n", n->value);
                fprintf(file, "Bias :\n%.30f\n", n->bias);
                fprintf(file, "Links :\n");

                for (unsigned long j = 1; j <= n->nb_link; j++) {

                    fprintf(file, "%.30f / ", n->links[j]);
                }
                fprintf(file, "\n\n");
                current_n = current_n->next;

            }
        else {
            for (unsigned long k = 1; k <= list_neuron->length; k++) {
                fprintf(file, "Value :\n%.30f\n", n->value);
                fprintf(file, "Bias :\n%.30f\n", n->bias);

                if (current_n->next) {
                    current_n = (current_n->next);
                    n = (Neuron *) (current_n->value);
                }
                fprintf(file, "\n");
            }
        }
        neurons = neurons->next;


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
    double *link = malloc(sizeof(double) * nb_neurons_per_layer[index - 2]);
    n->links = link;

    if (index != 1) {
        n->nb_link=nb_neurons_per_layer[index - 2];
        fscanf(file, "Links :\n");
        for (size_t i = 0; i < nb_neurons_per_layer[index - 2]; i++) {
            n->links[i] = get_link();
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
    size_t nb_neurons_per_layer[nb_layer];


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