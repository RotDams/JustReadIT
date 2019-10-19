#include <SDL/SDL.h>
#include <unistd.h>
#include "libs/image_segmentation/index.h"
#include "libs/neural_network/neuralnetwork.h"
#include "libs/neural_network/index.h"


int main(int argc, char *argv[]) {

	if (argc == 1) {
		printf("Error: Please specify the path of the image\nEx: ./output.exe -p \"my_image.bmp\"\n");
		return EXIT_FAILURE;
	}
	// List of all possible arguments
	char *path = "";

  // for each arg
	int i = 1;
	while (i < argc) {
		if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--path") == 0) {
			i++;
			if (i >= argc) {
				printf("Error: Please specify the path of the image\nEx: ./output.exe -p \"my_image.bmp\"\n");
				return EXIT_FAILURE;
			}
			path = argv[i];
		}
		i++;
	}
	if (access(path, F_OK) == -1) {
		printf("Error: The file does not exist.\n");
		return EXIT_FAILURE;
	}

	List paragraphs = image_segmentation(path);
	print_list(paragraphs);
  xor_neural_network(1000000, 0, 0);
	return 0;

}
