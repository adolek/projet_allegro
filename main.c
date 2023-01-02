#include <allegro.h>
#include <stdio.h>
#include <time.h>
#include "structure.h"

/// SOUS PROG MENU

void menu()
{
    int mode=0;

    set_color_depth(desktop_color_depth());

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);

    }

    install_mouse();
    show_mouse(screen);

    BITMAP *avion;
    BITMAP *page;
    BITMAP *play;
    BITMAP *end;
    BITMAP *lvlselect;

    page=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);

    avion=load_bitmap_check("fond menu.bmp");
    play=load_bitmap_check("play.bmp");
    end=load_bitmap_check("exit.bmp");
    lvlselect=load_bitmap_check("level selection.bmp");


    while (mode==0)
    {

        blit(avion,page,0,0,0,0,800,600);
        blit(play,page,0,0,320,230,136,38);
        blit(end,page,0,0,664,0,136,38);
        textprintf_ex(page, font, 150,540, makecol(255,255,255), -1, "Press 'B' to play against Boss");
        masked_blit(lvlselect, page, 0,0,SCREEN_W - lvlselect->w*1.25,SCREEN_H - lvlselect->h*1.25,246,195);

        if ( mouse_b&1 && mouse_x>=664 && mouse_y>=0 && mouse_x<=800 && mouse_y<=36)
            exit(0);

        if(mouse_b&1 && mouse_x>=320 && mouse_y>=250 && mouse_x<=455 && mouse_y<=288 )
            mode=1; // lancement du premier niveau

        if(mouse_b&1 && mouse_x>=528 && mouse_y>=450 && mouse_x<=571 && mouse_y<=503 )
            mode=1; // lancement du niveau 1

        if(mouse_b&1 && mouse_x>=593 && mouse_y>=450 && mouse_x<=637 && mouse_y<=503 )
            mode=2; // lancement du niveau 2

        if(mouse_b&1 && mouse_x>=662 && mouse_y>=450 && mouse_x<=706 && mouse_y<=503 )
            mode=3; // lancement du niveau 3

        if(key[KEY_ENTER])
            mode=1;

        if(key[KEY_B])
            mode=4;

        if(key[KEY_ESC])
            exit(0);

        blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);

    }


    switch(mode)
    {

    case 1:
        niveau1();
        break;

    case 2:
        niveau2();
        break;

    case 3:
        niveau3();
        break;

    case 4:
        boss();
        break;

    }

}


/// PROGRAMME PRINCIPAL

int main()
{
    // Lancement mode graphique
    allegro_init();
    install_keyboard();
    install_mouse();
    show_mouse(screen);

    set_color_depth(desktop_color_depth());

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);

    }

    while(!0)
    {

        menu();

    }

    return 0;

}
END_OF_MAIN();
