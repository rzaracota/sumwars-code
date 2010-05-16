#include "Widget.h"
#include <assert.h>
#include "InputSystem.h"

//#include "MemoryManager.h"

Widget::Widget()
{
	mouseState=lastMouseState=MOUSE_NOT_OVER;
}

void Widget::Update(long time)
{
	lastMouseState=mouseState;

	if (isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
		if (inputSystem->MouseButtonPressed(0))
			SetMousePressed();
		else
			SetMouseOver();
	else
		SetMouseNotOver();
}

MouseState Widget::GetMouseStateIndex(void)
{
	return mouseState;
}

MouseState Widget::GetLastMouseStateIndex(void)
{
	return lastMouseState;
}

bool Widget::isMouseOver(int X, int Y)
{
	return X > x && Y > y && X < width+x && Y < height+y;
}