#include <SDL/SDL.h>
#include "libs/imageSegmentation/index.h"
#include "libs/list_manipulation/index.h"

int main() {
    //ExecAndSAve("src/assets/test-image-1.bmp", "image2.bmp");

    List list = create_list();

    print_list(list);
    printf("Taille de la liste: %lu\n", get_length(list));

    list = push_back_list(list, 14, ListType);
    print_list(list);
    printf("Taille de la liste: %lu\n", get_length(list));

    list = push_back_list(list, 6, ListType);
    list = push_back_list(list, 23, ListType);
    print_list(list);
    printf("Taille de la liste: %lu\n", get_length(list));

    list = pop_front_list(list);
    list = pop_front_list(list);
    print_list(list);
    printf("Taille de la liste: %lu\n", get_length(list));

    list = pop_front_list(list);
    print_list(list);
    printf("Taille de la liste: %lu\n", get_length(list));

    return 0;
}
