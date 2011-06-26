#include "contenteditor.h"
#include "OgreRoot.h"
#include "debug.h"

#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"
#include <OgreHardwarePixelBuffer.h>
#include <OgreMeshManager.h>
#include <OISMouse.h>

#include "graphicmanager.h"
#include "worldobject.h"
#include "objectloader.h"
#include "world.h"

#include "renderinfoeditor.h"
#include "fixedobjecteditor.h"
#include "itemeditor.h"

using namespace CEGUI;

template<> ContentEditor* Ogre::Singleton<ContentEditor>::ms_Singleton = 0;

void ContentEditor::init(bool visible)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::System* guiSystem = System::getSingletonPtr();
	CEGUI::WindowManager* winManager = WindowManager::getSingletonPtr();
	CEGUI::Window* gameScreen = winManager->getWindow("GameScreen");
	m_lastVisibilitySwitch = 0;
	
	m_rootWindow = winManager->createWindow("TaharezLook/FrameWindow", "ContentEditor");
	m_rootWindow->setPosition(UVector2(UDim(0.025f, 0.0f), UDim(0.025f, 0.0f)));
	m_rootWindow->setSize(UVector2(UDim(0.9f, 0.0f), UDim(0.85f, 0.0f)));
	m_rootWindow->setText((CEGUI::utf8*)"Content Editor");
	gameScreen->addChildWindow(m_rootWindow);
	
	CEGUI::Window* rootWindow;
	rootWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("ContentEditor.layout");
	rootWindow->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	rootWindow->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	m_rootWindow->addChildWindow(rootWindow);
	
	// initialisation of the selected tabs for more intuitive use
	static_cast<CEGUI::TabControl*>(win_mgr.getWindow("ObjectInfoTabControl"))->setSelectedTab("RenderInfoTab");
	static_cast<CEGUI::TabControl*>(win_mgr.getWindow("RenderInfoTabControl"))->setSelectedTab("RITab/BasicMesh");
	static_cast<CEGUI::TabControl*>(win_mgr.getWindow("FixedObjectTabControl"))->setSelectedTab("FOTab/Properties");

	// create SceneManager for renderering images for the content editor
	Ogre::SceneManager* editor_scene_mng = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC,"EditorSceneManager");
	editor_scene_mng->setAmbientLight(Ogre::ColourValue(1,1,1));
	
	// camera filming the content of the the editor scene
	Ogre::Camera* editor_camera = editor_scene_mng->createCamera("editor_camera");
	editor_camera->setNearClipDistance(0.1);
	editor_camera->setAspectRatio(1.0);
	Ogre::SceneNode *mainEditorNode = editor_scene_mng->getRootSceneNode()->createChildSceneNode("MainEditorCameraNode");
	Ogre::SceneNode *editorNode = mainEditorNode->createChildSceneNode("EditorCameraNode");
	Ogre::SceneNode *editorCamPitchNode = editorNode->createChildSceneNode("EditorCamerapitchNode");
	editorCamPitchNode->attachObject(editor_camera);
	mainEditorNode->setFixedYawAxis(true);
	editorNode->setFixedYawAxis(true);
	editorCamPitchNode->setFixedYawAxis(true);
	
	
	// texture that is creates from the camera image
	Ogre::TexturePtr editor_texture = Ogre::TextureManager::getSingleton().createManual( "editor_tex",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
   512, 512, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET );
	
   // connection texture and camera via RenderTarget
	Ogre::RenderTarget* editor_rt = editor_texture->getBuffer()->getRenderTarget();
	editor_rt->setAutoUpdated(false);
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
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&ContentEditor::onPreviewWindowMouseDown, this));
	label->subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&ContentEditor::onPreviewWindowMouseUp, this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&ContentEditor::onPreviewWindowMouseUp, this));
	label->subscribeEvent(CEGUI::Window::EventMouseWheel, CEGUI::Event::Subscriber(&ContentEditor::onPreviewWindowScrollWheel, this));
	
	CEGUI::PushButton* closebutton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("CloseButton"));
	closebutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ContentEditor::onClose, this));
	
	closebutton = static_cast<CEGUI::FrameWindow*>(m_rootWindow)->getCloseButton();
	closebutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ContentEditor::onClose, this));
	
	m_components["FOEditor"] = new FixedObjectEditor();
	m_components["RIEditor"] = new RenderInfoEditor();
	m_components["ItemEditor"] = new ItemEditor();
	
	std::map<std::string, ContentEditorTab*>::iterator it;
	for (it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->init(m_rootWindow);
	}
	
	if(!visible)
		m_rootWindow->setVisible(visible);
}

void ContentEditor::toggleVisibility()
{
	if((Ogre::Root::getSingleton().getTimer()->getMilliseconds() - m_lastVisibilitySwitch) > 500)
	{
		m_rootWindow->setVisible(!m_rootWindow->isVisible());
		m_lastVisibilitySwitch = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	}
}


void ContentEditor::update(OIS::Keyboard *keyboard, OIS::Mouse *mouse)
{
	if(m_leftMouseDown)
	{
		Ogre::SceneManager *editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
		Ogre::Radian rotX(mouse->getMouseState().X.rel * 0.05f);
		Ogre::Radian rotY(mouse->getMouseState().Y.rel * 0.05f);
		
		Ogre::SceneNode *editorNode = editor_scene_mng->getSceneNode("EditorCameraNode");
		Ogre::SceneNode *editorCamPitchNode = editor_scene_mng->getSceneNode("EditorCamerapitchNode");
		editorNode->yaw(-rotX);
		editorCamPitchNode->roll(rotY);
	}
	else if(m_rightMouseDown)
	{
		Ogre::SceneManager *editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
		Ogre::Camera *cam = editor_scene_mng->getCamera("editor_camera");
		
		Ogre::Vector3 vec = Ogre::Vector3(0.0f, 0.0f, mouse->getMouseState().Y.rel * 0.1f);
		cam->moveRelative(vec);
	}
	
	if(keyboard->isKeyDown(OIS::KC_LCONTROL) && keyboard->isKeyDown(OIS::KC_K))
		toggleVisibility();
	
	std::map<std::string, ContentEditorTab*>::iterator it;
	for (it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->update();
	}
}

void ContentEditor::fullUpdateComponent(std::string name)
{
	if (m_components.count(name) == 0)
		return;
	
	m_components[name]->setUpdateBaseContent();
}

bool ContentEditor::handleCloseWindow(const CEGUI::EventArgs& e)
{
	m_rootWindow->hide();
	return true;
}

void ContentEditor::updateTranslation()
{
	
}

bool ContentEditor::onPreviewWindowMouseDown(const CEGUI::EventArgs& evt)
{
	Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName("editor_tex");
	tex.getPointer()->getBuffer()->getRenderTarget()->setAutoUpdated(true);
	
	const CEGUI::MouseEventArgs* args = static_cast<const CEGUI::MouseEventArgs*>(&evt);
	if(args->button == CEGUI::LeftButton)
		m_leftMouseDown = true;
	if(args->button == CEGUI::RightButton)
		m_rightMouseDown = true;

	return true;
}

bool ContentEditor::onPreviewWindowMouseUp(const CEGUI::EventArgs& evt)
{
	Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName("editor_tex");
	tex.getPointer()->getBuffer()->getRenderTarget()->setAutoUpdated(false);
	
	const CEGUI::MouseEventArgs* args = static_cast<const CEGUI::MouseEventArgs*>(&evt);
	if(args->button == CEGUI::LeftButton)
		m_leftMouseDown = false;
	if(args->button == CEGUI::RightButton)
		m_rightMouseDown = false;

	return true;
}

bool ContentEditor::onPreviewWindowScrollWheel(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs* args = static_cast<const CEGUI::MouseEventArgs*>(&evt);
	
	Ogre::SceneManager *editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
	Ogre::Camera *cam = editor_scene_mng->getCamera("editor_camera");

	Ogre::Vector3 vec = Ogre::Vector3(0.0f, 0.0f, args->wheelChange * 0.02f);
	cam->moveRelative(vec);
	Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName("editor_tex");
	tex.getPointer()->getBuffer()->getRenderTarget()->update();

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

ContentEditorTab* ContentEditor::getComponent(std::string name)
{
	if (m_components.count(name) == 0)
		return 0;
	
	return m_components[name];
}

bool ContentEditor::onClose(const CEGUI::EventArgs& evt)
{
	m_rootWindow->setVisible(!m_rootWindow->isVisible());
	m_lastVisibilitySwitch = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	return true;
}

