#ifndef LUASCRIPTTAB_H
#define LUASCRIPTTAB_H

#include <CEGUI/CEGUIWindow.h>
#include "debugtab.h"
#include "filebrowser/filebrowser.h"

class LuaScriptTab : public CEGUI::Window, public DebugTab
{
public:
    LuaScriptTab(const CEGUI::String& type, const CEGUI::String& name);
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
	
	virtual bool handleNew(const CEGUI::EventArgs& e);
	virtual bool handleOpen(const CEGUI::EventArgs& e);
	virtual bool handleSave(const CEGUI::EventArgs& e);
	virtual bool handleClose(const CEGUI::EventArgs& e);
	virtual bool handleTabChanged(const CEGUI::EventArgs& e);
	virtual bool handleFileBrowserAcceptClicked(const CEGUI::EventArgs& e);
	
	void createMenu();
protected:
	FileBrowser *m_fb;
	int m_newFileCtr;
	CEGUI::Window *m_tabLayout;
	CEGUI::Editbox *m_filePathEditBox;
	CEGUI::Menubar *m_menubar;
	CEGUI::TabControl *m_fileTabControl;
public:
	static CEGUI::String WidgetTypeName;
};

#endif // LUASCRIPTTAB_H
