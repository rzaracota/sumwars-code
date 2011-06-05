#ifndef ContentEditor_H
#define ContentEditor_H

#include <OgreSingleton.h>
#include "CEGUI/CEGUI.h"
#include "contenttab.h"
#include "OISKeyboard.h"
#include "debug.h"

#include <map>

#include "graphicobjectrenderinfo.h"
#include "graphicobject.h"

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



protected:

	/**
	 * \param evt CEGUI event arguments
	 * \brief Called upon selecting a mesh from the combobox
	 * Updates the content of the editor scene to display the selected mesh.
	 */
	bool onMeshSelected(const CEGUI::EventArgs& evt);
	
	/**
	 * \param evt CEGUI event arguments
	 * \brief Called upon selecting a mesh from the submesh combobox
	 */
	bool onSubMeshSelected(const CEGUI::EventArgs& evt);
	
	/**
	 * \param evt CEGUI event arguments
	 * \brief Called clicking add Submesh button
	 */
	bool onSubMeshAdded(const CEGUI::EventArgs& evt);
	
	/**
	 * \param evt CEGUI event arguments
	 * \brief Called upon selecting a subobject from the subobject combobox
	 */
	bool onSubObjectSelected(const CEGUI::EventArgs& evt);
	
	/**
	 * \param evt CEGUI event arguments
	 * \brief Called upon changing any element in the submesh tab
	 */
	bool onSubMeshModified(const CEGUI::EventArgs& evt);
	
	/**
	 * \param evt CEGUI event arguments
	 * \brief Called upon clicking on the delete Submesh button
	 */
	bool onSubMeshDeleted(const CEGUI::EventArgs& evt);
	
	/**
	 * \param evt CEGUI event arguments
	 * \brief Called submitting new RenderinfoXML
	 */
	bool onRenderinfoXMLModified(const CEGUI::EventArgs& evt);
	
	/**
	 * \brief updates the content of the preview image and the GUI Elements
	 */
	void updateRenderInfoGUI();
	
	/**
	 * \brief updates the submesh editor component
	 * \param objectname name of the Object to be edited
	 * \param updateList if set to true, the list of objects will be updated, too
	 */
	void updateSubmeshEditor(std::string objectname="", bool updateList = true);
	
	/**
	 * \brief updates the list of bones to attach to
	 * \param minfo data structure with information on the mesh
	 */
	void updateBoneList();
	
	/**
	 * \brief updates the content of the Renderinfo XML editor
	 */
	void updateRenderInfoXML();
	
	/**
	 * \brief updates the preview Image
	 */
	void updatePreviewImage();
	
	/**
	 * \brief Checks for all attached Objects, whether their parent mesh and bone are still present
	 * Dangling submeshes are deleted
	 */ 
	void checkAttachedObjects();
	
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
	
	/**
	 * \brief marks if the renderinfo has been modified
	 */
	bool m_modified_renderinfo;
	
	/**
	 * \brief marks that the XML of the renderinfo was modified
	 */
	bool m_modified_renderinfo_xml;
	
	/**
	 * \brief Renderinfo edited with this window
	 */
	GraphicRenderInfo m_edited_renderinfo;
	
	/**
	 * \brief XML representation of the  Renderinfo edited with this window
	 */
	TiXmlDocument m_renderinfo_xml;  
	
	/**
	 * \brief GraphicObject of the Renderinfo edited with this window
	 */
	GraphicObject* m_edited_graphicobject;
	
	/**
	 * \brief while set to true, all CEGUI events will be ignored
	 * This allows setting some GUI elements in event handlers without getting inifinite loops due.
	 */
	bool m_no_cegui_events;
};

#endif // ContentEditor_H
