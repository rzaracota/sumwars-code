#ifndef ContentEditor_H
#define ContentEditor_H

#include <OgreSingleton.h>
#include "CEGUI/CEGUI.h"
#include "contenteditortab.h"
#include "OISKeyboard.h"
#include "debug.h"

#include <map>

#include "graphicobjectrenderinfo.h"
#include "graphicobject.h"
#include "fixedbase.h"

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
	
	
	static ContentEditor& getSingleton(void);
	static ContentEditor* getSingletonPtr(void);

	/**
	 * \brief Returns the Tab with the supplied name
	 * \param name Name of the editor tab
	 */
	ContentEditorTab* getComponent(std::string name);
	

protected:
	
	/**
	 * \param evt CEGUI event arguments
	 * \brief Called upon click on close button
	 */
	bool onClose(const CEGUI::EventArgs& evt);


	

	
	/**
	 * \brief updates the Translation of the labels
	 */
	virtual void updateTranslation();
	
	/**
	* \fn handleCloseWindow(const CEGUI::EventArgs& e);
	* \brief Handles clicks on the "Close Window" button
	* \param e Event Args from CEGUI
	*/
	virtual bool handleCloseWindow(const CEGUI::EventArgs& e);
	
	
private:

	/**
	* \var 	CEGUI::Window *m_rootWindow;
	* \brief  Pointer to the root window of this panel
	*/
	CEGUI::Window *m_rootWindow;
	
	/**
	* \var 	float m_lastVisibilitySwitch;
	* \brief Buffers the last visibility toggle time
	*/
	long m_lastVisibilitySwitch;
		
	/**
	 * \brief data structure with all editor components
	 */
	std::map<std::string, ContentEditorTab*> m_components;
};

#endif // ContentEditor_H
