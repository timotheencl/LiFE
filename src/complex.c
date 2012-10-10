/**
 * \file complex.c
 * \brief Fichier Source, Représentation des nombres complexes
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.0
 * \date 28/12/2012
 * 
 * Permet la représentation et l'utilisation des nombres complexes.
 * Les opérations arithmétiques de base y sont implémenté.
 * 
 */

#include "complex.h"

/**
 * \fn complex complexSet(double real, double imaginary);
 * \brief Crée un nouveau nombre complexe.
 * 
 * \param real Partie Réelle
 * \param imaginary Partie Imaginaire
 * \return Le nombre complexe 
 */
complex complexSet(double real, double imaginary)
{
	complex z;
	
	z.real = real;
	z.imag = imaginary;
	return z;
}

/**
 * \fn double complexRe(complex z);
 * \brief Renvoie la partie Réelle d'un nombre complexe Re(z)
 * 
 * \param z Nombre complexe
 * \return Un double représentant la partie réelle de z
 */
double complexRe(complex z)
{
	return z.real;
}

/**
 * \fn double complexIm(complex z);
 * \brief Renvoie la partie Imaginaire d'un nombre complexe Im(z)
 * 
 * \param z Nombre complexe
 * \return Un double représentant la partie imaginaire de z
 */
double complexIm(complex z)
{
	return z.imag;
}
/**
 * \fn complex complexConj(complex z);
 * \brief Calcule le conjugé d'un nombre complexe: \e a-ib
 * 
 * \param z Nombre complexe
 * \return Le nombre complexe conjugé 
 */
complex complexConj(complex z)
{
	z.imag = -z.imag;
	return z;
}

/**
 * \fn double complexAbs(complex z);
 * \brief Calcule la valeur absolue d'un nombre complexe. (Module)
 * 
 * \param z Nombre complexe
 * \return Un \e double représentant la valeur absolue
 */
double complexAbs(complex z)
{
	return sqrt(z.real*z.real + z.imag*z.imag);
}

/**
 * \fn double complexArg(complex z);
 * \brief Calcule l'argument d'un nombre complexe.
 * 
 * \param z Nombre complexe
 * \return Un \e double représentant l'argument
 */
double complexArg(complex z)
{
	return atan(z.imag/z.real);
}

/**
 * \fn complex complexAdd(complex z1, complex z2);
 * \brief Calcule la somme complexe de 2 nombres complexes: z1 + z2
 * 
 * \param z1 Nombre complexe 1
 * \param z2 Nombre complexe 2
 * \return Un complexe représentant la somme
 */
complex complexAdd(complex z1, complex z2)
{
	complex z;
	
	z.real = z1.real + z2.real;
	z.imag = z1.imag + z2.imag;
	
	return z;
}

/**
 * \fn complex complexDif(complex z1, complex z2);
 * \brief Calcule la différence complexe de 2 nombres complexes: z1 - z2
 * 
 * \param z1 Nombre complexe 1
 * \param z2 Nombre complexe 2
 * \return Un complexe représentant la différence
 */
complex complexDif(complex z1, complex z2)
{
	complex z;
	
	z.real = z1.real - z2.real;
	z.imag = z1.imag - z2.imag;
	
	return z;
}

/**
 * \fn complex complexMul(complex z1, complex z2);
 * \brief Calcule le produit complexe de 2 nombres complexes: z1 * z2
 * 
 * \param z1 Nombre complexe 1
 * \param z2 Nombre complexe 2
 * \return Un complexe représentant le produit
 */
complex complexMul(complex z1, complex z2)
{
	complex z;
	
	z.real = z1.real*z2.real - z1.imag*z2.imag;
	z.imag = z1.real*z2.imag + z1.imag*z2.real;
	
	return z;
}

/**
 * \fn complex complexDiv(complex z1, complex z2);
 * \brief Calcule le quotient complexe de 2 nombres complexes: z1 / z2
 * 
 * \param z1 Nombre complexe 1 (numérateur)
 * \param z2 Nombre complexe 2 (dénominateur)
 * \return Un complexe représentant le quotient complexe
 */
complex complexDiv(complex z1, complex z2)
{
	complex z;
	
	z.real = (z1.real*z2.real + z1.imag*z2.imag) / (z2.real*z2.real + z2.imag*z2.imag);
	z.imag = (z1.imag*z2.real - z2.imag*z1.real) / (z2.real*z2.real + z2.imag*z2.imag);
	
	return z;
}

/**
 * \fn complex complexPow(complex z, int n);
 * \brief Monte à la puissance \e n, le nombre complexe z
 * 
 * \param z Nombre complexe
 * \param n Puissance
 * \return Un complexe représentant z^n
 */
complex complexPow(complex z, int n)
{
	complex out = z;
	int i;
	
	for (i = 1; i < n; i++)
		out = complexMul(z, out);
	
	return out;
}

/**
 * \fn void complexPrintf(complex z);
 * \brief Affiche dans le terminal une chaine de caractères représentant le nombre complexe.
 * 
 * \param z Nombre complexe
 * \return Rien
 */
void complexPrintf(complex z)
{
	if (z.imag == 0.00)
		printf("%.4g", z.real);
	else if (z.imag == 1.00 && z.real != 0.00)
		printf("%.4g +i", z.real);
	else if (z.imag == -1.00 && z.real != 0.00)
		printf("%.4g -i", z.real);
	else if (z.imag == -1.00 && z.real == 0.00)
		printf("-i");
	else if (z.imag == +1.00 && z.real == 0.00)
		printf("i");
	else
		printf("%.4g %+.4gi", z.real, z.imag);
}

/**
 * \fn void complexSprintf(char *string, complex z);
 * \brief Ecrit le nombre complexe dans une chaine de caractère.
 * 
 * \param z Nombre complexe
 * \param *string Chaine de caractères représentant le nombre complexe.
 * \return Rien
 */
void complexSprintf(char *string, complex z)
{
	if (z.imag == 0.00)
		sprintf(string, "%.4g", z.real);
	else if (z.imag == 1.00 && z.real != 0.00)
		sprintf(string, "%.4g +i", z.real);
	else if (z.imag == -1.00 && z.real != 0.00)
		sprintf(string, "%.4g -i", z.real);
	else if (z.imag == -1.00 && z.real == 0.00)
		sprintf(string, "-i");
	else if (z.imag == +1.00 && z.real == 0.00)
		sprintf(string, "i");
	else
		sprintf(string, "%.4g %+.4gi", z.real, z.imag);
}
