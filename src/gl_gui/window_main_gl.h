#ifndef WINMAIN_H
#define WINMAIN_H


#include <sys/time.h>

class WindowMain
{
	public:


	WindowMain();
	~WindowMain(){}

	static void DisplayFunc ();
	static void ReshapeFunc (int,int);
	static void MouseFunc (int,int,int,int);
	static void SetCamera();
	static void KeyboardFunc (unsigned char,int,int);
	static void KeyboardUpFunc (unsigned char,int,int);
	static void SpecialKeyboardUpFunc (int key, int x, int y);
	static void SpecialKeyboardFunc (int key, int x, int y);
	static void TimerFunc(int dummy);


};

#pragma message("WinMain definiert")

#include <GL/glut.h>
#include "document.h"

#include <stdlib.h>
#include <math.h>

#pragma message("header ende")

#endif
