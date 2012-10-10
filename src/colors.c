/**
 * \file colors.c
 * \brief Fichier source, gestion des couleurs
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.0
 * \date 29/12/2012
 * 
 * Permet de convertir différents espaces de couleurs
 * 
 */

#include "colors.h"

/**
 * \fn colorRGB HSLtoRGB(colorHSL colorIn);
 * \brief Converti une couleur du format Teinte Saturation Luminosité vers le format Rouge Vert Bleu
 * \remark Adapté de: http://www.easyrgb.com/index.php?X=MATH&H=19#text19
 * 
 * \param colorIn La couleur au format TSL (HSL)
 * \return La couleur au format RVG (RGB))
 */
colorRGB HSLtoRGB(colorHSL colorIn)
{
	colorRGB colorOut;
	double red, green, blue, p, q;
	
	if (colorIn.saturation == 0)
	{
		red = green = blue = (colorIn.lightness * 255.0);
	}
	else
	{
		q = colorIn.lightness < 0.5 ? colorIn.lightness * (1 + colorIn.saturation) : (colorIn.lightness + colorIn.saturation) - (colorIn.lightness * colorIn.saturation);
		p = 2 * colorIn.lightness - q;
		
		
		red =	255.0 * hueToRGB(p, q, colorIn.hue + (1.0/3.0));
		green =	255.0 * hueToRGB(p, q, colorIn.hue);
		blue =	255.0 * hueToRGB(p, q, colorIn.hue - (1.0/3.0));
		
	}
	
	colorOut.red	= (int)(red);
	colorOut.green	= (int)(green);
	colorOut.blue	= (int)(blue);
	
	return colorOut;
}

/**
 * \fn double hueToRGB(double p, double q, double t);
 * \brief Converti la teinte vers la nuance de RVB
 * \remark C'est une sous-fonction de HSLtoRGB
 * 
 * \param p Réel p
 * \param q Réel q
 * \param t Réel t
 * \return Un double
 */
double hueToRGB(double p, double q, double t)
{
	if (t < 0)
		t += 1;
	if (t > 1)
		t -= 1;
	if (t < 1.0/6.0)
		return p + (q - p) * 6 * t;
	if (t < 0.5)
		return q;
	if (t < 2.0/3.0)
		return p + (q - p) * ((2.0/3.0) - t) * 6;
	
	return p;
}
