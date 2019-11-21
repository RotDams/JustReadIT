#include <err.h>
#include <math.h>
#include "../../main.h"
#include "utils.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"


Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y) {
	int bpp = surf->format->BytesPerPixel;
	return (Uint8 *) surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
	int bpp = surface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = pixel_ref(surface, x, y);

	switch (bpp) {
		case 1:
			return *p;

		case 2:
			return *(Uint16 *) p;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *) p;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}


SDL_Surface *cut_image(SDL_Surface *Source, int startX, int startY, int W, int H) {
	SDL_Rect cut, pos;
	SDL_Surface *screen;

	//Init of the new image
	screen = SDL_CreateRGBSurface(255, W, H, 32,
			0, 0,
			0, 0);

	//init of the start of the edition
	pos.x = 0;
	pos.y = 0;

	cut.x = startX; // X start
	cut.y = startY; // Y start
	cut.w = W;  // width
	cut.h = H;  // height

	SDL_BlitSurface(Source, &cut, screen, &pos);

	// Save image, to allow user to see the image after manipulation
	//  SDL_SaveBMP (screen, "ImageTest.bmp");      //TODO Debug mode

	return screen;
}


int is_blank_line(SDL_Surface *image, int height) {
	int imageHeight = image->h;
	int imageWidth = image->w;

	// If the height in parameter is superior than the size of the image
	if (height > imageHeight) {
		return 0;
	}

	// Else, check for all pixel
	Uint8 r, g, b;
	for (int i = 0; i < imageWidth; i++) {
		// Get RGB color
		SDL_GetRGB(get_pixel(image, i, height), image->format, &r, &g, &b);

		// Check if one of the RGB color is inferior than 250 (we aspect to have blank pixel)
		if (r < 250 || g < 250 || b < 250) {
			return 0;
		}
	}

	// This is a white line
	return 1;
}

// Check if the column "x" is white
int is_blank_column(SDL_Surface *image, int width) {
	int imageHeight = image->h;
	int imageWidth = image->w;

	if (width > imageWidth) {
		return 0;
	}

	Uint8 r, g, b;
	// check if the current column is blank
	for (int i = 0; i < imageHeight; i++) {
		SDL_GetRGB(get_pixel(image, width, i), image->format, &r, &g, &b);

		if (r < 250 || g < 250 || b < 250) {
			return 0;
		}
	}
	return 1;
}

// Fix the current pixel on the surface
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel) {
	Uint8 *p = pixel_ref(surface, x, y);

	switch (surface->format->BytesPerPixel) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *) p = pixel;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *) p = pixel;
			break;
	}
}

// Check and correct if black pixel are solo into the image
SDL_Surface *correct_image(SDL_Surface *image, int Seuil) {
	Uint8 r, g, b;
	int bool;

	// Browse the current image
	for (int x = 2; x < image->w - 2; ++x) {
		for (int y = 2; y < image->h - 2; ++y) {

			// Pick the current pixel color
			SDL_GetRGB(get_pixel(image, x, y), image->format, &r, &g, &b);

			// if the pixel is black check around and count the others black pixels
			if ((r <= Seuil && g <= Seuil && b <= Seuil)) {
				bool = 0;
				for (int i = x - 2; i <= x + 2; ++i) {
					for (int j = y - 2; j <= y + 2; ++j) {
						SDL_GetRGB(get_pixel(image, i, j), image->format, &r, &g, &b);
						if ((r <= Seuil && g <= Seuil && b <= Seuil)) {
							bool += 1;
						}
					}
				}
				// if there is less that 4 black pixel around, make it white
				if (bool < 5)
					put_pixel(image, x, y, SDL_MapRGBA(image->format, 255, 255, 255, 255));
			}
		}
	}
	return image;
}


void show_image(SDL_Surface *image, int id) {
    // TODO https://stackoverflow.com/questions/3741055/inputs-in-sdl-on-key-pressed
    extern PresentationState dev_mode;

    if ( id != -1 && (id >= dev_mode.size || !dev_mode.data[id])) {
        return;
    } else {
        if (id != -1)
            dev_mode.data[id] = 0;
    }



    SDL_Surface* screen_surface;

    screen_surface = display_image(image);

    wait_for_keypressed();

    SDL_FreeSurface(screen_surface);

    if (id == 5) {
        img_to_array(image, 28);
    }
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
             img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}


void img_to_array(SDL_Surface *image, int length) {
    // Get the max dimension between height and width
    int max = fmax(image->h, image->w);

    // Calculate ratio (to pass to the length pixel)
    double ratio = (double) length / (double) max;

    // Get the new image
    image = rotozoomSurface(image, 0, ratio,1);

    // Create the new array
    double array[length * length];

    Uint8 r,g,b;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (i >= image->h || j >= image->h) {
                array[i*length + j] = 0;
            } else {
                Uint32 pixel = (get_pixel(image, i,j));
                SDL_GetRGB(pixel, image->format, &r,&b,&g);
                double average = (double) ((255 - r) + (255 - g) - (255 - b)) / (double) 3;
                array[i*length + j] = (double) average / 255;
            }
        }
    }

    printf("%f\n", array[0]);

    for (int i = 0; i < image->w; i++) {
        for (int j = 0; j < image->h; j++) {
            Uint32 pixel = (get_pixel(image, i,j));
            SDL_GetRGB(pixel, image->format, &r,&b,&g);
            if (r > 200)
                printf("%d\n", r);
        }
    }

    show_image(image, -1);
}