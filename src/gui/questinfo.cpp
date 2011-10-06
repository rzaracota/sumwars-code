/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "questinfo.h"

QuestInfo::QuestInfo (Document* doc)
	:Window(doc)
{
	m_questname = "";
	
	DEBUGX("setup main menu");
	// GUI Elemente erzeugen

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	

	// Rahmen fuer das Menue Savegame auswaehlen
	CEGUI::FrameWindow* quest_info = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "QuestInfo");
	m_window = quest_info;
	
	quest_info->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f))); //0.0/0.8
	quest_info->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.8f))); //1.0/0.2
	quest_info->setProperty("FrameEnabled","false");
	quest_info->setProperty("TitlebarEnabled","false");
	quest_info->setProperty("CloseButtonEnabled","false");
	quest_info->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	
	// Bestandteile der Kontrollleiste hinzufuegen
	CEGUI::Window* label;

	
	CEGUI::Listbox* questlist = (CEGUI::Listbox*) win_mgr.createWindow("TaharezLook/Listbox", "QuestList");
	quest_info->addChildWindow(questlist);
	questlist->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f)));
	questlist->setSize(CEGUI::UVector2(cegui_reldim(0.9f), cegui_reldim( 0.2f)));
	questlist->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&QuestInfo::onQuestSelected, this));

	
	CEGUI::Checkbox * box;
	
	
	box = (CEGUI::Checkbox *) win_mgr.createWindow("TaharezLook/Checkbox", "QuestOpenBox");
	quest_info->addChildWindow(box);
	box->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.28f)));
	box->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f)));
	box->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&QuestInfo::onFilterSelected, this));
	box->setSelected(true);
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "QuestOpenLabel");
	quest_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.28f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.05f)));
	
	box = (CEGUI::Checkbox *) win_mgr.createWindow("TaharezLook/Checkbox", "QuestDoneBox");
	quest_info->addChildWindow(box);
	box->setPosition(CEGUI::UVector2(cegui_reldim(0.35f), cegui_reldim( 0.28f)));
	box->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f)));
	box->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&QuestInfo::onFilterSelected, this));
	box->setSelected(false);
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "QuestDoneLabel");
	quest_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.28f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.05f)));
	
	box = (CEGUI::Checkbox *) win_mgr.createWindow("TaharezLook/Checkbox", "QuestFailedBox");
	quest_info->addChildWindow(box);
	box->setPosition(CEGUI::UVector2(cegui_reldim(0.65f), cegui_reldim( 0.28f)));
	box->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05f)));
	box->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&QuestInfo::onFilterSelected, this));
	box->setSelected(false);
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "QuestFailedLabel");
	quest_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim( 0.28f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.05f)));
	
	CEGUI::MultiLineEditbox* quest_descr;
	quest_descr = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.createWindow("TaharezLook/MultiLineEditbox", "QuestDescription"));
	quest_info->addChildWindow(quest_descr);
	quest_descr->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.35f)));
	quest_descr->setSize(CEGUI::UVector2(cegui_reldim(0.9f), cegui_reldim( 0.6f)));
	quest_descr->setWantsMultiClickEvents(false);
	quest_descr->setReadOnly(true);
	quest_descr->setText("");
	
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
		if (state == Quest::STARTED && open || state == Quest::FINISHED && done || state == Quest::FAILED && failed)
		{
			CEGUI::utf8* name = (CEGUI::utf8*) quest->getName().getTranslation().c_str();
			newitem = new StrListItem(name,it->first);
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

