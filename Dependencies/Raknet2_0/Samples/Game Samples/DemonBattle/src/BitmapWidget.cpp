#include "BitmapWidget.h"
#include "ManagerState.h"
#include "..\Common\Include\d3dutil.h"
#include <assert.h>

//#include "MemoryManager.h"

BitmapWidget::BitmapWidget()
{
}

BitmapWidget::~BitmapWidget()
{
	Unload();
}

void BitmapWidget::Render(long time)
{
	assert(mouseOver.isLoaded());
	assert(mouseNotOver.isLoaded());
	assert(x >=0);
	assert(y >=0);
	assert(x < (int)manager->GetScreenWidth());
	assert(y < (int)manager->GetScreenHeight());

	if (mouseState==MOUSE_NOT_OVER)
	{
		mouseNotOver.Render(x,y);
	}
	else
	{
		mouseOver.Render(x,y);
	}
}

void BitmapWidget::SetLocation(int X, int Y)
{
	x=X; y=Y;
}

void BitmapWidget::Load(int X, int Y, const char *mouseOverImage, const char *mouseNotOverImage)
{
	mouseOver.Load((char *)mouseOverImage);
	mouseNotOver.Load((char *)mouseNotOverImage);
	width=mouseOver.GetWidth();
	height=mouseOver.GetHeight();
	x=X;
	y=Y;
}

void BitmapWidget::Unload(void)
{
	mouseOver.Free();
	mouseNotOver.Free();
}