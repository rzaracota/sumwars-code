#include "TextListContainer.h"
#include "Text.h"
#include <assert.h>
#include "ManagerState.h"
#include "String.h"
#include <d3d9.h>

TextListContainer::TextListContainer()
{
}

TextListContainer::~TextListContainer()
{
	Unload();
}

void TextListContainer::Load(int X, int Y, int LinesOfText, long FadeTime, int TextHeight, IDirect3DDevice9 *Device)
{
	assert(X>=0 && X < manager->GetScreenWidth());
	assert(Y>=0);
	assert (Y < manager->GetScreenHeight());
	assert(linesOfText >= 0);
	assert(fadeTime >= 0);

	x=X; y=Y; linesOfText=LinesOfText; fadeTime=FadeTime; textHeight=TextHeight; device=Device;

	lastMessageFadeTime=fadeTime;
	
//	outputText.Create(textHeight, "Arial", device);
}

void TextListContainer::Unload(void)
{
	Clear();
}

void TextListContainer::Clear(void)
{
	while (textList.size())
	{
		delete [] textList.pop();
	}

	lastMessageFadeTime=fadeTime;
}

void TextListContainer::Update(long time)
{
	if (textList.size() > (unsigned long)linesOfText)
	{
		lastMessageFadeTime-=time;

		if (lastMessageFadeTime <= 0L)
		{
			textList.end();
			delete textList.pop();
			lastMessageFadeTime = fadeTime;
		}
	}
}

char* TextListContainer::GetTextAtIndex(int index)
{
	assert(index >= 0);
	textList.beginning();
	while (index--)
	{
        if (index==-1)
			break;

		textList++;
	}

	return textList.peek();
}

void TextListContainer::AddText(char *text)
{
	if (text==0 || text[0]==0)
		return;

	textList.beginning();
	char *str;
	str = new char[strlen(text)+1];
	strcpy(str, text);
	textList.insert(str);
}