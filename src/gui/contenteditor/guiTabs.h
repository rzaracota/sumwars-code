#ifndef GUITABS_H
#define GUITABS_H

#include <contenttab.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreLog.h"

namespace Ogre
{
	class Root;
	class SceneManager;
	class Camera;
	class RenderTexture;
}

class GuiTabs : public CEGUI::Window, public ContentTab, public Ogre::LogListener
{
public:
	GuiTabs(const CEGUI::String& type, const CEGUI::String& name);
	
	virtual void update();

	virtual void messageLogged (const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName);

	/**
	 * \fn void addTabWindow(CEGUI::Window *tab);
	 * \brief Adds a tab ContentTab window to the ContentEditor
	 * \param tab The ContentTab to add
	 */
	void addTabWindow(std::string name, ContentTab *tab);
	
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
	 * \fn void tabExists(std::string tabName);
	 * \brief Checks if a ContentTab exists
	 * \param tabName Name of the ContentTab to check
	 */
	bool tabExists(std::string tabName);
private:
	CEGUI::ListboxTextItem *m_listItem;
	CEGUI::ListboxTextItem *m_subMeshListItem;
	CEGUI::Window *m_tabLayout;
	CEGUI::MultiLineEditbox *m_CapsBox;
	CEGUI::PushButton *m_startBenchmarkButton;
	Ogre::Log *m_log;
	Ogre::Root *m_ogreRoot;
	Ogre::SceneManager *m_sceneManager;
	/**
	 * \var 	CEGUI::Window *m_rootWindow;
	 * \brief  Pointer to the root window of this panel
	 */
	CEGUI::Window *m_rootWindow;
	/**
	 * \var 	std::map<std::string, ContentTab*>;
	 * \brief Holds all registered ContentTabs
	 */
	std::map<std::string, ContentTab*> m_tabs;
    CEGUI::TabControl* m_tabControl;
	/**
	 * \var 	CEGUI::WindowManager *m_winManager;
	 * \brief  Pointer to the CEGUI::WindowManager
	 */
	CEGUI::WindowManager *m_winManager;

	void initMeshSelector();
	bool onCloseButton(const CEGUI::EventArgs& evt);
	void onAddSubMesh(const CEGUI::EventArgs& evt);
public:
	static CEGUI::String WidgetTypeName;
};

#endif // GUITABS_H
