#include "textureman.h"
#include "..\Common\Include\d3dutil.h"
extern LPDIRECT3DDEVICE9 m_pd3dDevice;        // The D3D rendering device
//#include "MemoryManager.h"

#include <assert.h>

TextureManager TextureManager::I;

/*
HRESULT CreateTexture2 (LPDIRECT3DDEVICE9 device, char* name, LPDIRECT3DTEXTURE9* texture, int sx,int sy)
{
	D3DFORMAT d3dFormat;
	d3dFormat=D3DFMT_UNKNOWN;
    // Get the path to the texture
    TCHAR strPath[MAX_PATH];
    DXUtil_FindMediaFile( strPath, name );

    // Create the texture using D3DX
    return D3DXCreateTextureFromFileEx( device, strPath, 
                sx, sy, D3DX_DEFAULT, 0, d3dFormat, 
                D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
                D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, NULL, NULL, texture);
}*/

LPDIRECT3DTEXTURE9 TextureManager::LoadTexture (LPCSTR name)
{
	//Alle Texturen durchgene
	for (std::list<STexture>::iterator l=m_texture.begin();l!=m_texture.end();l++)
	{
		//texture gibts schon ?
		if (!strcmp(name,l->name))
			return FALSE;
	}

	//Texture laden
	LPDIRECT3DTEXTURE9 texture=NULL;
	HRESULT h=D3DUtil_CreateTexture (m_pd3dDevice, (LPSTR) name,&texture);

	//...
	if (!SUCCEEDED(h))
	{
//		g_toolz.ErrorBox ("Fehler beim laden der Texture");
	//	g_toolz.ErrorBox (name);
		assert (0);
	}
	assert (texture!=NULL);

	
	// Turn on mip-mapping
	m_pd3dDevice->SetTexture (0,texture);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//D3DUtil_SetColorKey (texture, RGB(0,0,0));
	

	//Speichern
	STexture t;
	t.name = new char[strlen(name)+1];
	
	t.texture=texture;
	strcpy (t.name, name);

	m_texture.push_back (t);
    
	return texture;
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture (LPCSTR name)
{
	//Alle Texturen durchgene
	for (std::list<STexture>::iterator l=m_texture.begin();l!=m_texture.end();l++)
	{
		//texture gibts schon ?
		if (!strcmp (l->name, name))
			return l->texture; 
	}

	assert (!strcmp ("In TextureManager::GetTexture : NULL", "123"));
	return NULL;
}

void TextureManager::RemoveAll ()
{
	if (m_texture._Mysize==0)
		return;

	//Alle Texturen durchgene
	for (std::list<STexture>::iterator l=m_texture.begin();l!=m_texture.end();l++)
	{
		if (l->name)
			delete [] l->name;

		if (l->texture)
		{
			l->texture->Release ();
		}
	}

	m_texture.clear ();
}