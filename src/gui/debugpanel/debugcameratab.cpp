#include "debugcameratab.h"
#include "CEGUI/CEGUI.h"
#include "OgreRoot.h"
#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"
#include <OgreHardwarePixelBuffer.h>
#include <OISInputManager.h>
#include <OISMouse.h>

using namespace CEGUI;

CEGUI::String DebugCameraTab::WidgetTypeName = "DebugCamera";

DebugCameraTab::DebugCameraTab ( const CEGUI::String& type, const CEGUI::String& name ) : CEGUI::Window ( type, name ), DebugTab()
{
	setText ( "Camera" );

	CEGUI::TabControl* camtabs = ( CEGUI::TabControl* ) WindowManager::getSingleton().createWindow ( "TaharezLook/TabControl", "DebugCameraTabTabs" );
	camtabs->setPosition ( UVector2 ( UDim ( 0.0f, 0.0f ), UDim ( 0.0f, 0.0f ) ) );
	camtabs->setSize ( UVector2 ( UDim ( 1.0f, 0.0f ), UDim ( 1.0f, 0.0f ) ) );
	this->addChildWindow ( camtabs );

	CEGUI::Window* playerCamTab = ( CEGUI::DefaultWindow* ) WindowManager::getSingleton().createWindow ( "TaharezLook/TabContentPane", "DebugCameraTab/PlayerCamOptions" );
	playerCamTab->setText ( "PlayerCam Options" );
	CEGUI::Combobox* cb = ( CEGUI::Combobox* ) WindowManager::getSingleton().createWindow ( "TaharezLook/Combobox", "DebugCameraTab/PlayerCamOptions/ViewModes" );
	cb->addItem ( new ListboxTextItem ( "Solid" ) );
	cb->addItem ( new ListboxTextItem ( "Wireframe" ) );
	cb->addItem ( new ListboxTextItem ( "Point" ) );
	cb->setPosition ( UVector2 ( UDim ( 0.05f, 0.0f ), UDim ( 0.05f, 0.0f ) ) );
	cb->setSize ( UVector2 ( UDim ( 0.4f, 0.0f ), UDim ( 1.0f, 0.0f ) ) );
	cb->subscribeEvent ( CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber ( &DebugCameraTab::onPlayerCameraModeChanged, this ) );
	playerCamTab->addChildWindow ( cb );
	camtabs->addTab ( playerCamTab );

	setupCamera();

	CEGUI::Window* debugCamTab = ( CEGUI::DefaultWindow* ) WindowManager::getSingleton().createWindow ( "TaharezLook/TabContentPane", "DebugCameraTab/DebugCamViewTab" );
	debugCamTab->setText ( "DebugCam" );
	camtabs->addTab ( debugCamTab );
	CEGUI::Window *dcv = WindowManager::getSingleton().createWindow ( "TaharezLook/StaticImage", "DebugCameraTab/DebugCamView" );
	dcv->setPosition ( UVector2 ( UDim ( 0.05f, 0.0f ), UDim ( 0.05f, 0.0f ) ) );
	dcv->setSize ( UVector2 ( UDim ( 0.9f, 0.0f ), UDim ( 0.9f, 0.0f ) ) );
	dcv->setProperty ( "Image", "set:debug_imageset image:debugCam_img" );
	dcv->subscribeEvent ( CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber ( &DebugCameraTab::onDebugWindowMouseDown, this ) );
	dcv->subscribeEvent ( CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber ( &DebugCameraTab::onDebugWindowMouseUp, this ) );
	dcv->subscribeEvent ( CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber ( &DebugCameraTab::onDebugWindowMouseUp, this ) );
	debugCamTab->addChildWindow ( dcv );

	m_leftMouseDown = m_rightMouseDown = false;
}

void DebugCameraTab::initialiseComponents ( void )
{
	CEGUI::Window::initialiseComponents();
}

bool DebugCameraTab::onPlayerCameraModeChanged ( const CEGUI::EventArgs& evt )
{
	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*> ( CEGUI::WindowManager::getSingleton().getWindow ( "DebugCameraTab/PlayerCamOptions/ViewModes" ) );
	CEGUI::String txt =  cbo->getSelectedItem()->getText();
	Ogre::Camera *cam = Ogre::Root::getSingleton().getSceneManager ( "DefaultSceneManager" )->getCamera ( "camera" );


	if ( txt == "Solid" )
		cam->setPolygonMode ( Ogre::PM_SOLID );
	else if ( txt == "Wireframe" )
		cam->setPolygonMode ( Ogre::PM_WIREFRAME );
	else
		cam->setPolygonMode ( Ogre::PM_POINTS );

	return true;
}


bool DebugCameraTab::onDebugWindowMouseDown ( const CEGUI::EventArgs& evt )
{
	const CEGUI::MouseEventArgs* args = static_cast<const CEGUI::MouseEventArgs*> ( &evt );
	if ( args->button == CEGUI::LeftButton )
		m_leftMouseDown = true;
	if ( args->button == CEGUI::RightButton )
		m_rightMouseDown = true;

	return true;
}

bool DebugCameraTab::onDebugWindowMouseUp ( const CEGUI::EventArgs& evt )
{
	const CEGUI::MouseEventArgs* args = static_cast<const CEGUI::MouseEventArgs*> ( &evt );
	if ( args->button == CEGUI::LeftButton )
		m_leftMouseDown = false;
	if ( args->button == CEGUI::RightButton )
		m_rightMouseDown = false;

	return true;
}


void DebugCameraTab::setupCamera()
{
	Ogre::SceneManager *s_mgr = Ogre::Root::getSingleton().getSceneManager ( "DefaultSceneManager" );

	Ogre::Camera* debugCam = s_mgr->createCamera ( "DebugCamera" );
	debugCam->setNearClipDistance ( 0.1f );
	debugCam->setFarClipDistance ( 5000.0f );

	// Create the camera's top node (which will only handle position).
	this->cameraNode = s_mgr->getRootSceneNode()->createChildSceneNode();
	//this->cameraNode->setPosition(40, 5, 80);
	// Create the camera's yaw node as a child of camera's top node.
	this->cameraYawNode = this->cameraNode->createChildSceneNode();
	// Create the camera's pitch node as a child of camera's yaw node.
	this->cameraPitchNode = this->cameraYawNode->createChildSceneNode();
	// Create the camera's roll node as a child of camera's pitch node
	// and attach the camera to it.
	this->cameraRollNode = this->cameraPitchNode->createChildSceneNode();
	this->cameraRollNode->attachObject ( debugCam );


	// texture that is creates from the camera image
	Ogre::TexturePtr debug_texture = Ogre::TextureManager::getSingleton().createManual ( "debug_tex",
	                                 Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
	                                 512, 512, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET );

	// connection texture and camera via RenderTarget
	Ogre::RenderTarget* editor_rt = debug_texture->getBuffer()->getRenderTarget();
	editor_rt->setAutoUpdated ( true );
	Ogre::Viewport *editor_view = editor_rt->addViewport ( debugCam );
	editor_view->setClearEveryFrame ( true );
	editor_view->setOverlaysEnabled ( false );
	editor_view->setBackgroundColour ( Ogre::ColourValue ( 0,0,0,1.0 ) );
	editor_rt->update();

	// create a CEGUI Image from the Texture
	CEGUI::Texture& debug_ceguiTex = static_cast<CEGUI::OgreRenderer*> ( CEGUI::System::getSingleton().getRenderer() )->createTexture ( debug_texture );

	CEGUI::Imageset& editor_textureImageSet = CEGUI::ImagesetManager::getSingleton().create ( "debug_imageset", debug_ceguiTex );

	editor_textureImageSet.defineImage ( "debugCam_img",
	                                     CEGUI::Point ( 0.0f, 0.0f ),
	                                     CEGUI::Size ( debug_ceguiTex.getSize().d_width, debug_ceguiTex.getSize().d_height ),
	                                     CEGUI::Point ( 0.0f, 0.0f ) );
}


void DebugCameraTab::update ( OIS::Keyboard *keyboard, OIS::Mouse *mouse )
{
	Ogre::Camera *debugCam = Ogre::Root::getSingleton().getSceneManager ( "DefaultSceneManager" )->getCamera ( "DebugCamera" );

	Ogre::Radian rotX = Ogre::Radian();
	Ogre::Radian rotY = Ogre::Radian();
	Ogre::Vector3 vec = Ogre::Vector3();

	if ( m_leftMouseDown )
	{
		rotX = Ogre::Radian ( mouse->getMouseState().X.rel * 0.05f );
		rotY = Ogre::Radian ( mouse->getMouseState().Y.rel * 0.05f );
	}
	else if ( m_rightMouseDown )
		vec = Ogre::Vector3 ( 0.0f, 0.0f, mouse->getMouseState().Y.rel * 5 );

	if ( m_leftMouseDown || m_rightMouseDown )
	{
		Ogre::Real pitchAngle;
		Ogre::Real pitchAngleSign;
		// Yaws the camera according to the mouse relative movement.
		this->cameraYawNode->yaw ( rotX );
		// Pitches the camera according to the mouse relative movement.
		this->cameraPitchNode->pitch ( rotY );
		// Translates the camera according to the translate vector which is
		// controlled by the keyboard arrows.
		//
		// NOTE: We multiply the mTranslateVector by the cameraPitchNode's
		// orientation quaternion and the cameraYawNode's orientation
		// quaternion to translate the camera accoding to the camera's
		// orientation around the Y-axis and the X-axis.
		Ogre::Vector3 trvec = this->cameraYawNode->getOrientation() * this->cameraPitchNode->getOrientation() * vec;
		this->cameraNode->translate ( trvec, Ogre::SceneNode::TS_LOCAL );
		// Angle of rotation around the X-axis.
		pitchAngle = ( 2 * Ogre::Degree ( Ogre::Math::ACos ( this->cameraPitchNode->getOrientation().w ) ).valueDegrees() );
		// Just to determine the sign of the angle we pick up above, the
		// value itself does not interest us.
		pitchAngleSign = this->cameraPitchNode->getOrientation().x;
		// Limit the pitch between -90 degress and +90 degrees, Quake3-style.
		if ( pitchAngle > 90.0f )
		{
			if ( pitchAngleSign > 0 )
				// Set orientation to 90 degrees on X-axis.
				this->cameraPitchNode->setOrientation ( Ogre::Quaternion ( Ogre::Math::Sqrt ( 0.5f ),
				                                        Ogre::Math::Sqrt ( 0.5f ), 0, 0 ) );
			else if ( pitchAngleSign < 0 )
				// Sets orientation to -90 degrees on X-axis.
				this->cameraPitchNode->setOrientation ( Ogre::Quaternion ( Ogre::Math::Sqrt ( 0.5f ),
				                                        -Ogre::Math::Sqrt ( 0.5f ), 0, 0 ) );
		}
	}

	Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName ( "debug_tex" );
	tex.getPointer()->getBuffer()->getRenderTarget()->update();
}


