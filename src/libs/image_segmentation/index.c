#include "index.h"

List image_segmentation(char* image_path) {
    SDL_Surface *image = extract_text(image_path, 2);
    List paragraphs = get_paragraphs(image);
    return paragraphs;
}