#include<allegro.h>
#include"structure.h"


/// SOUS PROG DE CALCUL ET D'IMAGE

BITMAP * load_bitmap_check(char *nomImage)
{
    BITMAP *bmp;
    bmp=load_bitmap(nomImage,NULL);
    if (!bmp)
    {
        allegro_message("pas pu trouver %s",nomImage);
        exit(EXIT_FAILURE);
    }
    return bmp;
}

int collisionDecor(t_joueur *vaisseau, BITMAP *decor, int couleur)
{
    int contact=0;

    if (       getpixel(decor, vaisseau->x, vaisseau->y) == couleur
               ||  getpixel(decor, vaisseau->x+vaisseau->tx, vaisseau->y) == couleur
               ||  getpixel(decor, vaisseau->x, vaisseau->y+vaisseau->ty) == couleur
               ||  getpixel(decor, vaisseau->x+vaisseau->tx, vaisseau->y+vaisseau->ty) == couleur )
        {
            contact=1;
        }

    return contact;
}


void collisionVaisseauEnnemi(BITMAP *bmp, t_joueur*vaisseau, t_ennemi*ennemi)
{
    int rouge = makecol(255,0,0);

    if (       getpixel(bmp, vaisseau->x, vaisseau->y) == rouge
               ||  getpixel(bmp, vaisseau->x+vaisseau->tx, vaisseau->y) == rouge
               ||  getpixel(bmp, vaisseau->x, vaisseau->y+vaisseau->ty) == rouge
               ||  getpixel(bmp, vaisseau->x+vaisseau->tx, vaisseau->y+vaisseau->ty) == rouge )
        {
            vaisseau->comportement=1;
            //ennemi->comportement=1;
        }

}


void collisionMissileVaisseau(BITMAP *bmp,t_joueur*vaisseau, t_acteur *acteur)
{
    int vert = makecol(0,255,0);
    int i=0;

    /*if (       getpixel(bmp, acteur->x, acteur->y) == vert
               ||  getpixel(bmp, acteur->x+acteur->ty, acteur->y) == vert )
        {
            vaisseau->comportement=1;

        }*/

        for(i=0; i<acteur->ty;i++)
        {
            if(getpixel(bmp, acteur->x + i , acteur->y) == vert)
            {
                    vaisseau->comportement=1;
            }
        }

}



