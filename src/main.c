#include <SDL/SDL.h>
#include <unistd.h>
#include "libs/image_segmentation/index.h"
#include "libs/neural_network/neuralnetwork.h"

int main(){//int argc, char *argv[]) {

//    if (argc == 1) {
//        printf("Error: Please specify the path of the image\nEx: ./output.exe -p \"my_image.bmp\"\n");
//        return EXIT_FAILURE;
//    }
//
//    // List of all possible arguments
//    char *path = "";
//    int is_dev_mode = 0;
//
//    // for each arg
//    int i = 1;
//    while (i < argc) {
//        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dev") == 0) {
//            is_dev_mode = 1;
//
//        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--path") == 0) {
//            i++;
//            if (i >= argc) {
//                printf("Error: Please specify the path of the image\nEx: ./output.exe -p \"my_image.bmp\"\n");
//                return EXIT_FAILURE;
//            }
//            path = argv[i];
//        }
//        i++;
//    }
//
//    if (access(path, F_OK) == -1) {
//        printf("Error: The file does not exist.\n");
//        return EXIT_FAILURE;
//    }

	NeuralNetwork *pointer = NULL, network;
	network = Load_Neural_Network();
	pointer = &network;

	printf("Value : %f\n",pointer->neurons[0].value);
	printf("Links : %f - %f\n",pointer->neurons[0].link[0], pointer->neurons[0].link[1]);

	/*int entry[2];
	entry[0] = 1;
	entry[1] = 0;*/

	//Run(pointer,entry,2);

	//Save_Neural_Network(pointer);

	/*

	for(int i =0;i<100;i++)
	{
		Learn(pointer,entry,1);
	}

	printf("\n=====================================\n\n");
	entry[1] = 0;
	entry[0] = 0;
	for(int i =0;i<100;i++)
	{
		Learn(pointer,entry,0);
	}

	printf("\n=====================================\n\n");
	entry[1] = 0;
	entry[0] = 0;
	for(int i =0;i<100;i++)
	{
		Learn(pointer,entry,0);
	}

	printf("\n=====================================\n\n");
	entry[1] = 1;
	entry[0] = 0;
	for(int i =0;i<100;i++)
	{
		Learn(pointer,entry,1);
	}

	printf("\n=====================================\n\n");
	entry[1] = 1;
	entry[0] = 1;
	for(int i =0;i<100;i++)
	{
		Learn(pointer,entry,0);
	}

	printf("\n=====================================\n\n");
	entry[1] = 1;
	entry[0] = 0;
	for(int i =0;i<100;i++)
	{
		Learn(pointer,entry,1);
	}

	printf("\n=====================================\n\n");
	entry[1] = 1;
	entry[0] = 0;
	for(int i =0;i<100;i++)
	{
		Learn(pointer,entry,1);
	}

	printf("\n=====================================\n\n");
	entry[1] = 0;
	entry[0] = 0;
	for(int i =0;i<100;i++)
	{
		Learn(pointer,entry,0);
	}

	entry[0] = 1;
	entry[1] = 0;
	Run(pointer,entry,2);*/


    //List paragraphs = image_segmentation("src/assets/image_white_bg.bmp");
    //print_list(paragraphs);
    return 0;
}
