#include <SDL/SDL.h>
#include "libs/imageSegmentation/index.h"
#include "libs/list_manipulation/index.h"

int main() {
    SDL_Surface* image =extract_text("src/assets/Tour_eiffel_meme.bmp",2);
    SDL_SaveBMP (image, "textOUT.jpg");
    GetParagraphAndLine(image,40);
    return 0;
}
