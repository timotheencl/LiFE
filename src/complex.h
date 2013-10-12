/**
 * \file complex.h
 * \brief Fichier d'entête, Représentation des nombres complexes
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.2
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

#ifdef USE_SSE
#include <pmmintrin.h>
#endif

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
 * \union complex
 * \brief Stocke un nombre complexe.
 * 
 * La structure \e complex permet de stocker un nombre complexe
 * sous la forme :\c a+ib
 * a est la partie réelle et b la partie imaginaire
*/
typedef union
{
	struct
	{
		double real;	/*!< Partie Réelle */
		double imag;	/*!< Partie Imaginaire */
	}

	double 	raw[2];		/*!< Tableau de 2 double */

	#ifdef USE_SSE
		__m128d	mmx;	/*!< Pour la vectorisation SEE */ 
	#endif

} complex;

/**
 * \fn complex complexSet(double real, double imaginary);
 * \brief Crée un nouveau nombre complexe.
 * 
 * \param real Partie Réelle
 * \param imaginary Partie Imaginaire
 * \return Le nombre complexe 
 */
inline complex complexSet(double real, double imaginary)
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
inline double complexRe(complex z)
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
inline double complexIm(complex z)
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
inline complex complexConj(complex z)
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
inline double complexAbs(complex z)
{
	#ifndef USE_SSE

		return sqrt(z.real*z.real + z.imag*z.imag);
	
	#else

		double ret;
		__m128d tmp = _mm_mul_pd(z.mmx,z.mmx);
		__m128d res = _mm_sqrt_sd(_mm_hadd_pd(tmp,tmp));
		_mm_store_sd(&ret,res);
		return ret;

	#endif
}

/**
 * \fn double complexArg(complex z);
 * \brief Calcule l'argument d'un nombre complexe.
 * 
 * \param z Nombre complexe
 * \return Un \e double représentant l'argument
 */
inline double complexArg(complex z)
{
	return atan2(z.imag,z.real);
}

/**
 * \fn complex complexAdd(complex z1, complex z2);
 * \brief Calcule la somme complexe de 2 nombres complexes: z1 + z2
 * 
 * \param z1 Nombre complexe 1
 * \param z2 Nombre complexe 2
 * \return Un complexe représentant la somme
 */
inline complex complexAdd(complex z1, complex z2)
{

	complex z ;

	#ifndef USE_SSE

		z.real = z1.real + z2.real;
		z.imag = z1.imag + z2.imag;

	#else

		__m128d res = _mm_add_pd(z1.mmx, z2.mmx);
		
		//double cpx[2] __attribute__((aligned(16)));
		_mm_store_pd(z.raw, res);
		
		//z.real = cpx[0];
		//z.imag = cpx[1];

	#endif

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
inline complex complexDif(complex z1, complex z2)
{

	complex z ;

	#ifndef USE_SSE

		z.real = z1.real - z2.real;
		z.imag = z1.imag - z2.imag;

	#else

		__m128d a = _mm_set_pd(z1.real, z1.imag);
		__m128d b = _mm_set_pd(z2.real, z2.imag);

		__m128d res = _mm_sub_pd(a, b);
		
		//double cpx[2] __attribute__((aligned(16)));
		_mm_store_pd(z.raw, res);
		
		//z.real = cpx[0];
		//z.imag = cpx[1];

	#endif

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
inline complex complexMul(complex z1, complex z2)
{
	complex z;

	#ifndef USE_SSE
		
		
		z.real = z1.real*z2.real - z1.imag*z2.imag;
		z.imag = z1.real*z2.imag + z1.imag*z2.real;
		
	#else

		__m128d mul_ab, real, imag, b_swp;

		// Partie réelle
		mul_ab = _mm_mul_pd(z1.mmx, z2.mmx);
		real = _mm_hsub_pd(mul_ab, mul_ab);
		
		// Partie imaginaire
		b_swp = _mm_shuffle_pd(z2.mmx,z2.mmx, _MM_SHUFFLE2(0, 1));
		mul_ab = _mm_mul_pd(z1.mmx, b_swp);
		imag = _mm_hadd_pd(mul_ab, mul_ab);

		_mm_store_sd(&z.real, real);
		_mm_store_sd(&z.imag, imag);

	#endif

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
inline complex complexDiv(complex z1, complex z2)
{
	complex z;

	#ifndef USE_SSE
		
		z.real = (z1.real*z2.real + z1.imag*z2.imag) / (z2.real*z2.real + z2.imag*z2.imag);
		z.imag = (z1.imag*z2.real - z2.imag*z1.real) / (z2.real*z2.real + z2.imag*z2.imag);
		
	#else

		__m128d mul_ab,
			mul_bb,
				hadd_mul_ab,
				hadd_mul_bb,
				real,
				imag,
				a_swp,
				hsub_mul_ab;

		// Partie réelle
		mul_ab = _mm_mul_pd(z1.mmx, z2.mmx);
		mul_bb = _mm_mul_pd(z2.mmx, z2.mmx);
		hadd_mul_ab = _mm_hadd_pd(mul_ab, mul_ab);
		hadd_mul_bb = _mm_hadd_pd(mul_bb, mul_bb);
		real = _mm_div_pd(hadd_mul_ab, hadd_mul_bb);

		// Partie imaginaire
		a_swp = _mm_shuffle_pd(z1.mmx,z1.mmx, _MM_SHUFFLE2(0, 1));
		mul_ab = _mm_mul_pd(a_swp, z2.mmx);
		hsub_mul_ab = _mm_hsub_pd(mul_ab, mul_ab);
		imag = _mm_div_pd(hsub_mul_ab, hadd_mul_bb);

		_mm_store_sd(&z.real, real);
		_mm_store_sd(&z.imag, imag);

	#endif

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
inline complex complexPow(complex z, int n)
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
inline void complexPrintf(complex z)
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
inline void complexSprintf(char *string, complex z)
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

#endif /* H_COMPLEX */
