#include "TextInputWidget.h"
#include <assert.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <ctype.h>
#include "InputSystem.h"

//#include "MemoryManager.h"

static const long BLINK_TIMER=500;

TextInputWidget::TextInputWidget()
{
	TextWidget::TextWidget();
	maximumTextLength=TEXT_LENGTH;
	prefixText[0]=0;
	messageText[0]=0;
	defaultText[0]=0;
	blinkTimer = BLINK_TIMER;
	blink=false;
}

TextInputWidget::~TextInputWidget()
{
	TextWidget::~TextWidget();
}

void TextInputWidget::Deinit(void)
{
	StopTextInput();
	if (messageText[0]==0 && defaultText[0]!=0)
		strcpy(messageText, defaultText);
}

void TextInputWidget::Init(void)
{
	enteringText=false;
}

void TextInputWidget::Update(long time)
{
	Widget::Update(time);

	// If the user clicks on it start or stop taking input for it
	if (lastMouseState!=MOUSE_PRESSED && mouseState==MOUSE_PRESSED)
		enteringText=!enteringText;

	blinkTimer-=time;
	if (blinkTimer < 0L)
	{
		blink = !blink;
		blinkTimer = BLINK_TIMER;

		GenerateMouseStateTextData();
	}
}

void TextInputWidget::Load(int X, int Y, int height, const char *faceName, IDirect3DDevice9* device, D3DCOLOR mouseNotOver, D3DCOLOR mouseOver, D3DCOLOR mousePressed, char *txt)
{
	TextWidget::Load(X,Y,height,faceName,device,mouseNotOver,mouseOver,mousePressed,txt);
	if (txt)
		strcpy(messageText,txt);
}

void TextInputWidget::SetDefaultText(char *txt)
{
	if (txt)
	{
		strcpy(defaultText, txt);
		if (messageText[0]==0)
			strcpy(messageText, defaultText);
	}
	else
		defaultText[0]=0;
}

void TextInputWidget::StopTextInput(void)
{
	enteringText = false;
	FixTextData();
}

void TextInputWidget::StartTextInput(void)
{
	enteringText = true;
}

// Need to call this everytime your data changes, or everytime the cursor blinks
void TextInputWidget::GenerateMouseStateTextData(void)
{
	mouseStateTextData[0]=0;

	if (prefixText[0])
		strcpy(mouseStateTextData, prefixText);

	if (messageText[0])
		strcat(mouseStateTextData, messageText);

	if (blink && enteringText)
		strcat(mouseStateTextData, "_");
}

bool TextInputWidget::KeyJustPressed(int key)
{
	if (enteringText==false)
		return false;

	switch(key)
	{
	case DIK_BACK:
		if(strlen(messageText) < 1) break;
		messageText[strlen(messageText)-1] = '\0';
		GenerateMouseStateTextData();
		break;
	case DIK_RETURN:
	case DIK_ESCAPE:
		{
			StopTextInput();
			break;
		}
	default:
		if((int)strlen(messageText) >= maximumTextLength) break;
		UINT c = MapVirtualKey(key,1); // Convert to virtual key
		c = MapVirtualKey(c,2); // Convert to char
		if(!(isgraph(c) || c == ' ')) break;
		if (c==' ' &&
			(messageText[0]==0 || messageText[strlen(messageText)-1]==' '))
			break; // Don't start with spaces and no 2 spaces in a row

		// "capitalize" other keys
		if (inputSystem->KeyPressed(DIK_LSHIFT) || inputSystem->KeyPressed(DIK_RSHIFT))
		{
			if (c=='1') c='!';
			else if (c=='2') c='@';
			else if (c=='3') c='#';
			else if (c=='4') c='$';
			else if (c=='5') c='%';
			else if (c=='6') c='^';
			else if (c=='7') c='&';
			else if (c=='8') c='*';
			else if (c=='9') c='(';
			else if (c=='0') c=')';
			else if (c=='-') c='_';
			else if (c=='=') c='+';
			else if (c=='[') c='{';
			else if (c==']') c='}';
			else if (c=='\\') c='|';
			else if (c==';') c=':';
			else if (c=='\'') c='"';
			else if (c==',') c='<';
			else if (c=='.') c='>';
			else if (c=='/') c='?';
			else if (c=='`') c='~';

			if (c=='%')
				break; // Don't allow % in names because it can screw up showmessage
		}
		int len = strlen(messageText);
		if(inputSystem->KeyPressed(DIK_LSHIFT) || inputSystem->KeyPressed(DIK_RSHIFT)) messageText[len] = toupper(c);
		else messageText[len] = tolower(c);

		messageText[len+1]=0;

		GenerateMouseStateTextData();
	}
	return true;
}

bool TextInputWidget::KeyJustReleased(int key)
{
	return false;
}

void TextInputWidget::SetPrefixText(char *text)
{
	assert(strlen(text) < TEXT_LENGTH);
	strcpy(prefixText, text);

	GenerateMouseStateTextData();
}

void TextInputWidget::FixTextData(void)
{
// Rules:
// Can't start with a space
// Can't end with spaces
// No more than 2 spaces in a row - done in key input
// Must be printable symbols or spaces - done in key input

	int length;
	if (messageText[0]==0)
		return;

	// Can't start with a space
	if (messageText[0]==' ')
	{
		memmove(messageText, messageText+1, strlen(messageText));  // Shift the name left, not forgetting to also shift the null terminator
	}

	if (messageText[0]==0)
		return;

	// Can't end with spaces
	length = strlen(messageText);
    
	while (messageText[length-1]==' ')
	{
		messageText[--length]=0;
		if (length==0)
			return;
	}

	GenerateMouseStateTextData();
}

void TextInputWidget::SetText(char *txt)
{
	if (txt)
	{
		strcpy(messageText, txt);
	}
	else
	{
		messageText[0]=0;
		StopTextInput();
	}

	GenerateMouseStateTextData();
	TextWidget::SetText(txt);
	mouseStateText.GetClippedTextDimensions(mouseStateTextData, x, y, &width, &height);
}

const char* TextInputWidget::GetUserInput(void) const
{
	if (messageText[0])
		return (const char*) messageText;
	else if (defaultText[0])
		return (const char*) defaultText;
	else return 0;
}
