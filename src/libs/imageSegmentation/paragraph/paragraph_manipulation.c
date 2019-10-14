#include <SDL/SDL.h>
#include <stdio.h>
#include "../utils.h"
#include "../../list_manipulation/index.h"
#include "paragraph_manipulation.h"

void GetParagraphSpace(SDL_Surface *image, int *paragraphJumpSpace) {
    // Set the 2 variables at 0
    int lineJumpSpace = 0;
    *paragraphJumpSpace = 0;
    int lineHeight = 0;


    // Create a counter for the number of space
    int spaceCount = 0;

    int lineHeightCount = 0;

    // For all line
    for (int i = 0; i < image->h; i++) {

        if (IsBlankLine(image, i)) {
            if (lineHeightCount != 0 && lineHeightCount > lineHeight) {
                lineHeight = lineHeightCount;
            }

            spaceCount++;
            lineHeightCount = 0;

        } else {
            // Check if we arrive in text part
            if (spaceCount != 0 && spaceCount > *paragraphJumpSpace) {
                lineJumpSpace = *paragraphJumpSpace;
                *paragraphJumpSpace = spaceCount;
            }

            spaceCount = 0;
            lineHeightCount++;
        }
    }
}

void GetParagraphAndLine(SDL_Surface *image, int paragraphJumpSpace) {

    // Index to remind the start of the text
    int saved_index = 0;

    // index for the while
    int index = 0;

    // Count the number of white line
    int white_counter = 0;

    int p = 1;
    int l = 1;

    List paragraphs = create_list();
    List line = create_list();


    while (index < image->h) {
        // If the line is blank
        if (IsBlankLine(image, index)) {

            // We were before on a black line
            if (white_counter == 0) {
                white_counter++;

                // Here we can insert line on the list ?
                SDL_Surface *new_img = CutImage(image, 0, saved_index, image->w, index - saved_index - 1);
                line = push_back_list(line, (void *) new_img, ListType);

                // We are
            } else {
                white_counter++;
            }

            // Increment the index
           index++;

            // We are on a right line
        } else {
            // We are before on white line
            if (white_counter != 0) {
                saved_index = index;
                l++;

                // We are in a new the paragraph
                if (white_counter > 0.9 * paragraphJumpSpace) {
                    p++;
                    paragraphs = push_back_list(paragraphs, (void *) line, ListType);
                    line = create_list();
                }

                white_counter = 0;

                // Increment the index (5px to go after accent)
                index += 5;

                // We aren't before on white line
            } else {
                index++;
            }

        }
    }

    SDL_Surface *new_img = CutImage(image, 0, saved_index, image->w, index - saved_index - 1);
    line = push_back_list(line,(void * ) new_img, ListType);
    paragraphs = push_back_list(paragraphs, (void *)line, ListType);


    // Get a node
    // Node node1 = *((Node*) (paragraphs->first));
    // List lines_of_p1 = ((List) (node1.value));
    // SDL_Surface *img = ((SDL_Surface *) (lines_of_p1->first->value));

    Node current_paragraph_node = *((Node*) (paragraphs->first));

    while (current_paragraph_node.value) {

        Node current_line_node = *((List) (current_paragraph_node.value))->first;

        while (current_line_node.value != NULL) {
            SDL_Surface *img = ((SDL_Surface *) current_line_node.value);
            printf("Size: %d\n", img->h);

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
    }

    printf("There is %d paragraphs and %d lines\n", p, l);
}