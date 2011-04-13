#ifndef ContentEditor_H
#define ContentEditor_H

#include <OgreSingleton.h>
#include "CEGUI/CEGUI.h"
#include "contenttab.h"
#include "OISKeyboard.h"
#include "debug.h"

#include <map>

class ContentEditor : public Ogre::Singleton<ContentEditor>
{
public:
	/**
	* \fn void init(bool visible);
	* \brief Initialises the panel
	* \param visible Whether the panel is visible or not at the begining
	*/
	void init(bool visible);
	
	
	/**
	* \fn void toggleVisibility();
	* \brief Toggle the visibility
	*/
	void toggleVisibility();

	
	/**
	* \fn void update();
	* \brief Updates all registered tabs
	*/
	void update(OIS::Keyboard *keyboard);

	/**
	* \fn void addTabWindow(CEGUI::Window *tab);
	* \brief Adds a tab ContentTab window to the ContentEditor
	* \param tab The ContentTab to add
	*/
	void addTabWindow(std::string name, ContentTab *tab);

	/**
	* \fn void tabExists(std::string tabName);
	* \brief Checks if a ContentTab exists
	* \param tabName Name of the ContentTab to check
	*/
	bool tabExists(std::string tabName);
	
	static ContentEditor& getSingleton(void);
	static ContentEditor* getSingletonPtr(void);


protected:
	/**
	* \fn createPanel(bool visible);
	* \brief Creates the panel
	* \param visible Whether the panel is visible or not at the begining
	*/
	void createPanel(bool visible);
	
	/**
	* \fn handleCloseWindow(const CEGUI::EventArgs& e);
	* \brief Handles clicks on the "Close Window" button
	* \param e Event Args from CEGUI
	*/
	virtual bool handleCloseWindow(const CEGUI::EventArgs& e);
	
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
	* \var 	std::map<std::string, ContentTab*>;
	* \brief Holds all registered ContentTabs
	*/
	std::map<std::string, ContentTab*> m_tabs;
    CEGUI::TabControl* m_tabControl;
	
	/**
	* \var 	float m_lastVisibilitySwitch;
	* \brief Buffers the last visibility toggle time
	*/
	long m_lastVisibilitySwitch;
	
};

#endif // ContentEditor_H
