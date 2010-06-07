#include "Text.h"
#include <D3dx9core.h>

//#include "MemoryManager.h"

void Text::Create(int height, const char *faceName, IDirect3DDevice9* device)
{
	D3DXFONT_DESC lf;
	memset(&lf, 0, sizeof(D3DXFONT_DESC));       // zero out structure
	lf.Height = height;                      // request a 12-pixel-height font
	strcpy(lf.FaceName, faceName);        // request a face name "Arial"
	D3DXCreateFontIndirect(device, &lf, &font);
}

Text::~Text()
{
	Unload();
}

void Text::Unload(void)
{
	if (font)
		font->Release();
	font=0;
}

// Returns number of vertical pixels used
int Text::Render(char* text, int x, int y, D3DCOLOR color, bool clip)
{
	if (font==0)
		return 0;

	int ORClip;
	RECT rect;
	rect.top=y;
	rect.bottom=y;
	rect.left=x;
	rect.right=x;

	if (clip==false)
		ORClip = DT_NOCLIP;
	else
		ORClip = 0;

	INT i = font->DrawText(0,text, -1, &rect, DT_CALCRECT | ORClip, color);
	rect.bottom = rect.top + i;
	return font->DrawText(0,text, -1, &rect, ORClip, color);
}

void Text::GetClippedTextDimensions(char* text, int x, int y, int *width, int *height)
{
	if (font==0)
		return;

	if (text==0)
	{
		*width=0;
		*height=0;
	}

	RECT rect;
	rect.top=y;
	rect.bottom=y;
	rect.left=x;
	rect.right=x;

	font->DrawText(0,text, -1, &rect, DT_CALCRECT, 0xFF0000FF);

	*width=rect.right-rect.left;
	*height=rect.bottom-rect.top;
}

void Text::GetTextDimensions(char* text, int x, int y, int *width, int *height)
{
	if (font==0)
		return;

	RECT rect;
	rect.top=y;
	rect.bottom=y;
	rect.left=x;
	rect.right=x;

	font->DrawText(0,text, -1, &rect, DT_CALCRECT | DT_NOCLIP, 0xFF0000FF);

	*width=rect.right-rect.left;
	*height=rect.bottom-rect.top;
}

// Returns number of vertical pixels used
int Text::Render(char* text, int x, int y, int a, int r, int g, int b, bool clip)
{
	if (font==0)
		return 0;

	return Render(text, x, y, D3DCOLOR_ARGB(a,r,g,b), clip);
}
	