#include<allegro.h>
#include"structure.h"


/// SOUS PROG GESTION ACTEUR


// Allouer et initialiser un acteur
t_acteur * creerActeur(int x,int y,int type,char *nomimage)
{
    t_acteur *acteur;

    // Allouer
    acteur=(t_acteur*)malloc(sizeof(t_acteur));
    //Initialiser
    acteur->x=x;
    acteur->y=y;
    acteur->type=type;
    acteur->comportement=0;
    acteur->vivant=1;

    switch (type)
    {
    // missile
    case 1:
        acteur->tx=14;
        acteur->ty=7;
        acteur->dx=3;
        acteur->dy=0;
        acteur->img=load_bitmap_check(nomimage);
        break;

    // tir special
    case 2:
        acteur->tx=21;
        acteur->ty=21;
        acteur->dx=10;
        acteur->dy=0;
        acteur->img=load_bitmap_check(nomimage);
        break;

    // tir ennemi
    case 3:
        acteur->dx=5;
        acteur->dy=0;
        acteur->tx=16;
        acteur->ty=7;
        acteur->img=load_bitmap_check(nomimage);
        break;

    // tir boss
    case 4:
        acteur->dx=7;
        acteur->dy=0;
        acteur->tx=17;
        acteur->ty=20;
        acteur->img=load_bitmap_check(nomimage);
        break;

    }

    return acteur;
}

void actualiserActeur(t_acteur *acteur, BITMAP*bmp)
{
    int blanc = makecol(255,255,255);

    rectfill(bmp, acteur->x, acteur->y, acteur->x + acteur->tx, acteur->y + acteur->ty, blanc);

    // déplacement
    acteur->x=acteur->x+acteur->dx;
    acteur->y=acteur->y+acteur->dy;

    // si dépasse le bord alors disparait
    if (acteur->x+acteur->tx<0 || acteur->x>SCREEN_W || acteur->y+acteur->ty<0 || acteur->y>SCREEN_H )
        acteur->vivant=0;

}

// Dessiner un acteur sur la bitmap bmp
void dessinerActeur(BITMAP *bmp,BITMAP *bmp2,t_acteur *acteur)
{
    int bleu = makecol(0,0,255);

    // Si pas d'explosion
    if (acteur->comportement==0)
    {
        switch(acteur->type)
        {
        case 1:
            draw_sprite(bmp,acteur->img,acteur->x,acteur->y);

            rectfill(bmp2, acteur->x, acteur->y, acteur->x + acteur->tx, acteur->y + acteur->ty, bleu);
            break;

        case 2:
            draw_sprite(bmp,acteur->img,acteur->x,acteur->y);

            rectfill(bmp2, acteur->x, acteur->y, acteur->x + acteur->tx, acteur->y + acteur->ty, bleu);
            break;
        }
    }

}


/// SOUS PROG GESTION LISTE DE ACTEUR

// Allouer et initialiser une liste (vide) de acteurs

t_listeActeurs * creerListeActeurs(int maxacteurs)
{
    t_listeActeurs *liste;
    int i;

    liste=(t_listeActeurs*)malloc(sizeof(t_listeActeurs));

    liste->max=maxacteurs;
    liste->n=0;
    liste->tab=(t_acteur**)malloc(maxacteurs*sizeof(t_acteur*));

    for (i=0; i<maxacteurs; i++)
        liste->tab[i]=NULL;

    return liste;
}

// Allouer et ajouter un acteur à la liste de acteurs

t_acteur * ajouterActeur(t_listeActeurs *liste,int x,int y,int type,BITMAP*bmp)
{
    int i=0;
    t_acteur *acteur;

    // Liste pleine, on alloue rien et on retourne NULL...
    if (liste->n >= liste->max)
        return NULL;

    // Allouer un acteur initialisé
    acteur=creerActeur(x,y,type,bmp);

    // Chercher un emplacement libre dans la liste
    while (liste->tab[i]!=NULL && i<liste->max)
        i++;

    if (i<liste->max)
    {
        // Accrocher le acteur à l'emplacement trouvé
        liste->tab[i]=acteur;
        liste->n++;
    }
    // Sinon c'est qu'il y a un problème de cohérence
    else
    {
        allegro_message("Anomalie gestion ajouterActeur : liste corrompue");
    }

    return acteur;
}


// Enlever et libérer un acteur qui était dans la liste en indice i
void enleverActeur(t_listeActeurs *liste,int i)
{
    // Si il y a bien un acteur accroché en indice i
    if (liste->tab[i]!=NULL)
    {
        // libérer la mémoire du acteur
        free(liste->tab[i]);

        // marquer l'emplacement comme libre
        liste->tab[i]=NULL;
        liste->n--;

    }
}


// Gérer l'évolution de l'ensemble de la liste
void actualiserListeActeurs(t_listeActeurs *liste, BITMAP*bmp)
{
    int i;
    // actualiser chaque acteur (vivant ou mort)
    for (i=0; i<liste->max; i++)
        if (liste->tab[i]!=NULL)
        {
            actualiserActeur(liste->tab[i], bmp);
            if (!liste->tab[i]->vivant)
            {
                enleverActeur(liste,i);
            }
        }
}

// Dessiner sur une bitmap l'ensemble des acteurs
void dessinerListeActeurs(BITMAP *bmp,BITMAP *bmp2,t_listeActeurs *liste)
{
    int i;

    for (i=0; i<liste->max; i++)
    {
        if (liste->tab[i]!=NULL)
        {
            dessinerActeur(bmp,bmp2,liste->tab[i]);
        }
    }
}


/// SOUS PROG GESTION COLLISION

/*
// collision entre un acteur (tir) et un ennemi
void collisionActeur(t_ennemi *ennemi,t_acteur *acteur)
{
    int vx[12]={0};
    int vy[12]={0};
    int d2[12]={0};
    int i=0;

    if ( acteur->comportement==0 )
    {
        // calcul du vecteur entre acteur et centre de l'ennemi
        for(i=0; i<12; i++)
        {
            vx[i] = (acteur->x+acteur->tx/2) - (ennemi->x[i]+ennemi->tx/2);
            vy[i] = (acteur->y+acteur->ty/2) - (ennemi->y[i]+ennemi->ty/2);
        }

        // calcul distance au carré entre acteur et centre de l'ennemi
        for(i=0; i<12; i++)
        {
            d2[i] = vx[i]*vx[i] + vy[i]*vy[i];
        }

        // si les 2 se touchent alors on appelle consequence
        for(i=0; i<12; i++)
        {
            if ( d2[i] < ennemi->tx * ennemi->tx / 4 )
                consequenceActeur(acteur);
        }

    }

}*/

/*
// collision entre les acteurs (tous les tirs) et un ennemi
void collisionListeActeurs(t_ennemi *ennemi,t_listeActeurs *liste)
{
    int i;

    for (i=0; i<liste->max; i++)
    {
        if (liste->tab[i]!=NULL)
        {
            collisionActeur(ennemi,liste->tab[i]);
        }

    }

}*/


/// SOUS PROG GESTION JOUEUR

// Allouer et initialiser un joueur
t_joueur * creerJoueur(char *nomimage)
{
    t_joueur *joueur;

    // Allouer
    joueur = (t_joueur*)malloc(sizeof(t_joueur));

    // Initialiser
    joueur->img=load_bitmap_check(nomimage);

    joueur->tx = joueur->img->w;
    joueur->ty = joueur->img->h;

    joueur->x = SCREEN_W/8-joueur->tx/2;
    joueur->y = SCREEN_H/2-joueur->ty/2;

    joueur->dx=3;
    joueur->dy=3;

    joueur->tempotir=0;
    joueur->comportement=0;
    joueur->tempexplo=0;

    return joueur;
}



// Actualiser joueur (bouger et tirer)
void actualiserJoueur(t_joueur *joueur,  t_listeActeurs*liste1, t_listeActeurs*liste2, BITMAP*bmp, BITMAP *bmp2)
{
    int blanc = makecol(255,255,255);

    // Déplacements instantanés, nouvelle position
    if (key[KEY_UP])
    {
        rectfill(bmp2,joueur->x, joueur->y, joueur->x + joueur->tx, joueur->y + joueur->ty, blanc );
        joueur->y -= joueur->dy;
        if (joueur->y<0)
            joueur->y=0;
    }

    if (key[KEY_DOWN])
    {
        rectfill(bmp2,joueur->x, joueur->y, joueur->x + joueur->tx, joueur->y + joueur->ty, blanc );
        joueur->y += joueur->dy;
        if (joueur->y+joueur->ty > SCREEN_H)
            joueur->y=SCREEN_H-joueur->ty;
    }

    if (key[KEY_LEFT])
    {
        rectfill(bmp2,joueur->x, joueur->y, joueur->x + joueur->tx, joueur->y + joueur->ty, blanc );
        joueur->x -= joueur->dx;
        if (joueur->x<0)
            joueur->x=0;
    }

    if (key[KEY_RIGHT])
    {
        rectfill(bmp2,joueur->x, joueur->y, joueur->x + joueur->tx, joueur->y + joueur->ty, blanc );
        joueur->x += joueur->dx;
        if (joueur->x+joueur->tx > SCREEN_W)
            joueur->x=SCREEN_W-joueur->tx;
    }

    // incrémenter la tempo des tirs
    if(key[KEY_SPACE])
    {
        joueur->tempotir++;
    }

    //Gestion du tir
    // espace = missile
    if(!key[KEY_SPACE] && joueur->tempotir>=15 && joueur->tempotir<100)
    {
        ajouterActeur(liste1,joueur->x+joueur->tx,joueur->y+joueur->ty/2-3,1,"tir1.bmp");
        joueur->tempotir=0;

    }

    if(!key[KEY_SPACE] && joueur->tempotir>=100)
    {
        ajouterActeur(liste2,joueur->x+joueur->tx,joueur->y+joueur->ty/2-3,2,"tir_special.bmp");
        joueur->tempotir=0;

    }

    if(joueur->tempotir>=100)
    {
        joueur->tempotir=100;
    }

    if (joueur->comportement==1)
    {
        joueur->tempexplo++;

        if (joueur->tempexplo > 10)
            joueur->vivant=0;
    }

}


// Dessiner joueur sur la bitmap bmp
void dessinerJoueur(BITMAP *bmp,BITMAP *bmp2, BITMAP *bmp3, t_joueur *joueur)
{
    int vert = makecol(0,255,0);
    int vitesseEcran = 2;

    // Si pas d'explosion
    if(joueur->comportement==0)
    {
        draw_sprite(bmp,joueur->img,joueur->x,joueur->y);

        //afficher nouvelle position
        rectfill(bmp2,joueur->x, joueur->y, joueur->x + joueur->tx, joueur->y + joueur->ty, vert);

    }

    // sinon on dessine l'explosion
    else
    {
        joueur->dx=0;
        joueur->dy=0;
        joueur->x=joueur->precx-vitesseEcran;
        draw_sprite(bmp,bmp3,joueur->x-2+joueur->tx/8, joueur->y);
    }

}

