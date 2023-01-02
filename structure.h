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
    //   2 tir sp�cial
    //   3 tir ennemi(rouge)
    //   4 tir boss
    int type;

    int comportement;   //   0 normal d�placement ou 1 explosion

    //   booleen 0 mort ou 1 vivant
    int vivant;

    BITMAP *img; // sprite (image charg�e)

} t_acteur;

// Une collection de acteurs
typedef struct listeActeurs
{
    // nombre maxi d'�l�ments
    // =taille du tableau de pointeurs
    int max;

    // nombre effectif de pointeurs utilis�s
    // (les autres sont � NULL)
    int n;

    // le tableau de pointeurs (allou� dynamiquement)
    t_acteur **tab;

} t_listeActeurs;


typedef struct joueur
{
    int x,y;     // position
    int tx,ty;   // taille
    int dx,dy;     // vitesse des d�placements (nombre de pixels)
    int precx,precy;  //position pr�c�dente
    int tempotir; // tempo armement
    int comportement;   //   0 normal d�placement ou 1 explosion
    int tempexplo; // temps depuis l'explosion
    int vivant;   //   booleen 0 mort ou 1 vivant
    BITMAP *img; // sprite (image charg�e)
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
    int dx;//d�placement x
    int dy;//d�placement y
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

