#include "index.h"
#include "utils.h"
#include <string.h>
#include "../neural_network/index.h"

//Keep the List of all bmp words
List image_segmentation(char *image_path) {
    SDL_Surface *image = extract_text(image_path, 20);
    List paragraphs = get_paragraphs(image);
    SDL_Quit();
    return paragraphs;
}


char number1 = '0';
char number2 = '0';
char number3 = '0';

void build_word(List letters, char **content, int bool) {
    Node *letter = letters->first;

    char filesname[] = "letter000.bmp"; //todo create letter

    while (letter != NULL) {
        if (bool) {
            filesname[8] = number3;
            filesname[6] = number2;
            filesname[7] = number1;
            if (number3 == '9') {
                number3 = '0';
                number1++;
            } else if (number1 == '9') {
                number1 = '0';
                number2++;
            } else number3++;

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
        build_word((List) word->value, &new_content,1); // change to not save

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