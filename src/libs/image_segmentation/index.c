#include "index.h"
#include "utils.h"
#include <string.h>
#include "../neural_network/index.h"
#include "../../main.h"

//Keep the List of all bmp words
List image_segmentation(char *image_path) {
    SDL_Surface *image = extract_text(image_path, 20);
    List paragraphs = get_paragraphs(image);
    SDL_Quit();
    return paragraphs;
}

char filesname[] = "letter000.bmp";
extern Bool save_letter;

void build_word(List letters, char **content) {
    Node *letter = letters->first;

    while (letter != NULL) {

        //save the letter
        if (save_letter) {
            if (filesname[8] == '9') {
                filesname[8] = '0';
                filesname[7]++;
            } else if (filesname[7] == '9') {
                filesname[7] = '0';
                filesname[6]++;
            } else filesname[8]++;
            SDL_SaveBMP((SDL_Surface *) (letter->value), filesname);
        }

        char new_content = get_letter((SDL_Surface *) (letter->value));
        char *c = calloc(2, sizeof(char));
        c[0] = new_content;
        *content = strcat(*content, c);

        // Get the next elements
        letter = letter->next;
    }
}

void build_line(List words, char **content) {
    Node *word = words->first;

    while (word != NULL) {
        char *new_content = calloc(10000, sizeof(char));
        build_word((List) word->value, &new_content);

        *content = strcat(*content, new_content);

        if (word->next != NULL)
            *content = strcat(*content, " ");

        // Get the next elements
        word = word->next;
    }
}

void build_paragraph(List lines, char **content) {
    Node *line = lines->first;

    while (line != NULL) {
        char *new_content = calloc(10000, sizeof(char));
        build_line((List) line->value, &new_content);

        *content = strcat(*content, new_content);

        // Get the next elements
        line = line->next;
    }
}

char *build_text(char *path) {
    List paragraphs = image_segmentation(path);
    char *content = calloc(1000000, sizeof(char));
    Node *paragraph = paragraphs->first;

    while (paragraph != NULL) {
        char *new_content = calloc(10000, sizeof(char));
        build_paragraph((List) paragraph->value, &new_content);

        content = strcat(content, new_content);

        if (paragraph->next != NULL)
            content = strcat(content, "\n\n");

        paragraph = paragraph->next;
    }

    return content;
}