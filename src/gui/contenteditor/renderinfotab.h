#ifndef RenderInfoTab_H
#define RenderInfoTab_H

#include <contenttab.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreLog.h"

namespace Ogre
{
	class Root;
	class SceneManager;
	class Camera;
	class RenderTexture;
	class Entity;
}

class RenderInfoTab : public CEGUI::Window, public ContentTab, public Ogre::LogListener
{
public:
	RenderInfoTab(const CEGUI::String& type, const CEGUI::String& name);
	virtual void update();
	virtual void initialiseComponents(void );

	virtual void messageLogged (const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName);

	virtual bool handleStartBenchmark(const CEGUI::EventArgs& e);
	
	/**
	 * \param evt CEGUI event arguments
	 * \brief Called upon selecting a mesh from the combobox
	 * Updates the content of the editor scene to display the selected mesh.
	 */
	bool onMeshSelected(const CEGUI::EventArgs& evt);
	
private:
	CEGUI::Window *m_tabLayout;
	CEGUI::ListboxTextItem *m_listItem;
	CEGUI::PushButton *m_startBenchmarkButton;
	Ogre::Log *m_log;
	Ogre::MeshManager *m_meshManager;
	Ogre::Root *m_ogreRoot;
	Ogre::SceneManager *m_sceneManager;
public:
	static CEGUI::String WidgetTypeName;
};

#endif // RenderInfoTab_H
