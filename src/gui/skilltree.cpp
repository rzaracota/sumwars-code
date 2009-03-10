
#include "skilltree.h"

SkillTree::SkillTree(Document* doc)
	:Window(doc)
{
	DEBUG4("setup skilltree");

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();


	// Rahmen fuer Skilltree Fenster

	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::Window* label;
	CEGUI::DefaultWindow* tab;

	std::ostringstream outStream;
	std::string name;
	std::string lname;
	unsigned int act;

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


	name =Action::getActionInfo(Action::FIRE_BOLT)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FIRE_STRIKE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FIRE_BALL)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::INFERNO_BALL)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::INFLAME)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FIRE_WALL)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FIRE_WAVE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FIRE_STORM)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));


	// Tab Eismagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeMage_2");
	skilltree_mage->addTab(tab);
	
	name =Action::getActionInfo(Action::ICE_BOLT)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::ICE_SPIKE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FREEZE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::ICE_RING)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CHILL)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FROST_RING)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::SNOW_STORM)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::BLIZZARD)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage",name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));



	// Tab Luftmagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeMage_3");
	skilltree_mage->addTab(tab);
	
	name =Action::getActionInfo(Action::LIGHTNING)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::LIGHTNING_STRIKE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CHAIN_LIGHTNING)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage",name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CHAIN_LIGHTNING2)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::THUNDERSTORM)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage",name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::THUNDERSTORM2)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::STATIC_SHIELD)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::IONISATION)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));


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


	/*
	name =Action::getActionInfo(Action::ATTACK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Action::ATTACK);
	label->setText(Action::getName(Action::ATTACK));
	label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SkillTree::onSkillMouseClicked, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&SkillTree::onAbilityHover, this));
	*/


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


	name =Action::getActionInfo(Action::BASH)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::HAMMER_BASH)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::HATE_MAGE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::AROUND_BLOW)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::WHIRL_BLOW)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::SMASH)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CHARGE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::STORM_CHARGE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));



	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeWarrior_2");
	skilltree_warrior->addTab(tab);

	name =Action::getActionInfo(Action::STEADFAST)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::BLOCK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::WEAPONMASTER)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::MONSTER_HUNTER)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::MONSTER_SLAYER)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::ENDURANCE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));


	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeWarrior_3");
	skilltree_warrior->addTab(tab);

	name =Action::getActionInfo(Action::DECOY)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::SCARE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::BERSERK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::WARCRY)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::REGENERATE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::ANGER)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FURY)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FIRESWORD)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FLAMEARMOR)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FLAMESWORD)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

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

	name =Action::getActionInfo(Action::TRIPLE_SHOT)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.35f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::GUIDED_TRIPLE_SHOT)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.35f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::PIERCE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.35f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::MULTISHOT)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::VOLLEY_SHOT)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::DEATH_ROULETTE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.35f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::WEAK_POINT)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.75f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::BLIND_RAGE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.75f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::EXPLODING_ARROW)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.55f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::EXPLOSION_CASCADE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.55f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::VACUUM)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.55f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));


	// Tab passive Faehigkeiten
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeArcher_2");
	skilltree_archer->addTab(tab);

	name =Action::getActionInfo(Action::EVADE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CRITICAL_STRIKE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CONCENTRATION)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::MENTAL_WALL)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::RESIST_ICE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::RESIST_AIR)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	// Tab3 <hier Name einsetzen>
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeArcher_3");
	skilltree_archer->addTab(tab);

	name =Action::getActionInfo(Action::AIMED_SHOT)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::WIND_WALK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::BOW_SPIRIT)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::WIND_ARROWS)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::STORM_ARROWS)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::ICE_ARROWS)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::FREEZING_ARROWS)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

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

	name =Action::getActionInfo(Action::BLAZING_SHIELD)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.55f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::MAGIC_SHIELD)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.55f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::BURNING_RAGE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.55f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CURE_BLIND_MUTE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.75f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CURE_BLIND_MUTE_PARTY)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.75f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::LIGHT_BEAM)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.35f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::BURNING_SUN)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.35f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::HOLY_LIGHT)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::HOLY_FIRE)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));


	// Tab Wassermagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreePriest_2");
	skilltree_priest->addTab(tab);

	name =Action::getActionInfo(Action::HEAL)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CURE_POIS_BURN)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::HEAL_PARTY)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::ACID)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CURE_POIS_BURN_PARTY)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::BREAK_BINDING)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::DISRUPT_BINDING)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));


	// Tab Windmagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreePriest_3");
	skilltree_priest->addTab(tab);

	name =Action::getActionInfo(Action::BLADE_STORM)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::KEEN_MIND)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CURE_CONF_BSRK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::CURE_CONF_BSRK_PARTY)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::HYPNOSIS)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.45f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::HYPNOSIS2)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim(0.85f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::DIVINE_WIND)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.25f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	name =Action::getActionInfo(Action::DIVINE_STORM)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.65f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));

	for (act =Action::HOLY_LIGHT;act <=Action::KEEN_MIND;act++)
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

	}

	updateTranslation();
	
}

void SkillTree::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
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

	// Schleife ueber alle Skill labels
	for (int j=i;j<i+24;j++)
	{
		out_stream.str("");
		out_stream << Action::getActionInfo((Action::ActionType) j)->m_enum_name << "Label";

		// Label welches das Skillicon zeigt
		label = win_mgr.getWindow(out_stream.str());

		// Alpha Wert des Labels
		float alpha = 0.2;

		if (player->checkAbilityLearnable((Action::ActionType) j))
		{
			// Faehigkeit kann gelernt werden
			alpha = 0.6;
		}

		if (player->checkAbility((Action::ActionType) j))
		{
			// Faehigkeit steht zur Verfuegung
			alpha = 1.0;
		}

		if (label->getAlpha() != alpha)
		{
			label->setAlpha(alpha);
		}
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
		DEBUG5("left button pressed on skill %i",id);
		if (m_document->getLocalPlayer())
		{
			if (m_document->getLocalPlayer()->checkAbility((Action::ActionType) id))
			{
				// Faehigkeit ist verfuegbar, setzen
				m_document->setLeftAction((Action::ActionType) id);
			}
			else
			{
				// versuche Faehigkeit zu lernen
				m_document->learnAbility((Action::ActionType) id);
			}
		}

	}

	if (we.button == CEGUI::RightButton)
	{
		DEBUG5("right button pressed on skill %i",id);
		m_document->setRightAction((Action::ActionType) id);
	}
	return true;
}


bool SkillTree::onAbilityHover(const CEGUI::EventArgs& evt)
{

	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	DEBUG5("mouse entered Ability %i",id);
	updateAbilityTooltip(id);
	return true;
}




