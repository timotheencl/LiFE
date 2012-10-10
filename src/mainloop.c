/**
 * \file mainloop.c
 * \brief Fichier source, boucle de l'affichage
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.4
 * \date 29/12/2012
 * 
 * Contient toutes les instructions de la boucle principale
 * 
 */

#include "mainloop.h"

/**
 * \fn void mainLoop(engineSettings* engine);
 * \brief Permet de gérer l'affichage
 * 
 * \return Rien
 */
void mainLoop(engineSettings* engine)
{
	
	SDL_Event event;
	GLuint listVertex = 0;
	fractalSettings zoom;
	
	/* Variables d'état du programme */
	bool showInfos = false;
	bool showHelp = false;
	bool isFractalDraw = false;
	bool modeDrawFractal = false;
	bool modeComplexFractal = false;
	bool modeRecursiveFractal = false;
	
	/* Variables utilisés pour les fractales */
	int fractalId;
	unsigned int recursiveIterationCounter = DEFAULT_RECURSIVE_ITERATION;
	unsigned int newtonPower = DEFAULT_NEWTON_POWER;
	int juliaCstId = 0;
	
	/* Constante C pour fractale de julia */
	complex cstJulia[4];
	cstJulia[0] = complexSet(0.3, 0.6);
	cstJulia[1] = complexSet(-0.75, 0.0);
	cstJulia[2] = complexSet(0.0, 1.0);
	cstJulia[3] = complexSet(-1.3, 0.0);
	
	/* Variables utilisés pour la gestion du zoom */
	bool showZoomBox = false;
	double sizeZoomBox = ZOOM_DEFAULT;
	zoomStack* pZoomStack = NULL;
	
	/* Chargement des images pour les menus */
	/* On stocke les pointeurs des images dans une structure */
	dataImages images;
	loadingImages(&images, engine);
	
	/* Couleur blanche */
	colorRGB blanc = {255, 255, 255};
	
	/* Boucle principale */
	while (engine->running)
	{
		
		/* On efface la fenêtre */
		glClear( GL_COLOR_BUFFER_BIT );	
		
		/* On limite l'affichage à un certain nombre d'images par secondes */
		limitFrameRate(MAX_FRAMERATE);
		
		/* ### Gestion évenements */
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				/* Pour sortir de la boucle et quitter */
				case SDL_QUIT:
					engine->running = false;
					break;
					
				case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					/* Pour quitter */
					case SDLK_q:
						engine->running = false;
						break;
						
					/* Pour afficher les infos: ips, résolution ... */
					case SDLK_i:
						showInfos = 1 - showInfos;
						break;
					
					/* Menu principal */	
					case SDLK_h:
						/* Si on est pas dans le menu, on re-initialise tout les paramètres */
						if (modeDrawFractal)
						{
							modeComplexFractal = false;
							modeRecursiveFractal = false;
							showZoomBox = false;
							showHelp = false;
							zoomStackClear(&pZoomStack);
							initZoom(&zoom, engine, fractalId);
							recursiveIterationCounter = DEFAULT_RECURSIVE_ITERATION;
							newtonPower = DEFAULT_NEWTON_POWER;
							isFractalDraw = false;
							if (listVertex != 0)
								glDeleteLists(listVertex, 1);
							
							/* Et on demande l'affichage du menu */	
							modeDrawFractal = false;
						}
						break;
						
					case SDLK_F2:
						/* Switch mode plein écran Linux uniquement*/
						#ifdef __linux__
							SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
						#endif
						break;
					case SDLK_ESCAPE:
						/* Si le mode zoom est activé, on le désactive */
						if (showZoomBox)
							showZoomBox = false;
						break;
					
					case SDLK_F1:
						/* Menu d'aide si on est en mode affichage */
						if (modeDrawFractal)
							showHelp = 1 - showHelp;
						break;
					
					case SDLK_F5:
						if (modeDrawFractal && modeComplexFractal)
						{
							/* On affiche la fractale de Mandelbrot */
							fractalId = COMPLEX_MANDELBROT;
							isFractalDraw = false;
							initZoom(&zoom, engine, fractalId);
							/* On vide la pile de zoom enregistrés */
							zoomStackClear(&pZoomStack);
						}
						if (modeDrawFractal && modeRecursiveFractal)
						{
							/* On affiche la fractale du triangle de Sierpinski */
							fractalId = RECURSIVE_SIERPINSKI_TRIANGLE;
							recursiveIterationCounter = DEFAULT_RECURSIVE_ITERATION;
							isFractalDraw = false;
						}
						break;
						
					case SDLK_F6:
						if (modeDrawFractal && modeComplexFractal)
						{
							/* On affiche la fractale de Julia */
							fractalId = COMPLEX_JULIA;
							isFractalDraw = false;
							initZoom(&zoom, engine, fractalId);
							zoomStackClear(&pZoomStack);
						}
						if (modeDrawFractal && modeRecursiveFractal)
						{
							/* On affiche la fractale du carré de Sierpinski */
							fractalId = RECURSIVE_SIERPINSKI_CARRE;
							recursiveIterationCounter = DEFAULT_RECURSIVE_ITERATION;
							isFractalDraw = false;
						}
						break;
						
					case SDLK_F7:
						if (modeDrawFractal && modeComplexFractal)
						{
							/* On affiche la fractale de Newton */
							fractalId = COMPLEX_NEWTON;
							isFractalDraw = false;
							newtonPower = DEFAULT_NEWTON_POWER;
							initZoom(&zoom, engine, fractalId);
							zoomStackClear(&pZoomStack);
						}
						if (modeDrawFractal && modeRecursiveFractal)
						{
							/* On affiche la fractale de Levy */
							fractalId = RECURSIVE_LEVY;
							recursiveIterationCounter = DEFAULT_RECURSIVE_ITERATION;
							isFractalDraw = false;
						}
						break;
						
					case SDLK_F8:
						if (modeDrawFractal && modeComplexFractal)
						{
							/* On affiche la fractale de Burning Ship */
							fractalId = COMPLEX_BURNING_SHIP;
							isFractalDraw = false;
							initZoom(&zoom, engine, fractalId);
							zoomStackClear(&pZoomStack);
						}
						break;
					
					case SDLK_SPACE:
						if (modeDrawFractal)
						{
							/* On change la constante (indice du tableau) de la fractale de Julia */
							if (modeComplexFractal && juliaCstId <= 2 && fractalId == COMPLEX_JULIA)
							{
								juliaCstId++;
								isFractalDraw = false;
							}
							else
							{
								juliaCstId = 0;
								isFractalDraw = false;
							}
						}
					
						break;
					/* Touche + du pavé numérique et flèche du haut */
					case SDLK_KP_PLUS:
					case SDLK_UP:
						if (modeDrawFractal)
						{
							/* Vérifie qu'on dépasse pas le seuil max d'itération */
							/* Pour les fractales de Sierpinski */
							if (recursiveIterationCounter < 6 && fractalId != RECURSIVE_LEVY)
							{
								recursiveIterationCounter++;
								isFractalDraw = false;
							}
							
							/* Idem mais pour la fractale de lévy qui accepte plus d'itérations */
							if (recursiveIterationCounter < 16 && fractalId == RECURSIVE_LEVY)
							{
								recursiveIterationCounter++;
								isFractalDraw = false;
							}
						}
						break;
					
					/* Touche - du pavé numérique et flèche du bas */	
					case SDLK_KP_MINUS:
					case SDLK_DOWN:
						if (modeDrawFractal)
						{
							/* Vérifie que le seuil max d'itération est positif */
							/* Pour les fractales de Sierpinski */
							if(recursiveIterationCounter > 1)
							{
								recursiveIterationCounter--;
								isFractalDraw = false;
							}
						}
						break;
					
					/* Touches pour la fractale de Newton
					 * On modifie la puissance et on demande le redessin
					 */
					
					case SDLK_KP2:
						newtonPower = 2;
						isFractalDraw = false;
						break;
					case SDLK_KP3:
						newtonPower = 3;
						isFractalDraw = false;
						break;
					case SDLK_KP4:
						newtonPower = 4;
						isFractalDraw = false;
						break;
					case SDLK_KP5:
						newtonPower = 5;
						isFractalDraw = false;
						break;
					case SDLK_KP6:
						newtonPower = 6;
						isFractalDraw = false;
						break;
					case SDLK_KP7:
						newtonPower = 7;
						isFractalDraw = false;
						break;
					case SDLK_KP8:
						newtonPower = 8;
						isFractalDraw = false;
						break;
					case SDLK_KP9:
						newtonPower = 9;
						isFractalDraw = false;
						break;
					
					default:
					break;
				}
				break;
				
				case SDL_MOUSEBUTTONDOWN:
					
					/* Si on est en mode affichage du menu principal */
					if (!modeDrawFractal)
					{
						if (checkImageClick(images.menuComplex,
								event.motion.x,
								engine->screen.height - event.motion.y))
						{
							/* On lance le dessin des fractales complexes */
							modeDrawFractal = true;
							modeComplexFractal = true;
							modeRecursiveFractal = false;
							fractalId = COMPLEX_MANDELBROT;
							initZoom(&zoom, engine, fractalId);
							
							break;
						}
						
						if (checkImageClick(images.menuRecursive,
								event.motion.x,
								engine->screen.height - event.motion.y))
						{
							/* On lance le dessin des fractales récursives */
							modeDrawFractal = true;
							modeRecursiveFractal = true;
							modeComplexFractal = false;
							fractalId = RECURSIVE_SIERPINSKI_CARRE;
							
							break;
						}
						
					}
					
					/* Si on est en mode affichage de fractale complexe, on peut activer le zoom */
					if (modeDrawFractal && modeComplexFractal)
					{
						/* Si clique gauche et mode zoom */
						if (event.button.button == 1 && showZoomBox)
						{
							/* On empile les anciennes valeurs de zoom */
							zoomStackPush(&pZoomStack, 	zoom.realMin,
											zoom.realMax,
											zoom.imagMin,
											zoom.imagMax);
							
							/* On met à jour la nouvelle échelle */
							setNewScale( &zoom, event.motion.x,
									engine->screen.height - event.motion.y,
									sizeZoomBox);
									
							/* On demande le redessin de la fractale */
							isFractalDraw = false;
							
							/* On active/désactive le mode zoom */
							showZoomBox = 1 - showZoomBox;
						}
						else if (event.button.button == 1 && !showZoomBox)
							showZoomBox = 1 - showZoomBox; /* Activation du mode zoom au clique */
						
						/* Si on dézoom clique droit et que la pile de zoom n'est pas vide */
						if (event.button.button == 3 && pZoomStack != NULL)
						{
							/* On empile les anciennes valeurs de zoom */
							zoomStackPop(&pZoomStack, 	&zoom.realMin,
											&zoom.realMax,
											&zoom.imagMin,
											&zoom.imagMax);
							
							/* On demande le redessin de la fractale */
							isFractalDraw = false;
						
						}
					
						/* Si le mode zoom est activé, la molette vers le haut permet de zoomer */
						if (event.button.button == 4 && showZoomBox)
						{
							if (sizeZoomBox < ZOOM_MAX)
								sizeZoomBox += ZOOM_PITCH;
						}
						
						/* Si le mode zoom est activé, la molette vers le bas permet dézoomer */
						if (event.button.button == 5 && showZoomBox)
						{
							if (sizeZoomBox > ZOOM_MIN)
								sizeZoomBox -= ZOOM_PITCH;
							if (sizeZoomBox <= ZOOM_MIN)
								sizeZoomBox = ZOOM_PITCH;
						}
						
					}
					
				break;
			}
		} /* ### Fin gestion évenements */
		
		
		
		
			
		/* Debut affichage --- */
		
		
		if (!modeDrawFractal)
		{
			/* On dessine le menu d'accueil*/
			
			imageDraw(images.menuComplex);
			imageDraw(images.menuRecursive);
		}
		else /* On dessine les fractales */
		{
			/* On dessine la fractale */
			if (isFractalDraw == false)
			{
				
				/* Si la liste de vertex existe déjà on la supprime */
				if (listVertex != 0)
					glDeleteLists(listVertex, 1);
				
				/* On alloue une liste de vertex */
				listVertex = glGenLists(1);
				
				/* Si on est en mode fractale complexes */
				if (modeComplexFractal)
				{
					/* Commence la liste, en spécifiant quelle sera compilée,
					 * et non directement affiché à l'écran */
					glNewList(listVertex, GL_COMPILE);
					
					/* On commence avec les vertex points */
					glBegin(GL_POINTS);
					
					/* On affiche le point selon le type de fractale complexe */
					switch(fractalId)
					{
						case COMPLEX_MANDELBROT:
							computeMandelbrot(zoom);
							break;
							
						case COMPLEX_BURNING_SHIP:
							computeBurningShip(zoom);
							break;
						
						case COMPLEX_JULIA:
							computeJulia(zoom, cstJulia[juliaCstId]);
							break;
							
						case COMPLEX_NEWTON:
							computeNewton(zoom, newtonPower);
							break;
					}
					
					/* On termine les vertex points */
					glEnd();
					
					/* On termine la liste des vertex */
					glEndList();
				}
				/* Sinon si on est en mode récursif */
				else if (modeRecursiveFractal)
				{
					/* On commence la liste des vertex */
					glNewList(listVertex, GL_COMPILE);
					
					/* On calcule la fractale récursive */
					switch(fractalId)
					{
						case RECURSIVE_SIERPINSKI_TRIANGLE:
							computeTriangleSierpinski(engine->screen.width, engine->screen.height, recursiveIterationCounter);
							break;
							
						case RECURSIVE_SIERPINSKI_CARRE:
							computeCarreSierpinski(engine->screen.width, engine->screen.height, recursiveIterationCounter);
							break;
						
						case RECURSIVE_LEVY:
							computeLevy(engine->screen.width, engine->screen.height, recursiveIterationCounter);
							break;
					}
					
					/* On termine la liste des vertex */
					glEndList();
				}
				
				/* On dit que la fractale est compilé, donc on pourra l'afficher
				 * au prochain tour de boucle */
				isFractalDraw = true;
			
			}
			else
			{
				/* On appelle la liste enregistrée en mémoire */
				/* Ce qui affiche la fractale */
				glCallList(listVertex);
				
			}
			
			/* On affiche le nombre d'itération des fractales récursives */
			if (modeDrawFractal && modeRecursiveFractal)
			{
				char str[50];
				sprintf(str, "Nombre d'itérations: %d", recursiveIterationCounter);
				fontPrint(engine->font1, str, 20, 20, blanc);	
			}
			
			/* On affiche la constante complexe pour la fractale de julia */
			if (modeDrawFractal && modeComplexFractal && fractalId == COMPLEX_JULIA)
			{
				char str2[60];
				char str3[60];
				complexSprintf(str2, cstJulia[juliaCstId]);
				sprintf(str3, "Constance complexe c = %s", str2);
				fontPrint(engine->font1, str3, 20, 20, blanc);	
			}
			
			
			/* Dessine le cadre de zoom */
			if (showZoomBox)
				drawZoomBox(engine, event.motion.x,engine->screen.height - event.motion.y,sizeZoomBox);
			
			
		}
		
		/* On affiche l'aide */
		/* Touche F1 */
		if (showHelp)
			drawHelp(&images, fractalId);
		
		
		/* On affiche les infos de debug
		 * Touche 'i' */
		if (showInfos)
			showEngineInfos(engine);
		
		/* Fin affichage ---
		 * On envoie tout au GPU */
		glFlush();
		SDL_GL_SwapBuffers();
			
		
	} /* Fin while(running) */
	
	if (listVertex != 0)
		glDeleteLists(listVertex, 1);
		
	
	/* Déchargement des images utilisés */
	imageFree(images.menuComplex);
	imageFree(images.menuRecursive);
	imageFree(images.helpComplex);
	imageFree(images.helpRecursive);
	imageFree(images.docMandelbrot);
	imageFree(images.docJulia);
	imageFree(images.docBurningShip);
	imageFree(images.docNewton);
	imageFree(images.docLevy);
	imageFree(images.docSierpinskiTriangle);
	imageFree(images.docSierpinskiCarre);

	
}


/**
 * \fn void loadingImages(dataImages *images, engineSettings *engine);
 * \brief Permet de gérer le chargement des images du programme
 * 
 * \param *images Pointeur sur la structure stockant les images
 * \param *engine Pointeur sur le moteur graphique
 * \return Rien
 */
void loadingImages(dataImages *images, engineSettings *engine)
{
	/* Chargement de l'image gauche du menu principal */
	images->menuComplex =	imageLoad("img/menu_complex.png");
	images->menuComplex->x = (engine->screen.width - 2 * images->menuComplex->w) / 4;
	images->menuComplex->y = (engine->screen.height - images->menuComplex->h ) / 2;
	
	/* Chargement de l'image droite du menu principal */
	images->menuRecursive =	imageLoad("img/menu_recursive.png");
	images->menuRecursive->x = ((engine->screen.width - 2 * images->menuRecursive->w) / 4) + (engine->screen.width / 2);
	images->menuRecursive->y = (engine->screen.height - images->menuRecursive->h ) / 2;
	
	/* Chargement de l'image d'aide des fractales complexes */
	images->helpComplex =	imageLoad("img/help_complex.png");
	images->helpComplex->x = (engine->screen.width - images->helpComplex->w) / 2;
	images->helpComplex->y = engine->screen.height - images->helpComplex->h;
	
	/* Chargement de l'image d'aide des fractales récursive */
	images->helpRecursive =	imageLoad("img/help_recursive.png");
	images->helpRecursive->x = (engine->screen.width - images->helpRecursive->w) / 2;
	images->helpRecursive->y = engine->screen.height - images->helpRecursive->h;
	
	/* Chargement de l'image de description de la fractale de Mandelbrot */
	images->docMandelbrot =	imageLoad("img/doc_mandelbrot.png");
	images->docMandelbrot->x = (engine->screen.width - images->docMandelbrot->w) / 2;
	images->docMandelbrot->y = 0;
	
	/* Chargement de l'image de description de la fractale de Julia */
	images->docJulia = imageLoad("img/doc_julia.png");
	images->docJulia->x = (engine->screen.width - images->docJulia->w) / 2;
	images->docJulia->y = 0;
	
	/* Chargement de l'image de description de la fractale du "burning ship" */
	images->docBurningShip = imageLoad("img/doc_burning_ship.png");
	images->docBurningShip->x = (engine->screen.width - images->docBurningShip->w) / 2;
	images->docBurningShip->y = 0;
	
	/* Chargement de l'image de description de la fractale de Newton */
	images->docNewton = imageLoad("img/doc_newton.png");
	images->docNewton->x = (engine->screen.width - images->docNewton->w) / 2;
	images->docNewton->y = 0;
	
	/* Chargement de l'image de description de la fractale de Levy */
	images->docLevy = imageLoad("img/doc_levy.png");
	images->docLevy->x = (engine->screen.width - images->docLevy->w) / 2;
	images->docLevy->y = 0;
	
	/* Chargement de l'image de description de la fractale de Sierpinski (Triangle) */
	images->docSierpinskiTriangle =	imageLoad("img/doc_sierpinski_triangle.png");
	images->docSierpinskiTriangle->x = (engine->screen.width - images->docSierpinskiTriangle->w) / 2;
	images->docSierpinskiTriangle->y = 0;
	
	/* Chargement de l'image de description de la fractale de Sierpinski (Carré) */
	images->docSierpinskiCarre = imageLoad("img/doc_sierpinski_carre.png");
	images->docSierpinskiCarre->x = (engine->screen.width - images->docSierpinskiCarre->w) / 2;
	images->docSierpinskiCarre->y = 0;
}

/**
 * \fn bool checkImageClick(image *img, int mouseX, int mouseY);
 * \brief Permet de déterminer si l'utilisateur clique sur une image
 * 
 * \param *image Pointeur sur la structure image
 * \param mouseX Position en abscisse du curseur
 * \param mouseY Position en ordonnée du curseur
 * \return Un booléen, Vrai si l'utilisateur à cliqué sur l'image, Faux sinon.
 */
bool checkImageClick(image *img, int mouseX, int mouseY)
{
	/* Si le curseur est sur l'image on renvoie Vrai, sinon Faux */
	if (mouseX >= img->x && mouseX <= (img->x + img->w) && mouseY >= img->y && mouseY <= (img->y + img->h))
		return true;
	else
		return false;
}


/**
 * \fn void initZoom(fractalSettings *zoom, engineSettings *engine, int fractalId);
 * \brief Permet de déterminer si l'utilisateur clique sur une image
 * 
 * \param *zoom Pointeur sur la structure stockant les paramètres du repère complexe
 * \param *engine Pointeur sur le moteur graphique
 * \param fractalId Stocke l'identifiant de la fractale en cours d'exploration
 * \return Rien
 */
void initZoom(fractalSettings *zoom, engineSettings *engine, int fractalId)
{
	zoom->screenWidth = engine->screen.width;
	zoom->screenHeight = engine->screen.height;
	
	/* Selon la fractale à dessiner on fixe le repère */
	switch(fractalId)
	{
		
		case COMPLEX_MANDELBROT:
			
			zoom->realMin = -2.0;
			zoom->realMax = 1.0;
			zoom->imagMin = -1.1;
			
			zoom->iterMax = 50;
		
		break;
		
		case COMPLEX_JULIA:
			
			zoom->realMin = -2.0;
			zoom->realMax = 2.0;
			zoom->imagMin = -1.35;
			
			zoom->iterMax = 50;
		
		break;
		
		case COMPLEX_BURNING_SHIP:
			
			zoom->realMin = -2.0;
			zoom->realMax = 1.2;
			zoom->imagMin = -1.6;
			
			zoom->iterMax = 40;
		
		break;
		
		case COMPLEX_NEWTON:
			
			zoom->realMin = -2.0;
			zoom->realMax = 2.0;
			zoom->imagMin = -1.5;
			
			zoom->iterMax = 25;
		
		break;
		
		
	}
	
	/* On calcule dynamiquement la hauteur du repère en fonction de la résolution de l'écran */
	/* Cela évite de déformer l'image */
	zoom->imagMax = zoom->imagMin + ((zoom->realMax-zoom->realMin)*(zoom->screenHeight)/(zoom->screenWidth));
	
}

/**
 * \fn void initZoom(fractalSettings *zoom, engineSettings *engine, int fractalId);
 * \brief Permet de déterminer si l'utilisateur clique sur une image
 * 
 * \param *images Pointeur sur la structure stockant les images utilisés dans le programme
 * \param fractalId Stocke l'identifiant de la fractale en cours d'exploration
 * \return Rien
 */
void drawHelp(dataImages *images, int fractalId)
{
	switch(fractalId)
	{
		case COMPLEX_MANDELBROT:
			imageDraw(images->docMandelbrot);
			imageDraw(images->helpComplex);
		break;
		
		case COMPLEX_JULIA:
			imageDraw(images->docJulia);
			imageDraw(images->helpComplex);
		break;
		
		case COMPLEX_BURNING_SHIP:
			imageDraw(images->docBurningShip);
			imageDraw(images->helpComplex);
		break;
		
		case COMPLEX_NEWTON:
			imageDraw(images->docNewton);
			imageDraw(images->helpComplex);
		break;
		
		case RECURSIVE_LEVY:
			imageDraw(images->docLevy);
			imageDraw(images->helpRecursive);
		break;
		
		case RECURSIVE_SIERPINSKI_TRIANGLE:
			imageDraw(images->docSierpinskiTriangle);
			imageDraw(images->helpRecursive);
		break;
		
		case RECURSIVE_SIERPINSKI_CARRE:
			imageDraw(images->docSierpinskiCarre);
			imageDraw(images->helpRecursive);
		break;
	}
}
