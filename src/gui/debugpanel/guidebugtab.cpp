#include "guidebugtab.h"

#include <iostream>

GuiDebugTab::GuiDebugTab(const CEGUI::String& type, const CEGUI::String& name): DebugTab(type, name)
{
	
}


void GuiDebugTab::initialiseComponents(void )
{
    CEGUI::Window::initialiseComponents();
}

void GuiDebugTab::update()
{
	std::cout << "update!!!!" << std::endl;
}


void GuiDebugTab::onSized(CEGUI::WindowEventArgs& e)
{
    CEGUI::Window::onSized(e);
}

void GuiDebugTab::onShown(CEGUI::WindowEventArgs& e)
{
    CEGUI::Window::onShown(e);
}

void GuiDebugTab::onEnabled(CEGUI::WindowEventArgs& e)
{
    CEGUI::Window::onEnabled(e);
}

void GuiDebugTab::onDisabled(CEGUI::WindowEventArgs& e)
{
    CEGUI::Window::onDisabled(e);
}

