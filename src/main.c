#include <SDL/SDL.h>
#include <unistd.h>
#include "libs/imageSegmentation/index.h"
#include "libs/list_manipulation/index.h"

int main(int argc, char *argv[]) {

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


    SDL_Surface *image = extract_text("src/assets/Tour_eiffel_meme.bmp", 2);
    List paragraphs = GetParagraph(image);
    SDL_SaveBMP (image, "textOUT.jpg");
    return 0;
}
