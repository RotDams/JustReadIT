#include <math.h>
#include "../utils.h"
#include "../../list_manipulation/index.h"
#include "../segmentation.h"
#include "../index.h"

// take the space between 2 words
int get_words_space(SDL_Surface *image) {

    unsigned int max_count = 0;
    unsigned int min_count = image->w;
    unsigned int count = 0;

    // Navigate into the line and count the max white space without black
    for (int x = 0; x < image->w; ++x) {
        if (is_blank_column(image, x)) {
            count++;
        } else {
            if (count > max_count)
                max_count = count;
            if (count < min_count && count != 0)
                min_count = count;
            count = 0;
        }
    }

    if (min_count >= max_count - 5)
        return 666;
    return max_count - (min_count) / 2 ;
}


// Get a word and cut it in letters
List get_letters(SDL_Surface *image) {

    // Remove the white in excess
    //SDL_Surface* image = get_all_text(imag, 250);

    List letters_list = create_list();
    int save_index = 0;

    // Browse the line
    for (int x = 0; x <= image->w; x++) {

        // If we are enter 2 words
        if (is_blank_column(image, x) || x == image->w) {

            // We was already in the space
            if (x == save_index) {
                save_index += 1;
            } else {
                // Push the letter into the list
                SDL_Surface*ni = malloc(sizeof(SDL_Surface));

                SDL_Surface *new_img = cut_image(image, save_index, 0, x - save_index, image->h);
                new_img = get_all_text(new_img,50);
                new_img = resize_image(new_img, 32);
                show_image(SDL_ConvertSurface(new_img, new_img->format, SDL_SWSURFACE), 1);
                letters_list = push_back_list(letters_list,
                                              (SDL_Surface *) new_img,
                                              LetterType);
                save_index = x + 1;
            }
        }
    }
    return letters_list;
}


// Get a line and cut it in words
List get_words_and_letters(SDL_Surface *image) {
    extern PresentationState *presentationState;

    // Remove all white excess and get space
    image = get_all_text(image, 50);
    int space_limite = get_words_space(image);
    space_limite -= (int) (space_limite * 0.4);

    // Init
    int wait_for_black_column = 0;
    List words_list = create_list();
    int save_index = 0;
    int blank_count = 0;

    // Browse the line
    for (int x = 0; x <= image->w; x++) {

        //Count the number of the curernt white space
        if (wait_for_black_column && !is_blank_column(image, x) )
            wait_for_black_column = 0;

        if (x != image->w && is_blank_column(image, x) && !wait_for_black_column)
            blank_count += 1;
        else
            blank_count = 0;

        // If the current space is bigger than the limite
        if (blank_count >= space_limite || x == image->w) {

            // Change the word to List of letters
            // and push it into the List
            SDL_Surface *new_img = cut_image(image, save_index, 0, x - save_index, image->h);
            show_image(new_img, 1);

            words_list = push_back_list(words_list,
                                        (void *) get_letters(new_img),
                                        WordType);

            presentationState->data[1] = fmax(0, presentationState->data[1] - 1);
            wait_for_black_column =1;
            save_index = x + 1;
            blank_count = 0;
        }
    }
    return words_list;
}


