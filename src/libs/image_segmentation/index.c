#include "index.h"
#include "utils.h"

//Keep the List of all bmp words
List image_segmentation(char *image_path) {
    SDL_Surface *image = extract_text(image_path, 20);
    show_image(image, 1);
	List paragraphs = get_paragraphs(image);
	return paragraphs;
}
