#ifndef __WIDGET_H
#define __WIDGET_H

// This class is just shared data for derived widgets.

enum MouseState {MOUSE_NOT_OVER, MOUSE_OVER, MOUSE_PRESSED};

class Widget
{
public:
	Widget();
	void Update(long time);
	int GetXLocation(void) const {return x;}
	int GetYLocation(void) const {return y;}
	int GetWidth(void) const {return width;}
	int GetHeight(void) const {return height;}
	void SetMouseOver(void) {mouseState=MOUSE_OVER;}
	void SetMouseNotOver(void) {mouseState=MOUSE_NOT_OVER;}
	void SetMousePressed(void) {mouseState=MOUSE_PRESSED;}
	bool isMouseOver(int X, int Y);
	MouseState GetMouseStateIndex(void);
	MouseState GetLastMouseStateIndex(void);
protected:
	int x,y;
	MouseState mouseState, lastMouseState;
	int width, height;
};

#endif
