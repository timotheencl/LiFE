/**
 * \file engine.h
 * \brief Fichier d'entête, gestion de l'affichage
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.4
 * \date 26/12/2012
 * 
 * Contient toutes les fonctions permettant de dessiner avec SDL/OpenGL
 * Ainsi que la création de fenêtre et la gestion clavier/souris
 * 
 */

#ifndef H_ENGINE
#define H_ENGINE

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "complex.h"
#include "colors.h"

/* Macros */

/**
 * \def FSAA
 * \brief Full Scene Anti Aliasing. Evite l'effet escalier.
 * 
 * Par défaut fixé à 8x.
 * Valeurs possibles: de 1 à 16
 */
#define FSAA 1

/**
 * \def VERSION
 * \brief Numéro de version du moteur
 */
#define VERSION "1.2"


/* Typedef */
typedef colorRGB** screenBuffer;

/* Structures */

/**
 * \struct screenInfos
 * \brief Stocke les informations de l'écran
 * \remark Elle est utilisé conjointement avec la structure engineSettings
 * 
 * La structure \e screenInfos permet de stocker les informations sur l'écran:
 * La résolution et le nombre de bits par pixels.
 * 
*/
typedef struct
{
	int	width;		/*!< Largeur de l'écran */
	int	height;		/*!< Hauteur de l'écran */
	int	bitsPerPixel;	/*!< Nombre de bits par pixels */
} screenInfos;


/**
 * \struct engineSettings
 * \brief Stocke les informations du moteur graphique
 * 
 * La structure \e engineSettings permet de stocker les données importantes pour
 * le moteur graphique ...
 * 
*/
typedef struct
{
	bool running;		/*!< Etat du moteur graphique */
	screenInfos screen;	/*!< Informations sur l'écran */
	TTF_Font* font1;	/*!< Pointeur vers la police à utiliser */
} engineSettings;

/**
 * \struct image
 * \brief Stocke une image
 * 
 * La structure \e image permet de stocker une image facilement.
 * La taille de l'image et la position de celle-ci peuvent être enregistré.
 * 
*/
typedef struct
{
	SDL_Surface *surface;	/*!< Surface SDL */
	int x;			/*!< Position horizontale de l'image */
	int y;			/*!< Position verticale de l'image */
	int w;			/*!< Largeur de l'image */
	int h;			/*!< Hauteur de l'image */
} image;


/**
 * \struct fractalSettings
 * \brief Stocke les paramètres généraux de la fractale
 * 
 * La structure \e screenInfos permet de stocker les paramètres tel que, les
 * coordonnées de la fractale dans le repère complexe, le nombre d'itérations
 * par pixels ...
 * 
*/
typedef struct
{
	double	realMax;	/*!< Largeur de l'écran */
	double	realMin;	/*!< Hauteur de l'écran */
	double	imagMax;	/*!< Nombre de bits par pixels */
	double	imagMin;	/*!< Nombre de bits par pixels */
	
	int	screenWidth;	/*!< Largeur de l'écran */
	int	screenHeight;	/*!< Hauteur de l'écran */
	
	int	iterMax;	/*!< Nombre d'itérations */
} fractalSettings;

/**
 * \struct zoomStack
 * \brief Stocke les valeur de zoom, pour permettre de dézoomer
 * \remark *previous est NULL si c'est l'élement en tête de liste.
 * 
 * La structure \e zoomStack est utilisé conjointement avec une liste chainée. (pile)
 * Cela pour pouvoir faire des zoom arrières
 * 
*/
typedef struct zoomStack
{
	double xMin;			/*!< Valeur du zoom minimum en abscisse */
	double xMax;			/*!< Valeur du zoom maximum en abscisse */
	double yMin;			/*!< Valeur du zoom minimum en ordonnée */
	double yMax;			/*!< Valeur du zoom maximum en ordonnée */
	struct zoomStack *previous;	/*!< Pointeur vers le zoomStack précedent */
} zoomStack;

/* Prototypes */

/* Fonctions internes au moteur */
void createWindow(int width, int height, const char* title);
void startLoop();
extern void mainLoop(engineSettings* engine);
void destroyWindow();
int getFrameRate();
void limitFrameRate(int framesPerSecond);
void showEngineInfos(engineSettings* engine);

/* Affichage de texte */
TTF_Font* fontOpen(char* name, int size);
void fontClose(TTF_Font* font);
void fontPrint(TTF_Font* font, const char* string, int x, int y, colorRGB c);

/* Affichage d'images */
image* imageLoad(char* filename);
void imageFree(image *img);
void imageDraw(image *img);

/* Gestion des fractales */
void drawZoomBox(engineSettings* engine, int x, int y, double size);
void setNewScale(fractalSettings *zoom, int x, int y, double sizeZoom);

/* Pile pour le dézoom */
void zoomStackClear(zoomStack **stack);
void zoomStackPush(zoomStack **stack, double xMin, double xMax, double yMin, double yMax);
void zoomStackPop(zoomStack **stack, double *xMin, double *xMax, double *yMin, double *yMax);

#endif /* H_ENGINE */
