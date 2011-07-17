#ifndef GUIDEBUGTAB_H
#define GUIDEBUGTAB_H

#include "debugtab.h"
#include "CEGUI/CEGUIWindow.h"

class GuiDebugTab : public CEGUI::Window, public DebugTab
{

public:
    GuiDebugTab(const CEGUI::String& type, const CEGUI::String& name);
	virtual void update(OIS::Keyboard *keyboard, OIS::Mouse *mouse);
	
	virtual void initialiseComponents(void );

protected:
	virtual void onSized(CEGUI::WindowEventArgs& e);

private:
	CEGUI::Window *m_tabLayout;

public:
	static CEGUI::String WidgetTypeName;
};

#endif // GUIDEBUGTAB_H
