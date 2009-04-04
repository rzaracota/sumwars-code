
#include "skilltree.h"


SkillTree::SkillTree(Document* doc, OIS::Keyboard *keyboard)
	:Window(doc)
{
	DEBUG4("setup skilltree");
	m_keyboard = keyboard;
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();


	// Rahmen fuer Skilltree Fenster

	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::Window* label;
	CEGUI::PushButton* button;
	CEGUI::DefaultWindow* tab;

	std::ostringstream outStream;
	std::string name;
	std::string lname,bname;
	unsigned int act;
	CEGUI::UVector2 pos;
	
	m_skill_position[Action::FIRE_BOLT] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action:: FIRE_STRIKE] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f));
	m_skill_position[Action::FIRE_WAVE] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.65f));
	m_skill_position[Action::FIRE_STORM] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.85f));
	m_skill_position[Action::FIRE_BALL] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	m_skill_position[Action::INFERNO_BALL] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f));
	m_skill_position[Action::FIRE_WALL] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f));
	m_skill_position[Action::INFLAME] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.45f));
	
	m_skill_position[Action::ICE_BOLT] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::ICE_SPIKE] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	m_skill_position[Action::SNOW_STORM] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f));
	m_skill_position[Action::BLIZZARD] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.85f));
	m_skill_position[Action::ICE_RING] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::FROST_RING] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f));
	m_skill_position[Action::FREEZE] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f));
	m_skill_position[Action::CHILL] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f));
	
	m_skill_position[Action::LIGHTNING] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::LIGHTNING_STRIKE] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	m_skill_position[Action::THUNDERSTORM] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f));
	m_skill_position[Action::THUNDERSTORM2] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.85f));
	m_skill_position[Action::CHAIN_LIGHTNING] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::CHAIN_LIGHTNING2] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.45f));
	m_skill_position[Action::STATIC_SHIELD] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.65f));
	m_skill_position[Action::IONISATION] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f));

	m_skill_position[Action::BASH] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::HAMMER_BASH] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f));
	m_skill_position[Action::AROUND_BLOW] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::WHIRL_BLOW] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f));
	m_skill_position[Action::SMASH] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f));
	m_skill_position[Action::HATE_MAGE] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	m_skill_position[Action::CHARGE] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f));
	m_skill_position[Action::STORM_CHARGE] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.85f));
	
	m_skill_position[Action::BLOCK] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	m_skill_position[Action::STEADFAST] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::MONSTER_HUNTER] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::MONSTER_SLAYER] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f));
	m_skill_position[Action::ENDURANCE] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.85f));
	m_skill_position[Action::WEAPONMASTER] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f));
	
	m_skill_position[Action::FIRESWORD] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	m_skill_position[Action::FLAMESWORD] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.65f));
	m_skill_position[Action::FLAMEARMOR] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f));
	m_skill_position[Action::BERSERK] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::WARCRY] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.85f));
	m_skill_position[Action::REGENERATE] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f));
	m_skill_position[Action::DECOY] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::SCARE] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.45f));
	m_skill_position[Action::ANGER] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f));
	m_skill_position[Action::FURY] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f));
	
	m_skill_position[Action::TRIPLE_SHOT] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::GUIDED_TRIPLE_SHOT] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f));
	m_skill_position[Action::MULTISHOT] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f));
	m_skill_position[Action::VOLLEY_SHOT] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f));
	m_skill_position[Action::PIERCE] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.05f));
	m_skill_position[Action::WEAK_POINT] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::BLIND_RAGE] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.45f));
	m_skill_position[Action::VACUUM] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	m_skill_position[Action::DEATH_ROULETTE] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f));
	m_skill_position[Action::EXPLODING_ARROW] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f));
	m_skill_position[Action::EXPLOSION_CASCADE] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.85f));
	
	m_skill_position[Action::EVADE] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::CRITICAL_STRIKE] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::CONCENTRATION] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f));
	m_skill_position[Action::MENTAL_WALL] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.85f));
	m_skill_position[Action::RESIST_ICE] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	m_skill_position[Action::RESIST_AIR] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.65f));
	
	m_skill_position[Action::AIMED_SHOT] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::BOW_SPIRIT] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f));
	m_skill_position[Action::ICE_ARROWS] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f));
	m_skill_position[Action::FREEZING_ARROWS] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.85f));
	m_skill_position[Action::WIND_ARROWS] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::STORM_ARROWS] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f));
	m_skill_position[Action::WIND_WALK] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	
	m_skill_position[Action::HOLY_LIGHT] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.45f));
	m_skill_position[Action::HOLY_FIRE] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f));
	m_skill_position[Action::BURNING_RAGE] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::CURE_BLIND_MUTE] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	m_skill_position[Action:: CURE_BLIND_MUTE_PARTY] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.65f));
	m_skill_position[Action::BLAZING_SHIELD] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::LIGHT_BEAM] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f));
	m_skill_position[Action::BURNING_SUN] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.85f));
	
	m_skill_position[Action::BREAK_BINDING] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.45f));
	m_skill_position[Action::DISRUPT_BINDING] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.85f));
	m_skill_position[Action::MAGIC_SHIELD] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::CURE_POIS_BURN] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	m_skill_position[Action::CURE_POIS_BURN_PARTY] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.65f));
	m_skill_position[Action::ACID] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f));
	m_skill_position[Action::HEAL] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::HEAL_PARTY] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f));
	
	m_skill_position[Action::DIVINE_WIND] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f));
	m_skill_position[Action::DIVINE_STORM] = CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f));
	m_skill_position[Action::BLADE_STORM] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f));
	m_skill_position[Action::CURE_CONF_BSRK] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f));
	m_skill_position[Action::CURE_CONF_BSRK_PARTY] = CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f));
	m_skill_position[Action::HYPNOSIS] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f));
	m_skill_position[Action::HYPNOSIS2] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.85f));
	m_skill_position[Action::KEEN_MIND] = CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f));
	
	
	// Skilltree Magier
	CEGUI::TabControl* skilltree_mage = (CEGUI::TabControl*) win_mgr.createWindow("TaharezLook/TabControl", "SkilltreeMage");
	m_windows[0] = skilltree_mage;
	
	skilltree_mage->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	skilltree_mage->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	skilltree_mage->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent,  (Window*) this));
	skilltree_mage->setWantsMultiClickEvents(false);
	skilltree_mage->setAlpha(0.7);

	// Tab Feuermagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeMage_1");
	skilltree_mage->addTab(tab);

	name =Action::getActionInfo(Action::MAGIC_ATTACK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Action::MAGIC_ATTACK);
	label->setProperty("Image", "set:skills image:magic_attack");
	label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillMouseClicked, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&SkillTree::onAbilityHover, this));

	for (int i = Action::FIRE_BOLT; i <= Action::INFLAME; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	}

	// Tab Eismagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeMage_2");
	skilltree_mage->addTab(tab);
	
	for (int i = Action::ICE_BOLT; i <= Action::CHILL; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
		
	}


	// Tab Luftmagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeMage_3");
	skilltree_mage->addTab(tab);
	
	for (int i = Action::LIGHTNING; i <= Action::IONISATION; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	}
	
	// Label beschriften und verknuepfen

	for (act =Action::FIRE_BOLT;act <=Action::IONISATION;act++)
	{
		name =Action::getActionInfo((Action::ActionType) act)->m_enum_name;
		//lname =Action::getName((Action::ActionType) act);
		//label = win_mgr.getWindow(name.append( "Label"));
		label = win_mgr.getWindow(name + "Label");
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setID(act);
		label->setProperty("Image", "set:skills image:" + name);
		label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillMouseClicked, this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&SkillTree::onAbilityHover, this));

		button = static_cast<CEGUI::PushButton*>(win_mgr.getWindow(name + "Button"));
		button->setText("+");
		button->setID(act);
		button->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillLearnMouseClicked, this));
	}


	skilltree_mage->setVisible(false);


	// Skilltree Krieger
	CEGUI::TabControl* skilltree_warrior = (CEGUI::TabControl*) win_mgr.createWindow("TaharezLook/TabControl", "SkilltreeWarrior");
	m_windows[1] = skilltree_warrior;

	
	skilltree_warrior->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	skilltree_warrior->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	skilltree_warrior->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&SkillTree::consumeEvent,  (Window*) this));
	skilltree_warrior->setWantsMultiClickEvents(false);
	skilltree_warrior->setAlpha(0.7);


	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeWarrior_1");
	skilltree_warrior->addTab(tab);


	name =Action::getActionInfo(Action::ATTACK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Action::ATTACK);
	label->setProperty("Image", "set:skills image:attack");
	label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillMouseClicked, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&SkillTree::onAbilityHover, this));

	for (int i = Action::BASH; i <= Action::STORM_CHARGE; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	}
	

	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeWarrior_2");
	skilltree_warrior->addTab(tab);

	
	for (int i = Action::BLOCK; i <= Action::WEAPONMASTER; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	}
	

	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeWarrior_3");
	skilltree_warrior->addTab(tab);

	
	for (int i = Action::FIRESWORD; i <= Action::FURY; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	}
	
	for (act =Action::BASH;act <=Action::FURY;act++)
	{
		name = Action::getActionInfo((Action::ActionType) act)->m_enum_name;
		//lname = Action::getName((Action::ActionType) act);
		//label = win_mgr.getWindow(name.append( "Label"));
		label = win_mgr.getWindow(name + "Label");
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setID(act);
		label->setProperty("Image", "set:skills image:" + name);
		label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillMouseClicked, this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&SkillTree::onAbilityHover, this));

		button = static_cast<CEGUI::PushButton*>(win_mgr.getWindow(name + "Button"));
		button->setText("+");
		button->setID(act);
		button->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillLearnMouseClicked, this));
		
		
	}

	skilltree_warrior->setVisible(false);


	// Skilltree Schuetze
	CEGUI::TabControl* skilltree_archer = (CEGUI::TabControl*) win_mgr.createWindow("TaharezLook/TabControl", "SkilltreeArcher");
	m_windows[2] = skilltree_archer;

	
	skilltree_archer->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	skilltree_archer->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	skilltree_archer->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&SkillTree::consumeEvent, (Window*) this));
	skilltree_archer->setWantsMultiClickEvents(false);
	skilltree_archer->setAlpha(0.7);


	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeArcher_1");
	skilltree_archer->addTab(tab);

	name =Action::getActionInfo(Action::RANGE_ATTACK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Action::RANGE_ATTACK);
	label->setProperty("Image", "set:skills image:range_attack");
	label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillMouseClicked, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&SkillTree::onAbilityHover, this));

	
	for (int i = Action::TRIPLE_SHOT; i <= Action::EXPLOSION_CASCADE; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	}

	// Tab passive Faehigkeiten
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeArcher_2");
	skilltree_archer->addTab(tab);

	for (int i = Action::EVADE; i <= Action::RESIST_AIR; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	}
	
	// Tab3 <hier Name einsetzen>
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeArcher_3");
	skilltree_archer->addTab(tab);
	
	for (int i = Action::AIMED_SHOT; i <= Action::WIND_WALK; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	}
	
	for (act =Action::TRIPLE_SHOT;act <=Action::WIND_WALK;act++)
	{
		name =Action::getActionInfo((Action::ActionType) act)->m_enum_name;
		//lname =Action::getName((Action::ActionType) act);
		//label = win_mgr.getWindow(name.append( "Label"));
		label = win_mgr.getWindow(name + "Label");
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setID(act);
		label->setProperty("Image", "set:skills image:" + name);
		label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillMouseClicked, this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&SkillTree::onAbilityHover, this));

		button = static_cast<CEGUI::PushButton*>(win_mgr.getWindow(name + "Button"));
		button->setText("+");
		button->setID(act);
		button->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillLearnMouseClicked, this));
	}

	skilltree_archer->setVisible(false);


	// Skilltree Priester
	CEGUI::TabControl* skilltree_priest = (CEGUI::TabControl*) win_mgr.createWindow("TaharezLook/TabControl", "SkilltreePriest");
	m_windows[3] = skilltree_priest;

	
	skilltree_priest->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	skilltree_priest->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	skilltree_priest->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&SkillTree::consumeEvent, (Window*) this));
	skilltree_priest->setWantsMultiClickEvents(false);
	skilltree_priest->setAlpha(0.7);


	// Tab Lichtmagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreePriest_1");
	skilltree_priest->addTab(tab);

	name =Action::getActionInfo(Action::HOLY_ATTACK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Action::HOLY_ATTACK);
	label->setProperty("Image", "set:skills image:holy_attack");
	label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillMouseClicked, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&SkillTree::onAbilityHover, this));

	for (int i = Action::HOLY_LIGHT; i <= Action::BURNING_SUN; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	}
	


	// Tab Wassermagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreePriest_2");
	skilltree_priest->addTab(tab);

	for (int i = Action::BREAK_BINDING; i <= Action::HEAL_PARTY; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
		button->setText("+");
	}

	// Tab Windmagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreePriest_3");
	skilltree_priest->addTab(tab);

	for (int i = Action::DIVINE_WIND; i <= Action::KEEN_MIND; i++)
	{
		name =Action::getActionInfo((Action::ActionType) i)->m_enum_name;
		lname = name;
		label = win_mgr.createWindow("TaharezLook/StaticImage", lname.append( "Label") );
		tab->addChildWindow(label);
		label->setPosition(m_skill_position[i]);
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		
		pos = m_skill_position[i];
		pos += CEGUI::UVector2(cegui_reldim(0.14f), cegui_reldim( 0.05f));
		bname = name;
		button = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", bname.append( "Button") ));
		tab->addChildWindow(button);
		button->setPosition(pos);
		button->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	}
	

	for (act =Action::HOLY_LIGHT;act <=Action::KEEN_MIND;act++)
	{
		name =Action::getActionInfo((Action::ActionType) act)->m_enum_name;
		label = win_mgr.getWindow(name + "Label");
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setID(act);
		label->setProperty("Image", "set:skills image:" + name);
		label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillMouseClicked, this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&SkillTree::onAbilityHover, this));

		button = static_cast<CEGUI::PushButton*>(win_mgr.getWindow(name + "Button"));
		button->setText("+");
		button->setID(act);
		button->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillLearnMouseClicked, this));
	}

	updateTranslation();
	
}

void SkillTree::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	CEGUI::PushButton* button;
	std::ostringstream out_stream;

	Player* player = m_document->getLocalPlayer();

	// Enum Wert bei dem die Skillz des Spielers anfangen
	int i=Action::BASH;
	if (player->getTypeInfo()->m_subtype == "mage")
		i = Action::FIRE_BOLT;
	if (player->getTypeInfo()->m_subtype == "archer")
		i= Action::TRIPLE_SHOT;
	if (player->getTypeInfo()->m_subtype == "priest")
		i= Action::HOLY_LIGHT;

	bool vis;
	
	// Schleife ueber alle Skill labels
	for (int j=i;j<i+24;j++)
	{
		out_stream.str("");
		out_stream << Action::getActionInfo((Action::ActionType) j)->m_enum_name << "Label";

		// Label welches das Skillicon zeigt
		label = win_mgr.getWindow(out_stream.str());

		// Alpha Wert des Labels
		float alpha = 0.2;


		if (player->checkAbility((Action::ActionType) j))
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
		out_stream << Action::getActionInfo((Action::ActionType) j)->m_enum_name << "Button";
		
		button = static_cast<CEGUI::PushButton*>(win_mgr.getWindow(out_stream.str()));
		vis = player->checkAbilityLearnable((Action::ActionType) j) && (player->getSkillPoints()>0);
		if (button->isVisible() != vis)
		{
			button->setVisible(vis);
		}
	}
	
	// Markierer fuer Shortkeys einbauen
	// Zaehler fuer die Fenster
	static int acount =0;
	std::stringstream stream;
	
	Document::ShortkeyMap& shortkeys = m_document->getShortkeys();
	Document::ShortkeyMap::iterator it;
	int nr =0;
	
	CEGUI::Window *label2;
	Action::ActionType act;
	bool right;
	std::string keyname;
	int key;
	CEGUI::UVector2 pos;
	
	for (it = shortkeys.begin(); it != shortkeys.end(); ++it)
	{
		if (it->second < Document::USE_SKILL_LEFT || it->second >Document::USE_SKILL_RIGHT+200)
			continue;
		
		DEBUG5("shortkey %i to %i",it->first,it->second);
		key = it->first;
		
		act= (Action::ActionType) (it->second-Document::USE_SKILL_LEFT);
		right = false;
		if (it->second >= Document::USE_SKILL_RIGHT)
		{
			act= (Action::ActionType)  (it->second-Document::USE_SKILL_RIGHT);
			right = true;
		}
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
		stream << Action::getActionInfo((Action::ActionType) act)->m_enum_name << "Label";
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
		
		if ((int) label->getID() != act)
		{
			label->setID(act);
			pos = m_skill_position[act];
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
		nr ++;
	}
}


void SkillTree::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::DefaultWindow* tab;
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreeMage_1");
	tab->setText((CEGUI::utf8*) gettext("Fire magic"));
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreeMage_2");
	tab->setText((CEGUI::utf8*) gettext("Ice magic"));
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreeMage_3");
	tab->setText((CEGUI::utf8*) gettext("Air magic"));
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreeWarrior_1");
	tab->setText((CEGUI::utf8*) gettext("Fighting skills"));
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreeWarrior_2");
	tab->setText((CEGUI::utf8*) gettext("Passive skills"));
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreeWarrior_3");
	tab->setText((CEGUI::utf8*) gettext("Fighting spirit"));
	
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreeArcher_1");
	tab->setText((CEGUI::utf8*) gettext("Marksmanship"));
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreeArcher_2");
	tab->setText((CEGUI::utf8*) gettext("Passive skills"));
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreeArcher_3");
	tab->setText((CEGUI::utf8*) gettext("Hunter magic"));
	
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreePriest_1");
	tab->setText((CEGUI::utf8*) gettext("Light Magic"));
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreePriest_2");
	tab->setText((CEGUI::utf8*) gettext("Water Magic"));
	
	tab = (CEGUI::DefaultWindow*) win_mgr.getWindow("SkilltreePriest_3");
	tab->setText((CEGUI::utf8*) gettext("Wind Magic"));
}

void SkillTree::updateAbilityTooltip(unsigned int pos)
{

	if (pos!=Action::NOACTION)
	{
		DEBUG5("update tooltip for %i", pos);

		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Window* label;
		std::ostringstream out_stream;
		out_stream.str("");
		out_stream << Action::getActionInfo((Action::ActionType) pos)->m_enum_name << "Label";

		label = win_mgr.getWindow(out_stream.str());

		std::string tooltip = m_document->getAbilityDescription((Action::ActionType) pos);

		label->setTooltipText((CEGUI::utf8*) tooltip.c_str());

	}

}

bool SkillTree::onSkillMouseClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();

	if (we.button == CEGUI::LeftButton)
	{
		m_document->setLeftAction((Action::ActionType) id);
	}

	if (we.button == CEGUI::RightButton)
	{
		DEBUG5("right button pressed on skill %i",id);
		m_document->setRightAction((Action::ActionType) id);
	}
	return true;
}

bool SkillTree::onSkillLearnMouseClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	
	// versuche Faehigkeit zu lernen
	m_document->learnAbility((Action::ActionType) id);
	
	return true;
}

bool SkillTree::onAbilityHover(const CEGUI::EventArgs& evt)
{

	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	DEBUG5("mouse entered Ability %i",id);
	updateAbilityTooltip(id);
	m_document->getGUIState()->m_hover_ability = (Action::ActionType) (id);
	return true;
}

bool SkillTree::onAbilityHoverLeave(const CEGUI::EventArgs& evt)
{
	m_document->getGUIState()->m_hover_ability = Action::NOACTION;
	return true;
}

