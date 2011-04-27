#include "guiTabs.h"

#include "Ogre.h"
#include "OgrePlatformInformation.h"
#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"

#include "Poco/Environment.h"

#include "CEGUIWindowManager.h"
#include "CEGUI.h"

#include <eventsystem.h>
#include <contenteditor.h>

#include "contenttab.h"
#include "debug.h"

using namespace CEGUI;
using Poco::Environment;

CEGUI::String GuiTabs::WidgetTypeName = "GuiTabs";

GuiTabs::GuiTabs(const CEGUI::String& type, const CEGUI::String& name): CEGUI::Window(type, name), ContentTab(), Ogre::LogListener()
{
	//m_tabLayout->getChild("windowUnderMouseStaticText")->setText(win->getName());
	m_tabLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout("ContentEditor.layout");
	m_tabLayout->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	m_tabLayout->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	this->addChildWindow(m_tabLayout);

	WindowManager::getSingleton().getWindow("RITab/BasicMesh")->setVisible(true);

	initMeshSelector();

	//m_rootWindow->getChild("CloseButton")->subscribeEvent(PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiTabs::onCloseButton, this));
	//WindowManager::getSingleton().getWindow("RITab/SubMesh/AddSubMeshButton")->subscribeEvent(PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiTabs::onAddSubMesh, this));
}

void GuiTabs::initMeshSelector()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	//get the mainmesh selector combobox and fill it with the names of all meshes
	Combobox* selector = static_cast<Combobox*>(WindowManager::getSingleton().getWindow("RITab/BM/MeshSelector"));
	
	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;
	std::string file;

	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("General","*.mesh");
	
	std::list<std::string> filenames;
	for (it = files->begin(); it != files->end(); ++it)
	{
		filenames.push_back(it->filename);
	}
	filenames.sort();
	
	for (std::list<std::string>::iterator it = filenames.begin(); it != filenames.end(); ++it)
	{
		try
		{
			file = *it;
			m_listItem = new ListboxTextItem(file);
			selector->addItem(m_listItem);
		}
		catch (Ogre::Exception& e)
		{
			DEBUG("failed with exception %s",e.what());
		}
	}
	
	selector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&GuiTabs::onMeshSelected, this));
	
	
	// create SceneManager for renderering images for the content editor
	Ogre::SceneManager* editor_scene_mng = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC,"EditorSceneManager");
	editor_scene_mng->setAmbientLight(Ogre::ColourValue(1,1,1));
	
	// camera filming the content of the the editor scene
	Ogre::Camera* editor_camera = editor_scene_mng->createCamera("editor_camera");
	editor_camera->setPosition(Ogre::Vector3(3, 1.2,0));
	editor_camera->lookAt(Ogre::Vector3(0,1.2,0));
	editor_camera->setNearClipDistance(0.1);
	editor_camera->setAspectRatio(1.0);
	Ogre::SceneNode *editorNode = editor_scene_mng->getRootSceneNode()->createChildSceneNode("EditorCameraNode");
	editorNode->attachObject(editor_camera);
	
	// texture that is creates from the camera image
	Ogre::TexturePtr editor_texture = Ogre::TextureManager::getSingleton().createManual( "editor_tex",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
   512, 512, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET );

   // connection texture and camera via RenderTarget
   Ogre::RenderTarget* editor_rt = editor_texture->getBuffer()->getRenderTarget();
	editor_rt ->setAutoUpdated(false);
	Ogre::Viewport *editor_view = editor_rt->addViewport(editor_camera );
	editor_view->setClearEveryFrame( true );
	editor_view->setOverlaysEnabled (false);
	editor_view->setBackgroundColour(Ogre::ColourValue(0,0,0,1.0) );
	editor_rt->update();

	// create a CEGUI Image from the Texture
    CEGUI::Texture& editor_ceguiTex = static_cast<CEGUI::OgreRenderer*>(CEGUI::System::getSingleton().getRenderer())->createTexture(editor_texture);
    
	CEGUI::Imageset& editor_textureImageSet = CEGUI::ImagesetManager::getSingleton().create("editor_imageset", editor_ceguiTex);

	editor_textureImageSet.defineImage( "editor_img",
			CEGUI::Point( 0.0f, 0.0f ),
			CEGUI::Size( editor_ceguiTex.getSize().d_width, editor_ceguiTex.getSize().d_height ),
			CEGUI::Point( 0.0f, 0.0f ) );
	
	// place the image in a the CEGUI label
	CEGUI::Window* label = win_mgr.getWindow("RITab/BM/meshPreview");
	label->setProperty("Image", "set:editor_imageset image:editor_img"); 
}

bool GuiTabs::onMeshSelected(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
	static_cast<const CEGUI::MouseEventArgs&>(evt);
	
	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*>(we.window);
	
	CEGUI::ListboxItem* item = cbo->getSelectedItem();
	
	if (item != 0)
	{
		// place the required mesh in the editor scene
		std::string meshname = item->getText().c_str();
		DEBUG("selected mesh %s",meshname.c_str());
		Ogre::SceneManager* editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
		
		// radical solutions: clear the scene
		editor_scene_mng->clearScene();	
		
		Ogre::Entity* entity;
		try
		{
			entity = editor_scene_mng->createEntity(meshname, meshname);
			Ogre::SceneNode *entityNode = editor_scene_mng->getRootSceneNode()->createChildSceneNode("EditorEntityNode");
			entityNode->setPosition(Ogre::Vector3(0,0,0));
			entityNode->attachObject(entity);
			
			// update the texture
			Ogre::Resource* res= Ogre::TextureManager::getSingleton().createOrRetrieve ("editor_tex",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first.getPointer();
			Ogre::Texture* texture = dynamic_cast<Ogre::Texture*>(res);
			Ogre::RenderTarget* target = texture->getBuffer()->getRenderTarget();
			target->update();
		}
		catch (Ogre::Exception& e)
		{
			WARNING("can't create mesh %s", meshname.c_str());
		}
	}
	
	return true;
}

void GuiTabs::onCloseButton(const CEGUI::EventArgs& evt)
{
	m_rootWindow->setVisible(false);
}

void GuiTabs::onAddSubMesh(const CEGUI::EventArgs& evt)
{
	//get SubMesh info, get optional Bone Info, add Submesh to mainmesh
	//add a line for this bone with a button "kill me"
}

void GuiTabs::addTabWindow(std::string name, ContentTab* tab)
{
	//most likely obsolete
	if(!tabExists(name))
	{
		m_tabs[name] = tab;
		m_tabControl->addTab(dynamic_cast<CEGUI::Window*>(tab));
	}
}

bool GuiTabs::tabExists(std::string tabName)
{
	//most likely obsolete
	if(m_tabs.find(tabName) != m_tabs.end())
		return true;

	return false;
}

void GuiTabs::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName)
{
	//obsolete
}


void GuiTabs::update()
{
}
