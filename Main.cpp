#include "config_sdl.h"
#include "Main.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

enum class GUIS { MENU, GAME, GRAPH, EDITEUR};

Article* articles;
ArticleCree ArticlesCree[2000];
int NombreArticleCreer = 0;
int NombreLienTotal = 0;

Noeud* graphi;
int destinations[10][1]; // Destinations affichées à l'écran, le deuxième int correspond à si il est hovered (surligné bleu)
int nombreLiens = 0;
int currentArticle = 0;
GUIS currentGUI = GUIS::MENU;
bool continuer = true;
int fontSize = 10;

// dice drawn
bool diceDrawned = false;
int diceValue = -1;

//editeur
char fichierArticle[100];

std::vector<Button> btnList;
std::vector<TextField> textFields;
int textFieldSelected = -1;
const int LEN_MAX = 16000;

char filePath[] = "testExport.lb";
char saveFile[] = "save.txt";
char* EnvoieAvecPointLB;

int offset = 0;
int valueOffset = 62;

ifstream inStream;
ofstream outStream;
int xMouse, yMouse;

int NombreArticle(char* CheminAcces) {
	ifstream Flux(CheminAcces, ios::in);
	char StockageTmp[100];
	int NBArt = 0;

	if (Flux) {
		Flux.getline(StockageTmp, 100, '\n');
	}
	else {
		cout << "Probleme d'ouverture sur NombreArticle"<< endl;
	}
	NBArt = atoi(StockageTmp);
	return NBArt;
}

bool initArticle(Article* tableau, int nombreArticle, char* filePathInFonction)
{
	ifstream lecture(filePathInFonction, ios::in);
	char* trash;
	trash = new char[200000];
	if (lecture)
	{
		lecture.getline(trash, 200000, '\n');
		char* id, * nBlien;
		int i = 0;
		for (int i(0); i < nombreArticle; i++)
		{
			int nombreLien = 0;
			lecture.getline(trash, 200000, ',');
			id = &trash[0];
			tableau[i].id = atoi(id);
			lecture.getline(trash, 200000);
			tableau[i].texte = new char[200000];
			strcpy_s(tableau[i].texte, 200000, trash);
			lecture.getline(trash, 200000);
			nBlien = &trash[0];
			nombreLien = atoi(nBlien);
			tableau[i].tLiens = new Lien[nombreLien];
			tableau[i].nombreLien = nombreLien;
			for (int j(0); j < nombreLien; j++)
			{
				lecture.getline(trash, 200000, ',');
				id = &trash[0];
				tableau[i].tLiens[j].destination = atoi(id);
				lecture.getline(trash, 200000);
				tableau[i].tLiens[j].texte = new char[200000];
				strcpy_s(tableau[i].tLiens[j].texte, 200000, trash);
			}

			bool hasDice = (replacestr(tableau[i].texte, "Pick a number from the <a href=\"#random\">Random Number Table</a>.", "Roll a dice") || replacestr(tableau[i].texte, "Pick a number from the <a href=\"#random\">Random Number Table</a>", "Roll a dice"));
			tableau[i].hasDice = hasDice;
		}
	}
	else
		return false;
	return true;
}

void afficherArticle(Article* tableau)
{
	for (int i(0); i < 350; i++)
	{
		cout << tableau[i].id << endl;
		cout << tableau[i].nombreLien << endl;
	}
}

void sauvegarder(Article articleCourant)
{
	outStream.open(saveFile, ios::app);
	if (outStream)
	{
		outStream << articleCourant.id << endl;
		outStream.close();
	}
	else
		cout << "Sauvegarde impossible !" << endl;
}

int detruireSauvegarde(char* filePath)
{
	outStream.close();
	inStream.close();
	return remove(filePath);
}

int donnesSauvegarde(char* filePath)
{
	inStream.open(saveFile, ios::in);
	char trash[1000];
	int numArticle = 0;
	while (numArticle == 0) {
		while (inStream.getline(trash, 1000))
		{
			numArticle = atoi(trash);
		}
	}
	inStream.close();
	return numArticle;
}

char* AjoutDuPointLb(char* NomCheminDuFichierLb){
	char PointLb[] = ".lb";
	int TCheminFichier = strlen(NomCheminDuFichierLb);
	EnvoieAvecPointLB = new char[TCheminFichier + 3];

	strcpy(EnvoieAvecPointLB, NomCheminDuFichierLb);
	strcat(EnvoieAvecPointLB, PointLb);
	return EnvoieAvecPointLB;
}

void NombreArticleTotal(char* NomCheminDuFichierLb, int NombreArticleTotal) {
	ofstream monFlux(AjoutDuPointLb(NomCheminDuFichierLb), ios_base::app);
	if (monFlux) {
		monFlux << NombreArticleTotal << endl;
	}
	else
		cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
}

void CreationArticle(char* NomCheminDuFichierLb, int NumeroArticle, char* TitreArticle, int NombreDeLien, int PointeurOne, int PointeurTwo, int PointeurThree, int
	PointeurFourth, char* TxtPointeurOne, char* TxtPointeurTwo, char* TxtPointeurThree, char* TxtPointeurFourth) {

	NombreArticleCreer++;
	//if (NumeroArticle == NombreArticleCreer) {

		ofstream monFlux(AjoutDuPointLb(NomCheminDuFichierLb), ios_base::app);
		if (monFlux)
		{
			//monFlux << NombreArticleCreer << endl;
			monFlux << NumeroArticle << "," << TitreArticle << endl;
			monFlux << NombreDeLien << endl;
			if (NombreDeLien == 1) {
				monFlux << PointeurOne << "," << TxtPointeurOne << endl;
			}
			if (NombreDeLien == 2) {
				monFlux << PointeurOne << "," << TxtPointeurOne << endl;
				monFlux << PointeurTwo << "," << TxtPointeurTwo << endl;
			}
			if (NombreDeLien == 3) {
				monFlux << PointeurOne << "," << TxtPointeurOne << endl;
				monFlux << PointeurTwo << "," << TxtPointeurTwo << endl;
				monFlux << PointeurThree << "," << TxtPointeurThree << endl;
			}
			else if (NombreDeLien >= 3) {
				monFlux << PointeurOne << "," << TxtPointeurOne << endl;
				monFlux << PointeurTwo << "," << TxtPointeurTwo << endl;
				monFlux << PointeurThree << "," << TxtPointeurThree << endl;
				monFlux << PointeurFourth << "," << TxtPointeurFourth << endl;
			}
		}
		else
			cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
	/*}
	else {
		cout << "Vous n'avez pas creer l'article numéro " << NombreArticleCreer << " merci de ne pas esseyer de creer un autre article comme le : " << NumeroArticle << endl;
		NombreArticleCreer--;
	}*/
		
}

bool VerificationIntegrite(char* NomDuFichierPointLb) {
	int NombreArticles = NombreArticle(NomDuFichierPointLb);
	int i = 0;
	int compteur = 0;
	bool test;
	Article* UnTableau;
	UnTableau = new Article[NombreArticles];
	cout << "Nb article " << NombreArticles;
	initArticle(UnTableau, NombreArticles, NomDuFichierPointLb);

	for(i =0; i< NombreArticles;i++){
		if (UnTableau[i].id != 0 ) {

			for (int k = 0; k < NombreArticles;k++) {
				for (int j = 0; j < UnTableau[k].nombreLien; j++) {
					if (UnTableau[k].tLiens[j].destination == UnTableau[i].id) {
						compteur++;
						cout << "Lien " << UnTableau[k].tLiens[j].destination << "  id" << UnTableau[i].id << endl;
					}
				}
			}
			
		}
	}
	if (compteur == NombreArticles)
		return true;
	else {
		return false;
		cout << "Noeud isolé " << endl;
	}
}

bool initialisationSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec à l'ouverture";
		return false;
	}

	win = SDL_CreateWindow("Un livre dont vous êtes le héros", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR, HAUTEUR, SDL_WINDOW_FULLSCREEN /* SDL_WINDOW_MAXIMIZED*/);
	if (win == NULL) {
		cout << "erreur ouverture fenetre";
		return false;
	}
	render = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);

	TTF_Init();
	font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 20);
	font_menu = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 45);
	font_graph = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", fontSize);

	return true;
}

void destructionSDL() {
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void add_textField(const char* name, int id, int x, int y, int w, int h, const char* defaultText) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	TextField nbrArticle;
	nbrArticle.id = id;
	nbrArticle.rect = rect;
	nbrArticle.titre = new char[LEN_MAX];
	nbrArticle.texte = new char[LEN_MAX];
	strcpy_s(nbrArticle.titre, LEN_MAX, name);
	strcpy_s(nbrArticle.texte, LEN_MAX, defaultText);
	
	textFields.push_back(nbrArticle);
}

void draw_image(const char* name, int x, int y, int width, int heigth) {
	SDL_Rect positionImage;
	positionImage.w = width;
	positionImage.h = heigth;
	positionImage.x = x;
	positionImage.y = y;

	SDL_Texture* texture = loadImage(render, name, 255, -1, -1, -1);
	SDL_QueryTexture(texture, NULL, NULL, &positionImage.w, &positionImage.h);
	SDL_RenderCopy(render, texture, NULL, &positionImage);
}

void add_button(const char* name, int id, int x, int y, int w, int h, TTF_Font* font, bool isImage, const char* imagePath, bool isDisabled) {
	SDL_Rect rect;
	rect.w = w;
	rect.h = h;
	rect.x = x;
	rect.y = y;
	Button btn;
	btn.id = id;
	btn.font = font;
	btn.rect = rect;
	if (!isImage) {
		btn.name = new char[LEN_MAX];
		strcpy_s(btn.name, LEN_MAX, name);
	}
	btn.isImage = isImage;
	btn.imagePath = imagePath;
	btn.isDisabled = isDisabled;

	btnList.push_back(btn);
}

void draw_buttons() {
	for (std::vector<Button>::size_type i = 0; i < btnList.size(); i++) {
		if (btnList[i].isImage) {
			draw_image(btnList[i].imagePath, btnList[i].rect.x, btnList[i].rect.y, btnList[i].rect.w, btnList[i].rect.h);
		}
		else {

			SDL_SetRenderDrawColor(render, 150, 150, 150, 255);
			SDL_RenderFillRect(render, &btnList[i].rect);
			draw_text(btnList[i].name, btnList[i].rect.x + 20, btnList[i].rect.y + btnList[i].rect.h / 2 - 10, { 255,255,255 }, btnList[i].font);

			if (btnList[i].isDisabled) {
				SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
				SDL_RenderDrawLine(render, btnList[i].rect.x, btnList[i].rect.y, btnList[i].rect.x + btnList[i].rect.w, btnList[i].rect.y + btnList[i].rect.h);
				SDL_RenderDrawLine(render, btnList[i].rect.x, btnList[i].rect.y + btnList[i].rect.h, btnList[i].rect.x + btnList[i].rect.w, btnList[i].rect.y);
			}
		}
	}
}

void draw_text(char* str, int x, int y, SDL_Color color, TTF_Font* font) {
	SDL_Rect positionTexte;
	positionTexte.x = x;
	positionTexte.y = y;

	SDL_Texture* texture = loadText(render, str, color, font);
	SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
	SDL_RenderCopy(render, texture, NULL, &positionTexte);
	SDL_DestroyTexture(texture);
}

void draw_text_limited(char* str, int x, int y, SDL_Color color, TTF_Font* font, int lenght) {
	SDL_Rect positionTexte;
	positionTexte.x = x;
	positionTexte.y = y;

	SDL_Texture* texture = loadText(render, str, color, font, lenght);
	SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
	SDL_RenderCopy(render, texture, NULL, &positionTexte);
	SDL_DestroyTexture(texture);
}

void draw_article(Article article, int x, int y) {
	if (article.hasDice || diceDrawned) {
		add_button("", 8, LARGEUR - 64 - 5, HAUTEUR - 64 - 5, 64, 64, NULL, true, "images/dices.png", diceDrawned);
	}
	if (diceDrawned && diceValue != -1) {
		draw_dice(diceValue, LARGEUR - 200, HAUTEUR - 100, true);
	}
	draw_text_limited(article.texte, x, y, { 0, 0, 0 }, font, LARGEUR - 200);
	nombreLiens = article.nombreLien;

	if (nombreLiens == 0) {
		currentArticle = 0;
		char tmp[50];
		strcpy_s(tmp, 50, "Your path stops now.");
		draw_text(tmp, ESPACEMENT + 50, (HAUTEUR - 150), { 0, 0, 0}, font);
		//TODO: Afficher un message vous avez perdu/gagné
		return;
	}

	for (int i = 0; i < nombreLiens; i++) {
		int x = (ESPACEMENT);
		int y = (HAUTEUR - 150) + (30 * i);
		SDL_Color color = { 0, 0, 0 };
		if (xMouse > x - 60 && xMouse < x + LARGEUR && yMouse > y && yMouse < y + 16) {
			color = { 25, 25, 255 };
		}

		char buf2[] = { "Go to" };
		char buf[16];
		sprintf_s(buf, "%d <--", article.tLiens[i].destination);

		draw_text(buf, x - 60, y, color, font);
		draw_text((strlen(article.tLiens[i].texte) == 0 ? buf2 : article.tLiens[i].texte), x, y, color, font);
	}
}

bool fexists(const char* filename) {
	std::ifstream ifile(filename);
	return (bool)ifile;
}

int replacestr(char* line, const char* search, const char* replace)
{
	int count;
	char* sp;
	if ((sp = strstr(line, search)) == NULL) {
		return(0);
	}
	count = 1;
	int sLen = strlen(search);
	int rLen = strlen(replace);
	if (sLen > rLen) {
		char* src = sp + sLen;
		char* dst = sp + rLen;
		while ((*dst = *src) != '\0') { dst++; src++; }
	}
	else if (sLen < rLen) {
		int tLen = strlen(sp) - sLen;
		char* stop = sp + rLen;
		char* src = sp + sLen + tLen;
		char* dst = sp + rLen + tLen;
		while (dst >= stop) { *dst = *src; dst--; src--; }
	}
	memcpy(sp, replace, rLen);
	count += replacestr(sp + rLen, search, replace);
	return(count);
}

void draw_dice(int value, int x, int y, bool showText) {
	char de_filename[10][999] = { "images/dice-zero.png", "images/dice-one.png", "images/dice-two.png", "images/dice-three.png", "images/dice-four.png", "images/dice-five.png", "images/dice-six.png", "images/dice-seven.png", "images/dice-eight.png", "images/dice-nine.png" };
	draw_image(de_filename[value], x, y, 64, 64);
	if (showText) {
		char buf[50];
		sprintf_s(buf, "Vous avez obtenu %d", value );
		draw_text(buf, LARGEUR - 250, HAUTEUR - 25, { 0, 0, 0 }, font);
	}
}

void animation_dice() {
	diceDrawned = true;
	
	int t[30];
	for (int i = 0; i < 30; i++) {
		t[i] = rand() % 10;
	}
	std::random_shuffle(&t[0], &t[30]);

	int tValue;
	for (int i = 0; i < 10; i++) {
		dessin();
		tValue = t[i];
		draw_dice(tValue, LARGEUR - 200, HAUTEUR - 100, false);
		SDL_RenderPresent(render);
		SDL_Delay(1 + (15 * i));
	}

	
	diceValue = tValue;
	draw_dice(tValue, LARGEUR - 200, HAUTEUR - 100, true);
	SDL_RenderPresent(render);
}

void draw_background() {
	SDL_SetRenderDrawColor(render, 255, 246, 226, 255); // Beige fond
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = LARGEUR;
	rect.h = HAUTEUR;
	SDL_RenderFillRect(render, &rect);
}

void accueil() {
	draw_image("images/image.jpg", 0, 0, LARGEUR, HAUTEUR);
	
	btnList.clear();

	add_button("       NOUVELLE PARTIE", 1, (LARGEUR / 2) - 500 / 2, HAUTEUR / 2 - 300, 500, 200, font_menu, false, "", false);
	add_button("     REPRENDRE PARTIE", 2, (LARGEUR / 2) - 500 / 2, HAUTEUR / 2, 500, 200, font_menu, false, "", (!fexists("save.txt")));
	add_button("", 3, LARGEUR - 64 - 5, 5, 64, 64, NULL, true, "images/settings.png", false);
	
	draw_buttons();
	/*
	Article t;
	t.id;
	CreationRectangleGraph(t, 200, 200);*/
}

void CreationRectangleGraph(Article NbDeLienEtId, int x, int y) {
	char buf[16];
	sprintf_s(buf, "%d", NbDeLienEtId.id);
	SDL_Color Black;
	Black.r = 0;
	Black.g = 0;
	Black.b = 0;
	Black.a = 0;
	SDL_Rect NomDuRectangle;
	NomDuRectangle.x = x;
	NomDuRectangle.x = y;
	NomDuRectangle.w = fontSize / 2;
	NomDuRectangle.h = fontSize * 2;

	draw_text(buf, x + 10, y / 2, Black, font);
	RectangleCreation(NomDuRectangle, Black, 0);

	SDL_RenderDrawRect(render, &NomDuRectangle);
}

void article() {
	btnList.clear();
	draw_background();
	
	draw_article(articles[currentArticle], ESPACEMENT, ESPACEMENT - 50);
	add_button("", 4, 5, 0, 64, 64, NULL, true, "images/arrow.png", false);
	add_button("", 5, LARGEUR - 64 - 5, 0, 64, 64, NULL, true, "images/graph.png", false);

	draw_buttons();
}

void graph()
{
	btnList.clear();
	draw_background();

	add_button("", 9, 5, 0, 64, 64, NULL, true, "images/arrow.png", false);

	draw_buttons();
	 
	graphi = new Noeud[350];

	inStream.open(saveFile, ios::app);
	int indice = 0, i = 0, compteur = 0;
	char articleParcouru[50];
	while (inStream.getline(articleParcouru, 50))
	{
		indice = atoi(articleParcouru);
		graphi[i].article = &articles[indice];
		i++;
		compteur = i;
	}
	int width = fontSize / 2;
	int height = fontSize * 2;
	int x = 20, y = HAUTEUR / 2, w = 0;
	SDL_Rect* noeud;
	noeud = new SDL_Rect;
	Arete* arete;
	arete = new Arete;
	arete->origine = new Noeud;
	arete->destination = new Noeud;
	char buf[4]; i = 0;
	for (i = 0; i < compteur; i++)
	{
		sprintf_s(buf, "%d", graphi[i].article->id - 1);
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		noeud->w = width * 2 * strlen(buf);
		noeud->h = height;
		noeud->x = x + w + 5;
		x = noeud->x;
		w = noeud->w;
		if (noeud->x + w >= LARGEUR - 20)
		{
			x = 20;
			y = HAUTEUR / 2 + 60;
		}
		noeud->x = x;
		noeud->y = y;
		arete->origine->x = x + w;
		arete->destination->x = arete->origine->x + 5;
		arete->origine->y = y + height / 2;
		arete->destination->y = arete->origine->y;
		SDL_RenderDrawRect(render, noeud);
		draw_text(buf, noeud->x + noeud->w / 4, noeud->y + noeud->h / 4, { 255,0,0 }, font_graph);
		if (graphi[i + 1].article && arete->destination->x > arete->origine->x)
		{
			SDL_RenderDrawLine(render, arete->origine->x, arete->origine->y, arete->destination->x, arete->destination->y);
		}
	}
	inStream.close();
}

void RectangleCreation(SDL_Rect NomDuRectangle, SDL_Color couleur, int PleinOuVide /*0 Pour vide 1 pour plein*/) {
	SDL_SetRenderDrawColor(render, couleur.r, couleur.g, couleur.b, couleur.a);
	if (PleinOuVide == 0)
		SDL_RenderDrawRect(render, &NomDuRectangle);
	else {
		SDL_RenderFillRect(render, &NomDuRectangle);
	}

}

void draw_textFields()
{
	for (std::vector<TextField>::size_type i = 0; i < textFields.size(); i++) {
			draw_text(textFields[i].titre, textFields[i].rect.x + 3, textFields[i].rect.y - 20, { 0, 0, 0 }, font);
			draw_image("images/delete.png", textFields[i].rect.x + textFields[i].rect.w + 5, textFields[i].rect.y, 32, 32);
			RectangleCreation(textFields[i].rect, { 0, 0, 0 }, 0);
			draw_text_limited(textFields[i].texte, textFields[i].rect.x + 1, textFields[i].rect.y + 6, { 0, 0, 0 }, font, textFields[i].rect.w);
	}
}

void EditeurLDVELH() {
	draw_background();

	btnList.clear();

	add_button("", 4, 5, 0, 64, 64, NULL, true, "images/arrow.png", false);
	add_button("Ajouter article", 6, (LARGEUR - (160 * 2) - 20) - 160 / 2, HAUTEUR - 25 - 10, 160, 25, font, false, "", false);
	add_button("Finir livre", 7, (LARGEUR - (160) - 10) - 160 / 2, HAUTEUR - 25 - 10, 160, 25, font, false, "", false);

	draw_buttons();
	draw_textFields();	
}

TextField getTextFieldByID(int id) {
	TextField null;
	for (std::vector<TextField>::size_type i = 0; i < textFields.size(); i++) {
		if (textFields[i].id == id) {
			return textFields[i];
		}
	}
	return null;
}

void dessin() {
	SDL_RenderClear(render);
	if (currentGUI == GUIS::MENU) 
		accueil();
	else if (currentGUI == GUIS::GAME) 
		article();
	else if (currentGUI == GUIS::GRAPH)
		graph();
	else if (currentGUI == GUIS::EDITEUR) 
		EditeurLDVELH();

	SDL_RenderPresent(render);
}

void keystroke(SDL_Event* event) {
	switch (event->type)
	{
	case SDL_QUIT:
		continuer = false;
		break;
	case SDL_TEXTINPUT:
		for (std::vector<TextField>::size_type i = 0; i < textFields.size(); i++) {
			if (textFields[i].id == textFieldSelected) {
				size_t l = strlen(event->text.text);
				size_t l_copy = textFields[i].longueur + l < LEN_MAX ? l : LEN_MAX - textFields[i].longueur;
				strcat(textFields[i].texte, event->text.text);
				textFields[i].longueur += l_copy;

				dessin();
			}
		}
		break;
	case SDL_KEYDOWN:

		if (event->key.keysym.sym == SDLK_F1) {
			SDL_RenderClear(render);
			fullgraph();
			SDL_RenderPresent(render);
		}

		for (std::vector<TextField>::size_type i = 0; i < textFields.size(); i++) {
			if (textFields[i].id == textFieldSelected) {
				if (event->key.keysym.sym == SDLK_BACKSPACE && textFields[i].longueur)
				{
					textFields[i].texte[textFields[i].longueur - 1] = 0;
					textFields[i].longueur--;
				}
				if (event->key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL) && SDL_HasClipboardText())
				{
					char* tmp = SDL_GetClipboardText();
					size_t l = strlen(tmp);
					size_t l_copy = textFields[i].longueur + l < LEN_MAX ? l : LEN_MAX - textFields[i].longueur;
					strcpy_s(textFields[i].texte, LEN_MAX, tmp);
					textFields[i].longueur += l_copy;
					SDL_free(tmp);
				}
				if (event->key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL))  //event.key.keysym.sym != SDLK_KP_ENTER 
				{
					SDL_SetClipboardText(textFields[i].texte);
				}
				dessin();
			}
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (event->button.button == SDL_BUTTON_LEFT) {
			for (std::vector<Button>::size_type i = 0; i < btnList.size(); i++) {
				if (event->button.x > btnList[i].rect.x && event->button.x < btnList[i].rect.x + btnList[i].rect.w && event->button.y > btnList[i].rect.y && event->button.y < btnList[i].rect.y + btnList[i].rect.h) {
					if (btnList[i].isDisabled)
						continue;
				
					switch (btnList[i].id)
					{
					case 1:
						detruireSauvegarde(saveFile);
						diceDrawned = false;
						diceValue = -1;
						currentArticle = 0;
						currentGUI = GUIS::GAME;
						dessin();
						break;
					case 2:
						currentArticle = donnesSauvegarde(saveFile) - 1;
						currentGUI = GUIS::GAME;
						dessin();
						break;
					case 3:
						currentGUI = GUIS::EDITEUR;
						dessin();
						break;
					case 4:
						currentGUI = GUIS::MENU;
						dessin();
						break;
					case 5:
						currentGUI = GUIS::GRAPH;
						dessin();
						break;
					case 6:  // Ajouter un article
						offset = 0;

						if (strlen(fichierArticle) == 0) {
							strcpy_s(fichierArticle, 100, getTextFieldByID(23).texte);
							NombreArticleTotal(fichierArticle, atoi(getTextFieldByID(24).texte));
							
						}
						else {
							CreationArticle(fichierArticle, atoi(getTextFieldByID(1).texte), getTextFieldByID(2).texte, atoi(getTextFieldByID(3).texte), atoi(getTextFieldByID(4).texte), atoi(getTextFieldByID(6).texte), atoi(getTextFieldByID(8).texte), atoi(getTextFieldByID(10).texte), getTextFieldByID(5).texte, getTextFieldByID(7).texte, getTextFieldByID(9).texte, getTextFieldByID(11).texte);
						}

						textFields.clear();
						add_textField("Entrez le numero de l'article", 1, ESPACEMENT, ESPACEMENT, 9 * 27, 1 * 30, "");
						add_textField("Entrez le texte de l'article", 2, ESPACEMENT + 600, ESPACEMENT + (offset++ * valueOffset), 9 * 60, 16 * 30, "");
						add_textField("Entrez le nombre de lien(s) (Pour une fin - 0)", 3, ESPACEMENT, ESPACEMENT + (offset++ * valueOffset), 9 * 27, 1 * 30, "");

						add_textField("Lien 1 : ", 4, ESPACEMENT, ESPACEMENT + (offset++ * valueOffset), 9 * 27, 1 * 30, "");
						add_textField("Texte du lien 1 : ", 5, ESPACEMENT, ESPACEMENT + (offset++ * valueOffset), 9 * 56, 1 * 30, "");

						add_textField("Lien 2 : ", 6, ESPACEMENT, ESPACEMENT + (offset++ * valueOffset), 9 * 27, 1 * 30, "");
						add_textField("Texte du lien 2 : ", 7, ESPACEMENT, ESPACEMENT + (offset++ * valueOffset), 9 * 56, 1 * 30, "");

						add_textField("Lien 3 : ", 8, ESPACEMENT, ESPACEMENT + (offset++ * valueOffset), 9 * 27, 1 * 30, "");
						add_textField("Texte du lien 3 : ", 9, ESPACEMENT, ESPACEMENT + (offset++ * valueOffset), 9 * 56, 1 * 30, "");

						add_textField("Lien 4 : ", 10, ESPACEMENT, ESPACEMENT + (offset++ * valueOffset), 9 * 27, 1 * 30, "");
						add_textField("Texte du lien 4 : ", 11, ESPACEMENT, ESPACEMENT + (offset++ * valueOffset), 9 * 56, 1 * 30, "");

						dessin();
						break;
					case 7: // Finaliser livre
						currentGUI = GUIS::MENU;
						dessin();
						break;
					case 8:
						animation_dice();
						break;
					case 9:
						currentGUI = GUIS::GAME;
						dessin();
						break;
					default:
						//TODO: play sound
						break;
					}
				}
			}

			if (currentGUI == GUIS::GAME) { // Interface de jeu/article
				int x = 5;
				int y = 0;

				// Liens
				for (int i = 0; i < nombreLiens; i++) {
					int x = (ESPACEMENT);
					int y = (HAUTEUR - 150) + (30 * i);

					if (event->button.x > x - 60 && event->button.x < x + LARGEUR && event->button.y > y && event->button.y < y + 16) {
						currentArticle = articles[currentArticle].tLiens[i].destination - 1;
						sauvegarder(articles[currentArticle]);

						diceDrawned = false;
						diceValue = -1;
						dessin();
					}
				}
			}
			else if (currentGUI == GUIS::EDITEUR) {

				textFieldSelected = -1;
				for (std::vector<TextField>::size_type i = 0; i < textFields.size(); i++) {
					if (event->button.x > textFields[i].rect.x + textFields[i].rect.w + 5 && event->button.x < textFields[i].rect.x + textFields[i].rect.w + 5 + 24 && event->button.y > textFields[i].rect.y && event->button.y < textFields[i].rect.y + 24) {
						memset(textFields[i].texte, 0, LEN_MAX);
						textFields[i].longueur = 0;
						dessin();
					}

					if (event->button.x > textFields[i].rect.x&& event->button.x < textFields[i].rect.x + textFields[i].rect.w && event->button.y > textFields[i].rect.y&& event->button.y < textFields[i].rect.y + textFields[i].rect.h)
					{
						textFieldSelected = textFields[i].id;
					}
				}
			}
			else if (currentGUI == GUIS::GRAPH) // Menu graphe
			{

			}
		}
		break;
	case SDL_MOUSEMOTION:
		if (currentGUI == GUIS::GAME) {
			SDL_GetMouseState(&xMouse, &yMouse);
			bool inOne = false;

			for (int i = 0; i < nombreLiens; i++) {
				int x = (ESPACEMENT);
				int y = (HAUTEUR - 150) + (30 * i);
				if (xMouse > x - 60 && xMouse < x + LARGEUR && yMouse > y && yMouse < y + 16) {
					if (destinations[i][0] == 0) {
						destinations[i][0] = 1;
						dessin();
					}
					inOne = true;
				}
			}

			if (!inOne) {
				for (int i = 0; i < nombreLiens; i++) {
					if (destinations[i][0] == 1) {
						destinations[i][0] = 0;
						dessin();
					}
				}

			}
		}
		else if (currentGUI == GUIS::GRAPH)
		{
			SDL_GetMouseState(&xMouse, &yMouse);
		}
		break;
	}
}

void CPPaHTML(Article* tab) {

	CreateDirectoryA("HTML", NULL);
	for (int i = 0; i < NombreArticle(filePath); i++) {
		char buf[16];
		sprintf_s(buf, "HTML\\%d.html", tab[i].id);
		const char* p = buf;
		ofstream monFlux(p);

		monFlux << "<!DOCTYPE html>" << endl;
		monFlux << endl;
		monFlux << "<head>" << endl;
		monFlux << "<title>" << "Article Numero " << buf << " <\/title>" << endl;
		monFlux << "<\/head>" << endl;

		monFlux << "<body>" << endl;
		// Titre monFlux << "<div id=\"top\">" << "<h1>Vous etes sur la page numero " << tab[i].id << " </h1> </div>"<< endl;
		monFlux << "<p>" << tab[i].texte << "<\/p>" << endl;

		for (int a = 0; a < tab[i].nombreLien; a++) {
			monFlux << "<a href=\"" << tab[i].tLiens[a].destination << ".html" << "\"> " << tab[i].tLiens[a].texte << "Aller au " << tab[i].tLiens[a].destination << "<\/a> <br \/> " << endl;
		}

		monFlux << "<\/body>" << endl;
		//cout << "J affiche tmp[0]" << tab[i].tLiens[0].destination;

	}
}

void afficherNoeud(Noeud * noeud, SDL_Rect * rect)
{
	char buf[4];
	SDL_RenderFillRect(render, rect);
	//draw_text(buf, rect->x + rect->w / 4, rect->y + rect->h / 4, { 255,0,0 }, font_graph);
}

void fullgraph()
{
	char buf[4];
	//draw_text(buf, noeud->x + noeud->w / 4, noeud->y + noeud->h / 4, { 255,0,0 }, font_graph);
	draw_background();
	btnList.clear();
	add_button("", 4, 5, 0, 64, 64, NULL, true, "images/arrow.png", false);

	draw_buttons();

	inStream.open(filePath, ios::in);
	char taille[100];
	inStream.getline(taille, 100);
	int tailleFichier = atoi(taille);
	graphi = new Noeud[tailleFichier];
	cout << "t:" << tailleFichier << endl;

	for (int i(0); i < tailleFichier; i++)
	{
		graphi[i].article = &articles[i];
	}

	int width = 10;
	int height = 10;
	int w = 0;

	SDL_Rect noeud[350];

	Arete ar;

	int i(0), j(0), tailleColonne(0);
	int positionX = width, positionY = (HAUTEUR - height) / 2;
	for (int i(0); i < tailleFichier; i++)
	{
		graphi[i].x = rand() % LARGEUR - width;
		graphi[i].y = rand() % HAUTEUR - height;
		noeud[i].x = graphi[i].x;
		noeud[i].y = graphi[i].y;
		noeud[i].w = width;
		noeud[i].h = height;
	}


	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	for (int i(0); i < tailleFichier; i++)
	{
		for (int j(0); j < graphi[i].article->nombreLien; j++)
		{
			ar.origine = &graphi[i];
			ar.destination = &graphi[graphi[i].article->tLiens[j].destination];
			SDL_RenderDrawLine(render, ar.origine->x + width / 2, ar.origine->y + height / 2, ar.destination->x + width / 2, ar.destination->y + height / 2);
		}
	}

	for (int i(0); i < tailleFichier; i++)
	{
		afficherNoeud(&graphi[i], &noeud[i]);
	}

	inStream.close();
}

int main(int argc, char* argv[]) {

	srand(time(NULL));

	inStream.close();
	outStream.close();

	if (!initialisationSDL())
		return EXIT_FAILURE;

	dessin();
	//TODO: Faire un écran de chargement 

	ifstream lecture("testExport.lb", ios::in);
	char trash[200000];

	lecture.getline(trash, 200000, '\n');
	int nombreArticle = atoi(trash);
	articles = new Article[nombreArticle];

	if (!initArticle(articles, nombreArticle, filePath)) {
		cout << "Erreur lors de l'initialisation des articles. Fermeture du programme.";
		return EXIT_FAILURE;
	}
	cout << "Importation finie." << endl;

	textFields.clear();

	add_textField("Entrez un nombre d'articles", 24, ESPACEMENT, ESPACEMENT, 9 * 27, 1 * 30, "");
	add_textField("Nom du fichier ou stocker le livre", 23, ESPACEMENT, ESPACEMENT + 62, 9 * 27, 1 * 30, "");

	// Ne pas tout le temps faire l'importation HTML sinon c'est lent
	//CPPaHTML(articles);

	SDL_Event event;
	while (continuer) // Main loop
	{
		SDL_WaitEvent(&event);
		keystroke(&event);
	}

	destructionSDL();

	delete[] articles;

	return EXIT_SUCCESS;
}