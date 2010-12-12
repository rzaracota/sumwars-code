#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <OgreSingleton.h>
#include "CEGUI/CEGUI.h"
#include "debugtab.h"
#include "OISKeyboard.h"

#include <map>


class FileBrowser
{
public:
	~FileBrowser();

	/**
	* \fn void init(CEGUI::String defaultDir, bool visible);
	* \brief Initialises the panel
	* \brief dir The directory that will be opened when initialzing
	* \param visible Whether the panel is visible or not at the begining
	*/
	void init(CEGUI::String defaultDir, bool visible);
	
	/**
	* \fn handleCloseWindow(const CEGUI::EventArgs& e);
	* \brief Handles clicks on the "Close Window" button
	* \param e Event Args from CEGUI
	*/
	virtual bool handleCloseWindow(const CEGUI::EventArgs& e);

	/**
	* \fn handleBrowserDblClick(const CEGUI::EventArgs& e);
	* \brief Handles Doubleclicks on items in the browser
	* \param e Event Args from CEGUI
	*/
	virtual bool handleBrowserDblClick(const CEGUI::EventArgs& e);

protected:
	/**
	* \fn void fillBrowser(CEGUI::String dir);
	* \brief Fills the browser window
	* \brief dir The directory that will be opened when initialzing
	*/
	void fillBrowser(CEGUI::String dir);

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
	* \var 	CEGUI::Window* m_gameScreen;
	* \brief  Pointer to the root gamescreen
	*/
	CEGUI::Window* m_gameScreen;

	/**
	* \var 	CEGUI::Window *m_rootWindow;
	* \brief  Pointer to the root window of this panel
	*/
	CEGUI::Window *m_rootWindow;	

	/**
	* \var 	CEGUI::Editbox* m_pathBox;
	* \brief  Pointer to the Editbox that shows the current path
	*/
	CEGUI::Editbox* m_pathBox;

	/**
	* \var 	CEGUI::ItemListbox* m_browserBox;
	* \brief  Pointer to the Editbox that shows the current path
	*/
	CEGUI::ItemListbox* m_browserBox;

};

#endif // DEBUGPANEL_H
