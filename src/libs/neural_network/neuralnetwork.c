#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "neuralnetwork.h"

double sigmoide(double value)
{
	return 1/(1+exp((-1)*value));
}

double derivative(double value)
{
	return value*(1-value);
}

void Init(NeuralNetwork *network)
{
	Neuron n;
	n.value = 0;
	n.bias = 0;
	double weight[2] = {0.5,0.5};
	n.link[2] = *weight;
	n.nb_link = 2;

	NeuralNetwork net;
	net.error = 0.05;
	Neuron list_neurons[5] = {n,n,n,n,n};
	net.neurons[5] = *list_neurons;
	net.nb_neurons = 5;
	*network = net;
}

void Print_Info(NeuralNetwork *network)
{
	printf("Link : \n");
	printf("%f, %f\n",network->neurons[2].link[0],network->neurons[2].link[1]);
	printf("%f, %f\n",network->neurons[3].link[0],network->neurons[3].link[1]);
	printf("%f, %f\n",network->neurons[4].link[0],network->neurons[4].link[1]);

	printf("Values :\n");
	printf("%f, %f \n",network->neurons[0].value, network->neurons[1].value);
	printf("%f, %f \n",network->neurons[2].value, network->neurons[3].value);
	printf("%f \n",network->neurons[4].value);
}

void Propagation(NeuralNetwork *network, int entry[], size_t len)
{
	// Check if there is enough entry or neuron
	if (len == 2)
	{
		// Place the entry data on the first layer
		for (int i = 0; i < 2; i++)
		{
			network->neurons[i].value = entry[i];
		}
		double val_neuron = 0;
		// Propagation throught the neural network
		for (int j = 2; j < 5; j++)
		{
			val_neuron = 0;
			for (int k = 0; k < 2; k++)
			{
				val_neuron+=network->neurons[j-1-k-(j%2)].value*network->neurons[j].link[k];
			}
			// Activation function
			network->neurons[j].value = sigmoide(val_neuron);
		}
	}
	else
	{
		printf("Error in Propagation\n");
	}
}

void Backpropagation(NeuralNetwork *network,int expected)
{
	double sum = 0;
	// Compute the error
	for (int i = 0; i < 1; i++)
	{
		network->neurons[4].value = expected - network->neurons[4].value;
	}
	
	for (int j = 4; j > 1; j--)
	{
		if(j==3)
		{
		    sum = 0;
		}
		for(int k = 0;k < 2; k++)
		{
		    sum += network->neurons[j-1-(j%2)].value * network->neurons[j-1-(j%2)].link[k];
		}
		sum = sigmoide(sum);
		//network->neurons[j-1].link[k] -= network->error * ((-1)*network->neurons[j].value * sum * (1-sum)*network->neurons[j-1-(j%2)].value);
	}

/*
	for (size_t i = network.lenvalue -1; i > 0; i--)
	{
			
		for (size_t j = 0; j < network.lenlayer-1; j++)
		{
				
			for (size_t k = 0; k < network.lenlayer[i]; k++)
			{
				sum = 0;
					
				for (size_t l = 0; l < &network.valuesize[i]; l++)
				{
					// On effectue la somme pondérée du neurone vers lequel pointe la connexion :
					sum += network.value[i-1][l] * network.link[i-l][l][k];	
				}
				sum = sigmoide(sum);// fonction sigmoide je crois

				// On met à jour le poids de la connexion
				network.link[i-1][j][k] -= network.error * ((-1) * network.value[i][k] * sum * (1-sum)* network.value[i-1][j]);
			}
				
			for (size_t j = 0; j < &network.valuesize[i-1]; j++)
			{
				sum = 0;
				
				for (size_t k = 0; k < &network.value[i]; k++)
				{
					// On met à jour les neurones de la prochaine couche en fonction de l'erreur qui se rétropropage
					sum += network.value[i][k] * network.link[i-1][j][k];
				}
				network.value[i-l][j] = sum;
			}
		}
	}*/
}

void Learn(NeuralNetwork *network, int entry[],int expected[])
{
	for (int i = 0; i < 100; i++)
	{
		Propagation(network,entry,100);
		Backpropagation(network,expected[i]);
	}
}

double Run(NeuralNetwork *network, int entry[], size_t len)
{
	Propagation(network, entry,len);
	return network->neurons[4].value;
}

void Save_Neural_Network(NeuralNetwork *network)
{
	FILE *file = NULL;
	// Set the file where to save data
	file = fopen("src/libs/NeuralNetwork/save_network.txt","w");

	// Write all info
	fprintf(file,"Neurons:\n");
	for(size_t i=0; i<network->nb_neurons;i++)
	{
		fprintf(file,"Value :\n%f\n",network->neurons[i].value);
		fprintf(file,"Links :\n%f; %f\n\n",network->neurons[i].link[0],network->neurons[i].link[1]);
	}

	// Close the file
	fclose(file);
}

void Load_Neural_Network(NeuralNetwork *network)
{
	FILE *file = NULL;
	char *filedata = NULL;
	// Get the file where network data is saved
	file = fopen("src/libs/NeuralNetwork/save_network.txt","r");
	// Check if the file exist
	if(file==NULL)
	{
		printf("Error: fail to load\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		for(size_t i = 0; i<network->nb_neurons; i++)
		{
			fgets(filedata,1000,file);
			fgets(filedata,1000,file);
			//network->neurons[i].value = (float)filedata;
			
		}

		// Close file
		fclose(file);
	}
}
