#ifndef DEBUGTAB_H
#define DEBUGTAB_H

#include <CEGUIWindow.h>


class DebugTab : public CEGUI::Window
{
public:
	DebugTab(const CEGUI::String& type, const CEGUI::String& name);
	virtual void update() = 0;
};

#endif // DEBUGTAB_H
