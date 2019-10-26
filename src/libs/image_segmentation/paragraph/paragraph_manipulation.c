#include <SDL/SDL.h>
#include "paragraph_manipulation.h"
#include "../utils.h"
#include "../../list_manipulation/index.h"
#include "../line/line_manipulation.h"


int get_paragraph_space(SDL_Surface *image) {
	// Set the 2 variables at 0
	int paragraphJumpSpace = 0;
	int lineHeight = 0;


	// Create a counter for the number of space
	int spaceCount = 0;
	int lineHeightCount = 0;

	// For all line
	for (int i = 0; i < image->h; i++) {

		if (is_blank_line(image, i)) {
			if (lineHeightCount != 0 && lineHeightCount > lineHeight) {
				lineHeight = lineHeightCount;
			}

			spaceCount++;
			lineHeightCount = 0;

		} else {
			// Check if we arrive in text part
			if (spaceCount != 0 && spaceCount > paragraphJumpSpace) {
				paragraphJumpSpace = spaceCount;
			}

			spaceCount = 0;
			lineHeightCount++;
		}
	}

	return paragraphJumpSpace;
}


List get_paragraphs_and_lines(SDL_Surface *image, int paragraphJumpSpace) {

	// Index to remind the start of the text
	int saved_index = 0;

	// index for the while
	int index = 0;

	// Count the number of white line
	int white_counter = 0;

	List paragraphs = create_list();
	List line = create_list();


	while (index < image->h) {
		// If the line is blank
		if (is_blank_line(image, index)) {

			// We were before on a black line
			if (white_counter == 0) {
				white_counter++;

				// Here we can insert line on the list ?
				SDL_Surface *new_img = cut_image(image, 0, saved_index, image->w, index - saved_index - 1);
				show_image(new_img, 3);
				List l = get_words_and_letters(new_img);
				line = push_back_list(line, (void *) l, LineType);

				// We are
			} else {
				white_counter++;
			}

			// Increment the index
			index++;

			// We are on a right line
		} else {
			// We are before on white line
			if (white_counter != 0) {
				saved_index = index;

				// We are in a new the paragraph
				if (white_counter > 0.9 * paragraphJumpSpace) {
					paragraphs = push_back_list(paragraphs, (void *) line, ParagraphType);
					line = create_list();
				}

				white_counter = 0;

				// Increment the index (5px to go after accent)
				index += 5;

				// We aren't before on white line
			} else {
				index++;
			}

		}
	}
	SDL_Surface *new_img = cut_image(image, 0, saved_index, image->w, index - saved_index - 1);
	List l = get_words_and_letters(new_img);
	line = push_back_list(line, (void *) l, LineType);
	paragraphs = push_back_list(paragraphs, (void *) line, ParagraphType);

	return paragraphs;
}

List get_paragraphs(SDL_Surface *image) {
	// Get the space between 2 paragraphs
	int paragraphJumpSpace = get_paragraph_space(image);

	// Get a list of paragraphs (a paragraphs is a list of lines)
	List paragraphs = get_paragraphs_and_lines(image, paragraphJumpSpace);

	return paragraphs;
}
