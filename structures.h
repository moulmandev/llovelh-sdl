#include <iostream>
#include "config_sdl.h"

typedef struct{
	char* texte = NULL; //texte possible sur le lien
	int destination = 0; //contient l'id de l'article destination
}Lien;

typedef struct {
	int id = 0; //identifiant de l'article
	char* texte = NULL; //texte de l'article
	Lien* tLiens = NULL; //tableau dynamique de liens vers les autres articles
	int nombreLien = 0;
	bool hasDice = false;
}Article;

typedef struct {
	Article* article = NULL; //l'article lié au noeud du graphe
	int x, y; //position du noeud dans la fenêtre
}Noeud;

typedef struct {
	Noeud* origine = NULL; //pointeur sur le noeud origine du lien
	Noeud* destination = NULL; //pointeur sur le noeud destination du lien
}Arete;

typedef struct {
	char* titre = NULL;
	int id;
	SDL_Rect rect; 
	char* texte = NULL;
	int longueur = 0;
	bool isSelected = false;
}TextField;

typedef struct {
	char* name = NULL;
	TTF_Font* font;
	bool isImage = false;
	const char* imagePath;
	int id;
	SDL_Rect rect;
	bool isHovered = false;
	bool isDisabled = false;
}Button;

typedef struct {
	int NombreDeLien;
}ArticleCree;
