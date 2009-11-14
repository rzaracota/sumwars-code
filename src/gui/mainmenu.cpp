

#include "mainmenu.h"

MainMenu::MainMenu (Document* doc)
	:Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	
	CEGUI::FrameWindow* start_menu = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "StartMenu");
	m_window = start_menu;
	
	
	start_menu->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.25f))); //0.0/0.8
	start_menu->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.5f))); //1.0/0.2
	start_menu->setProperty("FrameEnabled","false");
	start_menu->setProperty("TitlebarEnabled","false");
	start_menu->setProperty("CloseButtonEnabled","false");

	// Button Einzelspieler
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SinglePlayerButton"));
	start_menu->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.1f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.15f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onStartSinglePlayer, this));
	btn->setWantsMultiClickEvents(false);

		// Button Server beitreten
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "ServerJoinButton"));
	start_menu->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.3f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.15f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onStartMultiPlayer, this));
	btn->setWantsMultiClickEvents(false);

	// Button Server aufsetzen
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "ServerHostButton"));
	start_menu->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.5f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.15f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onStartMultiPlayerHost, this));
	btn->setWantsMultiClickEvents(false);
	
	// Button Server aufsetzen
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "EndGameButton"));
	start_menu->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.7f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.15f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onQuitGameHost, this));
	btn->setWantsMultiClickEvents(false);
	
	updateTranslation();
}

void MainMenu::update()
{
	
}

void MainMenu::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("SinglePlayerButton"));
	btn->setText((CEGUI::utf8*) gettext("Single player"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ServerJoinButton"));
	btn->setText((CEGUI::utf8*) gettext("Join game"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ServerHostButton"));
	btn->setText((CEGUI::utf8*) gettext("Host game"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("EndGameButton"));
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

bool MainMenu::onQuitGameHost(const CEGUI::EventArgs& evt)
{
	m_document->onButtonSaveExitClicked();
	return true;
}

