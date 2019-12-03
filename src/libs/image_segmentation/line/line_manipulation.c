#include "line_manipulation.h"
#include "../utils.h"
#include "../../list_manipulation/index.h"
#include "../segmentation.h"

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
    } // return the new limite

    if (min_count >= max_count - 5)
        return 666;
    return max_count - (min_count) / 2;
}


// Get a word and cut it in letters
List get_letters(SDL_Surface *image) {

    // Remove the white in excess
    image = get_all_text(image, 10);

    List letters_list = create_list();
    int save_index = 0;

    // Browse the line
    for (int x = 0; x <= image->w; ++x) {

        // If we are enter 2 words
        if (is_blank_column(image, x) || x == image->w) {

            // We was already in the space
            if (x == save_index) {
                save_index += 1;
            } else {
                // Push the letter into the list
                SDL_Surface *new_img = cut_image(image, save_index, 0, x - save_index, image->h);
                new_img = get_all_text(new_img,50);
                new_img = resize_image(new_img, 32);
                letters_list = push_back_list(letters_list,
                                              (void *) new_img,
                                              LetterType);
                save_index = x + 1;
            }
        }
    }
    return letters_list;
}


// Get a line and cut it in words
List get_words_and_letters(SDL_Surface *image) {

    // Remove all white excess and get space
    image = get_all_text(image, 10);
    int space_limite = get_words_space(image);
    space_limite -= (int) (space_limite * 0.4);

    // Init
    List words_list = create_list();
    int save_index = 0;
    int blank_count = 0;

    // Browse the line
    for (int x = 0; x <= image->w; ++x) {

        //Count the number of the curernt white space
        if (x != image->w && is_blank_column(image, x))
            blank_count += 1;
        else
            blank_count = 0;

        // If the current space is bigger than the limite
        if (blank_count >= space_limite || x == image->w) {

            // Change the word to List of letters
            // and push it into the List
            SDL_Surface *new_img = cut_image(image, save_index, 0, x - save_index, image->h);
            words_list = push_back_list(words_list,
                                        (void *) get_letters(new_img),
                                        WordType);
            save_index = x + 1;
            blank_count = 0;
        }
    }
    return words_list;
}


