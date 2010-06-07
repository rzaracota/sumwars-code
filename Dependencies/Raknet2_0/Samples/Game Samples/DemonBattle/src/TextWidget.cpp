#include "TextWidget.h"
#include "ManagerState.h"
#include <assert.h>
#include "..\Common\Include\d3dutil.h"

//#include "MemoryManager.h"

TextWidget::TextWidget()
{
	x=y=width=height=0;
	mouseStateTextData[0]=0;
}

TextWidget::~TextWidget()
{
	Unload();
}

void TextWidget::SetLocation(int X, int Y)
{
	assert(X >=0);
	assert(Y >=0);
	assert(X < (int)manager->GetScreenWidth());
	assert(Y < (int)manager->GetScreenHeight());
	x=X; y=Y;
	mouseStateText.GetTextDimensions(mouseStateTextData, x, y, &width, &height);
}

void TextWidget::Render(long time)
{
	assert(x >=0);
	assert(y >=0);
	assert(x < (int)manager->GetScreenWidth());
	assert(y < (int)manager->GetScreenHeight());
	if (mouseStateTextData[0]==0)
		return;

	// If the text is too long to fit on the screen scroll it left
	int newWidth=width;
	int charOffset=0;
	while (x+newWidth >= (int)manager->GetScreenWidth())
	{
		mouseStateText.GetTextDimensions(mouseStateTextData+ ++charOffset, x, y, &newWidth, &height);
	}

	mouseStateText.Render(mouseStateTextData+charOffset, x,y, mouseStateColor[GetMouseStateIndex()], true);
}

void TextWidget::SetText(char *txt)
{
	// The text must be created before you can calculate dimensions
	if (mouseStateText.isTextCreated()==false)
		return;

	assert(x >=0);
	assert(y >=0);
	assert(x < (int)manager->GetScreenWidth());
	assert(y < (int)manager->GetScreenHeight());
	
	if (txt)
	{
		strncpy(mouseStateTextData, txt, TEXT_LENGTH-1);
		mouseStateTextData[TEXT_LENGTH-1]=0;
		mouseStateText.GetTextDimensions(mouseStateTextData, x, y, &width, &height);
	}
}

void TextWidget::Load(int X, int Y, int height, const char *faceName, IDirect3DDevice9* device, D3DCOLOR mouseNotOver, D3DCOLOR mouseOver, D3DCOLOR mousePressed, char *txt)
{
	mouseStateText.Create(height, faceName, device);
	assert(X >=0);
	assert(Y >=0);
	assert(X < (int)manager->GetScreenWidth());
	assert(Y < (int)manager->GetScreenHeight());
	x=X; y=Y;
	mouseStateColor[0]=mouseNotOver;
	mouseStateColor[1]=mouseOver;
	mouseStateColor[2]=mousePressed;
	SetText(txt);
}

void TextWidget::Unload(void)
{
	mouseStateText.Unload();
}
