
#include <windows.h>
#include <D3d9types.h>

#ifndef __TEXT_H
#define __TEXT_H

struct IDirect3DDevice9;
struct ID3DXFont;

class Text
{
public:
	Text() {font=0;}
	void Unload(void);
	void Create(int height, const char *faceName, IDirect3DDevice9* device);
	// Returns number of vertical pixels used
	int Render(char* text, int x, int y, int a, int r, int g, int b, bool clip);
	int Render(char* text, int x, int y, D3DCOLOR color, bool clip);

	// Get the dimensions in the specified text with clipping or no clipping as desired
	void GetTextDimensions(char* text, int x, int y, int *width, int *height);
	void GetClippedTextDimensions(char* text, int x, int y, int *width, int *height);
	bool isTextCreated(void) const {return font!=0;}

	~Text();
private:
	ID3DXFont* font;
};

#endif