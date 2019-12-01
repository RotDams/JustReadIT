#include "libs/image_segmentation/rotation.h"
#include "libs/interface/interface.h"
#include "libs/neural_network/index.h"


int main(int argc, char *argv[]) {
    // Check if we want the interface
    printf("%c",get_letter(SDL_LoadBMP("src/assets/training/up_letter/L.bmp")));
    //training(26+26,4,100,0);
    init_interface(argc, argv);
    return 0;
}
