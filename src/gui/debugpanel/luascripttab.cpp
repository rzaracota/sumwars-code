#include "luascripttab.h"
#include "CEGUI.h"
#include <iostream>
#include "textfileeditwindow.h"

#include "OgreString.h"

using namespace CEGUI;

CEGUI::String LuaScriptTab::WidgetTypeName = "LuaScriptTab";

LuaScriptTab::LuaScriptTab(const CEGUI::String& type, const CEGUI::String& name): CEGUI::Window(type, name), DebugTab()
{
	setText("Lua");
	m_newFileCtr = 0;
	
	m_tabLayout = WindowManager::getSingleton().loadWindowLayout("LuaScriptTab.layout");
	m_tabLayout->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	m_tabLayout->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));

	m_fileTabControl = static_cast<TabControl*>(m_tabLayout->getChild("luaScriptTab/FileTabControl"));
	m_filePathEditBox = static_cast<Editbox*>(m_tabLayout->getChild("luaScriptTab/fileDirectoryEditBox"));
	m_filePathEditBox->setText("./data/lua/debug.lua");
	
	createMenu();
	
	this->addChildWindow(m_tabLayout);

	m_fileTabControl->subscribeEvent(TabControl::EventSelectionChanged, CEGUI::Event::Subscriber(&LuaScriptTab::handleTabChanged, this));
	
}

void LuaScriptTab::update()
{

}

void LuaScriptTab::onSized(CEGUI::WindowEventArgs& e)
{
    CEGUI::Window::onSized(e);
}

void LuaScriptTab::onTextChanged(CEGUI::WindowEventArgs& e)
{
    CEGUI::Window::onTextChanged(e);
}

void LuaScriptTab::onMouseMove(CEGUI::MouseEventArgs& e)
{
    CEGUI::Window::onMouseMove(e);
}

void LuaScriptTab::onMouseWheel(CEGUI::MouseEventArgs& e)
{
    CEGUI::Window::onMouseWheel(e);
}

void LuaScriptTab::onMouseButtonDown(CEGUI::MouseEventArgs& e)
{
    CEGUI::Window::onMouseButtonDown(e);
}

void LuaScriptTab::onMouseButtonUp(CEGUI::MouseEventArgs& e)
{
    CEGUI::Window::onMouseButtonUp(e);
}

void LuaScriptTab::onMouseClicked(CEGUI::MouseEventArgs& e)
{
    CEGUI::Window::onMouseClicked(e);
}

void LuaScriptTab::onMouseDoubleClicked(CEGUI::MouseEventArgs& e)
{
    CEGUI::Window::onMouseDoubleClicked(e);
}

void LuaScriptTab::onMouseTripleClicked(CEGUI::MouseEventArgs& e)
{
    CEGUI::Window::onMouseTripleClicked(e);
}

void LuaScriptTab::onKeyDown(CEGUI::KeyEventArgs& e)
{
    CEGUI::Window::onKeyDown(e);
}

void LuaScriptTab::onKeyUp(CEGUI::KeyEventArgs& e)
{
    CEGUI::Window::onKeyUp(e);
}

void LuaScriptTab::onCharacter(CEGUI::KeyEventArgs& e)
{
    CEGUI::Window::onCharacter(e);
}

bool LuaScriptTab::handleNew(const CEGUI::EventArgs& e)
{
	std::stringstream s;
	std::ifstream myfile (m_filePathEditBox->getText().c_str());
	if (myfile.is_open())
	{
			m_filePathEditBox->setText("File exists");
			myfile.close();
			return false;
	}
	s << m_filePathEditBox->getText() << m_newFileCtr;
	TextFileEditWindow *win = static_cast<TextFileEditWindow*>(WindowManager::getSingleton().createWindow("TextFileEditWindow", s.str()));
	win->setFilepath(m_filePathEditBox->getText());
	m_fileTabControl->addTab(win);
	win->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	win->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	win->handleTextChanged(CEGUI::EventArgs());
	m_newFileCtr++;
	return true;
}

bool LuaScriptTab::handleOpen(const CEGUI::EventArgs& e)
{
	CEGUI::String s = m_filePathEditBox->getText();
	TextFileEditWindow *win = static_cast<TextFileEditWindow*>(WindowManager::getSingleton().createWindow("TextFileEditWindow", s));

	
	if(win->load(s))
	{
		m_fileTabControl->addTab(win);
		win->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
		win->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	}
	else
	{
		m_filePathEditBox->setText("File failed to load");
		WindowManager::getSingleton().destroyWindow(win);
	}

}

bool LuaScriptTab::handleSave(const CEGUI::EventArgs& e)
{
	TextFileEditWindow* win = static_cast<TextFileEditWindow*>(m_fileTabControl->getTabContentsAtIndex(m_fileTabControl->getSelectedTabIndex()));
	win->save();
}

bool LuaScriptTab::handleClose(const CEGUI::EventArgs& e)
{
	TextFileEditWindow* win = static_cast<TextFileEditWindow*>(m_fileTabControl->getTabContentsAtIndex(m_fileTabControl->getSelectedTabIndex()));
	win->close();
	m_fileTabControl->removeTab(m_fileTabControl->getSelectedTabIndex());
	WindowManager::getSingleton().destroyWindow(win);
}

bool LuaScriptTab::handleTabChanged(const CEGUI::EventArgs& e)
{
	TextFileEditWindow* win = static_cast<TextFileEditWindow*>(m_fileTabControl->getTabContentsAtIndex(m_fileTabControl->getSelectedTabIndex()));
	m_filePathEditBox->setText(win->getFilepath());
}


void LuaScriptTab::createMenu()
{
	m_menubar = static_cast<CEGUI::Menubar*>(m_tabLayout->getChild("luaScriptTab/MenuBar"));
	
	MenuItem *fileItem = static_cast<MenuItem*>(WindowManager::getSingleton().createWindow("TaharezLook/MenuItem", "luaScriptTab/MenuBar/FileItem"));
	fileItem->setText("File");
	m_menubar->addItem(fileItem);
	
	PopupMenu *filePopup = static_cast<PopupMenu*>(WindowManager::getSingleton().createWindow("TaharezLook/PopupMenu", "luaScriptTab/MenuBar/FilePopup"));
	
	MenuItem *it = static_cast<MenuItem*>(WindowManager::getSingleton().createWindow("TaharezLook/MenuItem", "luaScriptTab/MenuBar/FileItemNew"));
	it->setText("New");
	filePopup->addItem(it);
	it->subscribeEvent(MenuItem::EventClicked, CEGUI::Event::Subscriber(&LuaScriptTab::handleNew, this));
	
	it = static_cast<MenuItem*>(WindowManager::getSingleton().createWindow("TaharezLook/MenuItem", "luaScriptTab/MenuBar/FileItemOpen"));
	it->setText("Open");
	filePopup->addItem(it);
	it->subscribeEvent(MenuItem::EventClicked, CEGUI::Event::Subscriber(&LuaScriptTab::handleOpen, this));
	
	it = static_cast<MenuItem*>(WindowManager::getSingleton().createWindow("TaharezLook/MenuItem", "luaScriptTab/MenuBar/FileItemSave"));
	it->setText("Save");
	filePopup->addItem(it);
	it->subscribeEvent(MenuItem::EventClicked, CEGUI::Event::Subscriber(&LuaScriptTab::handleSave, this));
	
	it = static_cast<MenuItem*>(WindowManager::getSingleton().createWindow("TaharezLook/MenuItem", "luaScriptTab/MenuBar/FileItemClose"));
	it->setText("Close");
	filePopup->addItem(it);
	it->subscribeEvent(MenuItem::EventClicked, CEGUI::Event::Subscriber(&LuaScriptTab::handleClose, this));
	
	fileItem->setPopupMenu(filePopup);
}