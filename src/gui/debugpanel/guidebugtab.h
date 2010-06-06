#ifndef GUIDEBUGTAB_H
#define GUIDEBUGTAB_H

#include "debugtab.h"

class GuiDebugTab : public DebugTab
{

public:
    GuiDebugTab(const CEGUI::String& type, const CEGUI::String& name);
    virtual void initialiseComponents(void );
	virtual void update();
	
protected:
    virtual void onSized(CEGUI::WindowEventArgs& e);
    virtual void onShown(CEGUI::WindowEventArgs& e);
    virtual void onEnabled(CEGUI::WindowEventArgs& e);
    virtual void onDisabled(CEGUI::WindowEventArgs& e);
};

#endif // GUIDEBUGTAB_H
