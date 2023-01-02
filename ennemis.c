#include<allegro.h>
#include"structure.h"



t_ennemi * creerEnnemi(char *nomimage)
{
    t_ennemi *ennemi;

    // Allouer
    ennemi = (t_ennemi*)malloc(sizeof(t_ennemi));

    // Initialiser
    ennemi->img=load_bitmap_check(nomimage);
    ennemi->comportement=0;

    ennemi->dx=2;
    ennemi->dy=0;
    ennemi->tx=63;
    ennemi->ty=40;

    return ennemi;
}



void determinerPosEnnemi(t_ennemi* ennemi)
{
    int i;

    for(i=0; i<12; i++)
    {
        ennemi->x[i]=600;
        ennemi->y[i]=rand()%(650-204+1);
    }

}


void actualiserEnnemi(BITMAP *bmp, t_ennemi* ennemi,int tempo,t_acteur *acteur)
{
    int i;
    int NbTir=0;
    int blanc = makecol(255,255,255);

    if(tempo>=100)
    {
        for(i=0; i<3; i++)
        {
            rectfill(bmp,ennemi->x[i]+300,ennemi->y[i], ennemi->x[i]+300 + ennemi->tx, ennemi->y[i] + ennemi->ty, blanc);

            ennemi->x[i] -=ennemi->dx;
        }
    }

    if(tempo>=600)
    {
        for(i=3; i<6; i++)
        {
            rectfill(bmp,ennemi->x[i]+300,ennemi->y[i], ennemi->x[i]+300 + ennemi->tx, ennemi->y[i] + ennemi->ty, blanc);

            ennemi->x[i] -=ennemi->dx;
        }
    }

    if(tempo>=1100)
    {
        for(i=6; i<9; i++)
        {
            rectfill(bmp,ennemi->x[i]+300,ennemi->y[i], ennemi->x[i]+300 + ennemi->tx, ennemi->y[i] + ennemi->ty, blanc);

            ennemi->x[i] -=ennemi->dx;
        }
    }

    if(tempo>=1600)
    {

        for(i=9; i<12; i++)
        {

            ennemi->x[i] -=ennemi->dx;

            if(ennemi->x[i]<=0)
                ennemi->x[i]=600;
            ///A FAIRE DISPARAITRE QUAND L'ENNEMI MEURT

            ennemi->missileEnnemi[NbTir].x = ennemi->x[i] - acteur->img->w+80;
            ennemi->missileEnnemi[NbTir].y = ennemi->y[i] - acteur->img->h/2+105;
            NbTir++;

            if(NbTir>3)
                NbTir=0;
        }
    }


}

void afficherEnnemi(BITMAP* bmp,BITMAP* bmp2,t_ennemi *ennemi,int tempo,t_acteur *acteur)
{
    int i,j,k;
    //int NbTir=0;
    int rouge = makecol(255,0,0);
    int bleu = makecol(0,0,255);

    if(ennemi->comportement==0)
    {
        if(tempo>=100)
        {
            for(i=0; i<3; i++)
            {
                draw_sprite_h_flip(bmp,ennemi->img,ennemi->x[i]+300,ennemi->y[i]);

                rectfill(bmp2,ennemi->x[i]+300,ennemi->y[i], ennemi->x[i]+300 + ennemi->tx, ennemi->y[i] + ennemi->ty, rouge);

            }
        }

        if(tempo>=600)
        {
            for(j=3; j<6; j++)
            {
                draw_sprite_h_flip(bmp,ennemi->img,ennemi->x[j]+300,ennemi->y[j]);

                rectfill(bmp2,ennemi->x[j]+300,ennemi->y[j], ennemi->x[j]+300 + ennemi->tx, ennemi->y[j] + ennemi->ty, rouge);
            }
        }

        if(tempo>=1100)
        {
            for(k=6; k<9; k++)
            {
                draw_sprite_h_flip(bmp,ennemi->img,ennemi->x[k]+300,ennemi->y[k]);

                rectfill(bmp2,ennemi->x[k]+300,ennemi->y[k], ennemi->x[k]+300 + ennemi->tx, ennemi->y[k] + ennemi->ty, rouge);
            }
        }

        if(tempo>=1600) /// ENNEMIS QUI TIRENT
        {
            for(k=9; k<12; k++)
            {
                draw_sprite_h_flip(bmp,ennemi->img,700,ennemi->y[k]);

                rectfill(bmp2,700,ennemi->y[k], 700 + ennemi->tx, ennemi->y[k] + ennemi->ty, rouge);
            }

            for(i=0; i<3; i++) // Deplacement de missile
            {
                ennemi->missileEnnemi[i].x = ennemi->missileEnnemi[i].x - acteur->dx;
                draw_sprite(bmp, acteur->img, ennemi->missileEnnemi[i].x +35, ennemi->missileEnnemi[i].y -85);

                rectfill(bmp2, ennemi->missileEnnemi[i].x +35, ennemi->missileEnnemi[i].y -85, ennemi->missileEnnemi[i].x +35 + 16, ennemi->missileEnnemi[i].y -85 + 7, bleu );
            }

        }

    }

    else{


    }


}
