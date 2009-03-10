#include "charcreate.h"

CharCreate::CharCreate (Document* doc)
	:Window(doc)
{
	DEBUG4("setup main menu");
	// GUI Elemente erzeugen

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	

	// Rahmen fuer das Menue Savegame auswaehlen
	CEGUI::FrameWindow* char_create = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "CharCreate");
	m_window = char_create;
	
	char_create->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f))); //0.0/0.8
	char_create->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 1.0f))); //1.0/0.2
	char_create->setProperty("FrameEnabled","false");
	char_create->setProperty("TitlebarEnabled","false");
	char_create->setProperty("CloseButtonEnabled","false");
	
	// Bestandteile der Kontrollleiste hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;

	// Label Name
	label = win_mgr.createWindow("TaharezLook/StaticText", "CharNameLabel");
	char_create->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.05f)));
	
	CEGUI::Editbox* namebox;
	
	namebox = static_cast<CEGUI::Editbox*>(win_mgr.createWindow("TaharezLook/Editbox", "NameBox"));
	char_create->addChildWindow(namebox);
	namebox->setPosition(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.05f)));
	namebox->setSize(CEGUI::UVector2(cegui_reldim(0.6f), cegui_reldim( 0.05f)));
	namebox->setWantsMultiClickEvents(false);
	namebox->setMaxTextLength(31);
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "CharClassLabel");
	char_create->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.12f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.05f)));
	
	CEGUI::Listbox* classlist = (CEGUI::Listbox*) win_mgr.createWindow("TaharezLook/Listbox", "ClassList");
	char_create->addChildWindow(classlist);
	classlist->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.2f)));
	classlist->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.2f)));
	classlist->addItem(new StrListItem((CEGUI::utf8*) gettext("Warrior"),"warrior"));
	classlist->addItem(new StrListItem((CEGUI::utf8*) gettext("Archer"),"archer"));
	classlist->addItem(new StrListItem((CEGUI::utf8*) gettext("Mage"),"mage"));
	classlist->addItem(new StrListItem((CEGUI::utf8*) gettext("Priest"),"priest"));
	classlist->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&CharCreate::onClassSelected, this));

	CEGUI::Listbox* looklist = (CEGUI::Listbox*) win_mgr.createWindow("TaharezLook/Listbox", "LookList");
	char_create->addChildWindow(looklist);
	looklist->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.42f)));
	looklist->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.45f)));
	looklist->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&CharCreate::onLookSelected, this));

	// Button Savegame akzeptieren
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "AbortButton"));
	char_create->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.9f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.05f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharCreate::onButtonAbort, this));
	btn->setWantsMultiClickEvents(false);
	
	// Button neu
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "CharCreateButton"));
	char_create->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.35f), cegui_reldim( 0.9f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.6f), cegui_reldim( 0.05f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharCreate::onButtonCharCreate, this));
	btn->setWantsMultiClickEvents(false);
	
	updateTranslation();
}


void CharCreate::update()
{
	// aktuell gewaehlte Klasse ermitteln
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Listbox* classlist = (CEGUI::Listbox*) win_mgr.getWindow("ClassList");
	CEGUI::ListboxItem * itm = classlist->getFirstSelectedItem();
	
	if (itm !=0)
	{
		StrListItem * sitm = (StrListItem *) itm;
		
		// alle moeglichen Looks fuer die Klasse ermitteln und Liste erneuern
		std::list< std::pair<bool, PlayerLook> > looks;
		std::list< std::pair<bool, PlayerLook> >::iterator it;
		Scene::getPlayerLook(sitm->m_data,looks);
		
		CEGUI::Listbox* looklist = (CEGUI::Listbox*) win_mgr.getWindow("LookList");
		looklist->resetList();
		
		for (it = looks.begin(); it !=	looks.end(); ++it)
		{
			looklist->addItem(new StrListItem(it->second,it->second));
		}
	}
}

void CharCreate::updateTranslation()
{
	textdomain ("sumwars_sumwars");
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	
	label = win_mgr.getWindow("CharNameLabel");
	label->setText((CEGUI::utf8*) gettext("Name"));
	
	label = win_mgr.getWindow("CharClassLabel");
	label->setText((CEGUI::utf8*) gettext("Class"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("AbortButton"));
	btn->setText((CEGUI::utf8*) gettext("Cancel"));

	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("CharCreateButton"));
	btn->setText((CEGUI::utf8*) gettext("Create"));
}

bool CharCreate::onClassSelected(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Listbox* classlist = (CEGUI::Listbox*) win_mgr.getWindow("ClassList");
	CEGUI::ListboxItem * itm = classlist->getFirstSelectedItem();


	if (itm !=0)
	{
		StrListItem * sitm = (StrListItem *) itm;
		DEBUG5("selected class %s", sitm->m_data.c_str());
	}
	
	update();
	
	CEGUI::Listbox* looklist = (CEGUI::Listbox*) win_mgr.getWindow("LookList");
	if (looklist->getItemCount() >0)
	{
		int idx =0;
		looklist->clearAllSelections();
		looklist->setItemSelectState(idx,true);
	}
	
	return true;
}

bool CharCreate::onLookSelected(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Listbox* classlist = (CEGUI::Listbox*) win_mgr.getWindow("LookList");
	CEGUI::ListboxItem * itm = classlist->getFirstSelectedItem();

	if (itm !=0)
	{
		StrListItem * sitm = (StrListItem *) itm;
		DEBUG5("selected look %s", sitm->m_data.c_str());
		
		CEGUI::Listbox* classlist = (CEGUI::Listbox*) win_mgr.getWindow("ClassList");
		CEGUI::ListboxItem * itm2 = classlist->getFirstSelectedItem();
		StrListItem * sitm2 = (StrListItem *) itm2;
		
		m_document->setNewCharacter(sitm2->m_data,sitm->m_data);
	}
	
	return true;
}

bool CharCreate::onButtonAbort(const CEGUI::EventArgs& evt)
{
	m_document->getGUIState()->m_shown_windows = Document::SAVEGAME_LIST;
	m_document->setModified(Document::WINDOWS_MODIFIED);
	return true;
}

bool CharCreate::onButtonCharCreate(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Editbox* namebox;
	namebox = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("NameBox"));
	std::string name = namebox->getText().c_str();
	
	if (name == "")
	{
		namebox ->activate();
		return true;
	}
	
	m_document->createNewCharacter(name);
	return true;
}


