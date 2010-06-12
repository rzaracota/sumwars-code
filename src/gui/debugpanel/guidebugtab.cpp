#include "guidebugtab.h"
#include "CEGUI.h"

#include <iostream>

using namespace CEGUI;

GuiDebugTab::GuiDebugTab(const CEGUI::String& type, const CEGUI::String& name): CEGUI::Window(type, name), DebugTab()
{
	setText("Guiuiui");

	/*//m_windowUnderMouse = WindowManager::getSingleton().loadWindowLayout("GuiDebugTab.layout");
	m_windowUnderMouse = WindowManager::getSingleton().createWindow("TaharezLook/Button", "GuiDebugTabWinUnderMouse");
	m_windowUnderMouse->setPosition(UVector2(UDim(0.1f, 0.0f), UDim(0.1f, 0.0f)));
	m_windowUnderMouse->setSize(UVector2(UDim(0.6f, 0.0f), UDim(0.6f, 0.0f)));*/

	CEGUI::Window *testWindow1 = WindowManager::getSingleton().createWindow("TaharezLook/FrameWindow", "Test124");
	testWindow1->setPosition(UVector2(UDim(0.1f, 0.0f), UDim(0.1f, 0.0f)));
	testWindow1->setSize(UVector2(UDim(0.6f, 0.0f), UDim(0.6f, 0.0f)));
	testWindow1->setText("Muh!");
	
	CEGUI::Window *Testwindow2 = WindowManager::getSingleton().createWindow("TaharezLook/Button", "Test45668");
	Testwindow2->setPosition(UVector2(UDim(0.1f, 0.0f), UDim(0.1f, 0.0f)));
	Testwindow2->setSize(UVector2(UDim(0.6f, 0.0f), UDim(0.6f, 0.0f)));
	testWindow1->addChildWindow(Testwindow2);

	this->addChildWindow(testWindow1);
	
}

void GuiDebugTab::update()
{
	
}

