#ifndef __TEXT_INPUT_WIDGET_H
#define __TEXT_INPUT_WIDGET_H

#include "TextWidget.h"

class TextInputWidget : public TextWidget
{
public:
	TextInputWidget();
	~TextInputWidget();
	void Init(void);
	void Deinit(void);
	// Returns true if handled
	bool KeyJustPressed(int key);
	bool KeyJustReleased(int key);
	void SetPrefixText(char *text);
	void Update(long time);
	void SetText(char *txt);
	void SetDefaultText(char *txt);
	void Load(int X, int Y, int height, const char *faceName, IDirect3DDevice9* device, D3DCOLOR mouseNotOver, D3DCOLOR mouseOver, D3DCOLOR mousePressed, char *txt);
	void SetMaximumTextLength(int max) {maximumTextLength=max; if (maximumTextLength>=TEXT_LENGTH) maximumTextLength=TEXT_LENGTH-1;}
	void StopTextInput(void); // Stop reading text input
	void StartTextInput(void); // Stop reading text input

	// Let the user read the text, but don't let them directly change it.  Use SetText for that
	const char* GetUserInput(void) const;

protected:
	void GenerateMouseStateTextData(void); // Create the output from the two inputs prefixText and messageText and the blink timer
	void FixTextData(void);
	char prefixText[TEXT_LENGTH];
	char messageText[TEXT_LENGTH];
	char defaultText[TEXT_LENGTH];
	bool enteringText, blink;
	long blinkTimer;
	int maximumTextLength;
};

#endif