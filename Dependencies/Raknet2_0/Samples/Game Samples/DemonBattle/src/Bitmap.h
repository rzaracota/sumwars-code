#ifndef __BITMAP_H
#define __BITMAP_H

#pragma once

#include <D3D9.h>
#include <D3DX9.h>
/*
#include <D3dx9tex.h>
struct IDirect3DSurface9;
*/
class Bitmap
{
public:
	Bitmap();
	~Bitmap();
	void Load(char* file);
	void Render(int x, int y);
	void RenderTiled(void);
	void Free(void);
	bool isLoaded(void) const {return bitmapLoaded;}
	int GetWidth(void) const {return imageInfo.Width;}
	int GetHeight(void) const {return imageInfo.Height;}
protected:
	D3DXIMAGE_INFO imageInfo;
	IDirect3DSurface9* surface;
	bool bitmapLoaded;
};

#endif
