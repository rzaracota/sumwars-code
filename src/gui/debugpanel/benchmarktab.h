#ifndef BENCHMARKTAB_H
#define BENCHMARKTAB_H

#include <debugtab.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreLog.h"

namespace Ogre
{
	class Root;
	class SceneManager;
	class Camera;
	class RenderTexture;
}

class BenchmarkTab : public CEGUI::Window, public DebugTab, public Ogre::LogListener
{
public:
	BenchmarkTab(const CEGUI::String& type, const CEGUI::String& name);
	
	virtual void update();

	virtual void messageLogged (const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName);

	virtual bool handleStartBenchmark(const CEGUI::EventArgs& e);
private:
	CEGUI::Window *m_tabLayout;
	CEGUI::MultiLineEditbox *m_CapsBox;
	CEGUI::PushButton *m_startBenchmarkButton;
	Ogre::Log *m_log;
	Ogre::Root *m_ogreRoot;
	Ogre::SceneManager *m_sceneManager;
public:
	static CEGUI::String WidgetTypeName;
};

#endif // BENCHMARKTAB_H
