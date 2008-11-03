
#include "controlpanel.h"

ControlPanel::ControlPanel (Document* doc)
	: ItemWindow(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	std::string name;

	// Rahmen fuer die untere Kontrollleiste
	CEGUI::FrameWindow* ctrl_panel = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "ControlPanel");
	m_window = ctrl_panel;
	
	ctrl_panel->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.88f))); //0.0/0.8
	ctrl_panel->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 0.12f))); //1.0/0.2
	ctrl_panel->setProperty("FrameEnabled","false");
	ctrl_panel->setProperty("TitlebarEnabled","false");
	ctrl_panel->setProperty("CloseButtonEnabled","false");
	ctrl_panel->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	ctrl_panel->setWantsMultiClickEvents(false);


	// Bestandteile der Kontrollleiste hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;

	// Balken fuer HP
	CEGUI::ProgressBar* bar = static_cast<CEGUI::ProgressBar*>(win_mgr.createWindow("TaharezLook/ProgressBar", "HealthProgressBar"));
	ctrl_panel->addChildWindow(bar);
	bar->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim( 0.05f)));
	bar->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.30f)));
	bar->setWantsMultiClickEvents(false);
	bar->setProgress(0.0);


	// Button Inventar
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "InventoryButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.4475f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.30f)));
	btn->setText("I");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ControlPanel::onButtonInventoryClicked, this));
	btn->setWantsMultiClickEvents(false);

	// Button Charakterinfo
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "CharInfoButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.3675f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.30f)));
	btn->setText("C");
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ControlPanel::onButtonCharInfoClicked, this));

	// Button Chat oeffnen
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "ChatOpenButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.64f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.30f)));
	btn->setText("M");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ControlPanel::onButtonOpenChatClicked, this));
	btn->setWantsMultiClickEvents(false);

	// Button SkillTree
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SkillTreeButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.5275f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.30f)));
	btn->setText("T");
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ControlPanel::onButtonSkilltreeClicked, this));

	// Button Party
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "PartyButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.255f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.30f)));
	btn->setText("P");
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ControlPanel::onButtonPartyClicked, this));

	// Button Speichern und Beenden
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SaveExitButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.835f), cegui_reldim( 0.30f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.155f), cegui_reldim( 0.30f)));
	btn->setText(gettext("main_saveexit"));
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ControlPanel::onButtonSaveExitClicked, this));

	// Button Optionen
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "OptionsButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.835f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.155f), cegui_reldim( 0.30f)));
	btn->setWantsMultiClickEvents(false);
	btn->setText(gettext("main_options"));

	// Anzeige linke Maustaste Faehigkeit
	label = win_mgr.createWindow("TaharezLook/StaticImage", "LeftClickAbilityImage");
	ctrl_panel->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.12f), cegui_reldim( 0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.90f)));

	// Balken fuer linke Maustaste Faehigkeit
	bar = static_cast<CEGUI::ProgressBar*>(win_mgr.createWindow("TaharezLook/ProgressBar", "LeftClickAbilityProgressBar"));
	ctrl_panel->addChildWindow(bar);
	bar->setPosition(CEGUI::UVector2(cegui_reldim(0.12f), cegui_reldim( 0.75f)));
	bar->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.20f)));
	bar->setWantsMultiClickEvents(false);
	bar->setProgress(0.0);
	bar->setAlpha(0.5);
	bar->setAlwaysOnTop(true);

	// Anzeige rechte Maustaste Faehigkeit
	label = win_mgr.createWindow("TaharezLook/StaticImage", "RightClickAbilityImage");
	ctrl_panel->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.725f), cegui_reldim( 0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.90f)));

	// Balken fuer rechte Maustaste Faehigkeit
	bar = static_cast<CEGUI::ProgressBar*>(win_mgr.createWindow("TaharezLook/ProgressBar", "RightClickAbilityProgressBar"));
	ctrl_panel->addChildWindow(bar);
	bar->setPosition(CEGUI::UVector2(cegui_reldim(0.725f), cegui_reldim( 0.75f)));
	bar->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.20f)));
	bar->setWantsMultiClickEvents(false);
	bar->setProgress(0.0);
	bar->setAlpha(0.5);
	bar->setAlwaysOnTop(true);

	// Inventar Guertel
	int i;
	std::ostringstream outStream;
	for (i=0;i<10;i++)
	{
		outStream.str("");
		outStream << "InventoryItem" << i;
		label = win_mgr.createWindow("TaharezLook/StaticImage", outStream.str());
		ctrl_panel->addChildWindow(label);
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.225f+i*0.05f), cegui_reldim( 0.05f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.045f), cegui_reldim( 0.5f)));
		label->setID(Equipement::SMALL_ITEMS+i);
		label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&ControlPanel::onItemMouseButtonPressed, (ItemWindow*) this));
		label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&ControlPanel::onItemMouseButtonReleased, (ItemWindow*) this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&ControlPanel::onItemHover, (ItemWindow*) this));
	}

}

void ControlPanel::update()
{
		// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label, *img;
	std::ostringstream out_stream;
	int timernr;
	float perc=0;
	std::string name;

	Player* player = m_document->getLocalPlayer();

	// Balken fuer HP
	CEGUI::ProgressBar* bar = static_cast<CEGUI::ProgressBar*>(win_mgr.getWindow( "HealthProgressBar"));
	float hperc = player->getDynAttr()->m_health / player->getBaseAttrMod()->m_max_health;
	if (bar->getProgress() != hperc)
	{
		bar->setProgress(hperc);
	}

	// Image Schaden Attacke links
	label =  win_mgr.getWindow( "LeftClickAbilityImage");
	name = Action::getActionInfo((Action::ActionType) player->getLeftAction())->m_enum_name;
	if (("set:skills image:" + name) != label->getProperty("Image"))
	{
		label->setProperty("Image", "set:skills image:" + name);
	}

	// Balken fuer Schaden Attacke links
	bar = static_cast<CEGUI::ProgressBar*>(win_mgr.getWindow( "LeftClickAbilityProgressBar"));
	timernr =  Action::getActionInfo(player->getLeftAction())->m_timer_nr;
	perc = player->getTimerPercent(timernr);

	if (bar->getProgress() != perc)
	{
		bar->setProgress(perc);
	}

	perc =0;

	// Image Attacke rechts
	label =  win_mgr.getWindow( "RightClickAbilityImage");
	name = Action::getActionInfo((Action::ActionType) player->getRightAction())->m_enum_name;
	if (("set:skills image:" + name) != label->getProperty("Image"))
	{
		label->setProperty("Image", "set:skills image:" + name);
	}

	// Balken fuer Schaden Attacke rechts
	bar = static_cast<CEGUI::ProgressBar*>(win_mgr.getWindow( "RightClickAbilityProgressBar"));
	timernr =  Action::getActionInfo(player->getRightAction())->m_timer_nr;
	perc = player->getTimerPercent(timernr);

	if (bar->getProgress() != perc)
	{
		bar->setProgress(perc);
	}

	Item* it;
	Equipement* equ = player->getEquipement();

	// Guertel
	for (int i=0;i<10;i++)
	{
		out_stream.str("");
		out_stream << "InventoryItem" << i;
		img =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::SMALL_ITEMS+i);
		out_stream.str("");
		if (it!=0)
		{
			out_stream<< getItemImage(it->m_subtype);;
		}
		if (img->getProperty("Image")!=out_stream.str())
		{
			img->setProperty("Image", out_stream.str());
		}
	}
}


bool ControlPanel::onButtonSaveExitClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonSaveExitClicked();
	return true;
}

bool ControlPanel::onButtonInventoryClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonInventoryClicked();
	return true;
}

bool ControlPanel::onButtonCharInfoClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonCharInfoClicked();
	return true;
}

bool ControlPanel::onButtonSkilltreeClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonSkilltreeClicked();
	return true;
}

bool ControlPanel::onButtonPartyClicked(const CEGUI::EventArgs& evt)
{
	// Debugging
	m_document->emitDebugSignal(0);
	return true;
}

bool ControlPanel::onButtonOpenChatClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonOpenChatClicked();
	return true;
}



