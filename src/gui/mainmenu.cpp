

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
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.2f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onStartSinglePlayer, this));
	btn->setWantsMultiClickEvents(false);

		// Button Server beitreten
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "ServerJoinButton"));
	start_menu->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.4f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.2f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onStartMultiPlayer, this));
	btn->setWantsMultiClickEvents(false);

		// Button Server aufsetzen
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "ServerHostButton"));
	start_menu->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.7f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.2f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::onStartMultiPlayerHost, this));
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
	btn->setText(gettext("Single player"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ServerJoinButton"));
	btn->setText(gettext("Join game"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ServerHostButton"));
	btn->setText(gettext("Host game"));

}

bool MainMenu::onStartSinglePlayer(const CEGUI::EventArgs& evt)
{
	DEBUG("start single player game");
	// Spieler ist selbst der Host
	m_document->setServer(true);

	// Verbindung aufbauen
	m_document->setState(Document::START_GAME);
	return true;
}

bool MainMenu::onStartMultiPlayer(const CEGUI::EventArgs& evt)
{
	DEBUG("start multi player game");

	m_document->setServer(false);


	// Verbindung aufbauen
	m_document->setState(Document::START_GAME);
	return true;
}

bool MainMenu::onStartMultiPlayerHost(const CEGUI::EventArgs& evt)
{
	DEBUG("start single player game");
	// Spieler ist selbst der Host
	m_document->setServer(true);


	// Verbindung aufbauen
	m_document->setState(Document::START_GAME);
	return true;
}
