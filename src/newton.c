/**
 * \file newton.c
 * \brief Fichier source, contient les fonctions permettant de construire la fractale de Newton
 * \author Timothée NICOLAS 
 * \version 1.0
 * \date 02/02/2012
 * 
 * Dessine la fractale de Newton dans le plan complexe, ou la partie
 * réelle est sur l'axe des abscisses et la partie imaginaire est sur
 * l'axe des ordonnées.
 * 
 */


#include "newton.h"


/**
 * \fn bool newtonCheckRoots(complex *roots, complex z, int power);
 * \brief Vérifie si la suite converge vers une des racines
 * 
 * \param *roots Tableau des racines du polynome précalculés
 * \param z Suite complexe calculé à partir du polynome
 * \param power Degré du polynome
 * \return Vrai si le polynome converge vers l'une des racines, Faux sinon.
 */
bool newtonCheckRoots(complex *roots, complex z, int power)
{
	int i;
	
	/* Pour chaque racines (il y a p racines, si la puissance du polynome est p) */
	/* Si la valeur absolue du nombre complexe Z calculé, est inférieur à une préscision donnée
	 * Alors la suite converge vers la racine.
	 * Donc on renvoie false pour sortir de la boucle */
	for (i=0; i < power; i++)
	{
		if (complexAbs(complexDif(z,roots[i])) <= FRACTAL_NEWTON_PRECISION ) 
			return false;
	}
	
	return true;
}

/**
 * \fn void newtonGetRoots(complex *roots, int power);
 * \brief Calcule les racines du polynome z^p - 1. En fonction du degré p
 * 
 * \param *roots Tableau stockant les futures racines du polynome
 * \param power Degré du polynome
 * \return Rien
 */
void newtonComputeRoots(complex *roots, int power)
{
	int i;
	
	/* Pour chaque racines du polynome, on calcule la partie réelle et imaginaire */
	for ( i=0; i < power; i++)
		roots[i] = complexSet(cos(2 * i * M_PI / power), sin(2 * i * M_PI / power) );
}

/**
 * \fn void newtonDrawRoots(complex *roots, complex z, int power, int x, int y, int iteration, int maxIterations);
 * \brief Dessine et colorie les racines du polynome.
 * 
 * \param *roots Tableau stockant les racines du polynome
 * \param z Suite complexe
 * \param power Degré du polynome
 * \param x Position horizontale du pixel
 * \param y Position verticale du pixel
 * \param iterations Nombre d'itérations qui à été utilisé pour calculer la suite
 * \return Rien
 */
void newtonDrawRoots(	complex *roots,
			complex z,
			int power,
			int x,
			int y,
			int iteration,
			int maxIterations)
{
	int i;
	double angle;
	colorRGB cRGB;
	colorHSL cHSL;
	
	
	
	double deltaIter = (double)(iteration)/(maxIterations);
	deltaIter = 1 - deltaIter;
	
	/* La luminosité est en fonction du nombre d'itérations */
	cHSL.saturation = 1.0;    /* entre 0 et 1*/
	cHSL.lightness = 0.5  * deltaIter ;    /* entre 0 et 0.5 et 1*/
	
	
	/* On teste chaque racine, pour retrouver pour laquelle z converge vers */
	for (i = 0; i < power; i++)
	{
		/* Des qu'on trouve la racine où z converge vers */
		if(complexAbs(complexDif(z,roots[i])) < FRACTAL_NEWTON_PRECISION)
		{
			/* L'angle est la position de la racine sur le cercle colorimétrique */
			angle = (double)i/power;
			/* Ainsi cela nous donne la valeur de la teinte */
			cHSL.hue = angle;
			/* On converti vers le RVB pour OpenGL */
			cRGB = HSLtoRGB(cHSL);
			/* On charge la couleur */
			glColor3ub( cRGB.red , cRGB.green, cRGB.blue);
			/* On dessine le point */
			glVertex2f(x,y);
		}
		
	}
	
}
