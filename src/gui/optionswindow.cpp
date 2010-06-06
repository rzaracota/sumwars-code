#include "optionswindow.h"
#include "sound.h"
#include "listitem.h"
#include "music.h"

OptionsWindow::OptionsWindow (Document* doc, OIS::Keyboard *keyboard)
	:Window(doc)
{
	m_keyboard = keyboard;

	DEBUGX("setup main menu");
	// GUI Elemente erzeugen

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::FrameWindow* options = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "OptionsWindow");
	m_window = options;
	options->setProperty("FrameEnabled","false");
	options->setProperty("TitlebarEnabled","false");
	options->setProperty("CloseButtonEnabled","false");
	options->setPosition(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim( 0.05f))); //0.0/0.8
	options->setSize(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim( 0.8f))); //1.0/0.2
	options->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&OptionsWindow::onAreaMouseButtonPressed, this));

	// Rahmen fuer das Menue Savegame auswaehlen
	CEGUI::TabControl* optionstab = (CEGUI::TabControl*) win_mgr.createWindow("TaharezLook/TabControl", "OptionsWindowTab");
	optionstab->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.00f)));
	optionstab->setSize(CEGUI::UVector2(cegui_reldim(0.9f), cegui_reldim( 0.9f)));
	optionstab->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&OptionsWindow::onAreaMouseButtonPressed, this));
	options->addChildWindow(optionstab);


	CEGUI::DefaultWindow* keys = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "OptionsShortkeys");
	optionstab->addTab(keys);
	CEGUI::DefaultWindow* sound = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "OptionsSound");
	optionstab->addTab(sound);
	CEGUI::DefaultWindow* graphic = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "OptionsGraphic");
	optionstab->addTab(graphic);
	CEGUI::DefaultWindow* misc = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "OptionsMisc");
	optionstab->addTab(misc);

	CEGUI::Window* label;

	int targets[9] = {SHOW_INVENTORY, SHOW_CHARINFO, SHOW_SKILLTREE, SHOW_PARTYMENU, SHOW_CHATBOX, SHOW_QUESTINFO, SHOW_MINIMAP, SWAP_EQUIP, SHOW_ITEMLABELS};

	std::ostringstream stream;
	for (int i=0; i<9; ++i)
	{
		stream.str("");
		stream << "ShortkeyLabel"<<i;

		label = win_mgr.createWindow("TaharezLook/StaticText", stream.str());
		keys->addChildWindow(label);
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( i*0.08+0.02f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.06f)));
		label->setID(targets[i]);
		label->setWantsMultiClickEvents(false);

		stream.str("");
		stream << "ShortkeyValueLabel"<<i;

		label = win_mgr.createWindow("TaharezLook/StaticText", stream.str());
		keys->addChildWindow(label);
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.38f), cegui_reldim( i*0.08+0.02f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
		label->setID(targets[i]);
		label->setWantsMultiClickEvents(false);
		label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&OptionsWindow::onShortkeyLabelClicked,  this));
	}

	label = win_mgr.createWindow("TaharezLook/StaticText", "MusicVolumeLabel");
	sound->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.5)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.06f)));

	CEGUI::Scrollbar* slider = static_cast<CEGUI::Scrollbar*>(win_mgr.createWindow("TaharezLook/HorizontalScrollbar", "MusicVolumeSlider"));
	sound->addChildWindow(slider);
	slider->setPageSize (0.01f);
	slider->setDocumentSize(1.0f);
	slider->setStepSize(0.01f);
	slider->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.52)));
	slider->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.02f)));
	slider->setWantsMultiClickEvents(false);
	slider->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&OptionsWindow::onMusicVolumeChanged,  this));

	label = win_mgr.createWindow("TaharezLook/StaticText", "SoundVolumeLabel");
	sound->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.7)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.06f)));

	slider = static_cast<CEGUI::Scrollbar*>(win_mgr.createWindow("TaharezLook/HorizontalScrollbar", "SoundVolumeSlider"));
	sound->addChildWindow(slider);
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

	label = win_mgr.createWindow("TaharezLook/StaticText", "LanguageLabel");
	misc->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.8)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.06f)));

	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*>(win_mgr.createWindow("TaharezLook/Combobox","LanguageBox"));
	misc->addChildWindow(cbo);
	cbo->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim( 0.8f)));
	cbo->setSize(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.3f)));

	cbo->addItem(new StrListItem("System default","",0));
	cbo->addItem(new StrListItem("German","de_DE",0));
	cbo->addItem(new StrListItem("English GB","en_GB",0));
	cbo->addItem(new StrListItem("English US","en_US",0));

	cbo->setReadOnly(true);

	cbo->setItemSelectState((size_t) 0,true);
	cbo->handleUpdatedListItemData();

	cbo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&OptionsWindow::onLanguageSelected, this));

	reset();
	updateTranslation();
}

void OptionsWindow::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	Options* options = Options::getInstance();

	std::ostringstream stream;
	KeyCode key;
	std::string keyname;
	// Schleife ueber die Labels der Kurztasten
	for (int i=0; i<9; i++)
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
			key = options->getMappedKey( (ShortkeyDestination) label->getID());
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

	slider = static_cast<CEGUI::Scrollbar*>(win_mgr.getWindow( "MusicVolumeSlider"));
	if ( fabs ( slider->getScrollPosition() - MusicManager::instance().getMusicVolume()) > 0.01f)
	{
		slider->setScrollPosition(MusicManager::instance().getMusicVolume());
	}

}

void OptionsWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;

	CEGUI::DefaultWindow* keys =  (CEGUI::DefaultWindow*) win_mgr.getWindow("OptionsShortkeys");
	keys->setText((CEGUI::utf8*) gettext("Shortkeys"));
	CEGUI::DefaultWindow* sound = (CEGUI::DefaultWindow*) win_mgr.getWindow("OptionsSound");
	sound->setText((CEGUI::utf8*) gettext("Audio"));
	CEGUI::DefaultWindow* graphic = (CEGUI::DefaultWindow*) win_mgr.getWindow("OptionsGraphic");
	graphic->setText((CEGUI::utf8*) gettext("Graphic"));
	CEGUI::DefaultWindow* misc = (CEGUI::DefaultWindow*) win_mgr.getWindow("OptionsMisc");
	misc->setText((CEGUI::utf8*) gettext("Language"));

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

	label = win_mgr.getWindow("ShortkeyLabel8");
	label->setText((CEGUI::utf8*) gettext("Item Labels"));

	label = win_mgr.getWindow("SoundVolumeLabel");
	label->setText((CEGUI::utf8*) gettext("Sound"));

	label = win_mgr.getWindow("MusicVolumeLabel");
	label->setText((CEGUI::utf8*) gettext("Music"));

	label = win_mgr.getWindow("LanguageLabel");
	label->setText((CEGUI::utf8*) gettext("Language"));

	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "OptionsCloseButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));
}


void OptionsWindow::reset()
{
	m_key_destination = NO_KEY;
}

bool OptionsWindow::onShortkeyLabelClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	int id = (int) we.window->getID();

	if (m_key_destination == id)
	{
		m_key_destination = NO_KEY;
	}
	else
	{
		m_key_destination =(ShortkeyDestination) id;
	}
	return true;
}

void OptionsWindow::setKeyCode(KeyCode kc)
{
	if (requestsForKey())
	{
		ShortkeyMap& ablt_map = m_document->getAbilityShortkeys();
		ablt_map.erase(kc);
		
		Options::getInstance()->setShortkey(kc,m_key_destination);
		m_key_destination = NO_KEY;
	}
}

bool OptionsWindow::onAreaMouseButtonPressed(const CEGUI::EventArgs& evt)
{
	m_key_destination = NO_KEY;
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
	DEBUGX("sound volume change to %f",vol);
	SoundSystem::setSoundVolume(vol);
	return true;
}

bool OptionsWindow::onMusicVolumeChanged(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);

	CEGUI::Scrollbar* slider = static_cast<CEGUI::Scrollbar*>(we.window);
	float vol = slider->getScrollPosition();
	DEBUGX("music volume change to %f",vol);
	MusicManager::instance().setMusicVolume(vol);
	return true;
}

bool OptionsWindow::onLanguageSelected(const CEGUI::EventArgs& evt)
{

	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);

	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*>(we.window);

	CEGUI::ListboxItem* item = cbo->getSelectedItem();

	if (item != 0)
	{
		DEBUGX("selected Language %s",item->getText().c_str());
		StrListItem* sitem = static_cast<StrListItem*>(item);
		Gettext::setLocale(sitem->m_data.c_str());
	}

	return true;
}

