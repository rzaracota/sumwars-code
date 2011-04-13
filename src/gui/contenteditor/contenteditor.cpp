#include "contenteditor.h"
#include "renderinfotab.h"
#include "OgreRoot.h"
#include "debug.h"

using namespace CEGUI;

template<> ContentEditor* Ogre::Singleton<ContentEditor>::ms_Singleton = 0;

void ContentEditor::init(bool visible)
{
	m_guiSystem = System::getSingletonPtr();
	m_winManager = WindowManager::getSingletonPtr();
	m_gameScreen = m_winManager->getWindow("GameScreen");
	m_lastVisibilitySwitch = 0;
	createPanel(visible);
}

void ContentEditor::toggleVisibility()
{
	if((Ogre::Root::getSingleton().getTimer()->getMilliseconds() - m_lastVisibilitySwitch) > 500)
	{
		m_rootWindow->setVisible(!m_rootWindow->isVisible());
		m_lastVisibilitySwitch = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	}
}


void ContentEditor::createPanel(bool visible)
{
	m_rootWindow = m_winManager->createWindow("TaharezLook/FrameWindow", "ContentEditor");
	m_rootWindow->setPosition(UVector2(UDim(0.1f, 0.0f), UDim(0.1f, 0.0f)));
	m_rootWindow->setSize(UVector2(UDim(0.6f, 0.0f), UDim(0.6f, 0.0f)));
	m_rootWindow->setText((CEGUI::utf8*)"Debug Panel");
	m_gameScreen->addChildWindow(m_rootWindow);

	m_rootWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, Event::Subscriber(&ContentEditor::handleCloseWindow, this));
	
	m_tabControl = static_cast<TabControl*>(m_winManager->createWindow("TaharezLook/TabControl", "ContentEditorTabControl"));
	m_tabControl->setPosition(UVector2(UDim(0.03f, 0.0f), UDim(0.06f, 0.0f)));
	m_tabControl->setSize(UVector2(UDim(0.95f, 0.0f), UDim(0.9f, 0.0f)));
	m_rootWindow->addChildWindow(m_tabControl);
	
	RenderInfoTab *renderInfoTab = static_cast<RenderInfoTab*>(m_winManager->createWindow("RenderInfoTab", "RenderInfoTab"));
	renderInfoTab->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	renderInfoTab->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	addTabWindow("RenderInfoTab", renderInfoTab);
	
	if(!visible)
	  m_rootWindow->setVisible(visible);
}

void ContentEditor::addTabWindow(std::string name, ContentTab* tab)
{
	if(!tabExists(name))
	{
		m_tabs[name] = tab;
		m_tabControl->addTab(dynamic_cast<CEGUI::Window*>(tab));
	}
}

bool ContentEditor::tabExists(std::string tabName)
{
	if(m_tabs.find(tabName) != m_tabs.end())
		return true;

	return false;
}


void ContentEditor::update(OIS::Keyboard *keyboard)
{
	if(m_tabs.size() < 1)
		return;
	
	if(keyboard->isKeyDown(OIS::KC_LCONTROL) && keyboard->isKeyDown(OIS::KC_K))
		toggleVisibility();

	std::map<std::string, ContentTab*>::iterator iter;
	for (iter = m_tabs.begin(); iter != m_tabs.end(); iter++)
	{
		ContentTab *dt = iter->second;
		dt->update();
	}
}


bool ContentEditor::handleCloseWindow(const CEGUI::EventArgs& e)
{
	m_rootWindow->hide();
	return true;
}


ContentEditor* ContentEditor::getSingletonPtr(void)
{
	return ms_Singleton;
}

ContentEditor& ContentEditor::getSingleton(void)
{
	assert( ms_Singleton );
	return ( *ms_Singleton );
}


