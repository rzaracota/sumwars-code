#include "TextManager.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "..\Common\Include\d3dutil.h"

//#include "MemoryManager.h"

TextManager TextManager::I;

TextManager::TextManager()
{
	device=0;
}

TextManager::~TextManager()
{
	Unload();
	
}

void TextManager::ClearMessages(void)
{
	TextStruct* s;

	while (textList.size())
	{
		s = textList.peek();
		s->text.Unload();
		delete textList.pop();
	}
}

void TextManager::Unload(void)
{
	ClearMessages();
}

void TextManager::ShowMessage(int duration, char *format, ...)
{
	char LogString[MAX_TEXT_LENGTH];
	va_list arg_ptr;
		
	if(strlen(format) > sizeof(LogString)-256) return;
	va_start(arg_ptr, format);
	vsprintf(LogString, format, arg_ptr);
	va_end(arg_ptr);
	if (LogString[0]==0)
		return;
	
	ShowMessageEx(duration, LogString);
}


bool TextManager::ShowMessageEx(int duration, char* text, int x, int y, int height, const char *faceName, int a, int r, int g, int b, bool clip)
{
	if (device==0)
		return false;

	TextStruct* s = new TextStruct;
    
	s->duration=duration;
	s->text.Create(height, faceName, device);
	s->x=x;
	s->y=y;
	s->a=a;
	s->r=r;
	s->g=g;
	s->b=b;
	s->clip;
	strncpy(s->msg, text, MAX_TEXT_LENGTH-1);
	s->msg[MAX_TEXT_LENGTH-1]=0;
	textList.add(s);

	return true;
}

// Render all messages.  You should call this every frame.
void TextManager::Render(unsigned long elapsedTime)
{
	int autoX=0, autoY=0,x,y,height;

	TextStruct* s;
	int count = textList.size();
	textList.beginning();

	while (count-->0)
	{
		s=textList.peek();

		if (s->x<0)
		{
			// Automatically choose where to render
			x=autoX; y=autoY;
		}
		else
		{
			x=s->x; y=s->y;
		}

		// Fade out if duration was originally above 0
		if (s->duration < 100 && s->duration>0)
		{
			// a means alpha
			int a = (int) (((float)s->duration/100.0f) * 255.0f);
			if (s->a < a)
				s->a=a;
		}

		if (s->duration!=-65535)
			s->duration-=elapsedTime;
        
		// Show the message
		height = s->text.Render(s->msg, x, y, s->a, s->r, s->g, s->b, s->clip);
		if (s->x<0)
		{
			autoY+=height;
			if (autoY > (int)manager->GetScreenHeight())
				return;
		}
		
		// Delete it if necessary
		if (s->duration<=0)
			delete textList.pop(); // Autoincrements
		else
			textList++;
	}
}