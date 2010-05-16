#ifndef __BITMAP_WIDGET_H
#define __BITMAP_WIDGET_H

#include "Widget.h"
#include "Bitmap.h"

class BitmapWidget : public Widget
{
public:
	BitmapWidget();
	~BitmapWidget();

	// Call Load first with all the parameters for the textWidget
	void Load(int X, int Y, const char *mouseOverImage, const char *mouseNotOverImage);
	void Unload(void);
	void Render(long time);
	void SetLocation(int X, int Y);

protected:
	Bitmap mouseOver, mouseNotOver;
};

#endif