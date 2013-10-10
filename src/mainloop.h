/**
 * \file mainloop.h
 * \brief Fichier d'entête, boucle de l'affichage
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.1
 * \date 29/12/2012
 * 
 * Contient toutes les instructions de la boucle principale
 * 
 */

#ifndef H_MAINLOOP
#define H_MAINLOOP

#include "engine.h"
#include "complexFractals.h"
#include "recursiveFractals.h"
#include <time.h>

/**
 * \def ZOOM_PITCH
 * \brief Epaisseur du viseur en pixels
 */
#define ZOOM_PITCH 0.05

/**
 * \def ZOOM_DEFAULT
 * \brief Taille du cadre de zoom par défaut (0 à 1)
 */
#define ZOOM_DEFAULT 0.25

/**
 * \def ZOOM_MIN
 * \brief Taille du cadre de zoom minimum (0 à 1)
 */
#define ZOOM_MIN 0.05

/**
 * \def ZOOM_MAX
 * \brief Taille du cadre de zoom maximum (0 à 1)
 */
#define ZOOM_MAX 1.0

/**
 * \def MAX_FRAMERATE
 * \brief Nombre d'image par seconde maximum
 * \remark Le nombre d'images par secondes (IPS) peut varier selon les performances de la machine.
 */
#define MAX_FRAMERATE 50

/**
 * \def DEFAULT_RECURSIVE_ITERATION
 * \brief Nombre d'itération par défaut pour les fractales récursives
 */
#define DEFAULT_RECURSIVE_ITERATION 1

/**
 * \def DEFAULT_NEWTON_POWER
 * \brief Puissance par défaut de la fractale de Newton
 */
#define DEFAULT_NEWTON_POWER 3

/* Structures */

/**
 * \struct dataImages
 * \brief Stocke les images du programme
 * 
 * La structure \e dataImages permet de stocker les images utilisés dans le programme.
 * 
*/
typedef struct
{
	image	*menuRecursive;		/*!< Image droite du menu principal */
	image	*menuComplex;		/*!< Image gauche du menu principal */
	image	*helpComplex;		/*!< Image d'aide des fractales complexes */
	image	*helpRecursive;		/*!< Image d'aide des fractales récursives */
	image	*docMandelbrot;		/*!< Image de description de la fractale de Mandelbrot */
	image	*docJulia;		/*!< Image de description de la fractale de Julia */
	image	*docBurningShip;	/*!< Image de description de la fractale du "burning ship" */
	image	*docNewton;		/*!< Image de description de la fractale de Newton */
	image	*docLevy;		/*!< Image de description de la fractale de Levy */
	image	*docSierpinskiTriangle;	/*!< Image de description de la fractale de Sierpinski (Triangle) */
	image	*docSierpinskiCarre;	/*!< Image de description de la fractale de Sierpinski (Carré) */
} dataImages;



/* Prototypes */

void loadingImages(dataImages *images, engineSettings *engine);
bool checkImageClick(image *img, int mouseX, int mouseY);
void initZoom(fractalSettings *zoom, engineSettings *engine, int fractalId);
void drawHelp(dataImages *images, int fractalId);

#endif /* H_MAINLOOP */
