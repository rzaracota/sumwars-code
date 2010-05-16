#ifndef __CHAT_MESSAGE_CONTAINER_H
#define __CHAT_MESSAGE_CONTAINER_H

#include "TextListContainer.h"
#include "Text.h"

class ChatMessageContainer : public TextListContainer
{
public:
	ChatMessageContainer();
	~ChatMessageContainer();
	void Load(int X, int Y, int LinesOfText, long FadeTime, int TextHeight, IDirect3DDevice9 *Device);
	void Unload(void);
	void Render(long time); // This renders from bottom to top
	void SetColor(unsigned char A, unsigned char R, unsigned char G, unsigned char B);
protected:
	Text outputText;
	unsigned char a,r,g,b;
};

#endif
