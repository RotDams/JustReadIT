#include "line_manipulation.h"
#include "../utils.h"
#include "../../list_manipulation/index.h"
#include "../segmentation.h"

int get_words_space(SDL_Surface *image) {
    unsigned int max_count = 0;
    unsigned int min_count = image->w;
    unsigned int count = 0;
    for (int x = 0; x < image->w; ++x) {

        if (is_blank_column(image, x)) {
            count++;
        } else {
            if (count > max_count)
                max_count = count;
            if (count < min_count)
                min_count = count;
            count = 0;
        }
    }
    return max_count - (min_count) / 2;
}

List get_letters(SDL_Surface *image) {
    image = GetAllText(image, 10);
    List letters_list = create_list();
    int save_index = 0;
    for (int x = 0; x <= image->w; ++x) {
        //SDL_SaveBMP(image,"Coco1.jpg");
        if (is_blank_column(image, x) || x == image->w) {
            if (x == save_index) {
                save_index += 1;
            } else {
                letters_list = push_back_list(letters_list,
                                              (void *) CutImage(image, save_index, 0, x - save_index, image->h),
                                              ElementType);
                SDL_SaveBMP(image, "CurrentWord.jpg");
                SDL_SaveBMP(GetAllText(CutImage(image, save_index, 0, x - save_index, image->h), 10), "CurrentLetters.jpg");
                save_index = x + 1;
            }
        }
    }
    return letters_list;
}

List get_words_and_letters(SDL_Surface *image) {
    image = GetAllText(image, 10);
    int space_limite = get_words_space(image)-6;
    space_limite -= (int)(space_limite* 0,1);
   // space_limite = (int) (space_limite *0,90);
    //SDL_SaveBMP(image,"Coco.jpg");
    // SDL_SaveBMP(GetAllText(image,10),"Cocospace.jpg");

    List words_list = create_list();
    int save_index = 0;
    int blank_count = 0;

    for (int x = 0; x <= image->w; ++x) {
        if (x != image->w && is_blank_column(image, x))
            blank_count += 1;
        else
            blank_count = 0;
        if (blank_count >= space_limite || x== image->w) {
            words_list = push_back_list(words_list,
                                        (void *) get_letters(CutImage(image, save_index, 0, x - save_index, image->h)),
                                        ListType);
            save_index = x + 1;
            blank_count = 0;
        }
    }
    return words_list;
}


