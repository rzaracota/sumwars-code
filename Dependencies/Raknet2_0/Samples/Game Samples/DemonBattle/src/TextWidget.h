#ifndef __TEXT_WIDGET_H
#define __TEXT_WIDGET_H

#include <windows.h>
#include <D3d9types.h>
#include "Text.h"
#include "Widget.h"

class TextWidget : public Widget
{
	
protected:
	static const int TEXT_LENGTH=200;
	char mouseStateTextData[TEXT_LENGTH];
	Text mouseStateText;
	D3DCOLOR mouseStateColor[3];

public:
	TextWidget();
	~TextWidget();

	// Call Load first with all the parameters for the textWidget
	void Load(int X, int Y, int height, const char *faceName, IDirect3DDevice9* device, D3DCOLOR mouseNotOver, D3DCOLOR mouseOver, D3DCOLOR mousePressed, char *txt);
	void Unload(void);
	void Render(long time);
	void SetLocation(int X, int Y);
	// Use this to modify the text that is displayed.
	void SetText(char *txt);

};

#endif