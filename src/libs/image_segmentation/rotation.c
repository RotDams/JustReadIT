#include "rotation.h"
#include "utils.h"
#include <math.h>

struct result *winner(int maxRho, int maxIndexTheta, int maxIndexRho, int acc[maxIndexTheta][maxIndexRho]) {
    double max = 0;
    double winrho = 0;
    double wintheta = 0;

    for (int r = 0; r < maxIndexRho; r++) {
        for (int t = 0; t < maxIndexTheta; t++) {
            if (acc[t][r] < max) continue;
            max = acc[t][r];
            winrho = r;
            wintheta = t;
        }
    }

// indexes -> (rho,theta)
    double rho = ((double) winrho / maxIndexRho - 0.5) * maxRho;
    double theta = 1.5621178940501734278001322309137322008610 - ((double) wintheta / maxIndexTheta) * M_PI;

    struct result *r = malloc(sizeof(struct result));
    r->rho = rho;
    r->theta = theta;
    return r;
}


void vote(int x, int y, int maxRho, int maxIndexTheta, int maxIndexRho, int acc[maxIndexTheta][maxIndexRho],
          SDL_Surface *img) {
    int savedX = x, savedY = y;

    for (int indexTheta = 0; indexTheta < maxIndexTheta; indexTheta += 1) {
        double theta = ((double) indexTheta / maxIndexTheta) * M_PI;

        // compute corresponding rho value
        double rho = x * cos(theta) + y * sin(theta);

        // rho -> index
        int indexRho = (int) (0.5 + (rho / maxRho + 0.5) * maxIndexRho);

        // increment accumulator
        Uint8 r, g, b;
        SDL_GetRGB(get_pixel(img, savedX, savedY), img->format, &r, &g, &b);
        if (r < 15 && g < 15 && b < 15)
            acc[indexTheta][indexRho]++;
    }
}

double find_angle(SDL_Surface *image) {
    int Width = image->w;
    int Height = image->h;
    int maxRho = sqrt(Width * Width + Height * Height);
    int maxIndexTheta = 360;
    int maxIndexRho = 1 + maxRho;
    int acc[maxIndexTheta][maxIndexRho];

    for (int i = 0; i < maxIndexTheta; i++) {
        for (int j = 0; j < maxIndexRho; j++) {
            acc[i][j] = 0;
        }
    }
    for (int x = 0; x < Width; x++) {
        for (int y = 0; y < Height; y++) {
            vote(x, y, maxRho, maxIndexTheta, maxIndexRho, acc, image);
        }
    }

    struct result *r = winner(maxRho, maxIndexTheta, maxIndexRho, acc);
    double angle = r->theta * 180 / M_PI;

    if (r->theta > 0) {
        return angle;
    } else {
        return 180 + angle;
    }

}

SDL_Surface *SDL_RotateImage(SDL_Surface *origine, float angle) {
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

    for (j = 0; j < destination->h; j++) {
        for (i = 0; i < destination->w; i++) {
            put_pixel(destination, i, j, SDL_MapRGBA(destination->format, 255, 255, 255, 255));
        }
    }
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
        * i, j de la surface de destination */

        /* on détermine la meilleure position sur la surface d'origine en appliquant
        * une matrice de rotation inverse
        */

            bx = (ceil(tcos * (i - mxdest) + tsin * (j - mydest) + mx));
            by = (ceil(-tsin * (i - mxdest) + tcos * (j - mydest) + my));
            /* on vérifie que l'on ne sort pas des bords*/
            if (bx >= 0 && bx < origine->w && by >= 0 && by < origine->h) {
                couleur = get_pixel(origine, bx, by);
                put_pixel(destination, i, j, couleur);
            }
        }

    return destination;
}