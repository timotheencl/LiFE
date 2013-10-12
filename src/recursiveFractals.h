/**
 * \file recursiveFractals.h
 * \brief Fichier d'entête, contient les fonctions permettant de construire les fractales de type récursive
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO 
 * \version 1.3
 * \date 03/02/2012
 * 
 * Dessine les fractale de type récursive
 * 
 */

#ifndef H_RECURSIVE_FRACTALS
#define H_RECURSIVE_FRACTALS

#include "engine.h"


/**
 * \def RECURSIVE_LEVY
 * \brief Identifiant de la fractale de Levy
 */
#define RECURSIVE_LEVY 10

/**
 * \def RECURSIVE_SIERPINSKI_TRIANGLE
 * \brief Identifiant de la fractale de Sierpinski version triangle
 */
#define RECURSIVE_SIERPINSKI_TRIANGLE 11

/**
 * \def RECURSIVE_SIERPINSKI_CARRE
 * \brief Identifiant de la fractale de Sierpinski version carré
 */
#define RECURSIVE_SIERPINSKI_CARRE 12

/* Structures */

/**
 * \struct new_point
 * \brief Structure servant aux coordonnées des points
 * 
 * 
*/
typedef struct
{
	float abs;	/*!< Abscisse du point */
	float ord;	/*!< Ordonnée du point */
} new_point;
		
		

/* Prototypes */
	
void computeLevy(int LargeurFenetre, int HauteurFenetre, int iteration);
void create_levy(int iteration, float abscisse_X, float ordonnee_X, float abscisse_Y, float ordonnee_Y);
void enleveTriangle(float x1,float y1,int iterationmax,float segment);
void computeTriangleSierpinski(int LargeurFenetre, int HauteurFenetre,int iteration);
void enleveCarre(float x1,float y1,int iterationmax,float segment);
void computeCarreSierpinski(int LargeurFenetre, int HauteurFenetre,int iteration);

#endif /* H_RECURSIVE_FRACTALS */
