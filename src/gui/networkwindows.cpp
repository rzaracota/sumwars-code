#include "networkwindows.h"

HostGameWindow::HostGameWindow (Document* doc)
	:Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	CEGUI::Editbox* box;
	
	CEGUI::FrameWindow* host_game = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "HostGameWindow");
	m_window = host_game;
	
	
	host_game->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.25f))); //0.0/0.8
	host_game->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.5f))); //1.0/0.2
	host_game->setProperty("FrameEnabled","false");
	host_game->setProperty("TitlebarEnabled","false");
	host_game->setProperty("CloseButtonEnabled","false");
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "HostGameStartButton"));
	host_game->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim( 0.88f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.1f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&HostGameWindow ::onStartHostGame, this));
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "PlayerNumberLabel");
	host_game->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim(0.02f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.1f)));	
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.createWindow("TaharezLook/Editbox", "PlayerNumberBox"));
	host_game->addChildWindow(box);
	box->setPosition(CEGUI::UVector2(cegui_reldim(0.60f), cegui_reldim(0.02f)));
	box->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.1f)));	
	box->setWantsMultiClickEvents(false);
	box->setMaxTextLength(31);
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "HostPortLabel");
	host_game->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim(0.12f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.1f)));	
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.createWindow("TaharezLook/Editbox", "HostPortBox"));
	host_game->addChildWindow(box);
	box->setPosition(CEGUI::UVector2(cegui_reldim(0.60f), cegui_reldim(0.12f)));
	box->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.1f)));	
	box->setWantsMultiClickEvents(false);
	box->setMaxTextLength(31);
	
	updateTranslation();
}

void HostGameWindow::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Editbox* box;
	
	Options* options = Options::getInstance();
	
	int port = options->getPort();
	int max_nr = options->getMaxNumberPlayers();
	
	std::ostringstream stream;
	stream.str("");
	stream << max_nr;
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("PlayerNumberBox"));
	if (box->getText() != stream.str())
	{
		box->setText(stream.str());
	}
	
	stream.str("");
	stream << port;
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("HostPortBox"));
	if (box->getText() != stream.str())
	{
		box->setText(stream.str());
	}

}

void HostGameWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "HostGameStartButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));
	
	label = win_mgr.getWindow("PlayerNumberLabel");
	label->setText((CEGUI::utf8*) gettext("Max. number of players"));
	
	label = win_mgr.getWindow("HostPortLabel");
	label->setText((CEGUI::utf8*) gettext("Port"));
}


bool HostGameWindow::onStartHostGame(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Editbox* box;
	
	Options* options = Options::getInstance();
	
	int port = options->getPort();
	int max_nr = options->getMaxNumberPlayers();
	
	std::stringstream stream,stream2;
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("HostPortBox"));
	stream.str(box->getText().c_str());
	stream >> port;
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("PlayerNumberBox"));
	stream2.str(box->getText().c_str());
	stream2 >> max_nr;
	
	options->setPort(port);
	options->setMaxNumberPlayers(max_nr);
	
	m_document->onButtonStartHostGame();
	return true;
}



JoinGameWindow::JoinGameWindow (Document* doc)
	:Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	CEGUI::Editbox* box;
	
	CEGUI::FrameWindow* join_game = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "JoinGameWindow");
	m_window = join_game;
	
	
	join_game->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.25f))); //0.0/0.8
	join_game->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.5f))); //1.0/0.2
	join_game->setProperty("FrameEnabled","false");
	join_game->setProperty("TitlebarEnabled","false");
	join_game->setProperty("CloseButtonEnabled","false");
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "JoinGameStartButton"));
	join_game->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim( 0.88f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.1f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&JoinGameWindow ::onStartJoinGame, this));
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "HostnameLabel");
	join_game->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim(0.02f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.1f)));	
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.createWindow("TaharezLook/Editbox", "HostnameBox"));
	join_game->addChildWindow(box);
	box->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim(0.02f)));
	box->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.1f)));	
	box->setWantsMultiClickEvents(false);
	box->setMaxTextLength(31);
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "PortLabel");
	join_game->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim(0.12f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.1f)));	
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.createWindow("TaharezLook/Editbox", "PortBox"));
	join_game->addChildWindow(box);
	box->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim(0.12f)));
	box->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.1f)));	
	box->setWantsMultiClickEvents(false);
	box->setMaxTextLength(31);
	
	updateTranslation();
}

void JoinGameWindow::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Editbox* box;
	
	Options* options = Options::getInstance();
	
	int port = options->getPort();
	std::ostringstream stream;
	std::string hostname = options->getServerHost();
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("HostnameBox"));
	if (box->getText() != hostname)
	{
		box->setText(hostname);
	}
	
	stream.str("");
	stream << port;
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("PortBox"));
	if (box->getText() != stream.str())
	{
		box->setText(stream.str());
	}

}

void JoinGameWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "JoinGameStartButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));
	
	label = win_mgr.getWindow("HostnameLabel");
	label->setText((CEGUI::utf8*) gettext("Host"));
	
	label = win_mgr.getWindow("PortLabel");
	label->setText((CEGUI::utf8*) gettext("Port"));
}


bool JoinGameWindow::onStartJoinGame(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Editbox* box;
	
	Options* options = Options::getInstance();
	
	int port = options->getPort();
	std::stringstream stream;
	std::string hostname = options->getServerHost();
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("HostnameBox"));
	hostname = box->getText().c_str();
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("PortBox"));
	stream.str(box->getText().c_str());
	stream >> port;
	
	DEBUG("join game %i %s",port,hostname.c_str());
	options->setPort(port);
	options->setServerHost(hostname);
	
	m_document->onButtonStartJoinGame();
	return true;
}

