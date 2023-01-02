#include <allegro.h>
#include <stdio.h>
#include <time.h>
#include "structure.h"


void niveau2()
{
    int screenx=0,screeny=0; // Position de l'écran réel dans le repère du décor...
    int vitesseEcran=2;
    int fin=0;
    int pause=0;
    int couleur;
    int NbTir=0;
    int TempoMechant=0;
    int NiveauSuivant =0;

    // Le vaisseau manipulé par le joueur
    t_joueur *vaisseau=NULL;
    // La collection de acteurs (les tirs normaux)
    t_listeActeurs *tirs=NULL;
    // les tirs speciaux (plus puissant)
    t_listeActeurs *tirspecial=NULL;

    t_acteur *missile=NULL;
    t_ennemi *ennemi=NULL;

    BITMAP *page=NULL;        // BITMAP buffer d'affichage
    BITMAP *decor=NULL;       // BITMAP image de fond
    BITMAP *mapCollision=NULL;
        BITMAP *decorCollision=NULL;

    BITMAP *collision=NULL;
    BITMAP *gameover=NULL;
    BITMAP *exit2=NULL;
    BITMAP *pause2=NULL;
    BITMAP *jauge=NULL;

    srand(time(NULL));


    set_color_depth(desktop_color_depth());

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);

    }

    install_mouse();
    show_mouse(screen);

    // CREATION DU BUFFER D'AFFICHAGE à la taille de l'écran
    page=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);
    // double buffer
    mapCollision=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(mapCollision);

    decor=load_bitmap_check("Map_niveau2.bmp");    // chargement du decor
        decorCollision=load_bitmap_check("test.bmp");

    collision=load_bitmap_check("explosion1.bmp");    // chargement de la carte permettant de gérer les collisions
    gameover=load_bitmap_check("gameover.bmp");
    exit2=load_bitmap_check("exit2.bmp");
    pause2=load_bitmap_check("pause2.bmp");
    jauge=load_bitmap_check("jauge de charge.bmp");

    // créer le vaisseau et le missile
    vaisseau=creerJoueur("vaisseau.bmp");
    // préparer la liste des acteurs (100 max), vide au début
    tirs=creerListeActeurs(100);
    tirspecial=creerListeActeurs(2);

    ennemi = creerEnnemi("ennemi_avion1.bmp");
    missile=creerActeur(0,0,3,"tir_ennemi.bmp");

    couleur=makecol(72,92,38);

    determinerPosEnnemi(ennemi);

    // Boucle de jeu
    while (!key[KEY_ESC])
    {
        // enregistrer position actuelle vaisseau
        vaisseau->precx=vaisseau->x;
        vaisseau->precy=vaisseau->y;

        NiveauSuivant++;   // compteur de passage au niveau suivant
        if(NiveauSuivant<=50)
            {
                textprintf_ex(decor, font, 250,50, makecol(255,255,255), -1, "WARNING ! AVOID THE GRASS OR YOUR ENEMIES WILL TORPILL YOU");

        }
        if(NiveauSuivant>=3000)
        {
            textprintf_ex(screen, font, 275,300, makecol(255,255,255), -1, "CONGRATULATIONS ! LEVEL 2 SUCCEEDED");
            rest(4000);
            niveau3();
        }

        // Ici initialisation du scrolling à gauche du décor
        screenx+=vitesseEcran;

        if ( screenx < 0 )
            screenx=0;
        if ( screenx > decor->w - SCREEN_W )
        {
            blit(decor,page,0,0,0,0,SCREEN_W,SCREEN_H);
            screenx=0;

        }

        // EFFACER BUFFER EN APPLIQUANT UNE PARTIE DU DECOR (TAILLE DE L'ECRAN)
        blit(decor,page,screenx,screeny,0,0,SCREEN_W,SCREEN_H);
                blit(decorCollision,mapCollision,screenx,screeny,0,0,SCREEN_W,SCREEN_H);

        draw_sprite(page, jauge, 20, 530 );

        // bouger tout le monde
        actualiserJoueur(vaisseau, tirs, tirspecial, decor, mapCollision);
        actualiserListeActeurs(tirs, mapCollision);
        actualiserListeActeurs(tirspecial, mapCollision);

        // afficher tout le monde
        dessinerJoueur(page, mapCollision, collision, vaisseau);
        dessinerListeActeurs(page, mapCollision,tirs);
        dessinerListeActeurs(page, mapCollision,tirspecial);

        // Apparition des ennemis (4 Vagues de 3 ennemis pour l'instant)
        TempoMechant++;
        actualiserEnnemi(mapCollision,ennemi,TempoMechant,missile);
        afficherEnnemi(page,mapCollision,ennemi,TempoMechant,missile);

        // gerer les collisions
        collisionVaisseauEnnemi(mapCollision, vaisseau, ennemi);

        rectfill(page, 53,549,53 + vaisseau->tempotir*174/100,558, makecol(254,237,1));

        // collision décor
        if(collisionDecor(vaisseau, page, couleur)==1)
        {
            fin=1;

            vaisseau->x=vaisseau->precx-vitesseEcran;
            vaisseau->dx=0;
            vaisseau->dy=0;
            vaisseau->comportement=1;

        }

        if(fin==1)
        {
            draw_sprite(page, gameover, SCREEN_W/2 - gameover->w/2, SCREEN_H/2 - gameover->h/2 );
             draw_sprite(page, exit2, SCREEN_W/2 - exit2->w/2, SCREEN_H/2 - exit2->h/2 +190);
            if ( mouse_b&1 && mouse_x>=332 && mouse_y>=471 && mouse_x<=468 && mouse_y<=509)
                menu();

        }

        // Menu pause

        if(key[KEY_P])
        {
            pause = 0;
            rest(200);

            while(!pause)
            {
                draw_sprite(screen, pause2, SCREEN_W/2 - pause2->w/2, SCREEN_H/2 - pause2->h/2 -40);
                draw_sprite(screen, exit2, SCREEN_W/2 - exit2->w/2, SCREEN_H/2 - exit2->h/2 +40);

                vitesseEcran=0;
                vaisseau->dx=0;
                vaisseau->dy=0;

                if ( mouse_b&1 && mouse_x>=332 && mouse_y>=321 && mouse_x<=468 && mouse_y<=359)
                    menu();

                if(key[KEY_P] || key[KEY_ESC] || (mouse_b&1 && mouse_x>=332 && mouse_y>=241 && mouse_x<=468 && mouse_y<=279 ))
                {
                    rest(200);
                    vitesseEcran=2;
                    vaisseau->dx=3;
                    vaisseau->dy=3;
                    pause=1;
                }

            }
        }
        // AFFICHAGE DU BUFFER MIS A JOUR A L'ECRAN
        blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);

        // ON FAIT UNE PETITE PAUSE
        rest(10);

    }


}



