#ifndef DEBUGPANEL_H
#define DEBUGPANEL_H

#include <OgreSingleton.h>
#include "CEGUI.h"
#include "debugtab.h"

#include <map>

class DebugPanel : public Ogre::Singleton<DebugPanel>
{
public:
	/**
	* \fn void init(bool visible);
	* \brief Initialises the panel
	* \param visible Whether the panel is visible or not at the begining
	*/
	void init(bool visible);
	
	/**
	* \fn void update();
	* \brief Updates all registered tabs
	*/
	void update();

	/**
	* \fn void addTabWindow(CEGUI::Window *tab);
	* \brief Adds a tab DebugTab window to the DebugPanel
	* \param tab The DebugTab to add
	*/
	void addTabWindow(std::string name, DebugTab *tab);

	/**
	* \fn void tabExists(std::string tabName);
	* \brief Checks if a DebugTab exists
	* \param tabName Name of the DebugTab to check
	*/
	bool tabExists(std::string tabName);
	
	static DebugPanel& getSingleton(void);
	static DebugPanel* getSingletonPtr(void);


protected:
	/**
	* \fn createPanel(bool visible);
	* \brief Creates the panel
	* \param visible Whether the panel is visible or not at the begining
	*/
	void createPanel(bool visible);
	
private:
	/**
	* \var 	CEGUI::System *m_guiSystem;
	* \brief  Pointer to the CEGUI::System
	*/
	CEGUI::System *m_guiSystem;
	
	/**
	* \var 	CEGUI::WindowManager *m_winManager;
	* \brief  Pointer to the CEGUI::WindowManager
	*/
	CEGUI::WindowManager *m_winManager;
	
	/**
	* \var 	CEGUI::Window *m_rootWindow;
	* \brief  Pointer to the root window of this panel
	*/
	CEGUI::Window *m_rootWindow;

	
	/**
	* \var 	CEGUI::Window* m_gameScreen;
	* \brief  Pointer to the root gamescreen
	*/
	CEGUI::Window* m_gameScreen;

	/**
	* \var 	std::map<std::string, DebugTab*>;
	* \brief Holds all registered DebugTabs
	*/
	std::map<std::string, DebugTab*> m_tabs;
    CEGUI::TabControl* m_tabControl;
	
	
};

#endif // DEBUGPANEL_H
