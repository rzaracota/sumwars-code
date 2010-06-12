#ifndef GUIDEBUGTAB_H
#define GUIDEBUGTAB_H

#include "debugtab.h"
#include "CEGUIWindow.h"

class GuiDebugTab : public CEGUI::Window, public DebugTab
{

public:
    GuiDebugTab(const CEGUI::String& type, const CEGUI::String& name);
	virtual void update();
	
protected:

private:
	CEGUI::Window *m_windowUnderMouse;
};

#endif // GUIDEBUGTAB_H
