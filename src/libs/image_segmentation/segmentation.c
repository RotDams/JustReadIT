#include "segmentation.h"
#include "utils.h"
#include "SDL/SDL_rotozoom.h"


/*permet de déterminer la valeur d'un pixel au position x,y*/
Uint32 SDL_LirePixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

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
            return 0;
    }

}


/*permet d'écrire un pixel au position x,y*/
void SDL_EcrirePixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;


    switch (bpp) {
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


/*effectue une rotation centrale, alloue automatiquement la mémoire*/
SDL_Surface *SDL_RotationCentralN(SDL_Surface *origine, float angle) {
    SDL_Surface *destination;
    int i;
    int j;
    Uint32 couleur;
    int mx, my, mxdest, mydest;
    int bx, by;
    float angle_radian;
    float tcos;
    float tsin;
    double largeurdest;
    double hauteurdest;

/* détermine la valeur en radian de l'angle*/
    angle_radian = -angle * M_PI / 180.0;

/*pour éviter pleins d'appel, on stocke les valeurs*/
    tcos = cos(angle_radian);
    tsin = sin(angle_radian);

/*calcul de la taille de l'image de destination*/
    largeurdest = ceil(origine->w * fabs(tcos) + origine->h * fabs(tsin)),
    hauteurdest = ceil(origine->w * fabs(tsin) + origine->h * fabs(tcos)),


/*
 * alloue la mémoire à l'espace de destination, attention,
 * la surface est de même taille
 */
    destination = SDL_CreateRGBSurface(SDL_HWSURFACE, largeurdest, hauteurdest, origine->format->BitsPerPixel,
                                       origine->format->Rmask, origine->format->Gmask, origine->format->Bmask,
                                       origine->format->Amask);

    /*on vérifie que la mémoire a été allouée*/
    if (destination == NULL)
        return NULL;

    /*calcul du centre des images*/
    mxdest = destination->w / 2.;
    mydest = destination->h / 2.;
    mx = origine->w / 2.;
    my = origine->h / 2.;

    for (j = 0; j < destination->h; j++)
        for (i = 0; i < destination->w; i++) {
/* on détermine la valeur de pixel qui correspond le mieux pour la position
 * i,j de la surface de destination */

/* on détermine la meilleure position sur la surface d'origine en appliquant
 * une matrice de rotation inverse
 */

            bx = (ceil(tcos * (i - mxdest) + tsin * (j - mydest) + mx));
            by = (ceil(-tsin * (i - mxdest) + tcos * (j - mydest) + my));
            /* on vérifie que l'on ne sort pas des bords*/
            if (bx >= 0 && bx < origine->w && by >= 0 && by < origine->h) {
                couleur = SDL_LirePixel(origine, bx, by);
                SDL_EcrirePixel(destination, i, j, couleur);
            }
        }

    return destination;
}


SDL_Surface *extract_text(char *source_location, int threshold) {
    SDL_Surface *image;
    Uint8 r, g, b;

    image = SDL_LoadBMP(source_location);


    image = SDL_RotationCentralN(image, 0);

    show_image(image, 0);

    put_in_black_and_white(image);

    show_image(image, -1);

    if (image == NULL) {
        printf("Fail with loading image");
        return NULL;
    }

    // Browse all pixels of the image
    for (int x = 0; x < image->w; ++x) {
        for (int y = 0; y < image->h; ++y) {

            // Take the color of the current pixel
            SDL_GetRGB(get_pixel(image, x, y), image->format, &r, &g, &b);

            // If the current pixel is not black -> convert white
            if (!(r <= threshold && g <= threshold && b <= threshold)) {
                put_pixel(image, x, y, SDL_MapRGBA(image->format, 255, 255, 255, 255));
            }
        }
    }
    // Corrects the finishes of pixels
    // image = correct_image(image, threshold + 2);
    return get_all_text(image, threshold);
}

SDL_Surface *get_all_text(SDL_Surface *image, int Seuil) {
    // default values
    int left = image->w;
    int top = image->h;
    int bottom = 0;
    int right = 0;

    //Find the borders of the text in the image
    Uint8 r, g, b;


    // Browse all pixels of the image
    for (int y = 0; y < image->h; ++y) {
        for (int x = 0; x < image->w; ++x) {

            // get the current pixel color
            SDL_GetRGB(get_pixel(image, x, y), image->format, &r, &g, &b);

            // take the border of the text
            if (r <= Seuil && x < left)
                left = x;

            if (r <= Seuil && x > right)
                right = x;

            if (r <= Seuil && y > bottom)
                bottom = y;

            if (r <= Seuil && y < top)
                top = y;
        }
    }

    //Create a new image with the good scale
    SDL_Surface *newImage = cut_image(image, left, top, right - left + 1, bottom - top + 1);
    return newImage;
}
