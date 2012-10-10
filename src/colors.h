/**
 * \file colors.h
 * \brief Fichier d'entête, gestion des couleurs
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.0
 * \date 29/12/2012
 * 
 * Permet de convertir différents espaces de couleurs
 * 
 */

#ifndef H_COLORS
#define H_COLORS


/* Structures */

/**
 * \struct colorRGB
 * \brief Stocke une couleur au format Rouge Vert Bleu (RGB)
 * 
 * La structure stocke les trois composante d'une couleur au format RVB
 * Chaque composante est représenté sur un octet de 0 à 255.
 * 
*/
typedef struct
{
	int red;	/*!< Composante Rouge [0-255] */
	int green;	/*!< Composante Verte [0-255] */
	int blue;	/*!< Composante Bleu  [0-255] */
} colorRGB;

/**
 * \struct colorHSL
 * \brief Stocke une couleur au format Teinte Saturation Luminosité
 * 
 * La structure stocke les trois composante d'une couleur au format TSL
 * Chaque composante est codé de 0 à 1
 * 
*/
typedef struct
{
	double hue;		/*!< Teinte	[0.0-1.0] */
	double saturation;	/*!< Saturation	[0.0-1.0] */
	double lightness;	/*!< Luminosité	[0.0-1.0] */
} colorHSL;


/* Prototypes */

colorRGB HSLtoRGB(colorHSL colorIn);
double hueToRGB(double p, double q, double t);

#endif /* H_COLORS */
