#include "minimapwindow.h"

#include "OgreCEGUIRenderer.h"
#include "OgreCEGUITexture.h"
#include "OgreCEGUIResourceProvider.h"
#include "scene.h"

MinimapWindow::MinimapWindow (Document* doc)
	:Window(doc)
{
	m_region_id = -1;
	
	m_shift = Vector(0,0);
	
	DEBUG4("setup main menu");
	// GUI Elemente erzeugen

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	

	// Rahmen fuer das Menue Savegame auswaehlen
	
	
	Ogre::SceneManager* scene_manager = Ogre::Root::getSingleton().getSceneManager("DefaultSceneManager");
	
	m_minimap_camera=scene_manager->createCamera("minimap_camera");
	m_minimap_camera->setNearClipDistance(500);
	m_minimap_camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	m_minimap_camera->setFOVy(Ogre::Degree(90.0));
	
	m_minimap_texture = Ogre::TextureManager::getSingleton().createManual( "minimap_tex", 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 
   512, 512, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET );
	
	m_minimap_rt = m_minimap_texture->getBuffer()->getRenderTarget();
	m_minimap_rt ->setAutoUpdated(false);
	
	Ogre::Viewport *v = m_minimap_rt->addViewport( m_minimap_camera );
	v->setClearEveryFrame( true );
	v->setOverlaysEnabled (false);
	v->setBackgroundColour(Ogre::ColourValue(0,0,0,0) );
	float ratio = Ogre::Real(v->getActualWidth()) / Ogre::Real(v->getActualHeight());
	DEBUG5("render target size %i %i",m_minimap_rt ->getWidth (), m_minimap_rt ->getHeight ());
	DEBUG5("viewport size %i %i ratio %f",v->getActualWidth(),v->getActualHeight(), ratio);
	m_minimap_camera->setAspectRatio(ratio);
	
	CEGUI::OgreCEGUITexture* ceguiTex = (CEGUI::OgreCEGUITexture*)((CEGUI::OgreCEGUIRenderer*)CEGUI::System::getSingleton().getRenderer())->createTexture((CEGUI::utf8*)"minimap_tex");
	
	CEGUI::Imageset* textureImageSet = CEGUI::ImagesetManager::getSingleton().createImageset("minimap", ceguiTex);
	
	textureImageSet->defineImage( "minimap_img", 
						CEGUI::Point( 0.0f, 0.0f ), 
						CEGUI::Size( ceguiTex->getWidth(), ceguiTex->getHeight() ), 
						CEGUI::Point( 0.0f, 0.0f ) ); 
	
	DEBUG5("size %i %i",ceguiTex->getWidth(), ceguiTex->getHeight() );
	
	CEGUI::FrameWindow* minimap = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "MinimapWindow");
	m_window = minimap;
	
	minimap->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f))); //0.0/0.8
	minimap->setSize(CEGUI::UVector2(cegui_reldim(0.9f), cegui_reldim( 0.8f))); //1.0/0.2
	minimap->setProperty("FrameEnabled","false");
	minimap->setProperty("TitlebarEnabled","false");
	minimap->setProperty("CloseButtonEnabled","false");
 	minimap->setAlpha (0.0);
	
	CEGUI::Window* label;
	label = win_mgr.createWindow("TaharezLook/StaticImage", "MinimapImage");
	minimap->addChildWindow(label);
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.00f), cegui_reldim( 0.00)));
	label->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
	//label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05)));
	//label->setSize(CEGUI::UVector2(cegui_reldim(0.9f), cegui_reldim( 0.9f)));
	label->setMousePassThroughEnabled(true);
	label->setProperty("Image", "set:minimap image:minimap_img"); 
	label->setInheritsAlpha (false);
	label->setAlpha(1.0);
	
	/*
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::Technique *technique = material->createTechnique();
	technique->createPass();
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	material->getTechnique(0)->getPass(0)->createTextureUnitState("minimap_tex");
	
	Ogre::Overlay* overlay = Ogre::OverlayManager::getSingleton().create("minimapOverlay");
	Ogre::OverlayContainer* container = (Ogre::OverlayContainer*) Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "minimapContainer");
	overlay->add2D(container);
	container->setMaterialName("RttMat");
	container->setDimensions(0.8, 0.8);
	//textArea->setMetricsMode(GMM_PIXELS);
	container->setPosition(0.1, 0.1);
	overlay->show();
	*/
}

void MinimapWindow::update()
{
	
	Player* player = m_document->getLocalPlayer();

	if (player ==0)
		return;
	if (player->getRegion() ==0)
		return;
	
	short rid = player->getRegion()->getId();
	if (rid != m_region_id)
	{
		short dimx = player->getRegion()->getDimX();
		short dimy = player->getRegion()->getDimY();
		
		m_region_id = rid;
		
		Ogre::SceneManager* scene_manager = Ogre::Root::getSingleton().getSceneManager("DefaultSceneManager");
		
		// Koordinaten des Spielers
		Vector pos = player->getShape()->m_center;
		
		DEBUG("camera pos %i %i %i",dimx*100,std::max(dimx,dimy)*300,dimy*100);
		m_minimap_camera->setPosition(Ogre::Vector3(dimx*100,std::max(dimx,dimy)*200,10+dimy*100));
		m_minimap_camera->lookAt(Ogre::Vector3(dimx*100,0,dimy*100));
		m_minimap_camera->setNearClipDistance(std::max(dimx,dimy)*100);
		
		//m_minimap_camera->setPosition(Ogre::Vector3(6600,1000,11400));
		//m_minimap_camera->lookAt(Ogre::Vector3(6600,0,11380));
		//Ogre::Vector3 up = m_minimap_camera->getUp();
		//m_minimap_camera->setFrustumExtents (0,dimx*200,0,dimy*200);
		//DEBUG("camera up %f %f %f",up.x, up.y, up.z);
		
		
		
		scene_manager->setAmbientLight(Ogre::ColourValue(1.0,1.0,1.0));
		m_minimap_rt->update();
		scene_manager->setAmbientLight(Ogre::ColourValue(0.4,0.4,0.4));
	}
	
}



