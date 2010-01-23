
#include "skilltree.h"


SkillTree::SkillTree(Document* doc, OIS::Keyboard *keyboard)
	:Window(doc)
{
	DEBUGX("setup skilltree");
	m_keyboard = keyboard;
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();


	// Rahmen fuer Skilltree Fenster

	// Bestandteile des Charakterfensters hinzufuegen

	std::ostringstream outStream;
	std::string name;
	std::string lname,bname;
	CEGUI::UVector2 pos;
	
	m_player_id =0;
	
	CEGUI::TabControl* skilltree = (CEGUI::TabControl*) win_mgr.createWindow("TaharezLook/TabControl", "Skilltree");
	m_window = skilltree;
	skilltree->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	skilltree->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	skilltree->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent,  (Window*) this));
	skilltree->setWantsMultiClickEvents(false);
	skilltree->setAlpha(0.7);
	
	m_nr_tabs =0;
	m_nr_skills =0;
	
}

void SkillTree::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	Player* player = m_document->getLocalPlayer();
	CEGUI::TabControl* skilltree = (CEGUI::TabControl*) win_mgr.getWindow("Skilltree");
	
	CEGUI::Window* wnd;
	std::vector<CEGUI::DefaultWindow*> tabs(3);
	CEGUI::Window* label;
	CEGUI::PushButton* button;
	std::stringstream stream;
	
	if (player->getId() != m_player_id)
	{
		m_player_id = player->getId();
		DEBUGX("new Player id %i",m_player_id);
		
		// alle bisherigen Tabs und Skills entfernen
		
		for (int i=0; i<m_nr_skills; i++)
		{
			stream.str("");
			stream << "SkillImage"<<i;
			wnd = win_mgr.getWindow(stream.str());
			
			wnd->getParent()->removeChildWindow(wnd);
			win_mgr.destroyWindow(wnd);
			
			stream.str("");
			stream << "SkillButton"<<i;
			wnd = win_mgr.getWindow(stream.str());
			
			wnd->getParent()->removeChildWindow(wnd);
			win_mgr.destroyWindow(wnd);
		}
		
		for (int i=0; i<m_nr_tabs; i++)
		{
			stream.str("");
			stream << "SkilltreeTab"<<i;
			wnd = win_mgr.getWindow(stream.str());
			
			skilltree->removeTab(wnd->getID());
			win_mgr.destroyWindow(wnd);
		}
		
		// neue Tabs
		// TODO: generisch ausprogrammieren
		
		m_nr_tabs =3;
		for (int i=0; i<m_nr_tabs; i++)
		{
			stream.str("");
			stream << "SkilltreeTab" << i;
			
			tabs[i] = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", stream.str());
			skilltree->addTab(tabs[i]);
		}
		
		CEGUI::UVector2 pos;
		std::string name,lname,bname;
		Vector spos;
		
		DEBUGX("nr of abilities %i", player->getLearnableAbilities().size());
		
		std::map<int,LearnableAbility>::iterator it;
		int cnt =0;
		for (it = player->getLearnableAbilities().begin(); it != player->getLearnableAbilities().end(); ++it)
		{
			// Label mit dem Bild
			DEBUGX("ability %s nr %i",it->second.m_type.c_str(),cnt);
			stream.str("");
			stream << "SkillImage"<<cnt;
			label = win_mgr.createWindow("TaharezLook/StaticImage", stream.str());
			tabs[it->second.m_skilltree_tab-1]->addChildWindow(label);
			
			spos = it->second.m_skilltree_position;
			pos = CEGUI::UVector2(cegui_reldim(spos.m_x), cegui_reldim( spos.m_y));
			label->setPosition(pos);
			label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
			// Button zum Faehigkeit lernen
			pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
			stream.str("");
			stream << "SkillButton"<<cnt;
			button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", stream.str()));
			tabs[it->second.m_skilltree_tab-1]->addChildWindow(button);
			button->setPosition(pos);
			button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
			
			// beschriften und verknuepfen
			label->setProperty("FrameEnabled", "true");
			label->setProperty("BackgroundEnabled", "true");
			label->setID(it->first);
			std::string imagename = it->second.m_image;
			if (imagename == "")
			{
				imagename = "set:skills image:";
				imagename += it->second.m_type;
			}
			label->setProperty("Image", imagename);
			label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillMouseClicked, this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&SkillTree::onAbilityHover, this));

			button->setText("+");
			button->setID(it->first);
			button->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillLearnMouseClicked, this));
			
			cnt ++;
		}
		
		m_nr_skills = cnt;
		
		updateTranslation();
	}
	
	std::ostringstream out_stream;
	std::map<int,LearnableAbility> &ablts = player->getLearnableAbilities();
	bool vis;
	Action::ActionType act;
	
	// Schleife ueber alle Skill labels
	for (int j=0;j<m_nr_skills;j++)
	{
		out_stream.str("");
		out_stream << "SkillImage" << j;

		// Label welches das Skillicon zeigt
		label = win_mgr.getWindow(out_stream.str());

		// Alpha Wert des Labels
		float alpha = 0.2;
		
		act = ablts[j].m_type;

		if (player->checkAbility(act))
		{
			// Faehigkeit steht zur Verfuegung
			alpha = 1.0;
		}
		else
		{
			alpha = 0.5;
		}

		if (label->getAlpha() != alpha)
		{
			label->setAlpha(alpha);
		}
		
		out_stream.str("");
		out_stream << "SkillButton" << j;
		
		button = static_cast<CEGUI::PushButton*>(win_mgr.getWindow(out_stream.str()));
		vis = player->checkAbilityLearnable(act) && (player->getSkillPoints()>0);
		if (button->isVisible() != vis)
		{
			button->setVisible(vis);
		}
	}
	
	
	// Markierer fuer Shortkeys einbauen
	// Zaehler fuer die Fenster
	static int acount =0;
	
	Document::ShortkeyMap& shortkeys = m_document->getShortkeys();
	Document::ShortkeyMap::iterator it;
	int nr =0;
	
	CEGUI::Window *label2;
	bool right;
	std::string keyname;
	int key;
	CEGUI::UVector2 pos;
	
	for (it = shortkeys.begin(); it != shortkeys.end(); ++it)
	{
		if (it->second < Document::USE_SKILL_LEFT || it->second >Document::USE_SKILL_RIGHT+200)
			continue;
		
		DEBUGX("shortkey %i to %i",it->first,it->second);
		key = it->first;
		
		right = false;
		int id =0;
		if (it->second >= Document::USE_SKILL_RIGHT)
		{
			id = it->second - Document::USE_SKILL_RIGHT;
			right = true;
		}
		else
		{
			id = it->second - Document::USE_SKILL_LEFT;
		}
		
		if (ablts.count(id) ==0)
			continue;
		act= ablts[id].m_type;
		
		stream.str("");
		stream << "SkillShortkeyLabel";
		stream << nr;
		
		if (nr >= acount)
		{
			acount ++;
			label = win_mgr.createWindow("TaharezLook/StaticText", stream.str());
			label->setProperty("FrameEnabled", "false");
			label->setProperty("BackgroundEnabled", "false");
			label->setProperty("HorzFormatting", "Centred");
			label->setID(0);
			label->setText("");
			label->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
			label->setAlpha(1.0);
			label->setMousePassThroughEnabled(true);
			label->setAlwaysOnTop(true);
		}
		else
		{
			label = win_mgr.getWindow(stream.str());
		}
		
		stream.str("");
		stream << "SkillImage" << id;
		label2 = win_mgr.getWindow(stream.str());
		
		
		if (label2->getParent() != label->getParent())
		{
			if (label->getParent() != 0)
			{
				label->getParent()->removeChildWindow (label);
			}
			label2->getParent()->addChildWindow (label);
		}
		
		keyname = m_keyboard->getAsString ( (OIS::KeyCode) key);
		if (label->getText() != keyname)
		{
			label->setText(keyname);
		}
		
		if ((int) label->getID() != id)
		{
			label->setID(id);
			pos = label2->getPosition();
			if (right)
			{
				pos += CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f));
			}
			else
			{
				pos += CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim( 0.05f));
			}
			label->setPosition(pos);
		}
		
		if (!label->isVisible())
		{
			label->setVisible(true);
		}
		nr ++;
	}
	
	
	// restliche Labels ausblenden
	for (; nr<acount; nr++)
	{
		stream.str("");
		stream << "SkillShortkeyLabel";
		stream << nr;
			
		label = win_mgr.getWindow(stream.str());
		label->setVisible(false);
	}
	
}


void SkillTree::updateTranslation()
{
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	std::vector<CEGUI::DefaultWindow*> tabs(m_nr_tabs);
	
	std::stringstream stream;
	
	for (int i=0; i<m_nr_tabs; i++)
	{
		stream.str("");
		stream << "SkilltreeTab" << i;
			
		tabs[i] = (CEGUI::DefaultWindow*) win_mgr.getWindow(stream.str());
	}
	
	// Ueberschriften der Tabs setzen
	Player* player = m_document->getLocalPlayer();
	if (m_nr_tabs <3)
		return;
	
	PlayerBasicData* pdata = ObjectFactory::getPlayerData(player->getSubtype());
	for (int i=0; i<3; i++)
	{
		tabs[i]->setText((CEGUI::utf8*) gettext(pdata->m_tabnames[i].c_str()));
	}
	
}

void SkillTree::updateAbilityTooltip(unsigned int pos)
{
	Player* player = m_document->getLocalPlayer();
	std::map<int,LearnableAbility> &ablts = player->getLearnableAbilities();
	
	
	DEBUGX("update tooltip for %i %s", pos, ablts[pos].m_type.c_str());

		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Window* label;
		std::ostringstream out_stream;
		out_stream.str("");
		out_stream << "SkillImage" << pos;

		label = win_mgr.getWindow(out_stream.str());

		std::string tooltip = m_document->getAbilityDescription(ablts[pos].m_type);

		label->setTooltipText((CEGUI::utf8*) tooltip.c_str());

	

}

bool SkillTree::onSkillMouseClicked(const CEGUI::EventArgs& evt)
{
	Player* player = m_document->getLocalPlayer();
	std::map<int,LearnableAbility> &ablts = player->getLearnableAbilities();
	
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();

	if (we.button == CEGUI::LeftButton)
	{
		m_document->setLeftAction(ablts[id].m_type);
	}

	if (we.button == CEGUI::RightButton)
	{
		DEBUGX("right button pressed on skill %i",id);
		m_document->setRightAction(ablts[id].m_type);
	}
	
	return true;
	
}

bool SkillTree::onSkillLearnMouseClicked(const CEGUI::EventArgs& evt)
{
	Player* player = m_document->getLocalPlayer();
	std::map<int,LearnableAbility> &ablts = player->getLearnableAbilities();
	
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	
	// versuche Faehigkeit zu lernen
	m_document->learnAbility(ablts[id].m_type);
	
	return true;
}

bool SkillTree::onAbilityHover(const CEGUI::EventArgs& evt)
{

	Player* player = m_document->getLocalPlayer();
	std::map<int,LearnableAbility> &ablts = player->getLearnableAbilities();
	
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	DEBUGX("mouse entered Ability %i",id);
	updateAbilityTooltip(id);
	m_document->getGUIState()->m_hover_ability = ablts[id].m_type;
	
	return true;
}

bool SkillTree::onAbilityHoverLeave(const CEGUI::EventArgs& evt)
{
	m_document->getGUIState()->m_hover_ability = "noaction";
	return true;
}

