/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Utility for CEGUI cross-version compatibility
#include "ceguiutility.h"

#include "debugcameratab.h"
#include "CEGUI/CEGUI.h"
#include "OgreRoot.h"

#ifdef CEGUI_07
#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"
#else
#include "CEGUI/RendererModules/Ogre/Renderer.h"
#endif

#include <OgreHardwarePixelBuffer.h>
#include <OISInputManager.h>
#include <OISMouse.h>

using namespace CEGUI;

CEGUI::String DebugCameraTab::WidgetTypeName = "DebugCamera";

DebugCameraTab::DebugCameraTab ( const CEGUI::String& type, const CEGUI::String& name ) : CEGUI::Window ( type, name ), DebugTab()
{
	setText ( "Camera" );

	CEGUI::TabControl* camtabs = static_cast<CEGUI::TabControl*>(WindowManager::getSingleton().createWindow(
      "TaharezLook/TabControl", "DebugCameraTabTabs" ));

	camtabs->setPosition ( UVector2 ( UDim ( 0.0f, 0.0f ), UDim ( 0.0f, 0.0f ) ) );
  CEGUIUtility::setWidgetSizeRel (camtabs, 1.0f, 1.0f);
  CEGUIUtility::addChildWidget (this, camtabs);

	CEGUI::Window* playerCamTab = static_cast<CEGUI::DefaultWindow*>(WindowManager::getSingleton().createWindow(
      "TaharezLook/TabContentPane", "PlayerCamOptions"));

	playerCamTab->setText ( "PlayerCam Options" );
	CEGUI::Combobox* cb = ( CEGUI::Combobox* ) WindowManager::getSingleton().createWindow ( "TaharezLook/Combobox", "ViewModes" );
	cb->addItem ( new ListboxTextItem ( "Solid" ) );
	cb->addItem ( new ListboxTextItem ( "Wireframe" ) );
	cb->addItem ( new ListboxTextItem ( "Point" ) );
	cb->setPosition ( UVector2 ( UDim ( 0.05f, 0.0f ), UDim ( 0.05f, 0.0f ) ) );
  CEGUIUtility::setWidgetSizeRel (cb, 0.4f, 1.0f);
	cb->subscribeEvent ( CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber ( &DebugCameraTab::onPlayerCameraModeChanged, this ) );
  CEGUIUtility::addChildWidget (playerCamTab, cb);

	camtabs->addTab ( playerCamTab );

	setupCamera();

	CEGUI::Window* debugCamTab = ( CEGUI::DefaultWindow* ) WindowManager::getSingleton().createWindow ( "TaharezLook/TabContentPane", "DebugCamViewTab" );
	debugCamTab->setText ( "DebugCam" );
	camtabs->addTab ( debugCamTab );
	CEGUI::Window *dcv = WindowManager::getSingleton().createWindow ( "TaharezLook/StaticImage", "DebugCamView" );
	dcv->setPosition ( UVector2 ( UDim ( 0.05f, 0.0f ), UDim ( 0.05f, 0.0f ) ) );
  CEGUIUtility::setWidgetSizeRel (dcv, 0.9f, 0.9f);

  CEGUI::String debug_camera_image_name("debugCam_img");
	dcv->setProperty ("Image", debug_camera_image_name); // prev: "set:debug_imageset image:debugCam_img"
	dcv->subscribeEvent ( CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber ( &DebugCameraTab::onDebugWindowMouseDown, this ) );
	dcv->subscribeEvent ( CEGUIUtility::EventMouseLeavesWindowArea(), CEGUI::Event::Subscriber ( &DebugCameraTab::onDebugWindowMouseUp, this ) );
	dcv->subscribeEvent ( CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber ( &DebugCameraTab::onDebugWindowMouseUp, this ) );
  CEGUIUtility::addChildWidget (debugCamTab, dcv);

	m_leftMouseDown = m_rightMouseDown = false;
}

void DebugCameraTab::initialiseComponents ( void )
{
	CEGUI::Window::initialiseComponents();
}

bool DebugCameraTab::onPlayerCameraModeChanged ( const CEGUI::EventArgs& evt )
{
	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*> (
    CEGUIUtility::getWindowForLoadedLayout(this, "DebugCameraTabTabs/PlayerCamOptions/ViewModes"));
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

	// get the OgreRenderer from CEGUI and create a CEGUI texture from the Ogre texture
	CEGUI::OgreRenderer* renderer_ptr = static_cast<CEGUI::OgreRenderer*>(CEGUI::System::getSingleton().getRenderer());
  CEGUI::String imageName("debugCam_img");

	// create a CEGUI Image from the Texture
#ifdef CEGUI_07
	CEGUI::Texture& debug_cegui_texture = renderer_ptr->createTexture (debug_texture);

	CEGUI::Imageset& editor_textureImageSet = CEGUI::ImagesetManager::getSingleton().create ( "debug_imageset", debug_cegui_texture );

	editor_textureImageSet.defineImage(
      imageName,
	    CEGUI::Point ( 0.0f, 0.0f ),
	    CEGUI::Size ( debug_cegui_texture.getSize().d_width, debug_cegui_texture.getSize().d_height ),
	    CEGUI::Point ( 0.0f, 0.0f ) );
#else
	CEGUI::Texture &debug_cegui_texture = renderer_ptr->createTexture(debug_texture->getName(), debug_texture);
	{
		
		CEGUI::TextureTarget*   d_textureTarget;
		CEGUI::BasicImage*      d_textureTargetImage;
		d_textureTarget = renderer_ptr->createTextureTarget();
		d_textureTargetImage = static_cast<CEGUI::BasicImage*>(&CEGUI::ImageManager::getSingleton().create("BasicImage", imageName));
		d_textureTargetImage->setTexture(&debug_cegui_texture);

		d_textureTargetImage->setArea(
        CEGUI::Rectf(
          0, 
          0, 
          debug_cegui_texture.getSize().d_width, 
          debug_cegui_texture.getSize ().d_height));
	}

#endif
}


void DebugCameraTab::update ( OIS::Keyboard *keyboard, OIS::Mouse *mouse )
{
	Ogre::Camera *debugCam = Ogre::Root::getSingleton().getSceneManager ( "DefaultSceneManager" )->getCamera ( "DebugCamera" );

	Ogre::Radian rotX = Ogre::Radian();
	Ogre::Radian rotY = Ogre::Radian();
	Ogre::Vector3 vec = Ogre::Vector3();

	if ( m_leftMouseDown )
	{
		rotX = Ogre::Radian ( mouse->getMouseState().X.rel * 0.005f );
		rotY = Ogre::Radian ( mouse->getMouseState().Y.rel * 0.005f );
	}
	else if ( m_rightMouseDown )
		vec = Ogre::Vector3 ( 0.0f, 0.0f, mouse->getMouseState().Y.rel * 5 );

	if ( m_leftMouseDown || m_rightMouseDown )
	{
		debugCam->yaw(rotX);
		debugCam->pitch(rotY);

		Ogre::Vector3 trvec = debugCam->getOrientation() * vec;
		debugCam->move(trvec);
	}

#if (OGRE_VERSION < ((1 << 16) | (9 << 8) | 0))
    Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName ( "debug_tex" );
#else
    Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName ( "debug_tex" ).staticCast<Ogre::Texture>();
#endif

    tex.getPointer()->getBuffer()->getRenderTarget()->update();

}


