/**
 * \file newton.h
 * \brief Fichier d'entête, contient les fonctions permettant de construire la fractale de Newton
 * \author Timothée NICOLAS 
 * \version 1.0
 * \date 02/02/2012
 * 
 * Dessine la fractale de Newton dans le plan complexe, où la partie
 * réelle est sur l'axe des abscisses et la partie imaginaire est sur
 * l'axe des ordonnées.
 * 
 */
 
#ifndef H_NEWTON
#define H_NEWTON 

#include "engine.h"

/* Macros */

/**
 * \def FRACTAL_NEWTON_PRECISION
 * \brief Valeur à la quelle on estime qu'il y a convergence de la suite
 * 
 * Par défaut fixé à 1E-4
 * 
 */
#define FRACTAL_NEWTON_PRECISION 1E-4



/* Prototypes */
bool newtonCheckRoots(complex *roots, complex z, int power);
void newtonComputeRoots(complex *roots, int power);
void newtonDrawRoots(complex *roots, complex z, int power,
			int x, int y, int iteration, int maxIteration);

#endif /* H_NEWTON */
