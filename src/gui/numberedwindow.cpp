
#include "numberedwindow.h"

NumberedWindow::NumberedWindow (const CEGUI::String &type, const CEGUI::String &name)
	: CEGUI::Window(type,name)
{
	setNumber(0);
}

CEGUI::Window *  NumberedWindowFactory::createWindow (const CEGUI::String &name)
{
	return new NumberedWindow(getTypeName(),name);
}

void  NumberedWindowFactory::destroyWindow (CEGUI::Window *window)
{
	delete window;
}


