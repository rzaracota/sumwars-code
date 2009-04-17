#include "optionswindow.h"
#include "sound.h"

OptionsWindow::OptionsWindow (Document* doc, OIS::Keyboard *keyboard)
	:Window(doc)
{
	m_keyboard = keyboard;
	
	DEBUG4("setup main menu");
	// GUI Elemente erzeugen

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	

	// Rahmen fuer das Menue Savegame auswaehlen
	
	
	CEGUI::FrameWindow* options = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "OptionsWindow");
	m_window = options;
	
	options->setPosition(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim( 0.05f))); //0.0/0.8
	options->setSize(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim( 0.8f))); //1.0/0.2
	options->setProperty("FrameEnabled","false");
	options->setProperty("TitlebarEnabled","false");
	options->setProperty("CloseButtonEnabled","false");
	options->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&OptionsWindow::onAreaMouseButtonPressed, this));
	
	CEGUI::Window* label;
	
	int targets[8] = {Document::SHOW_INVENTORY, Document::SHOW_CHARINFO, Document::SHOW_SKILLTREE, Document::SHOW_PARTYMENU, Document::SHOW_CHATBOX, Document::SHOW_QUESTINFO, Document::SHOW_MINIMAP, Document::SWAP_EQUIP};
	
	std::ostringstream stream;
	for (int i=0; i<8; ++i)
	{
		stream.str("");
		stream << "ShortkeyLabel"<<i;
		
		label = win_mgr.createWindow("TaharezLook/StaticText", stream.str());
		options->addChildWindow(label);
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( i*0.08+0.02f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.06f)));
		label->setID(targets[i]);
		label->setWantsMultiClickEvents(false);
	
		stream.str("");
		stream << "ShortkeyValueLabel"<<i;
		
		label = win_mgr.createWindow("TaharezLook/StaticText", stream.str());
		options->addChildWindow(label);
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.38f), cegui_reldim( i*0.08+0.02f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
		label->setID(targets[i]);
		label->setWantsMultiClickEvents(false);
		label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&OptionsWindow::onShortkeyLabelClicked,  this));
	}
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "SoundVolumeLabel");
	options->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.7)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.06f)));
	
	CEGUI::Scrollbar* slider = static_cast<CEGUI::Scrollbar*>(win_mgr.createWindow("TaharezLook/HorizontalScrollbar", "SoundVolumeSlider"));
	options->addChildWindow(slider);
	slider->setPageSize (0.01f);
	slider->setDocumentSize(1.0f);
	slider->setStepSize(0.01f);
	slider->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.72)));
	slider->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.02f)));
	slider->setWantsMultiClickEvents(false);
	slider->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&OptionsWindow::onSoundVolumeChanged,  this));
	
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "OptionsCloseButton"));
	options->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim( 0.94f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.05f)));
	btn->setID(5);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsWindow::onButtonOkClicked, this));
	
	reset();
	updateTranslation();
}

void OptionsWindow::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	std::ostringstream stream;
	KeyCode key;
	std::string keyname;
	// Schleife ueber die Labels der Kurztasten
	for (int i=0; i<8; i++)
	{
		stream.str("");
		stream << "ShortkeyValueLabel"<<i;
		label = win_mgr.getWindow(stream.str());
		
		if (m_key_destination == (int) label->getID())
		{
			// die Kurztaste soll gerade eingegeben werden
			keyname = "_";
		}
		else
		{
			key = m_document->getMappedKey( (Document::ShortkeyDestination) label->getID());
			keyname = m_keyboard->getAsString ( (OIS::KeyCode) key);
		}
		
		if (label->getText() != keyname)
		{
			label->setText(keyname);
		}
	}
	
	CEGUI::Scrollbar* slider = static_cast<CEGUI::Scrollbar*>(win_mgr.getWindow( "SoundVolumeSlider"));
	if ( fabs ( slider->getScrollPosition() - SoundSystem::getSoundVolume()) > 0.01f)
	{
		slider->setScrollPosition(SoundSystem::getSoundVolume());
	}
}

void OptionsWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	label = win_mgr.getWindow("ShortkeyLabel0");
	label->setText((CEGUI::utf8*) gettext("Inventory"));
	
	label = win_mgr.getWindow("ShortkeyLabel1");
	label->setText((CEGUI::utf8*) gettext("Character screen"));
	
	label = win_mgr.getWindow("ShortkeyLabel2");
	label->setText((CEGUI::utf8*) gettext("Skilltree"));
	
	label = win_mgr.getWindow("ShortkeyLabel3");
	label->setText((CEGUI::utf8*) gettext("Party screen"));
	
	label = win_mgr.getWindow("ShortkeyLabel4");
	label->setText((CEGUI::utf8*) gettext("Chatbox"));
	
	label = win_mgr.getWindow("ShortkeyLabel5");
	label->setText((CEGUI::utf8*) gettext("Quests"));
	
	label = win_mgr.getWindow("ShortkeyLabel6");
	label->setText((CEGUI::utf8*) gettext("Minimap"));
	
	label = win_mgr.getWindow("ShortkeyLabel7");
	label->setText((CEGUI::utf8*) gettext("Swap equipement"));
	
	label = win_mgr.getWindow("SoundVolumeLabel");
	label->setText((CEGUI::utf8*) gettext("Sound"));
	
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "OptionsCloseButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));
}


void OptionsWindow::reset()
{
	m_key_destination = Document::NO_KEY;
}

bool OptionsWindow::onShortkeyLabelClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	int id = (int) we.window->getID();
	
	if (m_key_destination == id)
	{
		m_key_destination = Document::NO_KEY;
	}
	else
	{
		m_key_destination =(Document::ShortkeyDestination) id;
	}
	return true;
}

void OptionsWindow::setKeyCode(KeyCode kc)
{
	if (requestsForKey())
	{
		m_document->installShortkey(kc,m_key_destination);
		m_key_destination = Document::NO_KEY;
	}
}

bool OptionsWindow::onAreaMouseButtonPressed(const CEGUI::EventArgs& evt)
{
	m_key_destination = Document::NO_KEY;
	return true;
}

bool OptionsWindow::onButtonOkClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonOptionsClicked();
	return true;
}

bool OptionsWindow::onSoundVolumeChanged(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	
	CEGUI::Scrollbar* slider = static_cast<CEGUI::Scrollbar*>(we.window);
	float vol = slider->getScrollPosition();
	DEBUG5("sound volume change to %f",vol);
	SoundSystem::setSoundVolume(vol);
	return true;
}
