/**
 * \file recursiveFractals.c
 * \brief Fichier source, contient les fonctions permettant de construire les fractales de type récursive
 * \author Nicolas SILVAIN
 * \author Nicolas NATIVO
 * \version 1.3
 * \date 03/02/2012
 *
 * Dessine les fractale de type récursive
 *
 */


#include "recursiveFractals.h"

/**
 * \fn void computeLevy(int LargeurFenetre, int HauteurFenetre);
 * \brief Construit la fractale de Levy avec le fond coloré
 *
 * \param LargeurFenetre Largeur de la fenetre en pixels
 * \param HauteurFenetre Hauteur de la fenetre en pixels
 * \param iteration Nombre de fois qu'il faut construire répeter l'opération de construction
 * \return Rien
 */
void computeLevy(int LargeurFenetre, int HauteurFenetre, int iteration)
{
	/* Points délimitant le segment de base */
	new_point a,b;

	/* Placement du segment au centre de la fenêtre graphique */
	a.abs = LargeurFenetre/3;
	a.ord = HauteurFenetre/2;

	b.abs = 2*LargeurFenetre/3;
	b.ord = HauteurFenetre/2;

	
	/* Fonction de traçage de la courbe de Lévy */
	create_levy(iteration, a.abs, a.ord, b.abs, b.ord);	/* Partie inférieur de la fenêtre */
	create_levy(iteration, b.abs, b.ord, a.abs, a.ord);	/* Partie supérieur de la fenêtre */
}

/**
 * \fn void create_levy(int iteration, float abscisse_X, float ordonnee_X, float abscisse_Y, float ordonnee_Y);
 * \brief Construit la fractale de Levy
 *
 * \param iteration nombre de fois que l'on veut découper le segment
 * \param abscisse_X abscisse du premier point du segment
 * \param ordonnee_X ordonnée du premier point du segment
 * \param abscisse_Y abscisse du deuxième point du segment
 * \param ordonnee_Y ordonnée du deuxième point du segment
 * \return Rien
 */
void create_levy(int iteration, float abscisse_X, float ordonnee_X, float abscisse_Y, float ordonnee_Y)
{
	/* Point intermédiaire (découpage d'un segment en deux) */
	float abscisse_C, ordonnee_C;


		/* Si l'on est à la dernière itération, on trace les lignes */
		if(iteration <= 1)
		{
			/* Changement de la couleur de la courbe en fonction de la position de la ligne */
			glColor3ub(255, 255, 255);
			
			/* On dessine la ligne */
			glBegin(GL_LINES);
				glVertex2f(abscisse_X, ordonnee_X);
				glVertex2f(abscisse_Y, ordonnee_Y);
			glEnd();

		} /* sinon on continue la division des segments à l'itération suivante */
		else
		{
			/* Calcul des coordonnées du 3ème point d'un triangle isocèle rectangle en ce point */
			abscisse_C = (abscisse_X + abscisse_Y)/2 + (ordonnee_Y - ordonnee_X)/2;
			ordonnee_C = (ordonnee_X + ordonnee_Y)/2 - (abscisse_Y - abscisse_X)/2;
			/* Découpe du segment à l'itération suivante */
				create_levy(iteration-1, abscisse_X, ordonnee_X, abscisse_C, ordonnee_C);
				create_levy(iteration-1, abscisse_C, ordonnee_C, abscisse_Y, ordonnee_Y);
		}
}


/**
 * \fn void enleveTriangle(float x1,float y1,int iterationmax,float segment);
 * \brief Fonction recursive qui enlève le centre de chaque triangle
 *
 * \param x1 Position en x du coin inférieur gauche
 * \param y1 Position en y du coin inférieur gauche
 * \param iterationmax Nombre de fois où il faut passer dans la fonction
 * \param segment Taille en pixel d'un segment du triangle
 * \return Rien
 */
void enleveTriangle(float x1,float y1,int iterationmax,float segment)
{

		static int iteration=1;
		float i,j,decalageLigne=1,x1ancien=x1,x2ancien=x1,x3ancien=x1,y1ancien=y1,y2ancien=y1,y3ancien=y1;


		if(iteration>iterationmax)
		{
			iteration=1;
			return;
		}
	
		/* Noir */
		glColor3ub(0,0,0);

		/* boucle pour les colonnes */
		for(i=pow(2,iteration)-1;i>0;i=i-2)
		{
			/* boucle pour les lignes */
			for(j=(pow(2,iteration)-2*(decalageLigne-1))-1;j>0;j--)
			{

				glBegin(GL_TRIANGLES);
					glVertex2f(x1ancien +  (segment / pow(2,iteration+1)),(y1ancien+  (segment * sqrt(3) / pow(2,iteration+1))));
					glVertex2f((x2ancien+ (3*  segment / pow(2,iteration+1) )),(y2ancien+  (segment * sqrt(3) / pow(2,iteration+1) )));
					glVertex2f(x3ancien+   (segment*2 / pow(2,iteration+1)),y3ancien);
				glEnd();

				/* calculs des positions en x suivantes */
				x1ancien=  2*segment / pow(2,iteration+1) +x1ancien;

				x2ancien=2*  segment / pow(2,iteration+1) +x2ancien;

				x3ancien=2*segment / pow(2,iteration+1)+x3ancien;

			}

			/* calculs des positions en x de la prochaine ligne */
			x1ancien=x1+decalageLigne*2*segment / pow(2,iteration+1);
			x2ancien=x1ancien;
			x3ancien=x1ancien;

			/* calculs des positions en y de la prochaine ligne */
			y1ancien= segment * (sqrt(3) / pow(2,iteration)) +y1ancien;
			y2ancien=  segment * (sqrt(3) / pow(2,iteration)) +y2ancien;
			y3ancien= segment * (sqrt(3) / pow(2,iteration)) + y3ancien;

			decalageLigne++;
		}

		iteration++;

		enleveTriangle(x1,y1,iterationmax,segment);



		return;
}

/**
 * \fn void computeTriangleSierpinski(int LargeurFenetre, int HauteurFenetre,int iteration);
 * \brief Construit le triangle de Sierpinski
 *
 * \param LargeurFenetre Largeur de la fenetre en pixels
 * \param HauteurFenetre Hauteur de la fenetre en pixels
 * \param iteration Nombre de fois qu'il faut construire le triangle
 * \return Rien
 */
void computeTriangleSierpinski(int LargeurFenetre, int HauteurFenetre,int iteration)
{
	float segment=500.0;
	float xhaut,yhaut,xbasgauche,ybasgauche,xbasdroit,ybasdroit;

	xhaut=LargeurFenetre/2;
	yhaut=HauteurFenetre/2+(segment*sqrt(3)/4);
	xbasgauche=(LargeurFenetre/2)-segment/2;
	ybasgauche=HauteurFenetre/2+(segment*sqrt(3)/4)-segment*sqrt(3)/2;
	xbasdroit=(LargeurFenetre/2)+segment/2;
	ybasdroit=HauteurFenetre/2+(segment*sqrt(3)/4)-segment*sqrt(3)/2;

	/* Blanc */
	glColor3ub(255,255,255);

	/*place le triangle de base*/
	glBegin(GL_TRIANGLES);
		glVertex2f(xhaut,yhaut);
		glVertex2f(xbasgauche,ybasgauche);
		glVertex2f(xbasdroit,ybasdroit);
	glEnd();

	enleveTriangle(xbasgauche,ybasgauche,iteration,segment);
}

/**
 * \fn void enleveCarre(float x1,float y1,int iterationmax,float segment);
 * \brief Fonction recursive qui enlève des carrés autour d'un autre carré
 *
 * \param x1 Position en x du coin inférieur gauche
 * \param y1 Position en y du coin inférieur gauche
 * \param iterationmax Nombre de fois où il faut passer dans la fonction
 * \param segment Taille en pixel d'un segment du triangle
 * \return Rien
 */
void enleveCarre(float x1,float y1,int iterationmax,float segment)
{

		static int iteration=1;
		float i,j,x1ancien,x2ancien,y1ancien,y2ancien;
		
		
		if(iteration>iterationmax)
		{
			iteration=1;
			return;
		}

		glColor3ub(0, 0, 0);

		/* calcul des points de bases */
			x1ancien=x1+segment/pow(3,iteration);
			x2ancien=x1ancien+segment/pow(3,iteration);
			y1ancien=y1+segment/pow(3,iteration);
			y2ancien=y1ancien+segment/pow(3,iteration);

		/* boucle pour les colonnes */
		for(i=0;i<pow(3,iteration-1);i++)
		{


			/* boucle pour les lignes */
			for(j=0;j<pow(3,iteration-1);j++)
			{
				
				glBegin(GL_QUADS);
					glVertex2f(x1ancien,y1ancien);
					glVertex2f(x2ancien,y1ancien);
					glVertex2f(x2ancien,y2ancien);
					glVertex2f(x1ancien,y2ancien);
				glEnd();

				/* calculs des positions en x suivantes */
				x1ancien=  3*segment / pow(3,iteration) +x1ancien;

				x2ancien= segment/pow(3,iteration) +x1ancien;



			}

			x1ancien=x1+segment/pow(3,iteration);
			x2ancien=x1ancien+segment/pow(3,iteration);

			y1ancien=y1ancien+3*segment/pow(3,iteration);
			y2ancien=y1ancien+segment/pow(3,iteration);
		}

		iteration++;

		enleveCarre(x1,y1,iterationmax,segment);


		
		
		return;
}


/**
 * \fn void computeCarreSierpinski(int LargeurFenetre, int HauteurFenetre,int iteration);
 * \brief Construit le carré de Sierpinski
 *
 * \param LargeurFenetre Largeur de la fenetre en pixels
 * \param HauteurFenetre Hauteur de la fenetre en pixels
 * \param iteration Nombre de fois qu'il faut construire le carre
 * \return Rien
 */
void computeCarreSierpinski(int LargeurFenetre, int HauteurFenetre,int iteration)
{
	float xbasgauche,ybasgauche,xhautdroit,yhautdroit,segment=500.0;

	xbasgauche=(LargeurFenetre/2)-segment/2;
	ybasgauche=HauteurFenetre/2-segment/2;
	xhautdroit=(LargeurFenetre/2)+segment/2;
	yhautdroit=HauteurFenetre/2+segment/2;

	glColor3ub(255,255,255);

	glBegin(GL_QUADS);
		glVertex2f(xbasgauche,ybasgauche);
		glVertex2f(xhautdroit,ybasgauche);
		glVertex2f(xhautdroit,yhautdroit);
		glVertex2f(xbasgauche,yhautdroit);
	glEnd();

	enleveCarre(xbasgauche,ybasgauche,iteration,segment);
	
	
}
