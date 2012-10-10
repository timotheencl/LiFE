/**
 * \file main.c
 * \brief Fichier source, Entrée principale du programme
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.0
 * \date 26/12/2012
 * 
 * Contient toutes les fonctions principales du programme
 * 
 */

#include "engine.h"

/**
 * \fn int main(void);
 * \brief Entrée du programme
 * 
 * \return EXIT_SUCCESS soit 0.
 */
int main(void)
{

	/* Création de la fenêtre */
	createWindow(1280, 720, "LiFE v1.2");
	
	/* Entrée dans la boucle pseudo-infinie */
	startLoop();
	
	/* Destruction de la fenêtre */
	destroyWindow();

	/* On sort du programme */
	return EXIT_SUCCESS;
}

/** \mainpage Documentation du code source
 *
 * 
 *
 * \section intro Introduction
 * 
 * LiFE est un logiciel d'exploration de fractales. Il permet donc de construire 
 * différentes fractales de type complexes et récursives. Un zoom intelligent 
 * facilite l'exploration des fractales. Ecrit en C ANSI, il
 * utilise la SDL pour la gestion de l'affichage et des évenements,
 * ainsi que OpenGL 2 pour le dessin des fractales.
 * 
 * \section contenu Contenu
 * 
 * LiFE permet de créer 2 types de fractales :
 * 
 * \subsection complex Fractales Complexes
 * 
 * - Ensemble de Mandelbrot (voir: http://fr.wikipedia.org/wiki/Ensemble_de_Mandelbrot)
 * - Ensemble de Julia (voir: http://fr.wikipedia.org/wiki/Ensemble_de_Julia)
 * - Fractale de Newton (voir: http://fr.wikipedia.org/wiki/Fractale_de_Newton)
 * - Fractale du Burning Ship (voir: http://fr.wikipedia.org/wiki/Fractale_burning_ship)
 * 
 * \subsection recursive Fractales Récursives
 * 
 * - Tapis de Sierpinski (voir: http://fr.wikipedia.org/wiki/Tapis_de_Sierpi%C5%84ski)
 * - Triangle de Sierpinski (voir: http://fr.wikipedia.org/wiki/Triangle_de_Sierpinski)
 * - Fracale de Lévy (voir: http://fr.wikipedia.org/wiki/Fractale_de_L%C3%A9vy)
 * 
 * \section compilation Instructions de compilation
 * 
 * -# Assurez-vous d'avoir installé toutes les bibliothèques nécessaires.
 * -# Placez-vous dans le dossier racine du projet.
 * -# Compilez à l'aide de la commande 'make'
 * -# Exécuter le programme avec la commande: ./LiFE
 * 
 * \section libs Bibliothèques
 * 
 * Les bibliothèques utilisés sont
 * 
 * - libc \e La bibliothèque C standard
 * - SDL 1.2 \e Pour la gestion des fenêtres et des évenements.
 * - SDL_Image 1.2 \e Pour charger différents formats d'image.
 * - SDL_ttf2.0 \e Pour écrire du texte à l'écran
 * - GL et GLU \e Pour utiliser OpenGL 2
 * 
 * <hr>
 * 
 * \date 13 février 2012
 * 
 * \authors Timothée NICOLAS	<timothee.nicolas@isen.fr>
 * \authors Nicolas NATIVO	<nicolas.nativo@isen.fr>
 * \authors Nicolas SILVAIN	<nicolas.silvain@isen.fr>
 * 
 */
