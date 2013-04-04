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

#include "networkwindows.h"

HostGameWindow::HostGameWindow (Document* doc)
	:Window(doc)
{
	DEBUG ("HostGameWindow being created");
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Editbox* box;

	// The host game window and holder
	CEGUI::FrameWindow* host_game = (CEGUI::FrameWindow*) win_mgr.loadWindowLayout("hostgamewindow.layout");
	if (!host_game)
	{
		DEBUG ("WARNING: Failed to load [%s]", "hostgamewindow.layout");
	}

	CEGUI::Window* host_game_holder = win_mgr.loadWindowLayout( "hostgamewindow_holder.layout" );
	if (!host_game_holder)
	{
		DEBUG ("WARNING: Failed to load [%s]", "hostgamewindow_holder.layout");
	}
	
	CEGUI::Window* wndHolder = win_mgr.getWindow("HostGameWindow_Holder");
	CEGUI::Window* wndCharInfo = win_mgr.getWindow("HostGameWindow");
	if (wndHolder && wndCharInfo)
	{
		wndHolder->addChildWindow (wndCharInfo);
	}
	else
	{
		if (!wndHolder) DEBUG ("ERROR: Unable to get the window holder for char screen.");
		if (!wndCharInfo) DEBUG ("ERROR: Unable to get the window for char screen.");
	}

	m_window = host_game_holder;
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("HostGameStartButton"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&HostGameWindow ::onStartHostGame, this));
		
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("PlayerNumberBox"));
	box->setWantsMultiClickEvents(false);
	box->setMaxTextLength(31);
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("HostPortBox"));
	box->setWantsMultiClickEvents(false);
	box->setMaxTextLength(31);
	
	// Connect the Cancel button to the cancel event.
	if (win_mgr.isWindowPresent ("HostGameCancelButton"))
	{
		btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("HostGameCancelButton"));
		btn->setID(5);
		btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&HostGameWindow::onCancelHostGame, this));
	}

	// Connect the window close button to the cancel event.
	if (win_mgr.isWindowPresent ("HostGameWindow__auto_closebutton__"))
	{
		CEGUI::Window* autoCloseButton;
		autoCloseButton = win_mgr.getWindow ("HostGameWindow__auto_closebutton__");
		if (autoCloseButton)
		{
			autoCloseButton->subscribeEvent (CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber (&HostGameWindow::onCancelHostGame, this));
		}
	}

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

	if (win_mgr.isWindowPresent ("HostGameCancelButton"))
	{
		btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "HostGameCancelButton"));
		btn->setText((CEGUI::utf8*) gettext("Cancel"));
	}

	if (win_mgr.isWindowPresent ("HostWindowTitle"))
	{
		// Old style host window title.
		label = win_mgr.getWindow("HostWindowTitle");
		label->setText((CEGUI::utf8*) gettext("Host_game"));
	}
	else if (win_mgr.isWindowPresent ("HostGameWindow"))
	{
		label = win_mgr.getWindow("HostGameWindow");
		label->setText((CEGUI::utf8*) gettext("Host_game"));
	}
	
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


bool HostGameWindow::onCancelHostGame (const CEGUI::EventArgs& evt)
{
	m_document->getGUIState()->m_shown_windows = Document::START_MENU;
	m_document->setModified(Document::WINDOWS_MODIFIED);

	return true;
}



JoinGameWindow::JoinGameWindow (Document* doc)
	:Window(doc)
{
	DEBUG ("JoinGameWindow being created");
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Editbox* box;

	// The join game window and holder.
	CEGUI::FrameWindow* join_game = (CEGUI::FrameWindow*) win_mgr.loadWindowLayout("joingamewindow.layout");
	if (!join_game)
	{
		DEBUG ("WARNING: Failed to load [%s]", "joingamewindow.layout");
	}

	CEGUI::Window* join_game_holder = win_mgr.loadWindowLayout( "joingamewindow_holder.layout" );
	if (!join_game_holder)
	{
		DEBUG ("WARNING: Failed to load [%s]", "joingamewindow_holder.layout");
	}
	
	CEGUI::Window* wndHolder = win_mgr.getWindow("JoinGameWindow_Holder");
	CEGUI::Window* wndCharInfo = win_mgr.getWindow("JoinGameWindow");
	if (wndHolder && wndCharInfo)
	{
		wndHolder->addChildWindow (wndCharInfo);
	}
	else
	{
		if (!wndHolder) DEBUG ("ERROR: Unable to get the window holder for char screen.");
		if (!wndCharInfo) DEBUG ("ERROR: Unable to get the window for char screen.");
	}

	m_window = join_game_holder;
		
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("JoinGameStartButton"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&JoinGameWindow ::onStartJoinGame, this));
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("HostnameBox"));
	box->setWantsMultiClickEvents(false);
	box->setMaxTextLength(31);
	
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("PortBox"));
	box->setWantsMultiClickEvents(false);
	box->setMaxTextLength(31);

	// Connect the Cancel button to the cancel event.
	if (win_mgr.isWindowPresent ("JoinGameCancelButton"))
	{
		btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("JoinGameCancelButton"));
		btn->setID(5);
		btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&JoinGameWindow::onCancelJoinGame, this));
	}

	// Connect the window close button to the cancel event.
	if (win_mgr.isWindowPresent ("JoinGameWindow__auto_closebutton__"))
	{
		CEGUI::Window* autoCloseButton;
		autoCloseButton = win_mgr.getWindow ("JoinGameWindow__auto_closebutton__");
		if (autoCloseButton)
		{
			autoCloseButton->subscribeEvent (CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber (&JoinGameWindow::onCancelJoinGame, this));
		}
	}
	
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
	
	if (win_mgr.isWindowPresent ("JoinGameCancelButton"))
	{
		btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "JoinGameCancelButton"));
		btn->setText((CEGUI::utf8*) gettext("Cancel"));
	}

	if (win_mgr.isWindowPresent ("JoinWindowTitle"))
	{
		label = win_mgr.getWindow("JoinWindowTitle");
		label->setText((CEGUI::utf8*) gettext("Join_game"));
	}
	else if (win_mgr.isWindowPresent ("JoinGameWindow"))
	{
		label = win_mgr.getWindow("JoinGameWindow");
		label->setText((CEGUI::utf8*) gettext("Join_game"));
	}
	
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


bool JoinGameWindow::onCancelJoinGame (const CEGUI::EventArgs& evt)
{
	m_document->getGUIState()->m_shown_windows = Document::START_MENU;
	m_document->setModified(Document::WINDOWS_MODIFIED);

	return true;
}

