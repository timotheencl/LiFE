/**
 * \file engine.c
 * \brief Fichier source, gestion de l'affichage
 * \author Timothée NICOLAS
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.5
 * \date 26/12/2012
 * 
 * Contient toutes les fonctions permettant de dessiner avec SDL/OpenGL
 * Ainsi que la création de fenêtre et la gestion clavier/souris
 * 
 */

#include "engine.h"


/**
 * \fn window createWindow(int width, int height, const char* title);
 * \brief Permet de créer une fenêtre
 * 
 * \param width Largeur de la fenêtre
 * \param height Hauteur de la fenêtre
 * \param title Titre de la fenêtre
 * \return window
 */
void createWindow(int width, int height, const char* title)
{
	SDL_Surface *w = NULL;
	SDL_Surface *icon = NULL;
	
	
	/* Initialisation de la SDL ou FSAA est une constante */
	SDL_Init(SDL_INIT_VIDEO);
	
	/* Anti-aliasing activé */
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  FSAA);
	
	/* On charge l'icone et on vérifie quelle est bien chargé */
	icon = SDL_LoadBMP("img/icon.bmp");
	if (icon == NULL)
	{
		fprintf(stderr, "Erreur lors du chargement de l'icone du programme : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	  
	/* On applique l'icone */
	SDL_WM_SetIcon(icon, NULL);
	
	/* On affecte le mode vidéo souhaité (OPENGL) */
	w = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER);
	
	
	/* On vérifie les erreurs possibles */
	if (w == NULL)
	{
		fprintf(stderr, "Erreur lors du chargement du mode video : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	/* On affecte le titre de la fenêtre */
	SDL_WM_SetCaption(title, title);
	
	/* Initialisation du système de police
	 * vérification des erreurs de chargement */
	if (TTF_Init() < 0)  
	{  
		fprintf(stderr, "Erreur lors du chargement du systeme de police: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);  
	} 
	
	/* On charge la matrice identité de OpenGL */
	glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        
        /* On désactive la 3D */
	glDisable(GL_DEPTH_TEST);
	
	/* On modifie le repère pour qu'il correspond à la taille de la fenêtre */
	gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);	
	
	/* On active la transparence */	
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	/* On désactive le rendu lisse sur les points */
	/*glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST) ;*/

}

/**
 * \fn void startLoop();
 * \brief Permet de démarrer la boucle d'affichage
 * 
 * \return Rien
 */
void startLoop()
{
	
	engineSettings* engine = NULL;
	SDL_Surface* window = NULL;
	
	/* Allocation en mémoire de la structure */
	engine = (engineSettings*) malloc( sizeof(engineSettings) );
	
	/* Informations sur l'écran */
	window = SDL_GetVideoSurface();
	
	
	engine->screen.width = window->w;
	engine->screen.height = window->h;
	engine->screen.bitsPerPixel = window->format->BitsPerPixel;
	
	/* On stocke le pointeur de la police */
	engine->font1 = fontOpen("font/VeraMono.ttf", 12);
	

	/* On démarre le moteur */
	engine->running = true;
	
	/* On appelle la fonction de la boucle
	 * principale
	 */	
	mainLoop(engine);
	
	
	/* Déchargement de la police */
	fontClose(engine->font1);
	
	/* Désallocation des structure */
	free(engine);
}

/**
 * \fn float getFrameRate();
 * \brief Demande le nombre d'image par secondes
 * 
 * \return Le nombre de IPS
 */
int getFrameRate()
{
	static float fps = 0;
	static float returnValue = 0;
	static float lastFrame = 0;
	
	/* On incrémente à chaque image affiché */
	fps++;
	
	/* On teste et regarde le nombre d'images calculé par secondes */
	if (lastFrame + 1000.0f <= SDL_GetTicks())
	{
		lastFrame = SDL_GetTicks();
		returnValue = fps;
		fps = 0;
	}
	
	return returnValue;
}

/**
 * \fn float getFrameRate();
 * \brief Demande le nombre d'image par secondes
 * 
 * \return Le nombre de IPS
 */
void limitFrameRate(int framesPerSecond)
{
	static int time = 0;
	static int lasttime = 0;
	static int diff = 0;
	static int timePerFrame;
	
	/* Calcul du temps écoulé par image */ 
	timePerFrame = 1000/framesPerSecond;
	/* On récupère le nombre de millisecondes écoulés
	 * depuis le lancement du programme */
	time = SDL_GetTicks();
	
	/* On fait la différence des deux temps (réel - ancien) */
	diff = time - lasttime;
	
	/* Si le temps par image est inférieur
	 * à la différence de temps (réel - ancien)
	 */
	if (diff < timePerFrame)
	{
		/* On attent le temps qu'il faut pour équilibrer */
		SDL_Delay(timePerFrame - diff);
		time = SDL_GetTicks();
		diff = time - lasttime;
	}
	
	/* On fixe l'ancien temps, comme temps actuel */
	lasttime = time;

}

/**
 * \fn void showEngineInfos(engineSettings* engine);
 * \brief Affiche les informations du moteur
 * 
 * \param engine Pointeur vers le Moteur
 * \return Rien
 */
void showEngineInfos(engineSettings* engine)
{
	char driverName[256];
	char infosString[50];
	colorRGB white = {255, 255, 255};
	
	/* On récupère les infos à afficher */
	int sWidth	= engine->screen.width;
	int sHeight	= engine->screen.height;
	int sBpp	= engine->screen.bitsPerPixel;
	SDL_VideoDriverName(driverName, 256);
	
	/* On récupère le pointeur de la police */
	TTF_Font* font	= engine->font1;
	
	/* Affiché en 10x et -20y */
	sprintf(infosString, "LiFE v%s - %s OpenGL", VERSION, driverName);
	fontPrint(font, infosString, 10, sHeight - 20, white);
	
	sprintf(infosString,
		"%dx%d @%dbpp - FSAA %dx",
		sWidth,
		sHeight,
		sBpp,
		FSAA);
	fontPrint(font, infosString, 10, sHeight - 38, white);
	
	sprintf(infosString, "FPS: %d", getFrameRate());
	fontPrint(font, infosString, 10, sHeight - 56, white);
}

/**
 * \fn void destroyWindow();
 * \brief Permet de détruire la fenêtre et décharger la SDL/OpenGL
 * 
 * \return Rien
 */
void destroyWindow()
{
	/* On libère le système de police */
	TTF_Quit();
	
	/* On libère le contexte OpenGL et SDL */
	SDL_Quit();
}

/**
 * \fn TTF_Font* fontOpen(char* name, int size);
 * \brief Charge une police.
 * 
 * \param name Chemin vers le fichier .ttf
 * \param size Taille de la police.
 * \return Pointeur vers \e TTF_font
 */
TTF_Font* fontOpen(char* name, int size)
{
	/* On charge la police */
	TTF_Font* font = TTF_OpenFont(name, size);
	 
	/* On vérifie le bon chargement */
	if (font == NULL)
	{
		fprintf(stderr, "Erreur lors du chargement de la"
				" police \"%s\" : "
				"%s\n", name , TTF_GetError());
		exit(EXIT_FAILURE);
	}
	
	return font;
}

/**
 * \fn void fontClose(TTF_Font* font);
 * \brief Décharge une police.
 * 
 * \param font Pointeur vers la police à décharger
 * \return Rien
 */
void fontClose(TTF_Font* font)
{
	/* On décharge la police */
	if (font != NULL)
		TTF_CloseFont(font);
}


/**
 * \fn void fontPrint(TTF_Font* font, const char* string, int x, int y, colorRGB c);
 * \brief Dessine le texte à l'écran, selon une police choisie.
 * 
 * \param font Pointeur vers la police à utiliser.
 * \param string Chaine de caractères représantant le texte à afficher.
 * \param x Position horizontale du texte.
 * \param y Position verticale du texte.
 * \param c Couleur du texte à afficher.
 * \return Rien
 */
void fontPrint(	TTF_Font* font,
		const char* string,
		int x,
		int y,
		colorRGB c)
{
	/* On déclare une couleur format SDL */
	SDL_Color color = {c.red, c.green, c.blue, 0};
	
	/* On déclare la surface à utiliser */
	SDL_Surface* surface = NULL;
	
	/* On déclare la texture */
	GLuint texture;
	GLenum textureFormat;
	GLint bytesPerPixel;
	
	/* On dessine la chaine de caractères */
	surface = TTF_RenderUTF8_Blended(font, string, color);
	
	/* On vérifie qu'il n'y à pas d'erreurs */
	if (surface == NULL)
	{
		fprintf(stderr, "Erreur lors de la création de la"
				"chaine de caractère \"%s\" : %s\n", 
				string, SDL_GetError());
		return;
	}
	
	/* Couleur noir et opacité alpha 30% */
	glColor4f(0,0,0, 0.6);
	
	/* On crée un vertex OpenGl pour asombrir le fond de la police
	 * Plus large de 2 px et haut de 1 px */
	glBegin(GL_QUADS);
		glVertex2f(x - 2, y + surface->h + 1);
		glVertex2f(x - 2, y - 1 );
		glVertex2f(x + surface->w + 2, y - 1);
		glVertex2f(x + surface->w + 2, y + surface->h + 1);
	glEnd();
	
	/* On active le mode texture de OpenGL */
	glEnable(GL_TEXTURE_2D);
	/* On reset la couleur et la transparence à 1*/
	glColor4ub(255,255,255,255);
	
	/* On regarde si la surface est en RVB ou RVBA (transparence) */
	bytesPerPixel = surface->format->BytesPerPixel;
	if (bytesPerPixel == 4)
	{
		if (surface->format->Rmask == 0x000000ff)
			textureFormat = GL_RGBA;
		else
			textureFormat = GL_BGRA;
	}
	else
	{
		if (surface->format->Rmask == 0x000000ff)
			textureFormat = GL_RGB;
		else
			textureFormat = GL_BGR;
	}
	
	/* On alloue la texture OpenGL */
	glGenTextures(1, &texture);
	
	/* On spécifie la texture à utiliser pour les prochains vertices */
	glBindTexture(GL_TEXTURE_2D, texture);
	
	/* On paramètre la texture OpenGL */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        /* On applique la surface SDL vers la texture OpenGL */
	glTexImage2D(GL_TEXTURE_2D, 0, bytesPerPixel, surface->w, 
			surface->h, 0, textureFormat, GL_UNSIGNED_BYTE,
			surface->pixels);
	
	/* On crée un vertex OpenGl pour afficher la texture */
	glBegin(GL_QUADS);
		glTexCoord2d(0,0); glVertex2f(x, y + surface->h);
		glTexCoord2d(0,1); glVertex2f(x, y);
		glTexCoord2d(1,1); glVertex2f(x + surface->w, y);
		glTexCoord2d(1,0); glVertex2f(x + surface->w, y + surface->h);
	glEnd();
	
	/* On libère la texture OpenGL et la surface SDL */
	SDL_FreeSurface(surface);
	glDeleteTextures(1, &texture);
	
}

/**
 * \fn image* imageLoad(char* filename);
 * \brief Charge en mémoire une image.
 * 
 * \param filename Chemin vers le fichier image
 * \return Un pointeur vers l'image chargé
 */
image* imageLoad(char* filename)
{
	image *img = NULL;
	SDL_Surface *surface = NULL;
	
	/* On alloue la structure image */
	img = (image*) malloc (sizeof(image));
	
	if (img == NULL)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'image"
				": \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}
	
	/* On charge l'image dans la surface SDL */
	surface = IMG_Load(filename);
	
	/* On vérifie les erreurs possibles */
	if (surface == NULL)
	{
		fprintf(stderr, "Erreur lors du chargement de l'image"
				" \"%s\" : "
				"%s\n", filename , IMG_GetError());
		exit(EXIT_FAILURE);
	}
	
	/* On remplit la structure image */
	img->surface = surface;
	img->w = surface->w;
	img->h = surface->h;
	
	/* On renvoit le pointeur */
	return img;	
}

/**
 * \fn void imageFree(image *img);
 * \brief Décharge une image.
 * 
 * \param img Pointeur vers l'image en mémoire
 * \return Rien
 */
void imageFree(image *img)
{
	/* On décharge l'image si le pointeur est
	 * différent de NULL
	 */
	if (img != NULL)
	{
		if (img->surface != NULL)
			SDL_FreeSurface(img->surface);
		
		free(img);
	}
}

/**
 * \fn void imageDraw(image *img);
 * \brief Affiche l'image à l'écran.
 * 
 * \param img Pointeur vers l'image en mémoire
 * \return Rien
 */
void imageDraw(image *img)
{
	/* Déclaration de la texture */
	GLuint texture;
	
	/* On génère la texture */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	/* On lui applique les paramètres par défaut */
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	/* On reset la couleur par defaut */
	glColor4ub(255,255,255,255);
	
	/* On vérouille la surface */
	SDL_LockSurface(img->surface);
	
	/* On stocke la texture en mémoire */	
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, img->w, img->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, img->surface->pixels);
	
	/* On dévérouille la surface */
	SDL_UnlockSurface(img->surface);
	
	/* On active le mode texture de OpenGL */
	glEnable (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	/* On spécifie les vertex */
	glBegin (GL_QUADS);
		glTexCoord2d (0,0); glVertex2f(img->x, img->y + img->h);
		glTexCoord2d (0,1); glVertex2f(img->x, img->y);
		glTexCoord2d (1,1); glVertex2f(img->x + img->w, img->y);
		glTexCoord2d (1,0); glVertex2f(img->x + img->w, img->y + img->h);
	glEnd ();
	
	/* On désactive le mode texture et
	 * on libère la texture
	 */
	glDisable (GL_TEXTURE_2D);
	glDeleteTextures(1, &texture);
}

/**
 * \fn void drawZoomBox(engineSettings* engine, int x, int y, double size);
 * \brief Dessine le cadre de zoom à l'écran.
 * 
 * \param engine Pointeur vers les paramètres du moteur
 * \param x Centre horizontal du cadre
 * \param y Centre vertical du cadre
 * \param size Facteur de zoom [0 à 1]
 * \return Rien
 */
void drawZoomBox(engineSettings* engine, int x, int y, double sizeBox)
{
	int width, height;
	
	/* Evite le facteur de zoom négatif */
	if (sizeBox > 1.0)
		sizeBox = 1.0;
	
	/* Demi largeur et hauteur de la boite de zoom */
	width = engine->screen.width * sizeBox / 2;
	height = engine->screen.height * sizeBox / 2;
	
	/* Couleur rouge */
	glColor3ub(255,0,0);
	
	
	glBegin(GL_LINES);
		/* Ligne gauche */
		glVertex2f(x - width, y - height);
		glVertex2f(x - width, y + height);
		
		/* Ligne haut */
		glVertex2f(x - width, y + height);
		glVertex2f(x + width, y + height);
		
		/* Ligne droite */
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y - height);
		
		/* Ligne bas */
		glVertex2f(x + width, y - height);
		glVertex2f(x - width, y - height);
	glEnd();
}


/**
 * \fn void setNewScale(fractalSettings* zoom, int x, int y, double sizeZoom);
 * \brief Met à jour le repère complexe en fonction des valeurs du zoom
 * 
 * \param zoom Pointeur vers les valeurs du repère à modifier
 * \param x Centre horizontal du repère
 * \param y Centre vertical du repère
 * \param size Facteur de zoom [0 à 1]
 * \return Rien
 */
void setNewScale(fractalSettings *zoom, int x, int y, double sizeZoom)
{
	/* Récupération de la résolution de la fenêtre */
	double scrW = zoom->screenWidth;
	double scrH = zoom->screenHeight;
	
	/* Récupération des anciennes valeurs du repère */
	double lastRealMin = zoom->realMin;
	double lastRealMax = zoom->realMax;
	double lastImagMin = zoom->imagMin;
	double lastImagMax = zoom->imagMax;
	
	/* Calcul de la dimension de la boite de zoom en pixel */
	double offsetHeight = scrH / 2.0 * sizeZoom; 
	double offsetWidth = scrW / 2.0 * sizeZoom; 
	
	/* On inverse l'axe des ordonnées OpenGL */
	y = scrH - y;
	
	/* On met à jour les nouvelle limites du repère */
	zoom->realMin = ((x - offsetWidth) / scrW) * (lastRealMax - lastRealMin) + lastRealMin;
	zoom->realMax = ((x + offsetWidth) / scrW) * (lastRealMax - lastRealMin) + lastRealMin;
	zoom->imagMin = ((y - offsetHeight) / scrH) * (lastImagMax - lastImagMin) + lastImagMin;
	zoom->imagMax = ((y + offsetHeight) / scrH) * (lastImagMax - lastImagMin) + lastImagMin;
}


/**
 * \fn void zoomStackClear(zoomStack** stack);
 * \brief Retire tous les élements de la pile
 * 
 * \param stack Adresse du pointeur de la pile
 * \return Rien
 */
void zoomStackClear(zoomStack **stack)
{
	while (*stack != NULL)
	{
		zoomStackPop(stack,NULL,NULL,NULL,NULL);
	}
}

/**
 * \fn void zoomStackPush(zoomStack** stack, double xMin, double xMax, double yMin, double yMax);
 * \brief Empile un élement dans la pile
 * 
 * \param stack Adresse du pointeur de la pile
 * \param xMin Valeur minimum  en abscisse du repère à stocker
 * \param xMax Valeur maximum  en abscisse du repère à stocker
 * \param yMin Valeur minimum  en ordonnée du repère à stocker
 * \param yMax Valeur maximum  en ordonnée du repère à stocker
 * \return Rien
 */
void zoomStackPush(	zoomStack **stack,
			double xMin,
			double xMax,
			double yMin,
			double yMax)
{
	zoomStack* newElement = NULL;
	
	newElement = (zoomStack*) malloc( sizeof(zoomStack) );
	
	if (newElement != NULL)
	{
		newElement->xMin = xMin;
		newElement->xMax = xMax;
		newElement->yMin = yMin;
		newElement->yMax = yMax;
		
		newElement->previous = *stack;
		*stack = newElement;
	}
	else
	{
		fprintf(stderr, "Erreur d'allocation lors de l'ajout des "
				"valeurs de zoom dans la pile\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * \fn void zoomStackPop(zoomStack** stack, double* xMin, double* xMax, double* yMin, double* yMax);
 * \brief Dépile un élement de la pile
 * 
 * \param stack Adresse du pointeur de la pile
 * \param xMin Pointeur vers la valeur minimum  en abscisse du repère à stocker
 * \param xMax Pointeur vers la valeur maximum  en abscisse du repère à stocker
 * \param yMin Pointeur vers la valeur minimum  en ordonnée du repère à stocker
 * \param yMax Pointeur vers la valeur maximum  en ordonnée du repère à stocker
 * \return Rien
 */
void zoomStackPop(	zoomStack **stack,
			double *xMin,
			double *xMax,
			double *yMin,
			double *yMax)
{
	if (*stack != NULL)
	{
		zoomStack* tmp = (*stack)->previous;
		if (xMin != NULL && xMax != NULL && yMin != NULL && yMax != NULL)
		{
			*xMin = (*stack)->xMin;
			*xMax = (*stack)->xMax;
			*yMin = (*stack)->yMin;
			*yMax = (*stack)->yMax;
		}
		free(*stack);
		*stack = tmp;
		
		
	}

}



