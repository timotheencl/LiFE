/**
 * \file complex.h
 * \brief Fichier d'entête, Représentation des nombres complexes
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.1
 * \date 28/12/2012
 * 
 * Permet la représentation et l'utilisation des nombres complexes.
 * Les opérations arithmétiques de base y sont implémenté.
 * 
 */

#ifndef H_COMPLEX
#define H_COMPLEX

/* Inclusions */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Macros */

/**
 * \def COMPLEX_I
 * \brief Constante mathématique complexe 'i'
 * 
 * C'est la racine carrée de -1
 * 
 */
#define COMPLEX_I complexSet(0,1)


/**
 * \def M_PI
 * \brief Constante mathématique Pi
 * \remark Elle est normalement définie dans math.h. En revanche, elle ne l'est pas si on compile en C ANSI
 * 
 */
#ifndef M_PI
	#define M_PI	3.14159265358979323846
#endif

/* Structures */

/**
 * \struct complex
 * \brief Stocke un nombre complexe.
 * 
 * La structure \e complex permet de stocker un nombre complexe
 * sous la forme :\c a+ib
 * a est la partie réelle et b la partie imaginaire
*/
typedef struct
{
	double real;	/*!< Partie Réelle */
	double imag;	/*!< Partie Imaginaire */
} complex;

/* Prototypes */

complex	complexSet(double real, double imaginary);
double	complexRe(complex z);
double	complexIm(complex z);

complex	complexConj(complex z);
double	complexAbs(complex z);
double	complexArg(complex z);
complex	complexAdd(complex z1, complex z2);
complex	complexDif(complex z1, complex z2);
complex	complexMul(complex z1, complex z2);
complex	complexDiv(complex z1, complex z2);
complex	complexPow(complex z, int n);
void	complexPrintf(complex z);
void	complexSprintf(char *string, complex z);

#endif /* H_COMPLEX */
