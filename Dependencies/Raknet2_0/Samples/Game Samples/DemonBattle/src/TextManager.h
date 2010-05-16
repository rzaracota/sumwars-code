#ifndef __TEXT_MANAGER_H
#define __TEXT_MANAGER_H

#include "LinkedList.h"
#include "Text.h"
#include "ManagerState.h"


class TextManager
{
public:
	~TextManager();
	void Initialize(IDirect3DDevice9* Device) {device=Device;}
	static inline TextManager* Instance(void) {return &I;}

	// ***
	// A better way to show a message is to create a Text object within the class and call that with different parameters to render.
	// Doing -1 to show message means we have to create and destroy the text every frame
	// ***

	// Easy way to show debugging messages in the upper left
	// Duration is in ms.  Duration of -65535 means infinite (Get rid of it by clearing the screen with ClearMessages)
	void ShowMessage(int duration, char *format, ...);

	// Show a message for the specified ms, use <=0 to show for only 1 frame (not efficient)
	// x,y are screen coordinates.  Use -1 to automatically choose coordinates
	// a(alpha),r,g,b are the colors you want to use (0-255), clip is whether you want to check for clipping or not.
	// Its faster not to check for clipping, but you only only do that if you are sure the message won't go outside the screen boundary
	// Use < 0 for x to automatically place message in upper left of the screen
	// Duration is in ms.  Duration of -65535 means infinite (Get rid of it by clearing the screen with ClearMessages)
	bool ShowMessageEx(int duration, char* text, int x=-1, int y=-1, int height=20, const char *faceName="Arial", int a=255, int r=255, int g=255, int b=255, bool clip=true);

	// Render all messages.  You should call this every frame.
	void Render(unsigned long elapsedTime);

	void Unload(void);
	void ClearMessages(void);
private:
	TextManager();
	static const int MAX_TEXT_LENGTH=1000;
	struct TextStruct
	{
		int duration;
		int x,y,a,r,g,b;
		bool clip;
		Text text;
		char msg[MAX_TEXT_LENGTH];
	};

	IDirect3DDevice9* device;
	BasicDataStructures::LinkedList<TextStruct*> textList;
	static TextManager I;
};

#define textManager TextManager::Instance()

#endif