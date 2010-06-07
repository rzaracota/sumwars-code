#ifndef TEXTUREMAN_H
#define TEXTUREMAN_H

#include "list"
#include "windows.h"
#include "D3DHeaders.h"
#include "..\Common\Include\d3dutil.h"

class TextureManager
{
public :

	static TextureManager* Instance(void) {return &I;}

	//Lädt eine Textur, aber nur wenn sie noch nicht geladen wurde
	LPDIRECT3DTEXTURE9 LoadTexture (LPCSTR name);

	//returned eine Texture beim Namen, sonst NULL
	LPDIRECT3DTEXTURE9 GetTexture (LPCSTR name);

	//Released alle Texturen
	void RemoveAll ();
private :
	//Wird intern zum speichern der Texturen verwendet
	struct STexture
	{
		STexture () {texture=NULL;name=NULL;};
		LPDIRECT3DTEXTURE9 texture;
		LPSTR name;
	};

	//Hier werden die Texturen gespeichert
	std::list<STexture> m_texture;

	static TextureManager I;
};

#define textureManager TextureManager::Instance()

#endif