#include <SDL/SDL.h>

#ifndef JUSTREADIT_ROTATION_H
#define JUSTREADIT_ROTATION_H

struct result {
    double rho;
    double theta;
};



/* vote
 * Input:
 *      - maxRho                            => int (the max value of rho)
 *      - maxIndexTheta                     => int (the max value of rho)
*       - maxIndexRho                       => int (the max value of rho)
 *      - acc[maxIndexTheta][maxIndexRho]   => int[][] (the max value of rho)
 *
 * Description:
 *      - Return the rho and theta value
 * */
struct result *winner(int maxRho, int maxIndexTheta, int maxIndexRho, int acc[maxIndexTheta][maxIndexRho]);


/* vote
 * Input:
 *      - x                                 => int (the x position of the pixel)
 *      - y                                 => int (the y position of the pixel)
 *      - maxRho                            => int (the max value of rho)
 *      - maxIndexTheta                     => int (the max value of rho)
*       - maxIndexRho                       => int (the max value of rho)
 *      - acc[maxIndexTheta][maxIndexRho]   => int[][] (the max value of rho)
 *      - img                               => SDL_Surface ()
 *
 * Description:
 *      - Add a vote to calc degree
 * */
void vote(int x, int y, int maxRho, int maxIndexTheta, int maxIndexRho, int acc[maxIndexTheta][maxIndexRho],
          SDL_Surface *img);


/* find_angle
 * Input:
 *      - *image    => SDL_Surface (the image to get angle)
 *
 * Return:
 *      - double    => the angle (in deg) to rotate the image
 *
 * Description:
 *      - Calc the number of degree that we must rotate the image
 * */
double find_angle(SDL_Surface *image);


/* SDL_Rotate
 * Input:
 *      - *origine      => SDL_Surface (the image to rotate)
 *      - angle         => float (an angle in degree to rotate)
 *
 * Return:
 *      - SDL_Surface   => the angle (in deg) to rotate the image
 *
 * Description:
 *      - Return a rotated image
 * */
SDL_Surface *SDL_RotateImage(SDL_Surface *origine, float angle);
#endif
