#include<SDL.h>
#include"structures.h"

// TAILLE FENETRE
const int LARGEUR = 1920;
const int HAUTEUR = 1080;

// ESPACEMENT ENTRE LE TEXTE ET LE DEBUT DE LA FENETRE
const int ESPACEMENT = 100;

SDL_Renderer* render;
SDL_Window* win;
TTF_Font* font;
TTF_Font* font_menu;
TTF_Font* font_graph;

//algo
int NombreArticle(char* CheminAcces);
bool initArticle(Article *tableau, int nombreArticle);
void afficherArticle(Article *tableau);
void CPPaHTML(Article* tab);
void afficherNoeud(Noeud* noeud, SDL_Rect* rect);
bool fexists(const char * filename);
int replacestr(char * line, const char * search, const char * replace);
void animation_dice();
void draw_background();
bool fexists(const char * filename);
int replacestr(char * line, const char * search, const char * replace);

//dessin 
void accueil();
void article();
void graph();
void RectangleCreation(SDL_Rect NomDuRectangle, SDL_Color couleur, int PleinOuVide);
void EditeurLDVELH();
TextField getTextFieldByID(int id);
void CreationArticle(char * NomCheminDuFichierLb, int NumeroArticle, char * TitreArticle, int NombreDeLien, int PointeurOne, int PointeurTwo, int PointeurThree, int PointeurFourth, char * TxtPointeurOne, char * TxtPointeurTwo, char * TxtPointeurThree, char * TxtPointeurFourth);
void draw_image(const char * name, int x, int y, int width, int heigth);
void draw_dice(int value, int x, int y, bool showText);
void add_button(const char* name, int id, int x, int y, int w, int h, TTF_Font* font, bool isImage, const char* imagePath, bool isDisabled);
void draw_text(char* str, int x, int y, SDL_Color color, TTF_Font* font);
void draw_article(Article article, int x, int y);
void dessin();
void keystroke(SDL_Event* event);


void fullgraph();

int main(int argc, char * argv[]);