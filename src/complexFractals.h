/**
 * \file complexFractals.h
 * \brief Fichier d'entête, contient toutes les fractales de type complexe
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 2.0
 * \date 02/02/2012
 * 
 * Dessine les fractales complexes dans le plan Complexe, ou la partie
 * réelle est sur l'axe des abscisses et la partie imaginaire est sur
 * l'axe des ordonnées.
 * 
 */

#ifndef H_COMPLEX_FRACTALS
#define H_COMPLEX_FRACTALS

#include "newton.h"
#include "engine.h"

/* Macros */

/**
 * \def COMPLEX_MANDELBROT
 * \brief Identifiant de la fractale de mandelbrot
 */
#define COMPLEX_MANDELBROT 1

/**
 * \def COMPLEX_BURNING_SHIP
 * \brief Identifiant de la fractale du "burning ship"
 */
#define COMPLEX_BURNING_SHIP 2

/**
 * \def COMPLEX_JULIA
 * \brief Identifiant de la fractale de Julia
 */
#define COMPLEX_JULIA 3

/**
 * \def COMPLEX_NEWTON
 * \brief Identifiant de la fractale de Newton
 */
#define COMPLEX_NEWTON 4

/* Prototypes */

void computeMandelbrot(fractalSettings fSet);
void computeBurningShip(fractalSettings fSet);
void computeJulia(fractalSettings fSet, complex c);
void computeNewton(fractalSettings fSet, int power);


#endif /* H_COMPLEX_FRACTALS */
