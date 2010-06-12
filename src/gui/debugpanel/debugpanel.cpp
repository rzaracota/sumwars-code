#include "debugpanel.h"
#include "guidebugtab.h"
#include "luascripttab.h"

using namespace CEGUI;

template<> DebugPanel* Ogre::Singleton<DebugPanel>::ms_Singleton = 0;

void DebugPanel::init(bool visible)
{
	m_guiSystem = System::getSingletonPtr();
	m_winManager = WindowManager::getSingletonPtr();
	m_gameScreen = m_winManager->getWindow("GameScreen");
	createPanel(visible);
}

void DebugPanel::createPanel(bool visible)
{
	m_rootWindow = m_winManager->createWindow("TaharezLook/FrameWindow", "DebugPanel");
	m_rootWindow->setPosition(UVector2(UDim(0.1f, 0.0f), UDim(0.1f, 0.0f)));
	m_rootWindow->setSize(UVector2(UDim(0.6f, 0.0f), UDim(0.6f, 0.0f)));
	m_rootWindow->setText("Debug Panel");
	m_gameScreen->addChildWindow(m_rootWindow);

	m_tabControl = static_cast<TabControl*>(m_winManager->createWindow("TaharezLook/TabControl", "DebugPanelTabControl"));
	m_tabControl->setPosition(UVector2(UDim(0.03f, 0.0f), UDim(0.06f, 0.0f)));
	m_tabControl->setSize(UVector2(UDim(0.95f, 0.0f), UDim(0.9f, 0.0f)));
	m_rootWindow->addChildWindow(m_tabControl);
	
	GuiDebugTab *guiTab = static_cast<GuiDebugTab*>(m_winManager->createWindow("GuiDebugTab", "GuiDebugTab"));
	guiTab->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	guiTab->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	addTabWindow("GuiDebugTab", guiTab);

	LuaScriptTab *luaTab = static_cast<LuaScriptTab*>(m_winManager->createWindow("LuaScriptTab", "LuaScriptTab"));
	luaTab->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	luaTab->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	addTabWindow("LuaScriptTab", luaTab);
}

void DebugPanel::addTabWindow(std::string name, DebugTab* tab)
{
	if(!tabExists(name))
	{
		m_tabs[name] = tab;
		m_tabControl->addTab(dynamic_cast<CEGUI::Window*>(tab));
	}
}

bool DebugPanel::tabExists(std::string tabName)
{
	if(m_tabs.find(tabName) != m_tabs.end())
		return true;

	return false;
}


void DebugPanel::update()
{
	if(m_tabs.size() < 1)
		return;
	
	std::map<std::string, DebugTab*>::iterator iter;
	for (iter = m_tabs.begin(); iter != m_tabs.end(); iter++)
	{
		DebugTab *dt = iter->second;
		dt->update();
	}

}


DebugPanel* DebugPanel::getSingletonPtr(void)
{
	return ms_Singleton;
}

DebugPanel& DebugPanel::getSingleton(void)
{
	assert( ms_Singleton );
	return ( *ms_Singleton );
}


