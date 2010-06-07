#ifndef __TEXT_LIST_CONTAINER_H
#define __TEXT_LIST_CONTAINER_H

#include "LinkedList.h"

struct IDirect3DDevice9;

class TextListContainer
{
public:
	TextListContainer();
	~TextListContainer();
	void Load(int X, int Y, int LinesOfText, long FadeTime, int TextHeight, IDirect3DDevice9 *Device);
	void Unload(void);
	void Update(long time);
	char* GetTextAtIndex(int index);
	void AddText(char *text);
	void SetX(int X) {x=X;}
	void SetY(int Y) {y=Y;}
	void SetFadeTime (int FadeTime) {fadeTime=FadeTime;}
	int GetX(void) const {return x;}
	int GetY(void) const {return y;}
	int GetFadeTime(void) const {return fadeTime;}
	void Clear(void); // Clear all the messages currently in the buffer
protected:
	int x,y,linesOfText, fadeTime, lastMessageFadeTime, textHeight;
	IDirect3DDevice9 *device;
	BasicDataStructures::CircularLinkedList<char*> textList;
};

#endif