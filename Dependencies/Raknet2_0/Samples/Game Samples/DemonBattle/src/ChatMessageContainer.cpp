#include "ChatMessageContainer.h"
#include "ManagerState.h"

ChatMessageContainer::ChatMessageContainer() : a(255), r(255), g(255), b(255)
{
}

ChatMessageContainer::~ChatMessageContainer()
{
}

void ChatMessageContainer::Load(int X, int Y, int LinesOfText, long FadeTime, int TextHeight, IDirect3DDevice9 *Device)
{
	TextListContainer::Load(X,Y,LinesOfText,FadeTime,TextHeight,Device);
	
	outputText.Create(textHeight, "Arial", device);
}

void ChatMessageContainer::Unload(void)
{
	TextListContainer::Unload();

	outputText.Unload();
}

void ChatMessageContainer::Render(long time)
{
	int yOffset=0, height=0;
	int alpha;

	textList.beginning();

	for (int i=0L; i < (int)textList.size(); i++)
	{
		// Implement a fade out effect for old messages
		if (i >= linesOfText && i==(int)textList.size()-1)
			alpha = 255 * lastMessageFadeTime / fadeTime;
		else
			alpha = a;

		height = outputText.Render(textList.peek(), x,y-height*i, D3DCOLOR_ARGB(alpha,r,g,b), true);
		if (y-height*(i+1) <0)
			break; // Out of room to display

		textList++;
	}
}

void ChatMessageContainer::SetColor(unsigned char A, unsigned char R, unsigned char G, unsigned char B)
{
	a=A; r=R; g=G; b=B;
}
