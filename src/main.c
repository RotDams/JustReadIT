#include <SDL/SDL.h>
#include <unistd.h>
#include "libs/imageSegmentation/index.h"
#include "libs/imageSegmentation/line/line_manipulation.h"
#include "libs/list_manipulation/index.h"

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


    SDL_Surface *image = extract_text("src/assets/test_image_1.bmp", 2);
    List paragraphs = GetParagraph(image);
    printf("There is %lu paragraphs\n", paragraphs->length);
    print_list(paragraphs);
/*
    Node current_paragraph_node = *((Node*) (paragraphs->first));

    while (current_paragraph_node.value) {

        Node current_line_node = *((List) (current_paragraph_node.value))->first;

        while (current_line_node.value != NULL) {
            SDL_Surface *img = ((SDL_Surface *) current_line_node.value);

            List words = get_words_and_letters(img);

            current_line_node.value = &words;
            printf("Il y a %lu mots.\n", words->length);

            if (current_line_node.next == NULL) {
                current_line_node.value = NULL;
            } else {
                current_line_node = *(current_line_node.next);
            }
        }

        if (current_paragraph_node.next == NULL) {
            current_paragraph_node.value = NULL;
        } else {
            current_paragraph_node = *(current_paragraph_node.next);
        }
    }*/

    SDL_SaveBMP (image, "textOUT.jpg");
    return 0;
}
