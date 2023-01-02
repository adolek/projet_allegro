#include <allegro.h>


typedef struct acteur
{
    // position du coin sup. gauche
    int x,y;

    // vecteur deplacement
    int dx,dy;

    // largeur hauteur
    int tx,ty;

    //   1 missile
    //   2 tir spécial
    //   3 tir ennemi(rouge)
    //   4 tir boss
    int type;

    int comportement;   //   0 normal déplacement ou 1 explosion

    //   booleen 0 mort ou 1 vivant
    int vivant;

    BITMAP *img; // sprite (image chargée)

} t_acteur;

// Une collection de acteurs
typedef struct listeActeurs
{
    // nombre maxi d'éléments
    // =taille du tableau de pointeurs
    int max;

    // nombre effectif de pointeurs utilisés
    // (les autres sont à NULL)
    int n;

    // le tableau de pointeurs (alloué dynamiquement)
    t_acteur **tab;

} t_listeActeurs;


typedef struct joueur
{
    int x,y;     // position
    int tx,ty;   // taille
    int dx,dy;     // vitesse des déplacements (nombre de pixels)
    int precx,precy;  //position précédente
    int tempotir; // tempo armement
    int comportement;   //   0 normal déplacement ou 1 explosion
    int tempexplo; // temps depuis l'explosion
    int vivant;   //   booleen 0 mort ou 1 vivant
    BITMAP *img; // sprite (image chargée)
} t_joueur;


typedef struct ennemi
{
    int x[12],y[12];
    int dx,dy;
    int tx, ty;
    int vie;
    int comportement;
    BITMAP* img;
    t_acteur missileEnnemi [50];

} t_ennemi;


typedef struct boss
{
    int x; //position x
    int y; // position y
    int dx;//déplacement x
    int dy;//déplacement y
    int tx, ty;
    int tempsArme; //temps de recharge arme
    int vie; //point de vie du boss
    BITMAP *img; //bitmap du boss

}t_boss;



/// PROTOTYPES DES FONCTIONS

void menu();
void niveau1();
void niveau2();
void niveau3();
void boss();

BITMAP * load_bitmap_check(char *nomImage);
int collisionDecor(t_joueur *vaisseau, BITMAP *decor, int couleur);
void collisionVaisseauEnnemi(BITMAP *bmp, t_joueur*vaisseau, t_ennemi*ennemi);
void collisionMissileVaisseau(BITMAP *bmp,t_joueur*vaisseau, t_acteur *acteur);

t_acteur * creerActeur(int x,int y,int type,char *nomimage);
void actualiserActeur(t_acteur *acteur, BITMAP*bmp);
void dessinerActeur(BITMAP *bmp,BITMAP *bmp2,t_acteur *acteur);

t_listeActeurs * creerListeActeurs(int maxacteurs);
t_acteur * ajouterActeur(t_listeActeurs *liste,int x,int y,int type,BITMAP*bmp);
void enleverActeur(t_listeActeurs *liste,int i);
void actualiserListeActeurs(t_listeActeurs *liste, BITMAP*bmp);
void dessinerListeActeurs(BITMAP *bmp,BITMAP *bmp2,t_listeActeurs *liste);

void collisionActeur(t_ennemi *ennemi,t_acteur *acteur);
void collisionListeActeurs(t_ennemi *ennemi,t_listeActeurs *liste);

t_joueur * creerJoueur(char *nomimage);
void actualiserJoueur(t_joueur *joueur,  t_listeActeurs*liste1, t_listeActeurs*liste2, BITMAP*bmp,  BITMAP *bmp2);
void dessinerJoueur(BITMAP *bmp,BITMAP *bmp2, BITMAP *bmp3, t_joueur *joueur);

t_ennemi * creerEnnemi(char *nomimage);
void determinerPosEnnemi(t_ennemi* ennemi);
void actualiserEnnemi(BITMAP *bmp,t_ennemi* ennemi,int tempo,t_acteur *acteur);
void afficherEnnemi(BITMAP* bmp,BITMAP* bmp2,t_ennemi *ennemi,int tempo,t_acteur *acteur);

t_boss* creerBoss(char *nomimage);
void actualiserPositionBoss(t_boss *boss, BITMAP *bmp);
void bossTir(t_boss* boss,t_joueur* vaisseau,t_listeActeurs* liste);
void afficherBoss(BITMAP*bmp, BITMAP *bmp2, t_boss*boss);
void collisionBossVaisseau(BITMAP *bmp,t_joueur*vaisseau, t_boss*boss);

