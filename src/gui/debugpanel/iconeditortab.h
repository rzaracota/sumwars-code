#ifndef ICONEDITORTAB_H
#define ICONEDITORTAB_H

#include <CEGUI/CEGUIWindow.h>
#include "debugtab.h"

class IconEditorTab : public CEGUI::Window, public DebugTab
{
public:
	IconEditorTab(const CEGUI::String& type, const CEGUI::String& name);
	virtual void update();

protected:
	virtual void onSized(CEGUI::WindowEventArgs& e);
	virtual void onTextChanged(CEGUI::WindowEventArgs& e);
	virtual void onMouseMove(CEGUI::MouseEventArgs& e);
	virtual void onMouseWheel(CEGUI::MouseEventArgs& e);
	virtual void onMouseButtonDown(CEGUI::MouseEventArgs& e);
	virtual void onMouseButtonUp(CEGUI::MouseEventArgs& e);
	virtual void onMouseClicked(CEGUI::MouseEventArgs& e);
	virtual void onMouseDoubleClicked(CEGUI::MouseEventArgs& e);
	virtual void onMouseTripleClicked(CEGUI::MouseEventArgs& e);
	virtual void onKeyDown(CEGUI::KeyEventArgs& e);
	virtual void onKeyUp(CEGUI::KeyEventArgs& e);
	virtual void onCharacter(CEGUI::KeyEventArgs& e);

public:
	static CEGUI::String WidgetTypeName;
};

#endif // ICONEDITORTAB_H
