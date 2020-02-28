#include<iostream>
#include<ctime>
#include "config_sdl.h"


using namespace std;

extern "C" FILE* __cdecl __iob_func()
{
	struct _iobuf_VS2012 { // ...\Microsoft Visual Studio 11.0\VC\include\stdio.h #56
		char *_ptr;
		int   _cnt;
		char *_base;
		int   _flag;
		int   _file;
		int   _charbuf;
		int   _bufsiz;
		char *_tmpfname;
	};
	// VS2015 has only FILE = struct {void*}

	int const count = sizeof(_iobuf_VS2012) / sizeof(FILE);

	//// stdout
	//return (FILE*)(&(__acrt_iob_func(1)->_Placeholder) - count);

	// stderr
	return (FILE*)(&(__acrt_iob_func(2)->_Placeholder) - 2 * count);
}

SDL_Texture *loadImage(SDL_Renderer* rendu,const char *name, int transparency, int r, int g, int b)
{
/* Charge les images avec SDL Image dans une SDL_Surface */
SDL_Surface *loadedImage = NULL;
SDL_Texture *texture = NULL;
loadedImage = IMG_Load(name);
SDL_SetSurfaceAlphaMod(loadedImage,transparency);//transparence � 200
	if (loadedImage != NULL)
	{
		if (r!=-1)
			SDL_SetColorKey(loadedImage, SDL_TRUE,SDL_MapRGB(loadedImage->format,r,g,b));
	// Conversion de l'image en texture
	texture = SDL_CreateTextureFromSurface(rendu, loadedImage);
	// On se d�barrasse du pointeur vers une surface
	SDL_FreeSurface(loadedImage);
	loadedImage = NULL;
	}
return texture;
}
 
SDL_Texture *loadText(SDL_Renderer* rendu,const char *text, SDL_Color coul, TTF_Font* font){
	return loadText(rendu, text, coul, font, 9999999);
}

SDL_Texture *loadText(SDL_Renderer* rendu, const char *text, SDL_Color coul, TTF_Font* font, int lenght) {
	//on cr�e une surface � partir du texte
	SDL_Surface*surface = TTF_RenderText_Blended_Wrapped(font, text, coul, lenght);
	//on cr�e une texture � partir de la surface
	SDL_Texture*texture = SDL_CreateTextureFromSurface(rendu, surface);
	SDL_FreeSurface(surface);
	return texture;
}

