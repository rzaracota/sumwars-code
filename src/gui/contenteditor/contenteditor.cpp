#include "contenteditor.h"
#include "guiTabs.h"
#include "OgreRoot.h"
#include "debug.h"

#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"
#include <OgreHardwarePixelBuffer.h>


#include "graphicmanager.h"

using namespace CEGUI;

template<> ContentEditor* Ogre::Singleton<ContentEditor>::ms_Singleton = 0;

void ContentEditor::init(bool visible)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	m_guiSystem = System::getSingletonPtr();
	m_winManager = WindowManager::getSingletonPtr();
	m_gameScreen = m_winManager->getWindow("GameScreen");
	m_lastVisibilitySwitch = 0;
	
	m_rootWindow = m_winManager->createWindow("TaharezLook/FrameWindow", "ContentEditor");
	m_rootWindow->setPosition(UVector2(UDim(0.025f, 0.0f), UDim(0.025f, 0.0f)));
	m_rootWindow->setSize(UVector2(UDim(0.9f, 0.0f), UDim(0.85f, 0.0f)));
	m_rootWindow->setText((CEGUI::utf8*)"Content Editor");
	m_gameScreen->addChildWindow(m_rootWindow);
	
	CEGUI::Window* rootWindow;
	rootWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("ContentEditor.layout");
	rootWindow->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	rootWindow->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	m_rootWindow->addChildWindow(rootWindow);
	
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
	
	// add parts to the mesh selectors
	Combobox* selector = static_cast<Combobox*>(WindowManager::getSingleton().getWindow("RITab/BM/MeshSelector"));
	Combobox* subSelector = static_cast<Combobox*>(WindowManager::getSingleton().getWindow("RITab/SubMesh/Selector"));
	
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
	
	CEGUI::ListboxTextItem* listitem;
	for (std::list<std::string>::iterator it = filenames.begin(); it != filenames.end(); ++it)
	{
		try
		{
			file = *it;
			listitem = new ListboxTextItem(file);
			selector->addItem(listitem);
			listitem = new ListboxTextItem(file);
			subSelector->addItem(listitem);
		}
		catch (Ogre::Exception& e)
		{
			DEBUG("failed with exception %s",e.what());
		}
	}
	
	selector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&ContentEditor::onMeshSelected, this));
	subSelector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshSelected, this));
	
	if(!visible)
		m_rootWindow->setVisible(visible);
	
	TiXmlElement * renderinfo_root = new TiXmlElement("RenderInfo");  
	m_renderinfo_xml.LinkEndChild( renderinfo_root );  
	renderinfo_root->SetAttribute("name","editor_RI");
}

void ContentEditor::toggleVisibility()
{
	if((Ogre::Root::getSingleton().getTimer()->getMilliseconds() - m_lastVisibilitySwitch) > 500)
	{
		m_rootWindow->setVisible(!m_rootWindow->isVisible());
		m_lastVisibilitySwitch = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	}
}


void ContentEditor::update(OIS::Keyboard *keyboard)
{
	
	if(keyboard->isKeyDown(OIS::KC_LCONTROL) && keyboard->isKeyDown(OIS::KC_K))
		toggleVisibility();
	
	if (m_modified_renderinfo)
	{
		updateRenderInfoXML();
		updatePreviewImage();
	}
	
	m_modified_renderinfo = false;
}

void ContentEditor::updatePreviewImage()
{
	Ogre::SceneManager* old_scene_mng = GraphicManager::getSceneManager();
	Ogre::SceneManager* editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
	GraphicManager::setSceneManager(editor_scene_mng);
	
	// radical solutions: clear the scene
	editor_scene_mng->clearScene();
	
	
	GraphicManager::setSceneManager(old_scene_mng);
}

void ContentEditor::updateRenderInfoXML()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// update the XML representation
	m_edited_renderinfo.writeToXML(m_renderinfo_xml.FirstChildElement());
	
	// write to the editor
	TiXmlPrinter printer;
	m_renderinfo_xml.Accept(&printer);
	
	CEGUI::MultiLineEditbox* editor = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.getWindow("RITab/XML/RIXMLEditbox"));
	editor->setText(printer.CStr());
}

bool ContentEditor::handleCloseWindow(const CEGUI::EventArgs& e)
{
	m_rootWindow->hide();
	return true;
}

void ContentEditor::updateTranslation()
{
	
}

bool ContentEditor::onMeshSelected(const CEGUI::EventArgs& evt)
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
		
		// if the Renderinfo already has a main mesh, edit it
		// otherwise, create it
		MovableObjectInfo* minfo = m_edited_renderinfo.getObject("mainmesh");
		MovableObjectInfo newminfo;
		bool addnew = false;
		
		if (minfo == 0)
		{
			// no mainmesh yet
			minfo = &newminfo;
			addnew = true;
		}
		
		// edit the Object
		minfo->m_type = MovableObjectInfo::ENTITY;
		minfo->m_objectname = "mainmesh";
		minfo->m_source = meshname;
		
		if (addnew)
		{
			m_edited_renderinfo.addObject(*minfo);
		}
		
		m_modified_renderinfo = true;
	}
	
	return true;
}


bool ContentEditor::onSubMeshSelected(const CEGUI::EventArgs& evt)
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
		
		// if the Renderinfo already has a main mesh, edit it
		// otherwise, create it
		MovableObjectInfo* minfo = m_edited_renderinfo.getObject("mainmesh");
		MovableObjectInfo newminfo;
		bool addnew = false;
		
		if (minfo == 0)
		{
			// no mainmesh yet
			minfo = &newminfo;
			addnew = true;
		}
		
		// edit the Object
		minfo->m_type = MovableObjectInfo::ENTITY;
		minfo->m_objectname = "mainmesh";
		minfo->m_source = meshname;
		
		if (addnew)
		{
			m_edited_renderinfo.addObject(*minfo);
		}
		
		m_modified_renderinfo = true;
	}
	
	return true;
}

ContentEditor* ContentEditor::getSingletonPtr(void)
{
	return ms_Singleton;
}

ContentEditor& ContentEditor::getSingleton(void)
{
	assert( ms_Singleton );
	return ( *ms_Singleton );
}


