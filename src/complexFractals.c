/**
 * \file complexFractals.c
 * \brief Fichier source, contient toutes les fractales de type complexe
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

#include "complexFractals.h"


/**
 * \fn void computeMandelbrotFractal(fractalSettings fSet, bool colorationMode);
 * \brief Construit la fractale de Mandelbrot
 * 
 * \param fSet structure contenant les paramètre de la fractale
 * \param colorationMode variable décidant si il y a coloration
 * \return Rien
 */
void computeMandelbrot(fractalSettings fSet)
{
	
	int x, y, iteration;
	complex c,z;
	double temp, iterNormalized;
	colorHSL colorA;
	colorRGB colorB;
	
	/* Pour chaque pixels de l'écran */
	for (x = 0; x < fSet.screenWidth; x++)
	{
		for (y = 0; y < fSet.screenHeight; y++)
		{
			c.real = ((double)(x)/(fSet.screenWidth))*(fSet.realMax-fSet.realMin)+fSet.realMin;
			c.imag = ((double)(y)/(fSet.screenHeight))*(fSet.imagMax-fSet.imagMin)+fSet.imagMin;
			
			z = complexSet(0.0,0.0);
			
			iteration = 0;
			
			/* On itère la suite pour déterminer si elle converge */
			while((iteration < fSet.iterMax) && (complexAbs(z) < 2))
			{
				temp = (z.real*z.real-z.imag*z.imag) + c.real;
				z.imag=(2*z.real*z.imag) + c.imag;
				z.real = temp;
				iteration++;
			}
			
			
			if(iteration == fSet.iterMax) /* La suite converge, alors c'est le centre en noir de la fractale */
				glColor3f(0,0,0); 
			else /* Sinon on colorie l'extérieur en fonction du nombre d'itérations */
			{
				/* Nombre d'itérations compris entre 0 et 1 */
				iterNormalized = (double)(iteration)/(fSet.iterMax);
				
				/* On fait varier la teinte et la luminosité, plutôt que le rouge,vert,bleu */
				
				colorA.saturation = 1.0;		/* La saturation n'est pas modifié, 1 est la valeur normale */
				colorA.lightness = 0.5 * iterNormalized; /* Luminosité en fonction du nombre d'itérations */
				
				/* 0.882352941 : 225° teinte bleu roi
				 * 0.745098039 : 190° teinte turquoise
				 */
				
				/* On borne la valeur de l'itération en fonction d'une couleur minimum et une autre maximum */
				iterNormalized = (iterNormalized)*(0.745098039 - 0.882352941) + 0.745098039;
				
				colorA.hue = 0.882352941 * iterNormalized;	/* idem pour la teinte */
				
				/* On converti la couleur vers RVB */
				colorB = HSLtoRGB(colorA);
				
				/* On definit la couleur du point */
				glColor3ub(colorB.red,colorB.green,colorB.blue);
			}
			
			/* On dessine le point */
			glVertex2f(x, fSet.screenHeight - y);
		}
		
	}
	
	
	
}


/**
 * \fn void computeBurningShip(fractalSettings fSet);
 * \brief Construit la fractale Burning Ship
 * 
 * \param fSet structure contenant les paramètre de la fractale
 * \return Rien
 */
void computeBurningShip(fractalSettings fSet)
{
	complex z,cst;
	colorHSL colorA;
	colorRGB colorB;
	int x, y, iteration;
	double iterNormalized;
	
	/*On parcourt tous les pixels de la fenêtre pour vérifier si la fonction en ce pixel converge ou diverge*/
	for(x = 0; x < fSet.screenWidth; x++)
	{
	
		for(y = 0 ; y < fSet.screenHeight; y++) /* for(y=fSet.screenHeight; y >= 0; y--) */
		{				
		
			/*Détermination de la constante en fonction de la position du pixel sur la fenêtre graphique					
			Partie réelle de cst												Partie imaginaire de cst*/
			cst = complexSet( ((double)(x)/(fSet.screenWidth))*(fSet.realMax - fSet.realMin)+fSet.realMin, ((double)(y)/(fSet.screenHeight))*(fSet.imagMax - fSet.imagMin)+fSet.imagMin );
				
			/*Détermination de z0;*/
			z = complexSet(0.0, 0.0);
					
			iteration = 0;
					
			/*Tant que l'on atteint pas une valeur "infinie", on continue le calul de z*/
			while( (iteration < fSet.iterMax) && ( complexAbs(z) < 2) )
			{
					
				/*valeur absolue de la somme du réel de z et de l'imaginaire de z, le tout au carré, plus la constante*/
				z = complexSet(fabs(complexRe(z)), fabs(complexIm(z)));
 				z = complexAdd(complexMul(z,z), cst);

				iteration++;
			}	
					
			/* Si ça converge, on colorie le pixel en noir, sinon on joue sur les couleurs en fonction du nombre d'itérations */
			if(iteration == fSet.iterMax)
				glColor3ub(0,0,0);
			else
			{
				/* Nombre d'itérations compris entre 0 et 1 */
				iterNormalized = (double)(iteration)/(fSet.iterMax);
				
				/* On fait varier la teinte et la luminosité, plutôt que le rouge,vert,bleu */
				
				colorA.saturation = 1.0;		/* La saturation n'est pas modifié, 1 est la valeur normale */
				colorA.lightness = 0.5 * iterNormalized; /* Luminosité en fonction du nombre d'itérations */
				colorA.hue = 0.125 * iterNormalized;	/* idem pour la teinte, 0.125 est la teinte jaune (45°) */
				
				/* On converti la couleur vers RVB */
				colorB = HSLtoRGB(colorA);
				
				/* On definit la couleur du point */
				glColor3ub(colorB.red,colorB.green,colorB.blue);
										
			}
			
			/* On dessine le point */
			glVertex2f(x,fSet.screenHeight-y);
		}
				
	}
}


/**
 * \fn void computeJulia(fractalSettings fSet, complex c);
 * \brief Construit la fractale de Julia
 * 
 * \param fSet structure contenant les paramètre de la fractale
 * \param c Constante complexe
 * \return Rien
 */
void computeJulia(fractalSettings fSet, complex c)
{
	complex z;
	int x,y,iteration;
	double temp,iterNormalized;
	colorHSL colorA;
	colorRGB colorB;
	
	/* Pour chaque pixels de l'écran */
	for(x = 0; x < fSet.screenWidth; x++)
	{
		for(y = 0; y < fSet.screenHeight; y++)
		{
			
			/* Il faut juste inverser c et z0 */
			
			z.real = ((double)(x)/(fSet.screenWidth))*(fSet.realMax-fSet.realMin)+fSet.realMin;
			z.imag = ((double)(y)/(fSet.screenHeight))*(fSet.imagMax-fSet.imagMin)+fSet.imagMin;
			
			/*
			c.real = 0.3;
			c.imag = 0.6;
			*/
			
			iteration = 0;
			while((iteration < fSet.iterMax) && ((z.real*z.real+z.imag*z.imag) < 4))
			{
				/*temp=z.real;*/
				temp = (z.real*z.real-z.imag*z.imag) + c.real;
				z.imag=(2*z.real*z.imag) + c.imag;
				z.real = temp;
				iteration++;
			}	
			
			
			if(iteration == fSet.iterMax)
			{
				glColor3ub(0,0,0);
			}
			else
			{
				
				/* Nombre d'itérations compris entre 0 et 1 */
				iterNormalized = (double)(iteration)/(fSet.iterMax);
				
				/* On fait varier la teinte et la luminosité, plutôt que le rouge,vert,bleu */
				
				colorA.saturation = 1.0;		/* La saturation n'est pas modifié, 1 est la valeur normale */
				colorA.lightness = 0.5 * iterNormalized; /* Luminosité en fonction du nombre d'itérations */
				
				/* 0.470588235 : 120° teinte verte
				 * 0.352941176 : 90° teinte vert - jaune
				 */
				
				/* On borne la valeur de l'itération en fonction d'une couleur minimum et une autre maximum */
				iterNormalized = (iterNormalized)*(0.470588235 - 0.352941176) + 0.470588235;
				
				colorA.hue = 0.352941176 * iterNormalized;	/* idem pour la teinte */
				
				/* On converti la couleur vers RVB */
				colorB = HSLtoRGB(colorA);
				
				/* On definit la couleur du point */
				glColor3ub(colorB.red,colorB.green,colorB.blue);
				
				
			}
			
			/* On dessine le point */
			glVertex2f(x,fSet.screenHeight-y);
				
		}
		
	}
}


/**
 * \fn void computeLevy(fractaleSettings fSet);
 * \brief Construit la fractale de Newton
 * 
 * \param fSet structure contenant les paramètre de la fractale
 * \return Rien
 */
void computeNewton(fractalSettings fSet, int power)
{
	complex z,zn,zd;
	int x, y, iteration;
	complex r[power];
	
	/* On calcule les racines complexes du polynome, que l'on stocke dans le tableau r */
	newtonComputeRoots(r, power);
	
	/* Pour chaque pixel de l'écran */
	for(x = 0; x < fSet.screenWidth; x++)
	{
		for(y = 0 ; y < fSet.screenHeight; y++)
		{
			
			/* Paramètres initiaux */
			z.real = ((double)(x)/(fSet.screenWidth))*(fSet.realMax-fSet.realMin)+fSet.realMin;
			z.imag = ((double)(y)/(fSet.screenHeight))*(fSet.imagMax-fSet.imagMin)+fSet.imagMin;
			iteration = 0;
			
			/* On itère la suite */			
			while((iteration < fSet.iterMax) && newtonCheckRoots( r, z, power) )
			{
			
				if (complexAbs(z) > 0)
				{
					/* On calcule le numérateur */
					zn = complexAdd(complexMul(complexSet(power-1,0),complexPow(z,power)),complexSet(1,0));
					
					/* On calcule le dénominateur */
					zd = complexMul(complexSet(power,0),complexPow(z,power-1));
					
					/* On calcule le quotient des deux */
					z = complexDiv(zn, zd);
				}
				iteration++;
				
			}
			
			/* On apelle la fonction qui va colorier le pixel (newton.c) */
			newtonDrawRoots(r, z, power, x, fSet.screenHeight - y, iteration, fSet.iterMax);
		}
		
	}
}
