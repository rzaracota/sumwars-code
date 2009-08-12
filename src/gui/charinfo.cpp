#include "charinfo.h"
#include "player.h"
#include "damage.h"


CharInfo::CharInfo (Document* doc)
	: Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// Rahmen fuer CharInfo Fenster
	CEGUI::FrameWindow* char_info = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "CharInfo");
	m_window = char_info;
	
	
	char_info->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
	char_info->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	char_info->setProperty("FrameEnabled","false");
	char_info->setProperty("TitlebarEnabled","false");
	char_info->setProperty("CloseButtonEnabled","false");
	char_info->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	char_info->setWantsMultiClickEvents(false);
	char_info->setAlpha(0.7);

	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;

	// Label Name
	label = win_mgr.createWindow("TaharezLook/StaticText", "NameLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.1f)));
	
	// Label Klasse
	label = win_mgr.createWindow("TaharezLook/StaticText", "ClassLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.0f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.1f)));
	
	// Label Level
	label = win_mgr.createWindow("TaharezLook/StaticText", "LevelLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.0f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.1f)));
	label->setText("Level 1");

	// Label Staerke
	label = win_mgr.createWindow("TaharezLook/StaticText", "StrengthLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.15f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Staerke (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "StrengthValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.15f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Button Staerke erhoehen
	btn = static_cast<CEGUI::PushButton*>( win_mgr.createWindow("TaharezLook/Button", "StrengthAddButton"));
	char_info->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.15f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.06f)));
	btn->setText("+");
	btn->setID(CreatureBaseAttr::STRENGTH);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));


	// Label Geschick
	label = win_mgr.createWindow("TaharezLook/StaticText", "DexterityLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.23f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Geschick (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "DexterityValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.23f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Button Geschick erhoehen
	btn = static_cast<CEGUI::PushButton*>( win_mgr.createWindow("TaharezLook/Button", "DexterityAddButton"));
	char_info->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.23f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.06f)));
	btn->setText("+");
	btn->setID(CreatureBaseAttr::DEXTERITY);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));

	// Label Willenskraft
	label = win_mgr.createWindow("TaharezLook/StaticText", "WillpowerLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.31f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Willenskraft (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "WillpowerValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.31f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Button Willenskraft erhoehen
	btn = static_cast<CEGUI::PushButton*>( win_mgr.createWindow("TaharezLook/Button", "WillpowerAddButton"));
	char_info->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.31f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.06f)));
	btn->setText("+");
	btn->setID(CreatureBaseAttr::WILLPOWER);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));

	// Label Zauberkraft
	label = win_mgr.createWindow("TaharezLook/StaticText", "MagicpowerLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.39f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Zauberkraft (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "MagicpowerValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.39f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Button Zauberkraft erhoehen
	btn = static_cast<CEGUI::PushButton*>( win_mgr.createWindow("TaharezLook/Button", "MagicpowerAddButton"));
	char_info->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.39f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.06f)));
	btn->setText("+");
	btn->setID(CreatureBaseAttr::MAGIC_POWER);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));

	// Label Attributpunkte
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttrPointsLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.47f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label  Attributpunkte (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttrPointsValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.47f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
	label->setText("5");

	// Label Exp
	label = win_mgr.createWindow("TaharezLook/StaticText", "ExperienceLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.15f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Exp (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ExperienceValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.15f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0/100");

	// Label HP
	label = win_mgr.createWindow("TaharezLook/StaticText", "HitpointsLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.23f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label HP (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "HitpointsValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.23f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0/100");

	// Label Attacke
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttackLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.55f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Attacke (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttackValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.55f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Label Attackegeschwindigkeit
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttackSpeedLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.62f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Attacke (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttackSpeedValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.62f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Label Reichweite
	label = win_mgr.createWindow("TaharezLook/StaticText", "RangeLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.69f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Reichweite (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "RangeValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.69f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("2");

	// Label Durchschlagskraft
	label = win_mgr.createWindow("TaharezLook/StaticText", "PowerLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.76f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Durchschlagskraft
	label = win_mgr.createWindow("TaharezLook/StaticText", "PowerValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.76f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("30");

	// Label Ruestung
	label = win_mgr.createWindow("TaharezLook/StaticText", "ArmorLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.83f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Ruestung
	label = win_mgr.createWindow("TaharezLook/StaticText", "ArmorValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.83f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("40");

	// Label Block
	label = win_mgr.createWindow("TaharezLook/StaticText", "BlockLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.90f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Block
	label = win_mgr.createWindow("TaharezLook/StaticText", "BlockValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.90f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Label Basisschaden
	label = win_mgr.createWindow("TaharezLook/StaticText", "BaseDmgLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.48f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText((CEGUI::utf8*) gettext("Base damage"));

	// Label Basisschaden (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "BaseDmgValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.48f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("5-10");

	// Label Skill1 Schaden
	label = win_mgr.createWindow("TaharezLook/StaticText", "Skill1DmgLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.55f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText("Skill1");

	// Label Skill1 Schaden (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "Skill1DmgValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.55f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("10-20");

	// Label Skill2 Schaden
	label = win_mgr.createWindow("TaharezLook/StaticText", "Skill2DmgLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.62f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText("Skill2");

	// Label Skill2 Schaden (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "Skill2DmgValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.62f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("20-30");

	// Label physisches Resistenz
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistPhysLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.69f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label physische Resistenz (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistPhysValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.69f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0");

	// Label Feuerresistenz
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistFireLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.76f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Feuerresistenz (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistFireValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.76f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0");

	// Label Luftresistenz
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistAirLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.83f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Feuerresistenz (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistAirValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.83f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0");

	// Label Eisresistenz
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistIceLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.90f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	
	// Label Eisresistenz (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistIceValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.90f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0");
	
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
	label =  win_mgr.getWindow("NameLabel");
	out_stream.str("");
	out_stream.str(player->getName());
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Klasse
	label =  win_mgr.getWindow("ClassLabel");
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
	label =  win_mgr.getWindow("LevelLabel");
	out_stream.str("");
	out_stream << "Level "<<(int) player->getBaseAttr()->m_level;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Staerke
	label =  win_mgr.getWindow("StrengthValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_strength;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Zauberkraft
	label =  win_mgr.getWindow("MagicpowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_magic_power;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Willenskraft
	label =  win_mgr.getWindow("WillpowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_willpower;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Geschick
	label =  win_mgr.getWindow("DexterityValueLabel");
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
	btn =   static_cast<CEGUI::PushButton*>(win_mgr.getWindow("StrengthAddButton"));
	btn->setVisible(add_but_vis);
	btn =  static_cast<CEGUI::PushButton*>(win_mgr.getWindow("DexterityAddButton"));
	btn->setVisible(add_but_vis);
	btn =  static_cast<CEGUI::PushButton*>(win_mgr.getWindow("WillpowerAddButton"));
	btn->setVisible(add_but_vis);
	btn =  static_cast<CEGUI::PushButton*>(win_mgr.getWindow("MagicpowerAddButton"));
	btn->setVisible(add_but_vis);

	// Label freie Attributspunkte
	label =  win_mgr.getWindow("AttrPointsValueLabel");
	out_stream.str("");
	out_stream << player->getAttributePoints();
	//label->setVisible(add_but_vis);
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Ruestung
	label =  win_mgr.getWindow("ArmorValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_armor;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	out_stream.str("");
	if (fstat->m_last_attacker != "")
	{
		out_stream << "Damage dealt by "<<gettext(fstat->m_last_attacker.c_str())<<": "<<(int) (fstat->m_damage_got_perc*100) << "%";
	}
	if (label->getTooltipText() != (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setTooltipText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Attacke
	label =  win_mgr.getWindow("AttackValueLabel");
	out_stream.str("");
	out_stream << player->getBaseDamage().m_attack;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	out_stream.str("");
	if (fstat->m_last_attacked != "")
	{
		out_stream << "Chance to hit  "<<gettext(fstat->m_last_attacked.c_str())<<": "<<(int) (fstat->m_hit_chance*100) << "%";
	}
	if (label->getTooltipText() != (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setTooltipText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Block
	label =  win_mgr.getWindow("BlockValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_block;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	out_stream.str("");
	if (fstat->m_last_attacker != "")
	{
		out_stream << "Chance to get hit by "<<gettext(fstat->m_last_attacker.c_str())<<": "<<(int) (100-fstat->m_block_chance*100) << "%";
	}
	if (label->getTooltipText() != (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setTooltipText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label HP
	label =  win_mgr.getWindow("HitpointsValueLabel");
	out_stream.str("");
	out_stream << (int) player->getDynAttr()->m_health <<"/" <<(int) player->getBaseAttrMod()->m_max_health;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Exp
	label =  win_mgr.getWindow("ExperienceValueLabel");
	out_stream.str("");
	out_stream << (int) player->getDynAttr()->m_experience <<"/" <<(int) player->getBaseAttr()->m_max_experience;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Phys Resistenz
	label =  win_mgr.getWindow("ResistPhysValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::PHYSICAL];
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Feuer Resistenz
	label =  win_mgr.getWindow("ResistFireValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::FIRE];
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Eis Resistenz
	label =  win_mgr.getWindow("ResistIceValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::ICE];
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Luft Resistenz
	label =  win_mgr.getWindow("ResistAirValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::AIR];
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Angriffsgeschwindigkeit
	label =  win_mgr.getWindow("AttackSpeedValueLabel");
	out_stream.str("");
	out_stream << 0.001*std::min((short) 5000, player->getBaseAttrMod()->m_attack_speed);
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Reichweite
	label =  win_mgr.getWindow("RangeValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_attack_range;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}

	// Label Durchschlagskraft
	label =  win_mgr.getWindow("PowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseDamage().m_power;
	if (label->getText()!= (CEGUI::utf8*) out_stream.str().c_str())
	{
		label->setText((CEGUI::utf8*) out_stream.str().c_str());
	}
	
	out_stream.str("");
	if (fstat->m_last_attacked != "")
	{
		out_stream << "Damage dealt to  "<<gettext(fstat->m_last_attacked.c_str())<<": "<<(int) (fstat->m_damage_dealt_perc*100) << "%";
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
	label =  win_mgr.getWindow( "BaseDmgLabel");
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
	label =  win_mgr.getWindow("BaseDmgValueLabel");
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
	label =  win_mgr.getWindow( "Skill1DmgLabel");
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
	label =  win_mgr.getWindow( "Skill1DmgValueLabel");
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
	label =  win_mgr.getWindow( "Skill2DmgLabel");
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
	label =  win_mgr.getWindow( "Skill2DmgValueLabel");
	if (dist == Action::MELEE || dist == Action::RANGED)
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
}

void CharInfo::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;

	label = win_mgr.getWindow("NameLabel");
	label->setText((CEGUI::utf8*) gettext("Name"));
	
	label = win_mgr.getWindow("ClassLabel");
	label->setText((CEGUI::utf8*) gettext("Class"));
	
	
	label = win_mgr.getWindow("StrengthLabel");
	label->setText((CEGUI::utf8*) gettext("Strength"));
	label->setTooltipText((CEGUI::utf8*) gettext("Strength increases hitpoints, melee damage and attack power."));
	
	label = win_mgr.getWindow("DexterityLabel");
	label->setText((CEGUI::utf8*) gettext("Dexterity"));
	label->setTooltipText((CEGUI::utf8*) gettext("Dexterity increases ranged damage, attack accuracy and attack speed"));
	
	label = win_mgr.getWindow("WillpowerLabel");
	label->setText((CEGUI::utf8*) gettext("Willpower"));
	label->setTooltipText((CEGUI::utf8*) gettext("Willpower decreases the duration and the impact of status changes and increases magical melee damage."));
	
	
	label = win_mgr.getWindow("MagicpowerLabel");
	label->setText((CEGUI::utf8*) gettext("Magic power"));
	label->setTooltipText((CEGUI::utf8*) gettext("Magic power increases the damage of magical attacks."));
	
	label = win_mgr.getWindow( "AttrPointsLabel");
	label->setText((CEGUI::utf8*) gettext("Attr points"));
	label->setTooltipText((CEGUI::utf8*) gettext("Attribute points remaining to distribute"));

	label = win_mgr.getWindow("ExperienceLabel" );
 	label->setText((CEGUI::utf8*) gettext("Experience"));
	label->setTooltipText((CEGUI::utf8*) gettext("Current experience and experience necessary for next level-up."));
	
	label = win_mgr.getWindow("HitpointsLabel");
	label->setText((CEGUI::utf8*) gettext("Hitpoints"));
	label->setTooltipText((CEGUI::utf8*) gettext("Current and maximal hitpoints."));
	
	
	label = win_mgr.getWindow("AttackLabel" );
	label->setText((CEGUI::utf8*) gettext("Attack"));
	label->setTooltipText((CEGUI::utf8*) gettext("Attack value determines the chance to hit enemies that can block or evade."));
	
	
	label = win_mgr.getWindow("AttackSpeedLabel" );
	label->setText((CEGUI::utf8*) gettext("Attacks/s"));
	label->setTooltipText((CEGUI::utf8*) gettext("Number of attacks per second."));
	
	
	label = win_mgr.getWindow( "RangeLabel");
	label->setText((CEGUI::utf8*) gettext("Range"));
	label->setTooltipText((CEGUI::utf8*) gettext("Weapon Range in meters."));
	
	
	label = win_mgr.getWindow("PowerLabel" );
	label->setText((CEGUI::utf8*) gettext("Power"));
	label->setTooltipText((CEGUI::utf8*) gettext("Attack power influences the damage dealt to armored enemies."));
	
	
	label = win_mgr.getWindow( "ArmorLabel");
	label->setText((CEGUI::utf8*) gettext("Armor"));
	label->setTooltipText((CEGUI::utf8*) gettext("Armor reduces the damage of physical attacks."));
	
	
	label = win_mgr.getWindow("BlockLabel" );
	label->setText((CEGUI::utf8*) gettext("Block"));
	label->setTooltipText((CEGUI::utf8*) gettext("Block value determines the chance to avoid damage from physical attacks."));
	
	
	label = win_mgr.getWindow( "ResistPhysLabel");
	label->setText((CEGUI::utf8*) gettext("Physical"));
	
	label = win_mgr.getWindow("ResistAirLabel" );
	label->setText((CEGUI::utf8*) gettext("Air"));
	
	label = win_mgr.getWindow("ResistIceLabel" );
	label->setText((CEGUI::utf8*) gettext("Ice"));
	
	label = win_mgr.getWindow("ResistFireLabel" );
	label->setText((CEGUI::utf8*) gettext("Fire"));
	
	
}


bool CharInfo::onIncreaseAttributeButtonClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	DEBUG5("left button pressed on skill %i",id);
	m_document->increaseAttribute((CreatureBaseAttr::Attribute) id);
	return true;
}




