#include "OgreEntity.h"
#include "mainmenu.h"
#include <OgreRenderWindow.h>
#include <OgreSubEntity.h>
#include <OgreMeshManager.h>
#include <OgreParticleSystem.h>
#include "listitem.h"
#include "savegamelist.h"
#include "messageboxes.h"
#include "graphicmanager.h"
#include "scene.h"
#include "version.h"

#include <iostream>


MainMenu::MainMenu (Document* doc)
        :Window(doc)
{
	m_savegame_player ="";
	m_savegame_player_object =0;
	
	m_savegame_player_action.m_type = "noaction";
	m_savegame_player_action.m_time = 2000;
	m_savegame_player_action.m_elapsed_time = 0;
	
	
    CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Window* label;


    CEGUI::FrameWindow* start_menu_root = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "StartMenuRoot");
    m_window = start_menu_root;
    m_window->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f))); //0.0/0.8
    m_window->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f))); //1.0/0.20
    m_window->setAlpha(0);
    m_window->setProperty("FrameEnabled","false");
    m_window->setProperty("TitlebarEnabled","false");
    m_window->setProperty("CloseButtonEnabled","false");
    m_window->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&MainMenu::onShown, this));
    m_window->subscribeEvent(CEGUI::Window::EventHidden, CEGUI::Event::Subscriber(&MainMenu::onHidden, this));


    CEGUI::FrameWindow* start_menu = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "StartMenu");
    //m_window = start_menu;
    m_window->addChildWindow(start_menu);
    start_menu->setInheritsAlpha(false);

    start_menu->setAlpha(0.0f);
    //start_menu->setRotation(CEGUI::Vector3(0,15,0));

    start_menu->setPosition(CEGUI::UVector2(cegui_reldim(0.65f), cegui_reldim( 0.1f))); //0.0/0.8
    start_menu->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.5f))); //1.0/0.2
    start_menu->setProperty("FrameEnabled","false");
    start_menu->setProperty("TitlebarEnabled","false");
    start_menu->setProperty("CloseButtonEnabled","false");

	CEGUI::String btnType("TaharezLook/ImageButton");

    // Button Einzelspieler
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow(btnType, "SinglePlayerButton"));
    start_menu->addChildWindow(btn);
    btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.05f)));
    btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.10f)));
    btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onStartSinglePlayer, this));
    btn->setWantsMultiClickEvents(false);
    btn->setInheritsAlpha(false);
	btn->setAlpha(0.7f);
	btn->setProperty("NormalImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("DisabledImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("HoverImage", "set:MainMenu image:SPBtnHover");
	btn->setProperty("PushedImage", "set:MainMenu image:SPBtnPushed");
	label = win_mgr.createWindow("TaharezLook/StaticText", "SinglePlayerLabel");
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f )));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.95f), cegui_reldim( 0.95f)));
	label->setProperty("HorzFormatting", "HorzCentred");
	label->setMousePassThroughEnabled(true);
	btn->addChildWindow(label);



	// Button Server beitreten
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow(btnType, "ServerJoinButton"));
    start_menu->addChildWindow(btn);
    btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.20f)));
    btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.10f)));
    btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onStartMultiPlayer, this));
    btn->setWantsMultiClickEvents(false);
    btn->setInheritsAlpha(false);
    btn->setAlpha(0.7f);
	btn->setProperty("NormalImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("DisabledImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("HoverImage", "set:MainMenu image:SPBtnHover");
	btn->setProperty("PushedImage", "set:MainMenu image:SPBtnPushed");
	label = win_mgr.createWindow("TaharezLook/StaticText", "ServerJoinLabel");
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f )));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.95f), cegui_reldim( 0.95f)));
	label->setProperty("HorzFormatting", "HorzCentred");
	label->setMousePassThroughEnabled(true);
	btn->addChildWindow(label);


    // Button Server aufsetzen
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow(btnType, "ServerHostButton"));
    start_menu->addChildWindow(btn);
    btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.35f)));
    btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.10f)));
    btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onStartMultiPlayerHost, this));
    btn->setWantsMultiClickEvents(false);
    btn->setInheritsAlpha(false);
    btn->setAlpha(0.7f);
	btn->setProperty("NormalImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("DisabledImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("HoverImage", "set:MainMenu image:SPBtnHover");
	btn->setProperty("PushedImage", "set:MainMenu image:SPBtnPushed");
	label = win_mgr.createWindow("TaharezLook/StaticText", "ServerHostLabel");
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f )));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.95f), cegui_reldim( 0.95f)));
	label->setProperty("HorzFormatting", "HorzCentred");
	label->setMousePassThroughEnabled(true);
	btn->addChildWindow(label);

    // Button Credits
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow(btnType, "CreditsButton"));
    start_menu->addChildWindow(btn);
    btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.50f)));
    btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.10f)));
    btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onShowCredits, this));
    btn->setWantsMultiClickEvents(false);
    btn->setInheritsAlpha(false);
    btn->setAlpha(0.7f);
	btn->setProperty("NormalImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("DisabledImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("HoverImage", "set:MainMenu image:SPBtnHover");
	btn->setProperty("PushedImage", "set:MainMenu image:SPBtnPushed");
	label = win_mgr.createWindow("TaharezLook/StaticText", "CreditsLabel");
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f )));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.95f), cegui_reldim( 0.95f)));
	label->setProperty("HorzFormatting", "HorzCentred");
	label->setMousePassThroughEnabled(true);
	btn->addChildWindow(label);

    // Button Optionen
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow(btnType, "MainOptionsButton"));
    start_menu->addChildWindow(btn);
    btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.65f)));
    btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.10f)));
    btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onShowOptions, this));
    btn->setWantsMultiClickEvents(false);
    btn->setInheritsAlpha(false);
    btn->setAlpha(0.7f);
	btn->setProperty("NormalImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("DisabledImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("HoverImage", "set:MainMenu image:SPBtnHover");
	btn->setProperty("PushedImage", "set:MainMenu image:SPBtnPushed");
	label = win_mgr.createWindow("TaharezLook/StaticText", "MainOptionsLabel");
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f )));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.95f), cegui_reldim( 0.95f)));
	label->setProperty("HorzFormatting", "HorzCentred");
	label->setMousePassThroughEnabled(true);
	btn->addChildWindow(label);


    // Button beenden
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow(btnType, "EndGameButton"));
	start_menu->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.85f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.10f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onQuitGameHost, this));
	btn->setWantsMultiClickEvents(false);
	btn->setInheritsAlpha(false);
	btn->setAlpha(0.7f);
	btn->setProperty("NormalImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("DisabledImage", "set:MainMenu image:SPBtnNormal");
	btn->setProperty("HoverImage", "set:MainMenu image:SPBtnHover");
	btn->setProperty("PushedImage", "set:MainMenu image:SPBtnPushed");
	label = win_mgr.createWindow("TaharezLook/StaticText", "EndGameLabel");
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f )));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.95f), cegui_reldim( 0.95f)));
	label->setProperty("HorzFormatting", "HorzCentred");
	label->setMousePassThroughEnabled(true);
	btn->addChildWindow(label);


	label = win_mgr.createWindow("TaharezLook/StaticText", "SumwarsVersionLabel");
	start_menu_root->addChildWindow(label);
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.9f )));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.05f)));
	label->setText(CEGUI::String("Version: ").append(VERSION));
	label->setInheritsAlpha(false);
	label->setAlpha(0.7f);

	Ogre::Root *root = Ogre::Root::getSingletonPtr();
	m_sceneMgr = root->createSceneManager(Ogre::ST_GENERIC, "MainMenuSceneManager");
	m_mainMenuCamera = m_sceneMgr->createCamera("MainMenuCamera");
	m_mainMenuCamera->setNearClipDistance(0.1f);
	m_mainMenuCamera->setFarClipDistance(10000);

    m_sceneCreated = false;

    createSavegameList();

    updateTranslation();
}

void MainMenu::update()
{
	updateCharacterView();
}

void MainMenu::updateTranslation()
{
    //updating translations for the Menu
    CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
    CEGUI::PushButton* btn;

	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("SinglePlayerLabel"));
    btn->setText((CEGUI::utf8*) gettext("Single player"));

	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ServerJoinLabel"));
    btn->setText((CEGUI::utf8*) gettext("Join game"));

	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ServerHostLabel"));
    btn->setText((CEGUI::utf8*) gettext("Host game"));

	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("CreditsLabel"));
    btn->setText((CEGUI::utf8*) gettext("Credits"));

	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("MainOptionsLabel"));
    btn->setText((CEGUI::utf8*) gettext("Options"));

	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("EndGameLabel"));
    btn->setText((CEGUI::utf8*) gettext("Quit"));


}

bool MainMenu::onStartSinglePlayer(const CEGUI::EventArgs& evt)
{
    m_document->onButtonStartSinglePlayer();
    return true;
}

bool MainMenu::onStartMultiPlayer(const CEGUI::EventArgs& evt)
{
    m_document->onButtonJoinGame();
    return true;
}

bool MainMenu::onStartMultiPlayerHost(const CEGUI::EventArgs& evt)
{
    m_document->onButtonHostGame();
    return true;
}

bool MainMenu::onShowCredits(const CEGUI::EventArgs& evt)
{
    m_document->onButtonCredits();
    return true;
}

bool MainMenu::onShown( const CEGUI::EventArgs& evt )
{
	Ogre::Root *root = Ogre::Root::getSingletonPtr();
    if (!m_sceneCreated)
	{
        createScene();
		m_gameCamera = root->getAutoCreatedWindow()->getViewport(0)->getCamera();
	}
	m_saveGameList->update();

	root->getAutoCreatedWindow()->getViewport(0)->setCamera(m_mainMenuCamera);
	m_mainMenuCamera->lookAt(m_mainNode->getPosition());
    root->addFrameListener(this);
	m_mainMenuCamera->lookAt(Ogre::Vector3(-3.0643, -0.00965214, -45.8015));
	
    CEGUI::WindowManager::getSingleton().getWindow("MainMenu")->setAlpha(0);
    return true;
}

bool MainMenu::onHidden( const CEGUI::EventArgs& evt )
{
	Ogre::Root *root = Ogre::Root::getSingletonPtr();
    if (m_sceneCreated)
	{
		root->getAutoCreatedWindow()->getViewport(0)->setCamera(m_gameCamera);
	}
    CEGUI::WindowManager::getSingleton().getWindow("MainMenu")->setAlpha(1);

    root->removeFrameListener(this);
	
    return true;
}


bool MainMenu::onShowOptions(const CEGUI::EventArgs& evt)
{
    m_document->onButtonOptionsClicked();
    return true;
}


bool MainMenu::onQuitGameHost(const CEGUI::EventArgs& evt)
{
    m_document->onButtonSaveExitClicked();
    return true;
}

bool MainMenu::frameStarted(const Ogre::FrameEvent& evt)
{
    
	/*m_mainMenuCamera->setPosition(Ogre::Vector3(0,0,-0.6)+m_mainMenuCamera->getPosition());
	std::stringstream ss;
    ss << "V3: " << m_mainMenuCamera->getPosition().x << " " << m_mainMenuCamera->getPosition().y << " " << m_mainMenuCamera->getPosition().z << " " << std::endl;
	Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, ss.str().c_str());
	*/
	return Ogre::FrameListener::frameStarted(evt);
}


bool MainMenu::frameEnded(const Ogre::FrameEvent& evt)
{
	/*
	if(m_sceneMgr)
		m_sceneMgr->getEntity("MainMenuZombie")->getAnimationState("zombieWalk")->addTime(evt.timeSinceLastFrame);
	*/
    return Ogre::FrameListener::frameEnded(evt);
}

void MainMenu::createScene()
{
    if (!m_sceneCreated)
    {
        //m_mainMenuCamera->setPosition(Ogre::Vector3(-8.0899, 1.03773, -10.4865));
		//m_mainMenuCamera->setPosition(Ogre::Vector3(-15.2126, 0.597, -14.1));
		m_mainMenuCamera->setPosition(Ogre::Vector3(-17.2126, 0.597, -14.1));
        //m_mainMenuCamera->setOrientation(Ogre::Quaternion(0.967044, -0.00100842, -0.254607, -0.000271657));
		m_mainMenuCamera->pitch(Ogre::Radian(Ogre::Degree(15)));
		m_mainMenuCamera->yaw(Ogre::Radian(Ogre::Degree(5)));
       
        //	cam->moveRelative(Ogre::Vector3(0,-5,-20));
		//	cam->lookAt(Ogre::Vector3::ZERO);
		
        m_mainNode = m_sceneMgr->getRootSceneNode()->createChildSceneNode("MainMenuMainNode");
        m_mainNode->setPosition(Ogre::Vector3::ZERO);
		
		Ogre::SceneNode *n = m_mainNode->createChildSceneNode();
		Ogre::Entity *e;
		Ogre::MeshPtr *m;
		Ogre::ParticleSystem *p;
		Ogre::Light *l;

		m_sceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.3, 2));


		l = m_sceneMgr->createLight("mainMen_MoonLight");
		l->setType(Ogre::Light::LT_POINT);
		l->setPosition(Ogre::Vector3(0.929331, 16.9939, -29.9981));
		l->setDirection(0, 0, 1);
		l->setAttenuation(100, 0.2, 0.8, 0);
		l->setCastShadows(true);
		l->setDiffuseColour(Ogre::ColourValue(1, 1, 1, 1));
		l->setSpecularColour(Ogre::ColourValue(0.45098, 0.45098, 0.47451, 1));
		l->setPowerScale(1);

		l = m_sceneMgr->createLight("mainMen_FireLight1");
		l->setType(Ogre::Light::LT_POINT);
		l->setPosition(Ogre::Vector3(-10.5044, 0.121838, -21.5031));
		l->setDirection(0, 0, 1);
		l->setAttenuation(100, 0.5, 0.02, 0.008);
		l->setCastShadows(false);
		l->setDiffuseColour(Ogre::ColourValue(0.443137, 0.215686, 0.145098, 1));
		l->setSpecularColour(Ogre::ColourValue(0.407843, 0.176471, 0.0588235, 1));
		l->setPowerScale(3);

		
		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("long_sw.mesh");
		e->getSubEntity(0)->setMaterialName("Item");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-13.7902, 0.0892202, -24.5334));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.852776, 0.413687, 0.248387, -0.199852));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("gold.mesh");
		e->getSubEntity(0)->setMaterialName("Item");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-10.4538, 0.990349, -26.0507));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("leathArm.mesh");
		e->getSubEntity(0)->setMaterialName("Item");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-12.9917, -0.00965142, -23.8061));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.985556, 0, 0.16935, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("heal_2.mesh");
		e->getSubEntity(0)->setMaterialName("Item");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-11.8726, 0.990349, -25.4361));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("heal_1.mesh");
		e->getSubEntity(0)->setMaterialName("Item");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-12.286, 0.990349, -25.537));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("box_m3.mesh");
		e->getSubEntity(0)->setMaterialName("gimcrack");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-12.0797, -0.00965118, -25.5948));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.998848, 0, 0.0479781, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("box_m2.mesh");
		e->getSubEntity(0)->setMaterialName("gimcrack");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-12.3237, -0.00965214, -28.5043));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.975342, 0, 0.220697, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("box_m2.mesh");
		e->getSubEntity(0)->setMaterialName("gimcrack");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-10.4654, -0.00965118, -26.1655));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.992546, 0, -0.121869, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("buckler.mesh");
		e->getSubEntity(0)->setMaterialName("Item");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-13.3333, 0.356877, -25.1495));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.928377, 0.304607, 0.111193, -0.181566));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("campFire.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-10.4943, -0.00965166, -21.3849));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("tree4.mesh");
		e->getSubEntity(0)->setMaterialName("env_blaetter");
		e->getSubEntity(1)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-1.38935, -0.00965309, -32.0907));
		n->setScale(Ogre::Vector3(2.55586, 2.41194, 2.55586));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("chestBox.mesh");
		e->getSubEntity(0)->setMaterialName("gimcrack");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-15.4144, -0.00964898, -22.8803));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.598323, 4.69282e-006, 0.801255, 2.4053e-006));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stool_konz.mesh");
		e->getSubEntity(0)->setMaterialName("gimcrack");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-8.98199, -0.00965166, -21.4096));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stool_konz.mesh");
		e->getSubEntity(0)->setMaterialName("gimcrack");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-8.39954, -0.00965166, -26.3683));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("chestLid.mesh");
		e->getSubEntity(0)->setMaterialName("gimcrack");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-15.5435, 0.746074, -23.2522));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.374504, -0.624886, 0.501524, -0.466624));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stones3.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-10.5055, -0.00965118, -27.8262));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.743145, 0, -0.669131, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("tree3.mesh");
		e->getSubEntity(0)->setMaterialName("env_blaetter");
		e->getSubEntity(1)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-23.2304, -0.00965118, -34.9225));
		n->setScale(Ogre::Vector3(1.62948, 1.5, 1.62948));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("gobDog.mesh");
		e->getSubEntity(0)->setMaterialName("Monster_Aisen");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-9.9569, -0.00965178, -24.6495));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.354291, 0, -0.935135, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("gold_rng.mesh");
		e->getSubEntity(0)->setMaterialName("Item");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-10.1393, 0.990349, -25.7856));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("steel_arm.mesh");
		e->getSubEntity(0)->setMaterialName("Item");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-11.4281, 0, -23.1021));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.975342, 0, -0.220697, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stone2.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-5.92453, -0.00965118, -29.1621));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stone2.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-7.36273, -0.00965309, -28.5372));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stone2.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-3.0643, -0.00965214, -45.8015));
		n->setScale(Ogre::Vector3(5, 5, 5));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stone2.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-6.39304, -0.00965166, -27.8417));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stone_l1.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-16.307, -0.385546, -31.6074));
		n->setScale(Ogre::Vector3(1, 1, 1.57439));
		n->setOrientation(Ogre::Quaternion(0.963639, -0.000392383, -0.267202, 0.00166609));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stone_l1.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-4.65489, -0.00965118, -26.1386));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.995396, 0, -0.0958458, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stones1.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-14.9308, 0, -31.1942));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.973379, 6.74019e-011, -0.2292, -2.41509e-010));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stones2.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-20.6536, 0, -23.9446));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.973379, 6.74019e-011, -0.2292, -2.41509e-010));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stones3.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-19.3292, 0, -24.7372));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(0.612217, 2.04367e-010, -0.790689, -1.4527e-010));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stones3.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-10.1034, -0.00965118, -31.4112));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("stones3.mesh");
		e->getSubEntity(0)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-6.93122, -0.00965214, -42.3686));
		n->setScale(Ogre::Vector3(6, 6, 6));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("tree2.mesh");
		e->getSubEntity(0)->setMaterialName("env_blaetter");
		e->getSubEntity(1)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-17.1033, -0.00965214, -38.6637));
		n->setScale(Ogre::Vector3(1.30359, 1.2, 1.30359));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("tree4.mesh");
		e->getSubEntity(0)->setMaterialName("env_blaetter");
		e->getSubEntity(1)->setMaterialName("env_waldland");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-7.77576, -0.00965118, -37.878));
		n->setScale(Ogre::Vector3(1.41222, 1.3, 1.41222));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		n = m_mainNode->createChildSceneNode();
		e = m_sceneMgr->createEntity("vase2.mesh");
		e->getSubEntity(0)->setMaterialName("env_konzil");
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-14.3816, -0.00965166, -23.7364));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		p = m_sceneMgr->createParticleSystem("mainMen_Particle#0","CampFire");
		n = m_mainNode->createChildSceneNode();
		n->attachObject(p);
		n->setPosition(Ogre::Vector3(-10.5017, 0.150328, -21.4926));
		n->setScale(Ogre::Vector3(1, 1, 1));
		n->setOrientation(Ogre::Quaternion(1, 0, 0, 0));

		Ogre::Plane plane0(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshManager::getSingletonPtr()->createPlane("mainMen_Plane#0", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane0, 10, 10, 6, 6 ,true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		e = m_sceneMgr->createEntity("mainMen_Plane#0");
		e->setMaterialName("grass1");
		n = m_mainNode->createChildSceneNode();
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-12.0042, -0.00965166, -29.7625));
		n->setScale(Ogre::Vector3(5, 1, 5));
		n->setOrientation(Ogre::Quaternion(0.418659, -1.48815e-010, -0.908144, -2.01801e-010));

		Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshManager::getSingletonPtr()->createPlane("mainMen_Plane#1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1, 10, 10, 6, 6 ,true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
		e = m_sceneMgr->createEntity("mainMen_Plane#1");
		e->setMaterialName("env_nightsky");
		n = m_mainNode->createChildSceneNode();
		n->attachObject(e);
		n->setPosition(Ogre::Vector3(-2.64068, 14.1513, -57.029));
		n->setScale(Ogre::Vector3(5.8, 1, 4.83));
		n->setOrientation(Ogre::Quaternion(0.248194, -0.15685, 0.597531, 0.74616));

		m_sceneCreated = true;
		
	}
    
}

void MainMenu::updateCharacterView()
{
	bool update = false;
	
	Ogre::SceneManager* scene_mgr = Ogre::Root::getSingleton().getSceneManager("MainMenuSceneManager");
	GraphicManager::setSceneManager(scene_mgr);
	
	Player* pl = m_document->getLocalPlayer();

	static Ogre::Timer actiontimer;
	
	if ((pl ==0 && m_savegame_player!=""))
	{
		DEBUGX("deleting inv player");
		m_savegame_player="";
		GraphicManager::destroyGraphicObject(m_savegame_player_object);
		m_savegame_player_object =0;
		update = true;
	}

	if (pl !=0)
	{
		std::string correctname = pl->getNameId();
		correctname += pl->getPlayerLook().m_render_info;
		
		if ((correctname != m_savegame_player))
		{
			DEBUGX("updating inv player %s to %s",m_savegame_player.c_str(), correctname.c_str());
			GraphicManager::destroyGraphicObject(m_savegame_player_object);
			m_savegame_player_object =0;
			update = true;
		}
		
		m_savegame_player = correctname;
		
		if (m_savegame_player_object ==0)
		{
			update = true;
			
			GraphicObject::Type type = pl->getPlayerLook().m_render_info;
			m_savegame_player_object = GraphicManager::createGraphicObject(type,pl->getNameId(), pl->getId());
			
			m_savegame_player_object->getTopNode()->setPosition(Ogre::Vector3(-9.27905, -0.0096519, -22.9244));
			m_savegame_player_object->getTopNode()->setOrientation(Ogre::Quaternion(-0.190809, 0, 0.981627, 0));
			m_savegame_player_object->setExactAnimations(true);
		}
		
		update |= Scene::updatePlayerGraphicObject(m_savegame_player_object,pl);
	}	
	
	if (m_savegame_player_object !=0)
	{
		// set action for the renderer call
		Action actsave = *(pl->getAction());
		
		float frametime = actiontimer.getMicroseconds ()/1000.0;
		m_savegame_player_action.m_elapsed_time += frametime;
		if (m_savegame_player_action.m_elapsed_time > m_savegame_player_action.m_time)
		{
			// determine new action
			if (m_savegame_player_action.m_type != "noaction")
			{
				// never play to actions directly after each other
				m_savegame_player_action.m_type = "noaction";
				m_savegame_player_action.m_time = pl->getActionTime(m_savegame_player_action.m_type);
			}
			else
			{
				m_savegame_player_action.m_type = "noaction";
				
				// get a random action
				if (Random::random() < 0.5)
				{
					std::map<std::string, AbilityInfo>::iterator it;
					std::map<std::string, AbilityInfo>& abl = pl->getBaseAttrMod()->m_abilities;
					int abl_found = 0;
					for (it = abl.begin(); it != abl.end(); ++it)
					{
						if (it->first == "noaction" || it->first == "die" || it->first == "walk" || it->first == "dead" 
							|| it->first == "take_item" || it->first == "use" || it->first == "speak" || it->first == "trade")
							continue;
						
						Action::ActionInfo* aci2 = Action::getActionInfo(it->first);
						if (aci2->m_target_type == Action::PASSIVE)
							continue;
							
						abl_found++;
						
						if (Random::randi(abl_found) == 0)
						{
							m_savegame_player_action.m_type = it->first;
						}
					}
				}
				
				// determine time of the action
				m_savegame_player_action.m_time = pl->getActionTime(m_savegame_player_action.m_type);
				
				// apply attack speed
				Action::ActionType baseact = "noaction";
				Action::ActionInfo* ainfo = Action::getActionInfo(m_savegame_player_action.m_type);
				if (ainfo != 0)
				{
					baseact = ainfo->m_base_action;
				}
				
				if (baseact == "attack" || baseact == "range_attack" || baseact == "holy_attack" || m_savegame_player_action.m_type == "magic_attack")
				{
					float atksp = MathHelper::Min((short) 5000,pl->getBaseAttrMod()->m_attack_speed);
					m_savegame_player_action.m_time *= 1000000/atksp;
				}
				
				// reduce speed (just looks better :P )
				m_savegame_player_action.m_time *= 1.3;
			}
			m_savegame_player_action.m_elapsed_time = 0;
		}
		
		pl->setAction(m_savegame_player_action);
		std::string act = pl->getActionString();
		float perc = m_savegame_player_action.m_elapsed_time / m_savegame_player_action.m_time;
		
		m_savegame_player_object->updateAction(act,perc);
		m_savegame_player_object->update(0);
		
		pl->setAction(actsave);
	}
	actiontimer.reset();
}

void MainMenu::createSavegameList()
{
    CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *mmWin = win_mgr.getWindow("StartMenuRoot");

    // Rahmen fuer das Menue Savegame auswaehlen
    CEGUI::FrameWindow* savegameList = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "SaveGameSelectionFrame");
    mmWin->addChildWindow(savegameList);

    savegameList->setInheritsAlpha(false);
    savegameList->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f))); //0.0/0.8
    savegameList->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.4f))); //1.0/0.2
    savegameList->setProperty("FrameEnabled","false");
    savegameList->setProperty("TitlebarEnabled","false");
    savegameList->setProperty("CloseButtonEnabled","false");
    savegameList->setAlpha(0.0f);
    //savegameList->setRotation(CEGUI::Vector3(0.0f, -15.0f, 0.0f));

    SavegameList* sgl = new SavegameList(m_document);
    m_saveGameList = sgl;
    savegameList->addChildWindow(sgl->getCEGUIWindow());
    sgl->update();
    sgl->updateTranslation();

    MessageQuestionWindow * delchar = new MessageQuestionWindow(m_document,"DeleteChar",gettext("Really delete savegame?"), gettext("Yes"),CEGUI::Event::Subscriber(&SavegameList:: onDeleteCharConfirmClicked, sgl),gettext("No"),CEGUI::Event::Subscriber(&SavegameList::onDeleteCharAbortClicked, sgl));
    m_window->addChildWindow(delchar->getCEGUIWindow());

}
