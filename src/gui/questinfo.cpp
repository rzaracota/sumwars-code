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

#include "questinfo.h"

QuestInfo::QuestInfo (Document* doc, const std::string& ceguiSkinName)
	: Window (doc)
	, m_ceguiSkinName (ceguiSkinName)
{
	m_questname = "";
	
	DEBUGX("setup main menu");
	// GUI Elemente erzeugen

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	

	// Rahmen fuer das Menue Savegame auswaehlen
	CEGUI::FrameWindow* quest_info = (CEGUI::FrameWindow*) win_mgr.loadWindowLayout("questinfo.layout");
	if (!quest_info)
	{
		DEBUG ("WARNING: Failed to load [%s]", "questinfo.layout");
	}
	CEGUI::Window* quest_info_holder = win_mgr.loadWindowLayout( "questinfo_holder.layout" );
	if (!quest_info_holder)
	{
		DEBUG ("WARNING: Failed to load [%s]", "questinfo_holder.layout");
	}

	CEGUI::Window* wndHolder = win_mgr.getWindow("QuestInfo_Holder");
	CEGUI::Window* wndQuest = win_mgr.getWindow("QuestInfo");
	if (wndHolder && wndQuest)
	{
		wndHolder->addChildWindow (wndQuest);
	}
	else
	{
		if (!wndHolder) DEBUG ("ERROR: Unable to get the window holder for quests.");
		if (!wndQuest) DEBUG ("ERROR: Unable to get the window for quests.");
	}

	m_window = quest_info_holder;
	
	quest_info->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	
	// Bestandteile der Kontrollleiste hinzufuegen
	CEGUI::Window* label;

	
	CEGUI::Listbox* questlist = (CEGUI::Listbox*) win_mgr.getWindow("QuestList");
	questlist->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&QuestInfo::onQuestSelected, this));
	
	CEGUI::Checkbox * box;
	
	box = (CEGUI::Checkbox *) win_mgr.getWindow("QuestOpenBox");
	box->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&QuestInfo::onFilterSelected, this));
	box->setSelected(true);
	
	box = (CEGUI::Checkbox *) win_mgr.getWindow("QuestDoneBox");
	box->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&QuestInfo::onFilterSelected, this));
	box->setSelected(false);
	
	box = (CEGUI::Checkbox *) win_mgr.getWindow("QuestFailedBox");
	box->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&QuestInfo::onFilterSelected, this));
	box->setSelected(false);
	
	CEGUI::MultiLineEditbox* quest_descr;
	quest_descr = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.getWindow("QuestDescription"));
	quest_descr->setWantsMultiClickEvents(false);
	quest_descr->setReadOnly(true);
	quest_descr->setText("");
	
	if (win_mgr.isWindowPresent ("QuestInfoCloseButton"))
	{
		label = win_mgr.getWindow("QuestInfoCloseButton");
		label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&QuestInfo::onCloseButtonClicked, this));
	}
	else if (win_mgr.isWindowPresent ("QuestInfo__auto_closebutton__"))
	{
		CEGUI::Window* autoCloseButton;
		autoCloseButton = win_mgr.getWindow ("QuestInfo__auto_closebutton__");
		if (autoCloseButton)
		{
			autoCloseButton->subscribeEvent (CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber (&QuestInfo::onCloseButtonClicked, this));
		}
	}

	updateTranslation();
}


void QuestInfo::update()
{
	if (World::getWorld() ==0)
		return;
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// Auswahl ermitteln
	bool open,done,failed;
	CEGUI::Checkbox * box;
	
	box = (CEGUI::Checkbox *) win_mgr.getWindow("QuestOpenBox");
	open = box->isSelected();
	
	box = (CEGUI::Checkbox *) win_mgr.getWindow("QuestDoneBox");
	done = box->isSelected();
	
	box = (CEGUI::Checkbox *) win_mgr.getWindow("QuestFailedBox");
	failed = box->isSelected();
	
	CEGUI::Listbox* questlist = (CEGUI::Listbox*) win_mgr.getWindow("QuestList");
	questlist->resetList();
	
	// Liste mit den Quests aktualisieren
	std::map<std::string, Quest*>& quests = World::getWorld()->getQuests();
	std::map<std::string, Quest*>::iterator it;
	
	Quest::State state;
	CEGUI::ListboxItem* newitem;
	for (it = quests.begin(); it != quests.end(); ++it)
	{
		Quest* quest = it->second;
		state = it->second->getState();
		if ((state == Quest::STARTED && open) || 
		    (state == Quest::FINISHED && done) || 
		    (state == Quest::FAILED && failed))
		{
			std::string name = quest->getName().getTranslation().c_str();
			newitem = new StrListItem((CEGUI::utf8*) m_ceguiSkinName.c_str (), (CEGUI::utf8*) name.c_str(),it->first);
			questlist->addItem(newitem);
			DEBUGX("add quest %s %s",quest->getName().getTranslation().c_str(),it->first.c_str());
			
			if (it->first == m_questname)
			{
				questlist->setItemSelectState(newitem,true);
			}
		}
	}
	
	// wenn kein Quest selektiert ist, das erste auswaehlen
	if (questlist->getSelectedCount()==0)
	{
		if (questlist->getItemCount() >0)
		{
			int id =0;
			questlist->setItemSelectState (id,true);
		}
		updateDescription();
	}
	
}

void QuestInfo::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;

	if (win_mgr.isWindowPresent ("Quests"))
	{
		label =  win_mgr.getWindow("Quests");
		label->setText((CEGUI::utf8*) gettext("Quests"));
	}
	else if (win_mgr.isWindowPresent ("QuestInfo"))
	{
		label =  win_mgr.getWindow("QuestInfo");
		if (label->isPropertyPresent ("Text"))
		{
			label->setProperty ("Text", (CEGUI::utf8*) gettext("Quests"));
		}
	}

	label = win_mgr.getWindow("QuestOpenLabel");
	label->setText((CEGUI::utf8*) gettext("open"));
	
	label = win_mgr.getWindow("QuestDoneLabel");
	label->setText((CEGUI::utf8*) gettext("finished"));
	
	label = win_mgr.getWindow("QuestFailedLabel");
	label->setText((CEGUI::utf8*) gettext("failed"));
	
	
}

bool QuestInfo::onQuestSelected(const CEGUI::EventArgs& evt)
{
	updateDescription();
	
	return true;
}

bool QuestInfo::onFilterSelected(const CEGUI::EventArgs& evt)
{
	update();
	return true;
}

bool QuestInfo::onCloseButtonClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonQuestInfoClicked();
	return true;
}

void QuestInfo::updateDescription()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Listbox* classlist = (CEGUI::Listbox*) win_mgr.getWindow("QuestList");
	CEGUI::ListboxItem * itm = classlist->getFirstSelectedItem();
	
	CEGUI::MultiLineEditbox* quest_descr;
	quest_descr = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.getWindow("QuestDescription"));
	
	if (itm !=0 && World::getWorld()!=0)
	{
		std::map<std::string, Quest*>& quests = World::getWorld()->getQuests();
		std::map<std::string, Quest*>::iterator it;
		
		StrListItem * sitm = (StrListItem *) itm;
		DEBUGX("selected quest %s", sitm->m_data.c_str());
		
		m_questname = sitm->m_data;
		
		it = quests.find(m_questname);
		if (it != quests.end())
		{
			
			quest_descr->setText((CEGUI::utf8*) it->second->getDescription().c_str());
		}
		else
		{
			quest_descr->setText("");
		}
		
	}
	else
	{
		quest_descr->setText("");
	}
}

