#include "contenttab.h"

#include "Ogre.h"
#include "OgrePlatformInformation.h"

#include "Poco/Environment.h"

#include "CEGUIWindowManager.h"
#include "CEGUI.h"

#include <eventsystem.h>
#include <renderinfotab.h>

using namespace CEGUI;
using Poco::Environment;

CEGUI::String RenderInfoTab::WidgetTypeName = "RenderInfoTab";

RenderInfoTab::RenderInfoTab(const CEGUI::String& type, const CEGUI::String& name): CEGUI::Window(type, name), ContentTab(), Ogre::LogListener()
{
	setText("RenderInfoTab");

	m_tabLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout("RenderInfoTab.layout");
	m_tabLayout->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	m_tabLayout->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	this->addChildWindow(m_tabLayout);
	
	//get the mainmesh selector combobox and fill it with the names of all meshes
	Combobox* selector = static_cast<Combobox*>(m_tabLayout->getChild("RenderInfo/selectMainMesh"));
	
	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;
	std::string file;

	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("General","*.mesh");
	for (it = files->begin(); it != files->end(); ++it)
	{
		try
		{
			file = it->filename;
			m_listItem = new ListboxTextItem(file);
			selector->addItem(m_listItem);
		}
		catch (Ogre::Exception& e)
		{
			DEBUG("failed with exception %s",e.what());
		}
	}
}

bool RenderInfoTab::handleStartBenchmark(const CEGUI::EventArgs& e)
{
	return true;
}

void RenderInfoTab::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName)
{
}

void RenderInfoTab::update()
{
}
void RenderInfoTab::initialiseComponents()
{

}
