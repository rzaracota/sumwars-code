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

#include "scene.h"

#include "charcreate.h"

#include "listitem.h"

// Sound operations helper.
#include "soundhelper.h"

// Utility for CEGUI cross-version compatibility
#include "ceguiutility.h"


CharCreate::CharCreate (Document* doc, const std::string& ceguiSkinName)
	: Window (doc)
	, m_ceguiSkinName (ceguiSkinName)
{
	DEBUG ("CharCreate window created with cegui skin [%s]", m_ceguiSkinName.c_str ());
	// Create GUI Elements

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	

	// Rahmen fuer das Menue Savegame auswaehlen
	CEGUI::FrameWindow* char_create = (CEGUI::FrameWindow*) CEGUIUtility::loadLayoutFromFile ("charcreate.layout");
	m_window = char_create;
	m_window->setMousePassThroughEnabled(true);
	
	// pin the the window below the StartMenuRoot to allow display along with MainMenu Elements
	CEGUI::FrameWindow* start_menu = (CEGUI::FrameWindow*) CEGUIUtility::getWindow("StartMenuRoot");
	CEGUIUtility::addChildWidget(start_menu, char_create);
	
	// Bestandteile der Kontrollleiste hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Editbox* namebox;
	
	namebox = static_cast<CEGUI::Editbox*>(CEGUIUtility::getWindow("NameBox"));
	namebox->setWantsMultiClickEvents(false);
	namebox->setMaxTextLength(31);
	namebox->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&CharCreate::onGUIItemClick, this));
	
	CEGUI::Listbox* classlist = (CEGUI::Listbox*) CEGUIUtility::getWindow("ClassList");
	classlist->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&CharCreate::onClassSelected, this));
	classlist->subscribeEvent(CEGUIUtility::EventMouseEntersListBoxArea (), CEGUI::Event::Subscriber(&CharCreate::onGUIItemHover, this));

	CEGUI::Listbox* looklist = (CEGUI::Listbox*) CEGUIUtility::getWindow("LookList");
	looklist->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&CharCreate::onLookSelected, this));
	looklist->subscribeEvent(CEGUIUtility::EventMouseEntersListBoxArea (), CEGUI::Event::Subscriber(&CharCreate::onGUIItemHover, this));

	// Button Savegame akzeptieren
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow("AbortButton"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharCreate::onButtonAbort, this));
	btn->subscribeEvent(CEGUIUtility::EventMouseEntersPushButtonArea (), CEGUI::Event::Subscriber(&CharCreate::onGUIItemHover, this));
	btn->setWantsMultiClickEvents(false);
	
	// Button neu
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow("CharCreateButton"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharCreate::onButtonCharCreate, this));
	btn->subscribeEvent(CEGUIUtility::EventMouseEntersPushButtonArea (), CEGUI::Event::Subscriber(&CharCreate::onGUIItemHover, this));
	btn->setWantsMultiClickEvents(false);
	
	updateTranslation();
}


void CharCreate::update()
{
	// aktuell gewaehlte Klasse ermitteln
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Listbox* classlist = (CEGUI::Listbox*) CEGUIUtility::getWindow("ClassList");
	CEGUI::ListboxItem * itm = classlist->getFirstSelectedItem();
	
	if (itm !=0)
	{
		StrListItem * sitm = (StrListItem *) itm;
		
		// alle moeglichen Looks fuer die Klasse ermitteln und Liste erneuern
		std::list< PlayerLook > looks;
		std::list< PlayerLook >::iterator it;
		ObjectFactory::getPlayerLooks(sitm->m_data,looks);
		
		CEGUI::Listbox* looklist = (CEGUI::Listbox*) CEGUIUtility::getWindow("LookList");
		looklist->resetList();
		
		for (it = looks.begin(); it !=	looks.end(); ++it)
		{
			looklist->addItem(new StrListItem((CEGUI::utf8*) m_ceguiSkinName.c_str (), (CEGUI::utf8*) gettext(it->m_name.c_str()),it->m_name));
		}
	}
}

void CharCreate::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	
	label = CEGUIUtility::getWindow("CharNameLabel");
	label->setText((CEGUI::utf8*) gettext("Name"));
	
	label = CEGUIUtility::getWindow("CharClassLabel");
	label->setText((CEGUI::utf8*) gettext("Class"));
	
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow("AbortButton"));
	btn->setText((CEGUI::utf8*) gettext("Cancel"));

	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow("CharCreateButton"));
	btn->setText((CEGUI::utf8*) gettext("Create"));
}

void CharCreate::updateClassList()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Listbox* classlist = (CEGUI::Listbox*) CEGUIUtility::getWindow("ClassList");
	classlist->resetList();
	
	std::map<GameObject::Subtype, PlayerBasicData*>& pcdata = ObjectFactory::getPlayerData();
	std::map<GameObject::Subtype, PlayerBasicData*>::iterator it;
	for (it = pcdata.begin(); it != pcdata.end(); ++it)
	{
		classlist->addItem(new StrListItem((CEGUI::utf8*) m_ceguiSkinName.c_str (), (CEGUI::utf8*) gettext(it->second->m_name.c_str()),it->first));
	}
	
	if (classlist->getItemCount() >0)
	{
		int idx =0;
		classlist->clearAllSelections();
		classlist->setItemSelectState(idx,true);
	}
}

bool CharCreate::onClassSelected(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Listbox* classlist = (CEGUI::Listbox*) CEGUIUtility::getWindow("ClassList");
	CEGUI::ListboxItem * itm = classlist->getFirstSelectedItem();

	SoundHelper::playAmbientSoundGroup ("main_menu_click_item");

	if (itm !=0)
	{
		DEBUGX("selected class %s", static_cast<StrListItem*>(sitm)->m_data.c_str());
	}
	
	update();
	
	CEGUI::Listbox* looklist = (CEGUI::Listbox*) CEGUIUtility::getWindow("LookList");
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
	CEGUI::Listbox* classlist = (CEGUI::Listbox*) CEGUIUtility::getWindow("LookList");
	CEGUI::ListboxItem * itm = classlist->getFirstSelectedItem();

	SoundHelper::playAmbientSoundGroup ("main_menu_click_item");

	if (itm !=0)
	{
		StrListItem * sitm = (StrListItem *) itm;
		DEBUGX("selected look %s", sitm->m_data.c_str());
		
		CEGUI::Listbox* classlist = (CEGUI::Listbox*) CEGUIUtility::getWindow("ClassList");
		CEGUI::ListboxItem * itm2 = classlist->getFirstSelectedItem();
		StrListItem * sitm2 = (StrListItem *) itm2;
		
		if (sitm2 == 0)
		{
			CEGUI::Listbox* looklist = (CEGUI::Listbox*) CEGUIUtility::getWindow("LookList");
			looklist->resetList();
			return true;
		}
		
		PlayerLook* plook = ObjectFactory::getPlayerLook(sitm2->m_data,sitm->m_data);
		if (plook != 0)
		{
			m_document->setNewCharacter(sitm2->m_data,*plook);
		}
	}
	
	return true;
}

bool CharCreate::onButtonAbort(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Editbox* namebox;

	SoundHelper::playAmbientSoundGroup ("main_menu_click_item");

	namebox = static_cast<CEGUI::Editbox*>(CEGUIUtility::getWindow("NameBox"));
	namebox->setText("");
	
	m_document->getGUIState()->m_shown_windows = Document::START_MENU;
	m_document->setModified(Document::WINDOWS_MODIFIED);
	return true;
}

bool CharCreate::onButtonCharCreate(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Editbox* namebox;

	SoundHelper::playAmbientSoundGroup ("main_menu_click_item");

	namebox = static_cast<CEGUI::Editbox*>(CEGUIUtility::getWindow("NameBox"));
	std::string name = namebox->getText().c_str();
	
	// check if name is empty
	if (name == "")
	{
		namebox ->activate();
		
		CEGUI::FrameWindow* message = (CEGUI::FrameWindow*) CEGUIUtility::getWindow("WarningDialogWindow");
		message->setInheritsAlpha(false);
		message->setVisible(true);
		message->setModalState(true);
		CEGUIUtility::getWindow( "WarningDialogLabel")->setText((CEGUI::utf8*) gettext("You must enter a hero name!"));
		
		return true;
	}
	
	// check if name is valid
	bool valid = m_document->createNewCharacter(name);
	
	if (!valid)
	{
		namebox ->activate();
		
		CEGUI::FrameWindow* message = (CEGUI::FrameWindow*) CEGUIUtility::getWindow("WarningDialogWindow");
		message->setInheritsAlpha(false);
		message->setVisible(true);
		message->setModalState(true);
		CEGUIUtility::getWindow( "WarningDialogLabel")->setText((CEGUI::utf8*) gettext("A character with that name already exists!"));
		
		return true;
	}
	namebox ->setText("");
	return true;
}



/**
 * \fn bool onGUIItemClick(const CEGUI::EventArgs& evt)
 * \brief Handle the click of gui items.
 */
bool CharCreate::onGUIItemClick (const CEGUI::EventArgs& evt)
{
	SoundHelper::playAmbientSoundGroup ("main_menu_click_item");
	return true;
}



/**
 * \fn bool onGUIItemHover(const CEGUI::EventArgs& evt)
 * \brief Handle the hovering of gui items.
 */
bool CharCreate::onGUIItemHover (const CEGUI::EventArgs& evt)
{
	SoundHelper::playAmbientSoundGroup ("main_menu_hover_item");
	return true;
}
