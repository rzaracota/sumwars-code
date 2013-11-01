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

#include "charinfo.h"
#include "player.h"
#include "damage.h"

#include "ceguiutility.h"

CharInfo::CharInfo (Document* doc)
	: Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// The CharInfo window and holder
	CEGUI::FrameWindow* char_info = (CEGUI::FrameWindow*) (CEGUIUtility::loadLayoutFromFile ("characterscreen.layout"));
	if (!char_info)
	{
		DEBUG ("WARNING: Failed to load [%s]", "characterscreen.layout");
	}

	CEGUI::Window* char_info_holder = CEGUIUtility::loadLayoutFromFile ("characterscreen_holder.layout");
	if (!char_info_holder)
	{
		DEBUG ("WARNING: Failed to load [%s]", "characterscreen_holder.layout");
	}
	
	CEGUI::Window* wndHolder = CEGUIUtility::getWindow ("CharInfo_Holder");
	CEGUI::Window* wndCharInfo = CEGUIUtility::getWindow ("CharInfo");
	if (wndHolder && wndCharInfo)
	{
		CEGUIUtility::addChildWidget (wndHolder, wndCharInfo);
	}
	else
	{
		if (!wndHolder) DEBUG ("ERROR: Unable to get the window holder for char screen.");
		if (!wndCharInfo) DEBUG ("ERROR: Unable to get the window for char screen.");
	}

	m_window = char_info_holder;
	
	char_info->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	char_info->setWantsMultiClickEvents(false);

	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;


	
	// Label Level
	label = CEGUIUtility::getWindow ("LevelValueLabel");
	label->setText("1");

	// Label Staerke (Wert)
	label = CEGUIUtility::getWindow ("StrengthValueLabel");
	label->setText("20");

	// Button Staerke erhoehen
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow ("StrengthAddButton"));
	btn->setID(CreatureBaseAttr::STRENGTH);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));
	
	// Label Geschick (Wert)
	label = CEGUIUtility::getWindow ("DexterityValueLabel");
	label->setText("20");

	// Button Geschick erhoehen
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow ("DexterityAddButton"));
	btn->setID(CreatureBaseAttr::DEXTERITY);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));
	
	// Label Willenskraft (Wert)
	label = CEGUIUtility::getWindow ("WillpowerValueLabel");
	label->setText("20");

	// Button Willenskraft erhoehen
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow ("WillpowerAddButton"));
	btn->setID(CreatureBaseAttr::WILLPOWER);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));

	// Label Zauberkraft (Wert)
	label = CEGUIUtility::getWindow ("MagicpowerValueLabel");
	label->setText("20");

	// Button Zauberkraft erhoehen
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow ("MagicpowerAddButton"));
	btn->setID(CreatureBaseAttr::MAGIC_POWER);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));
	
	// Label  Attributpunkte (Wert)
	label = CEGUIUtility::getWindow ("AttrPointsValueLabel");
	label->setText("5");

	// Label Exp (Wert)
	label = CEGUIUtility::getWindow ("ExperienceValueLabel");
	label->setText("0/100");

	// Label HP (Wert)
	label = CEGUIUtility::getWindow ("HitpointsValueLabel");
	label->setText("0/100");

	// Label Attacke (Wert)
	label = CEGUIUtility::getWindow ("AttackValueLabel");
	label->setText("20");

	// Label Attacke (Wert)
	label = CEGUIUtility::getWindow ("AttackSpeedValueLabel");
	label->setText("20");

	// Label Reichweite (Wert)
	label = CEGUIUtility::getWindow ("RangeValueLabel");
	label->setText("2");

	// Label Durchschlagskraft
	label = CEGUIUtility::getWindow ("PowerValueLabel");
	label->setText("30");

	// Label Ruestung
	label = CEGUIUtility::getWindow ("ArmorValueLabel");
	label->setText("40");

	// Label Block
	label = CEGUIUtility::getWindow ("BlockValueLabel");
	label->setText("20");

	// Label Basisschaden
	label = CEGUIUtility::getWindow ("BaseDmgLabel");
	label->setText((CEGUI::utf8*) gettext("Base damage"));

	// Label Basisschaden (Wert)
	label = CEGUIUtility::getWindow ("BaseDmgValueLabel");
	label->setText("5-10");

	// Label Skill1 Schaden
	label = CEGUIUtility::getWindow ("Skill1DmgLabel");
	label->setText("Skill1");

	// Label Skill1 Schaden (Wert)
	label = CEGUIUtility::getWindow ("Skill1DmgValueLabel");
	label->setText("10-20");

	// Label Skill2 Schaden
	label = CEGUIUtility::getWindow ("Skill2DmgLabel");
	label->setText("Skill2");

	// Label Skill2 Schaden (Wert)
	label = CEGUIUtility::getWindow ("Skill2DmgValueLabel");
	label->setText("20-30");

	// Label physische Resistenz (Wert)
	label = CEGUIUtility::getWindow ("ResistPhysValueLabel");
	label->setText("0");

	// Label Feuerresistenz (Wert)
	label = CEGUIUtility::getWindow ("ResistFireValueLabel");
	label->setText("0");

	// Label Feuerresistenz (Wert)
	label = CEGUIUtility::getWindow ("ResistAirValueLabel");
	label->setText("0");

	// Label Eisresistenz (Wert)
	label = CEGUIUtility::getWindow ("ResistIceValueLabel");
	label->setText("0");
	
	if (CEGUIUtility::isWindowPresent ("CharInfoCloseButton"))
	{
		label = CEGUIUtility::getWindow ("CharInfoCloseButton");
		label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CharInfo::onCloseButtonClicked, this));
	}
	else
	{
		// If the panel also has an auto-close button, connect it to the Cancel/Abort event.
		if (CEGUIUtility::isWindowPresent ("CharInfo__auto_closebutton__"))
		{
			label = CEGUIUtility::getWindow ("CharInfo__auto_closebutton__");
			if (label)
			{
				label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CharInfo::onCloseButtonClicked, this));
			}
		}
	}


	updateTranslation();
}



 void CharInfo::update()
{
	// Spielerobjekt
	Player* player = m_document->getLocalPlayer();
	FightStatistic* fstat = &(player->getFightStatistic());
	std::string tooltip;
	CEGUI::String ttext="";

	// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	std::ostringstream out_stream;

	// Label Name
	// Prepare the name
	out_stream.str("");
	out_stream.str(player->getName().getRawText());
	if (CEGUIUtility::isWindowPresent ("NameLabel"))
	{
		label = CEGUIUtility::getWindow ("NameLabel");
		if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
		{
			label->setText((CEGUI::utf8*) out_stream.str().c_str());
		}
	}
	else if (CEGUIUtility::isWindowPresent ("CharInfo"))
	{
		label = CEGUIUtility::getWindow ("CharInfo");
		if (label->isPropertyPresent ("Text"))
		{
			label->setProperty ("Text", (CEGUI::utf8*) out_stream.str().c_str());
		}
	}


	// Label Klasse
	label = CEGUIUtility::getWindow ("ClassValueLabel");
	PlayerBasicData* pdata = ObjectFactory::getPlayerData(player->getSubtype());
	if (pdata != 0)
	{
		out_stream.str(gettext(pdata->m_name.c_str()));
	}
	else
	{
		out_stream.str("");
	}
	
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		DEBUG("set class label");
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Level
	label = CEGUIUtility::getWindow ("LevelValueLabel");
	out_stream.str("");
	out_stream << (int) player->getBaseAttr()->m_level;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Staerke
	label = CEGUIUtility::getWindow ("StrengthValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_strength;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Zauberkraft
	label = CEGUIUtility::getWindow ("MagicpowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_magic_power;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Willenskraft
	label = CEGUIUtility::getWindow ("WillpowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_willpower;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Geschick
	label = CEGUIUtility::getWindow ("DexterityValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_dexterity;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Buttons zum erhoehen von Attributen sind nur sichtbar wenn Attributpunkt zu verteilen sind
	bool add_but_vis = false;
	if ( player->getAttributePoints()>0)
	{
		add_but_vis=true;
	}

	// Sichtbarkeit der Buttons setzen
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow ("StrengthAddButton"));
	btn->setVisible(add_but_vis);
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow ("DexterityAddButton"));
	btn->setVisible(add_but_vis);
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow ("WillpowerAddButton"));
	btn->setVisible(add_but_vis);
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow ("MagicpowerAddButton"));
	btn->setVisible(add_but_vis);

	// Label freie Attributspunkte
	label = CEGUIUtility::getWindow ("AttrPointsValueLabel");
	out_stream.str("");
	out_stream << gettext("You have") << " " << player->getAttributePoints() << " " << gettext("attribute points to spend");
	//label->setVisible(add_but_vis);
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Ruestung
	label = CEGUIUtility::getWindow ("ArmorValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_armor;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	out_stream.str("");
	if (fstat->m_last_attacker.getTranslation() != "")
	{
		out_stream << gettext("Damage dealt by ")<<fstat->m_last_attacker.getTranslation()<<": "<<(int) (fstat->m_damage_got_perc*100) << "%";
	}
	if (label->getTooltipText() != (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setTooltipText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Attacke
	label = CEGUIUtility::getWindow ("AttackValueLabel");
	out_stream.str("");
	out_stream << player->getBaseDamage().m_attack;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	out_stream.str("");
	if (fstat->m_last_attacked.getTranslation()  != "")
	{
		out_stream << gettext("Chance to hit ")<< fstat->m_last_attacked.getTranslation() <<": "<<(int) (fstat->m_hit_chance*100) << "%";
	}
	if (label->getTooltipText() != (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setTooltipText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Block
	label = CEGUIUtility::getWindow ("BlockLabel" );
	CEGUI::utf8* blocktext;
	if (player->getShield() != 0)
	{	
		blocktext = (CEGUI::utf8*) gettext("Block");
	}
	else
	{
		blocktext = (CEGUI::utf8*) gettext("Evade");
	}
	if (label->getText() != blocktext)
	{
		label->setText(blocktext);
	}
	
	label = CEGUIUtility::getWindow ("BlockValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_block;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	out_stream.str("");
	if (fstat->m_last_attacker.getTranslation() != "")
	{
		out_stream << gettext("Chance to get hit by ")<<fstat->m_last_attacker.getTranslation()<<": "<<(int) (100-fstat->m_block_chance*100) << "%";
	}
	if (label->getTooltipText() != (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setTooltipText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label HP
	label = CEGUIUtility::getWindow ("HitpointsValueLabel");
	out_stream.str("");
	out_stream << (int) player->getDynAttr()->m_health <<"/" <<(int) player->getBaseAttrMod()->m_max_health;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Exp
	label = CEGUIUtility::getWindow ("ExperienceValueLabel");
	out_stream.str("");
	out_stream << (unsigned int) player->getDynAttr()->m_experience <<"/" <<(unsigned int) player->getBaseAttr()->m_max_experience;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Phys Resistenz
	label = CEGUIUtility::getWindow ("ResistPhysValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::PHYSICAL];
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Feuer Resistenz
	label = CEGUIUtility::getWindow ("ResistFireValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::FIRE];
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Eis Resistenz
	label = CEGUIUtility::getWindow ("ResistIceValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::ICE];
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Luft Resistenz
	label = CEGUIUtility::getWindow ("ResistAirValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::AIR];
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Angriffsgeschwindigkeit
	label = CEGUIUtility::getWindow ("AttackSpeedValueLabel");
	out_stream.str("");
	out_stream << 0.001*MathHelper::Min((short) 5000, player->getBaseAttrMod()->m_attack_speed);
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Reichweite
	label = CEGUIUtility::getWindow ("RangeValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_attack_range;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Durchschlagskraft
	label = CEGUIUtility::getWindow ("PowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseDamage().m_power;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	out_stream.str("");
	if (fstat->m_last_attacked.getTranslation() != "")
	{
		out_stream << gettext("Damage dealt to  ")<< fstat->m_last_attacked.getTranslation()<<": "<<(int) (fstat->m_damage_dealt_perc*100) << "%";
	}
	if (label->getTooltipText() != (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setTooltipText((CEGUI::utf8*) out_stream.str().c_str());
	}

	Damage dmgb = player->getBaseDamage();
	Damage dmgl = player->getLeftDamage();
	Damage dmgr = player->getRightDamage();
	
	dmgb.normalize();
	dmgl.normalize();
	dmgr.normalize();
	
	
	// Schaden Basisattacke
	float minb=dmgb.getSumMinDamage();
	float maxb=dmgb.getSumMaxDamage();
	// Schaden Attacke links
	float minl=dmgl.getSumMinDamage();
	float maxl=dmgl.getSumMaxDamage();
	// Schaden Attacke rechts
	float minr=dmgr.getSumMinDamage();
	float maxr=dmgr.getSumMaxDamage();



	// Label Schaden Basisattacke
	label = CEGUIUtility::getWindow ( "BaseDmgLabel");
	out_stream.str("");
	if (pdata != 0)
	{
		ttext = (CEGUI::utf8*) (Action::getDescription(pdata->m_base_ability).c_str());
		out_stream << Action::getName(pdata->m_base_ability);	
	}
	else
	{
		ttext= (CEGUI::utf8*) "";
	}
	
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	if (ttext != label->getTooltipText())
	{
		label->setTooltipText(ttext);
	}

	// Label Basisschaden
	label = CEGUIUtility::getWindow ("BaseDmgValueLabel");
	out_stream.str("");
	out_stream << (int) minb << "-" << (int) maxb;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	tooltip =dmgb.getDamageString();
	if (tooltip != label->getTooltipText())
	{
		label->setTooltipText(tooltip);
	}

	// Label Attacke links
	label = CEGUIUtility::getWindow ( "Skill1DmgLabel");
	out_stream.str("");
	out_stream << Action::getName(player->getLeftAction());
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	out_stream.str("");
	out_stream << Action::getDescription(player->getLeftAction());
	CEGUI::String str((CEGUI::utf8*)  out_stream.str().c_str());
	if (label->getTooltipText() != str)
	{
		
		label->setTooltipText(str);
	}

	
	// Label Schaden Attacke links
	label = CEGUIUtility::getWindow ( "Skill1DmgValueLabel");
	out_stream.str("");
	out_stream << (int) minl << "-" << (int) maxl;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	ttext = (CEGUI::utf8*) (dmgl.getDamageString().c_str());
	if (ttext != label->getTooltipText())
	{
		label->setTooltipText(ttext);
	}
	



	// Label Attacke rechts
	label = CEGUIUtility::getWindow ( "Skill2DmgLabel");
	out_stream.str("");
	out_stream << Action::getName(player->getRightAction());
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	out_stream.str("");
	out_stream << Action::getDescription(player->getRightAction());
	CEGUI::String str2((CEGUI::utf8*)  out_stream.str().c_str());
	if (label->getTooltipText() != str2)
	{
		
		label->setTooltipText(str2);
	}

	// Label Schaden Attacke rechts
	Action::TargetType dist = Action::getActionInfo(player->getRightAction())->m_target_type;
	label = CEGUIUtility::getWindow ( "Skill2DmgValueLabel");
	if (dist == Action::MELEE || dist == Action::RANGED || dist == Action::CIRCLE)
	{	
		out_stream.str("");
		out_stream << (int) minr << "-" << (int) maxr;
		if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
		{
			label->setText((CEGUI::utf8*) out_stream.str().c_str());
		}
		ttext = (CEGUI::utf8*) dmgr.getDamageString().c_str();
		if (ttext != label->getTooltipText())
		{
			label->setTooltipText(ttext);
		}
	}
	else
	{
		if (label->getText()!= "")
			label->setText("");
		if (label->getTooltipText()!="")
			label->setTooltipText("");
	}
	
	label = CEGUIUtility::getWindow ("Portrait");
	std::string portraitname = player->getEmotionImage("normal");
	if (label->getProperty("Image") != portraitname)
	{
		label->setProperty("Image",portraitname);
	}
	
}

void CharInfo::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;

	if (CEGUIUtility::isWindowPresent ("NameLabel"))
	{
		label = CEGUIUtility::getWindow ("NameLabel");
		label->setText((CEGUI::utf8*) gettext("Name"));
	}
	else if (CEGUIUtility::isWindowPresent ("CharInfo"))
	{
		label = CEGUIUtility::getWindow ("CharInfo");
		if (label->isPropertyPresent ("Text"))
		{
			label->setProperty ("Text", (CEGUI::utf8*) gettext("Name"));
		}
	}
	
	label = CEGUIUtility::getWindow ("Playerinfo");
	label->setText((CEGUI::utf8*) gettext("Player info"));
	
	label = CEGUIUtility::getWindow ("Attributes");
	label->setText((CEGUI::utf8*) gettext("Attributes"));
	
	label = CEGUIUtility::getWindow ("Offense");
	label->setText((CEGUI::utf8*) gettext("Offense"));
	
	label = CEGUIUtility::getWindow ("Defense");
	label->setText((CEGUI::utf8*) gettext("Defense"));

	label = CEGUIUtility::getWindow ("LevelLabel");
	label->setText((CEGUI::utf8*) gettext("Level"));

	label = CEGUIUtility::getWindow ("ClassLabel");
	label->setText((CEGUI::utf8*) gettext("Class"));
	
	
	label = CEGUIUtility::getWindow ("StrengthLabel");
	label->setText((CEGUI::utf8*) gettext("Strength"));
	label->setTooltipText((CEGUI::utf8*) gettext("Strength increases hitpoints, melee damage and attack power."));
	
	label = CEGUIUtility::getWindow ("DexterityLabel");
	label->setText((CEGUI::utf8*) gettext("Dexterity"));
	label->setTooltipText((CEGUI::utf8*) gettext("Dexterity increases ranged damage, attack accuracy and attack speed"));
	
	label = CEGUIUtility::getWindow ("WillpowerLabel");
	label->setText((CEGUI::utf8*) gettext("Willpower"));
	label->setTooltipText((CEGUI::utf8*) gettext("Willpower decreases the duration and the impact of status changes \n and increases magical melee damage."));
	
	label = CEGUIUtility::getWindow ("MagicpowerLabel");
	label->setText((CEGUI::utf8*) gettext("Magic power"));
	label->setTooltipText((CEGUI::utf8*) gettext("Magic power increases the damage of magical attacks."));

	label = CEGUIUtility::getWindow ("ExperienceLabel" );
 	label->setText((CEGUI::utf8*) gettext("Experience"));
	label->setTooltipText((CEGUI::utf8*) gettext("Current experience and experience necessary for next level-up."));
	
	label = CEGUIUtility::getWindow ("HitpointsLabel");
	label->setText((CEGUI::utf8*) gettext("Hitpoints"));
	label->setTooltipText((CEGUI::utf8*) gettext("Current and maximal hitpoints."));
	
	
	label = CEGUIUtility::getWindow ("AttackLabel" );
	label->setText((CEGUI::utf8*) gettext("Attack"));
	label->setTooltipText((CEGUI::utf8*) gettext("Attack value determines the chance to hit enemies that can block or evade."));
	
	
	label = CEGUIUtility::getWindow ("AttackSpeedLabel" );
	label->setText((CEGUI::utf8*) gettext("Attacks/s"));
	label->setTooltipText((CEGUI::utf8*) gettext("Number of attacks per second."));
	
	
	label = CEGUIUtility::getWindow ( "RangeLabel");
	label->setText((CEGUI::utf8*) gettext("Range"));
	label->setTooltipText((CEGUI::utf8*) gettext("Weapon Range in meters."));
	
	
	label = CEGUIUtility::getWindow ("PowerLabel" );
	label->setText((CEGUI::utf8*) gettext("Power"));
	label->setTooltipText((CEGUI::utf8*) gettext("Attack power influences the damage dealt to armored enemies."));
	
	
	label = CEGUIUtility::getWindow ( "ArmorLabel");
	label->setText((CEGUI::utf8*) gettext("Armor"));
	label->setTooltipText((CEGUI::utf8*) gettext("Armor reduces the damage of physical attacks."));
	
	
	label = CEGUIUtility::getWindow ("BlockLabel" );
	label->setTooltipText((CEGUI::utf8*) gettext("Block value determines the chance to avoid damage from physical attacks."));
	
	
	label = CEGUIUtility::getWindow ( "ResistPhysLabel");
	label->setText((CEGUI::utf8*) gettext("Physical"));
	
	label = CEGUIUtility::getWindow ("ResistAirLabel" );
	label->setText((CEGUI::utf8*) gettext("Air"));
	
	label = CEGUIUtility::getWindow ("ResistIceLabel" );
	label->setText((CEGUI::utf8*) gettext("Ice"));
	
	label = CEGUIUtility::getWindow ("ResistFireLabel" );
	label->setText((CEGUI::utf8*) gettext("Fire"));
	
	
}


bool CharInfo::onIncreaseAttributeButtonClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	DEBUGX("left button pressed on skill %i",id);
	m_document->increaseAttribute((CreatureBaseAttr::Attribute) id);
	return true;
}

bool CharInfo::onCloseButtonClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonCharInfoClicked();
	return true;
}



