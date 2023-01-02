#include <allegro.h>
#include <stdio.h>
#include <time.h>
#include "structure.h"


t_boss* creerBoss(char *nomimage)
{
    t_boss*boss=NULL;

    boss = (t_boss*)malloc(sizeof(t_boss));  //On alloue le boss
    if(boss == NULL)
    {
        printf("probleme : le boss ne s'alloue pas ");
        exit(0);
    }
    ///Initialisation de la structure boss
    boss-> x = SCREEN_W-200; // position x
    boss-> y = 100; // position y
    boss-> dx = 0; // deplacement x (ne se deplace que verticalement)
    boss-> dy = 2 ; // deplacement y
    boss->tx=150;
    boss->ty=248;
    boss-> vie = 100; //point de vie du boss
    boss->tempsArme = 0; //
    boss->img = load_bitmap_check(nomimage); //image du boss en bmp

    return boss;
}

void actualiserPositionBoss(t_boss *boss, BITMAP *bmp)
{
    int blanc =makecol(255,255,255);

    rectfill(bmp, boss->x, boss->y, boss->x+boss->tx, boss->y+boss->ty, blanc);

    //int distanceY;
    boss->y += boss->dy;

    if(boss->y ==0) ///si le boss est arrivé en haut
    {
        boss->dy=-(boss->dy);
    }
    if(boss->y==360) ///si le boss est arrivé en bas
    {
        boss->dy=-(boss->dy);
    }
}

void bossTir(t_boss* boss,t_joueur* vaisseau,t_listeActeurs* liste)
{
    int fx=1;
    int fy=1;

    boss->tempsArme ++;

    if(boss->tempsArme==0%50) //temps de recharge du tir
    {
        ajouterActeur(liste,boss->x,boss->y+boss->ty/2,4,"tir_boss.bmp");
    }
    /*if(boss->x - vaisseau->x > 0)
    {
        fx=0;
    }
    if(boss->y - vaisseau->y > 0)
    {
        fy=0;
    }*/
}

void afficherBoss(BITMAP*bmp, BITMAP *bmp2, t_boss*boss)
{
    int jaune = makecol(255,255,0);

    draw_sprite(bmp,boss->img,boss->x,boss->y);

    rectfill(bmp2, boss->x, boss->y, boss->x+boss->tx, boss->y+boss->ty, jaune);

    rectfill(bmp,590,30,790,60,0);    ///fond pour la barre de vie
    rectfill(bmp,590,30,790 - boss->vie*2,60,makecol(255,0,0));    ///X2 change en fonction des points de vie du boss
    ///barre de vie en rouge
}



void collisionBossVaisseau(BITMAP *bmp,t_joueur*vaisseau, t_boss*boss)
{
    int jaune = makecol(255,255,0);

    if (       getpixel(bmp, vaisseau->x, vaisseau->y) == jaune
               ||  getpixel(bmp, vaisseau->x+vaisseau->tx, vaisseau->y) == jaune
               ||  getpixel(bmp, vaisseau->x, vaisseau->y+vaisseau->ty) == jaune
               ||  getpixel(bmp, vaisseau->x+vaisseau->tx, vaisseau->y+vaisseau->ty) == jaune )
        {

            vaisseau->comportement=1;

        }

}



void boss()
{
    int fin=0;
    int pause=0;
    int screenx=0, screeny=0;

     // Le vaisseau manipulé par le joueur
    t_joueur *vaisseau=NULL;
    // La collection de acteurs (les tirs normaux)
    t_listeActeurs *tirs=NULL;
    // les tirs speciaux (plus puissant)
    t_listeActeurs *tirspecial=NULL;
    t_listeActeurs *tirboss=NULL;
    t_boss*boss=NULL;

    BITMAP *page=NULL;        // BITMAP buffer d'affichage
    BITMAP *decor=NULL;       // BITMAP image de fond
    BITMAP *decorCollision=NULL;
    BITMAP *mapCollision=NULL;
    BITMAP *collision=NULL;
    BITMAP *gameover=NULL;
    BITMAP *exit3=NULL;
    BITMAP *pause3=NULL;
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
    // double buffering pour gérer les collisions
    mapCollision=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(mapCollision);

    decor=load_bitmap_check("Map_boss.bmp");    // chargement du decor
    decorCollision=load_bitmap_check("test.bmp");
    collision=load_bitmap_check("explosion1.bmp");    // chargement de la carte permettant de gérer les collisions
    gameover=load_bitmap_check("gameover.bmp");
    exit3=load_bitmap_check("exit3.bmp");
    pause3=load_bitmap_check("pause3.bmp");
    jauge=load_bitmap_check("jauge de charge.bmp");

    // créer le vaisseau et le missile
    vaisseau=creerJoueur("vaisseau.bmp");
    // préparer la liste des acteurs (100 max), vide au début
    tirs=creerListeActeurs(100);
    tirspecial=creerListeActeurs(2);
    tirboss=creerListeActeurs(50);

    boss=creerBoss("vaisseau_boss.bmp");

    // Boucle de jeu
    while (!key[KEY_ESC])
    {
        // enregistrer position actuelle vaisseau
        vaisseau->precx=vaisseau->x+2;
        vaisseau->precy=vaisseau->y;

        // EFFACER BUFFER EN APPLIQUANT UNE PARTIE DU DECOR (TAILLE DE L'ECRAN)
        blit(decor,page, screenx, screeny,0,0,SCREEN_W,SCREEN_H);
        blit(decorCollision,mapCollision,0,0,0,0,SCREEN_W,SCREEN_H);

        draw_sprite(page,jauge, 20, 530);  //jauge

        // bouger tout le monde
        actualiserJoueur(vaisseau, tirs, tirspecial, page, mapCollision);
        actualiserListeActeurs(tirs, mapCollision);
        actualiserListeActeurs(tirspecial, mapCollision);
        actualiserListeActeurs(tirboss, mapCollision);

        // afficher tout le monde
        dessinerJoueur(page, mapCollision, collision, vaisseau);
        dessinerListeActeurs(page, mapCollision,tirs);
        dessinerListeActeurs(page, mapCollision,tirspecial);
        dessinerListeActeurs(page, mapCollision,tirboss);

        // gerer le boss
        boss->tempsArme ++;
        actualiserPositionBoss(boss, mapCollision); // actualiser position du boss par rapport au vaisseau
        bossTir(boss,vaisseau,tirboss); // gestion tir boss
        afficherBoss(page, mapCollision, boss); // afficher le boss et barre de vie

        // gerer les collisions
        collisionBossVaisseau(mapCollision, vaisseau, boss);

        rectfill(page, 53,549,53 + vaisseau->tempotir*174/100,558, makecol(254,237,1));   // remplissage jauge de charge


        if((boss->vie) < 0)
        {
            fin=1; ///boleen il est mort
        }
        if(fin==1)
        {
            draw_sprite(page, gameover, SCREEN_W/2 - gameover->w/2, SCREEN_H/2 - gameover->h/2 );
             draw_sprite(page, exit3, SCREEN_W/2 - exit3->w/2, SCREEN_H/2 - exit3->h/2 +190);
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
                draw_sprite(screen, pause3, SCREEN_W/2 - pause3->w/2, SCREEN_H/2 - pause3->h/2 -40);
                draw_sprite(screen, exit3, SCREEN_W/2 - exit3->w/2, SCREEN_H/2 - exit3->h/2 +40);

                vaisseau->dx=0;
                vaisseau->dy=0;

                if ( mouse_b&1 && mouse_x>=332 && mouse_y>=321 && mouse_x<=468 && mouse_y<=359)
                    menu();

                if(key[KEY_P] || key[KEY_ESC] || (mouse_b&1 && mouse_x>=332 && mouse_y>=241 && mouse_x<=468 && mouse_y<=279 ))
                {
                    rest(200);
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
