#include "mainwindow.h"

#include <string>
#include <sstream>



MainWindow::MainWindow(Ogre::Root* ogreroot, CEGUI::System* ceguisystem,Ogre::RenderWindow* window,Document* doc)
{
	m_ogre_root = ogreroot;
	m_cegui_system = ceguisystem;
	m_window = window;
	m_document = doc;
	m_scene_manager = m_ogre_root->getSceneManager("DefaultSceneManager");

	// Initialisieren
	bool res = init();
	if (res==false)
	{
		ERRORMSG("GUI erzeugen fehlgeschlagen");
	}
	
	m_key =0;
}

bool MainWindow::init()
{
	bool result = true;


	// Eingabegeraete initialisieren
	result &= initInputs();


	// Hauptmenue erzeugen
	result &= setupMainMenu();

	// Hauptspielfenster erzeugen
	result &= setupGameScreen();

	// Aktuelle Ebene setzen
	CEGUI::System::getSingleton().setGUISheet(m_main_menu);

	
	return result;

}


MainWindow::~MainWindow()
{
	m_ois->destroyInputObject(m_mouse);
	m_ois->destroyInputObject(m_keyboard);
	OIS::InputManager::destroyInputSystem(m_ois);

	// Dynamisch angelegte Objekte loeschen
	delete m_scene;
}

bool MainWindow::initInputs()
{
	DEBUG4("init inputs");
	OIS::ParamList pl;
	// Handle fuer das aktuelle Fenster
	unsigned long hWnd;
	m_window->getCustomAttribute("WINDOW", &hWnd);
	// OIS mitteilen fuer welches Fenster die Eingaben abgefangen werden sollen
	pl.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
	pl.insert( std::make_pair(std::string("x11_mouse_grab"), std::string("false") ) );
	pl.insert( std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));

	m_ois = OIS::InputManager::createInputSystem( pl );

	// Maus initialisieren
	m_mouse = static_cast<OIS::Mouse*>(m_ois->createInputObject( OIS::OISMouse, true ));
	m_mouse->setEventCallback(this);
	m_mouse->getMouseState().width = (int) m_window->getWidth();
	m_mouse->getMouseState().height = (int) m_window->getHeight();

	// Tastatur initialisieren
	m_keyboard = static_cast<OIS::Keyboard*>(m_ois->createInputObject( OIS::OISKeyboard, true));
	m_keyboard->setEventCallback(this);

	return true;
}

bool MainWindow::setupMainMenu()
{
	try
	{

		
		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

		// Oberstes Fenster der Hauptmenue Schicht
		m_main_menu = win_mgr.createWindow("DefaultWindow", "MainMenu");
		
		
		CEGUI::Window* img;
		img  = win_mgr.createWindow("TaharezLook/StaticImage", "StartScreenImage");
		m_main_menu->addChildWindow(img);
		img->setProperty("Image", "set:startscreen.png image:full_image");
		img->moveToBack ();
		img->setMousePassThroughEnabled(true);

		Window* wnd = new SavegameList(m_document);
		m_sub_windows["SavegameList"] = wnd;
		m_main_menu->addChildWindow(wnd->getCEGUIWindow());
		
		wnd = new MainMenu(m_document);
		m_sub_windows["MainMenu"] = wnd;
		m_main_menu->addChildWindow(wnd->getCEGUIWindow());
		
		wnd = new CharCreate(m_document);
		m_sub_windows["CharCreate"] = wnd;
		m_main_menu->addChildWindow(wnd->getCEGUIWindow());
		
		wnd = new OptionsWindow(m_document,m_keyboard);
		m_sub_windows["Options"] = wnd;
		m_main_menu->addChildWindow(wnd->getCEGUIWindow());
		

	// Verbinden mit dem Document
	}
	catch (CEGUI::Exception e)
	{
		ERRORMSG("Error message: %s",e.getMessage().c_str());
	}
	return true;
}

void MainWindow::update()
{
	// Eingaben abfangen
	m_mouse->capture();
	m_keyboard->capture();
	
	
	
	// Tastenwiederholung erzeugen
	if (m_key !=0)
	{
		if (m_key_repeat)
		{
			if ( m_key_repeat_timer.getTime()>60)
			{
				m_key_repeat_timer.start();
				m_cegui_system->injectChar(m_key);
				
				// Sonderbehandlung fuer Backspace
				if (m_key == 8)
				{
					m_cegui_system->injectKeyDown(OIS::KC_BACK);
				}
			}
		}
		else
		{
			if ( m_key_repeat_timer.getTime()>400)
			{
				m_key_repeat_timer.start();
				m_key_repeat = true;
			}
		}
	}

	
	if (m_document->getModified() & Document::SAVEGAME_MODIFIED)
	{
		m_scene->updateTempPlayer();
		m_document->setModified(m_document->getModified() & (~Document::SAVEGAME_MODIFIED));
	}
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	// Flags, welche Fenster gezeigt werden
	int wflags = m_document->getGUIState()->m_shown_windows;

	
	
	// Testen ob die Anzeige der Fenster geaendert werden muss
	if (m_document->getModified() & Document::GUISHEET_MODIFIED)
	{
		if (m_document->getGUIState()->m_sheet ==  Document::MAIN_MENU)
		{
			updateMainMenu();
			m_cegui_system->setGUISheet(m_main_menu);
			m_main_menu->addChildWindow(m_sub_windows["Options"]->getCEGUIWindow());
		}

		if (m_document->getGUIState()->m_sheet ==  Document::GAME_SCREEN)
		{
			m_cegui_system->setGUISheet(m_game_screen);
			m_game_screen->addChildWindow(m_sub_windows["Options"]->getCEGUIWindow());
		}
		m_document->setModified(m_document->getModified() & (~Document::GUISHEET_MODIFIED));
	}

	
	
	// Testen ob Anzeige der Subfenster geaendert werden muss
	if (m_document->getModified() & Document::WINDOWS_MODIFIED)
	{
		if (m_document->getGUIState()->m_sheet ==  Document::MAIN_MENU)
		{
			updateMainMenu();
		}
		DEBUG5("new shown windows %i",wflags);

		// Auswahlliste Savegames  anzeigen wenn entsprechendes Flag gesetzt
		CEGUI::FrameWindow* savelist = (CEGUI::FrameWindow*) win_mgr.getWindow("SavegameMenu");
		if (wflags & Document::SAVEGAME_LIST)
		{
			
			savelist->setVisible(true);
			m_sub_windows["SavegameList"]->update();
		}
		else
		{
			savelist->setVisible(false);
		}

		// Menu Spielstart anzeigen wenn entsprechendes Flag gesetzt
		CEGUI::FrameWindow* start_menu = (CEGUI::FrameWindow*) win_mgr.getWindow("StartMenu");
		if (wflags & Document::START_MENU)
		{
			start_menu->setVisible(true);
		}
		else
		{
			start_menu->setVisible(false);
		}
		
		CEGUI::FrameWindow* options = (CEGUI::FrameWindow*) win_mgr.getWindow("OptionsWindow");
		if (wflags & Document::OPTIONS)
		{
			static_cast<OptionsWindow*>(m_sub_windows["Options"])->reset();
			options->setVisible(true);
		}
		else
		{
			options->setVisible(false);
		}
		
		CEGUI::FrameWindow* char_create = (CEGUI::FrameWindow*) win_mgr.getWindow("CharCreate");
		if (wflags & Document::CHAR_CREATE)
		{
			char_create->setVisible(true);
		}
		else
		{
			char_create->setVisible(false);
		}

		if (m_document->getGUIState()->m_sheet ==  Document::GAME_SCREEN)
		{
			// Charinfo anzeigen wenn entsprechendes Flag gesetzt
			CEGUI::FrameWindow* char_info = (CEGUI::FrameWindow*) win_mgr.getWindow("CharInfo");
			if (wflags & Document::CHARINFO)
			{
				char_info->setVisible(true);
			}
			else
			{
				char_info->setVisible(false);
			}
	
			// Inventar anzeigen wenn entsprechendes Flag gesetzt
			CEGUI::FrameWindow* inventory = (CEGUI::FrameWindow*) win_mgr.getWindow("Inventory");
			if (wflags & Document::INVENTORY)
			{
				inventory->setVisible(true);
			}
			else
			{
				inventory->setVisible(false);
			}
			
			// QuestInfo anzeigen wenn entsprechendes Flag gesetzt
			CEGUI::FrameWindow* quest_info = (CEGUI::FrameWindow*) win_mgr.getWindow("QuestInfo");
			if (wflags & Document::QUEST_INFO)
			{
				quest_info->setVisible(true);
				m_sub_windows["QuestInfo"]->update();
			}
			else
			{
				quest_info->setVisible(false);
			}
			
			CEGUI::FrameWindow* minimap = (CEGUI::FrameWindow*) win_mgr.getWindow("MinimapWindow");
			if (wflags & Document::MINIMAP)
			{
				minimap->setVisible(true);
			}
			else
			{
				minimap->setVisible(false);
			}
			
			CEGUI::FrameWindow* trade = (CEGUI::FrameWindow*) win_mgr.getWindow("TradeWindow");
			if (wflags & Document::TRADE)
			{
				static_cast<TradeWindow*>(m_sub_windows["Trade"])->reset();
				m_sub_windows["Trade"]->update();
				trade->setVisible(true);
			}
			else
			{
				trade->setVisible(false);
			}
			
			// Chat Fenster anzeigen wenn entsprechendes Flag gesetzt
			CEGUI::FrameWindow* chat_window = (CEGUI::FrameWindow*) win_mgr.getWindow("ChatWindow");
			if (wflags & Document::CHAT)
			{
				// Fokus setzen, wenn das Fenster gerade geoeffnet wurde
				if (!chat_window->isVisible())
				{
					CEGUI::Editbox* chatline;
					chatline = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("Chatline"));
					chatline->activate();
				}
				
				chat_window->setVisible(true);
			}
			else
			{
				chat_window->setVisible(false);
				// Chatzeile deaktivieren
				CEGUI::Editbox* chatline;
				chatline = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("Chatline"));
				chatline->deactivate();
			}
			
			CEGUI::FrameWindow* party_info = (CEGUI::FrameWindow*) win_mgr.getWindow("PartyInfo");
			if (wflags & Document::PARTY)
			{
				party_info->setVisible(true);
			}
			else
			{
				party_info->setVisible(false);
			}
	
			if (m_document->getLocalPlayer()!=0)
			{
				// Skilltree anzeigen wenn entsprechendes Flag gesetzt
				CEGUI::TabControl* skilltree = (CEGUI::TabControl*) win_mgr.getWindow("SkilltreeMage");
				Player* player = m_document->getLocalPlayer();
	
				// Skilltree einstellen der der Spielerklasse entspricht
				if (player->getTypeInfo()->m_subtype == "warrior")
					skilltree = (CEGUI::TabControl*) win_mgr.getWindow("SkilltreeWarrior");
				if (player->getTypeInfo()->m_subtype == "archer")
					skilltree = (CEGUI::TabControl*) win_mgr.getWindow("SkilltreeArcher");
				if (player->getTypeInfo()->m_subtype == "priest")
					skilltree = (CEGUI::TabControl*) win_mgr.getWindow("SkilltreePriest");
	
				if (wflags & Document::SKILLTREE)
				{
					skilltree->setVisible(true);
				}
				else
				{
					skilltree->setVisible(false);
				}
				
				
			}
		}
		
		m_document->setModified(m_document->getModified() & ~Document::WINDOWS_MODIFIED);
	}
	
	if (wflags & Document::OPTIONS)
	{
		// Fenster Optionen aktualisieren
		m_sub_windows["Options"]->update();
	}

	// Objekte aus dem Dokument darstellen
	if (m_document->getLocalPlayer()!=0 && m_document->getLocalPlayer()->getRegion()!=0)
	{

		// Szene aktualisieren
		m_scene->update();

		// ObjectInfo aktualisieren
		updateObjectInfo();
		
		updateItemInfo();
		updateSpeechBubbles();
		
		// Bild am Curso aktualisieren
		updateCursorItemImage();
		
		// Informationen ueber Partymitglieder aktualisieren
		updatePartyInfo();

		if (wflags & Document::CHARINFO)
		{
			// Fenster CharacterInfo aktualisieren
			m_sub_windows["CharInfo"]->update();
		}

		if (wflags & Document::CHAT)
		{
			// Fenster CharacterInfo aktualisieren
			m_sub_windows["ChatWindow"]->update();
		}

		if (wflags & Document::MINIMAP)
		{
			// Fenster Minimap aktualisieren
			m_sub_windows["Minimap"]->update();
		}
		
		
		// Steuerleiste aktualisieren
		m_sub_windows["ControlPanel"]->update();


		if (wflags & Document::INVENTORY)
		{
			// Fenster Inventar aktualisieren
			m_sub_windows["Inventory"]->update();
		}
		if (wflags & Document::SKILLTREE)
		{
			// Skilltree aktualisieren
			m_sub_windows["SkillTree"]->update();
		}
		
		if (wflags & Document::PARTY)
		{
			// Skilltree aktualisieren
			m_sub_windows["PartyInfo"]->update();
		}
		
		if (wflags & Document::TRADE)
		{
			// Skilltree aktualisieren
			m_sub_windows["Trade"]->update();
		}
		
		// + Buttons fuer Levelup aktualisieren
		bool vis = false;
		CEGUI::PushButton* btn;
		btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("CharInfoUpgradeButton"));
		if (!(wflags & Document::CHARINFO) && m_document->getLocalPlayer()->getAttributePoints() >0)
		{
			vis = true;
		}
		
		if (btn->isVisible() != vis)
		{
			btn->setVisible(vis);
		}
		
		vis = false;
		btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("SkillUpgradeButton"));
		if (!(wflags & Document::SKILLTREE) && m_document->getLocalPlayer()->getSkillPoints() >0)
		{
			vis = true;
		}
		
		if (btn->isVisible() != vis)
		{
			btn->setVisible(vis);
		}
		
		/*
		vis = false;
		CEGUI::Window* label = win_mgr.getWindow("MinimapImage");
		if (wflags & Document::MINIMAP)
		{
			vis = true;
		}
		if (label->isVisible() != vis)
		{
			if (vis)
			{
				label->setProperty("Image", "set:minimap image:minimap_img"); 
			}
			label->setVisible(vis);
		}
		*/
	}
	
}


bool MainWindow::setupGameScreen()
{
	DEBUG4("setup game screen");
	try
	{
		// Szene erzeugen
		m_scene = new Scene(m_document,m_window);

		// GUI Elemente erzeugen
		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

		// Oberstes Fenster der Hauptmenue Schicht
		m_game_screen =  win_mgr.createWindow("DefaultWindow", "GameScreen");

		// Kontrollleiste anlegen
		setupControlPanel();

		// Inventar Fenster anlegen
		setupInventory();

		// CharInfo Fenster anlegen
		setupCharInfo();

		// Skilltree Fenster anlegen
		setupSkilltree();
		
		setupQuestInfo();

		// Leiste fuer Item/Objekt-Info anlegen
		setupObjectInfo();
		setupItemInfo();
		
		
		// Chatfenster anlegen
		setupChatWindow();
		
		// Fenster fuer Partyinfos
		setupPartyInfo();
		
		// Bild fuer das Item am Cursor
		setupCursorItemImage();
		
		setupMinimap();
		
		setupTrade();
		
		
		
	}
	catch (CEGUI::Exception e)
	{
		ERRORMSG("Error message: %s",e.getMessage().c_str());
	}

	return true;
}

void MainWindow::setupControlPanel()
{
	Window* wnd = new ControlPanel(m_document);
	m_sub_windows["ControlPanel"] = wnd;
	
	
	// Charakterinfo anfangs ausblenden
	m_game_screen->addChildWindow(wnd->getCEGUIWindow());
	
	// zwei weitere Buttons die Skilltree bzw Charinfo oeffnen, wenn Punkte zu verteilen sind
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "CharInfoUpgradeButton"));
	m_game_screen->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.02f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.07f)));
	btn->setText("+");
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ControlPanel::onButtonCharInfoClicked, static_cast<ControlPanel*>(wnd)));
	btn->setVisible(false);
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SkillUpgradeButton"));
	m_game_screen->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.92f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.07f)));
	btn->setText("+");
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ControlPanel::onButtonSkilltreeClicked, static_cast<ControlPanel*>(wnd)));
	btn->setVisible(false);
}

void MainWindow::setupCharInfo()
{
	Window* wnd = new CharInfo(m_document);
	m_sub_windows["CharInfo"] = wnd;
	
	
	// Charakterinfo anfangs ausblenden
	m_game_screen->addChildWindow(wnd->getCEGUIWindow());
	wnd->getCEGUIWindow()->setVisible(false);
}

void MainWindow::setupInventory()
{
	
	Window* wnd = new Inventory(m_document);
	m_sub_windows["Inventory"] = wnd;
	
	// Inventar anfangs ausblenden
	m_game_screen->addChildWindow(wnd->getCEGUIWindow());
	wnd->getCEGUIWindow()->setVisible(false);
	

}

void MainWindow::setupQuestInfo()
{
	Window* wnd = new QuestInfo(m_document);
	m_sub_windows["QuestInfo"] = wnd;
	
	// anfangs ausblenden
	m_game_screen->addChildWindow(wnd->getCEGUIWindow());
	wnd->getCEGUIWindow()->setVisible(false);
}


void MainWindow::setupSkilltree()
{
	
	SkillTree* wnd = new SkillTree(m_document);
	m_sub_windows["SkillTree"] = wnd;
	
	// Skilltree anfangs ausblenden
	m_game_screen->addChildWindow(wnd->getCEGUIWindow(0));
	m_game_screen->addChildWindow(wnd->getCEGUIWindow(1));
	m_game_screen->addChildWindow(wnd->getCEGUIWindow(2));
	m_game_screen->addChildWindow(wnd->getCEGUIWindow(3));
	
	wnd->getCEGUIWindow(0)->setVisible(false);
	wnd->getCEGUIWindow(1)->setVisible(false);
	wnd->getCEGUIWindow(2)->setVisible(false);
	wnd->getCEGUIWindow(3)->setVisible(false);
	
}

void MainWindow::setupChatWindow()
{
	Window* wnd = new ChatLine(m_document);
	m_sub_windows["ChatWindow"] = wnd;
	
	
	// Inventar anfangs ausblenden
	m_game_screen->addChildWindow(wnd->getCEGUIWindow());
	wnd->getCEGUIWindow()->setVisible(false);
}

void MainWindow::setupCursorItemImage()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::Window* label;
	label = win_mgr.createWindow("TaharezLook/StaticImage", "CursorItemImage");
	m_game_screen->addChildWindow(label);
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.04f), cegui_reldim( 0.06f)));
	label->setProperty("Image", "set:TaharezLook image:CloseButtonNormal"); 
	label->setVisible(false);
	label->setAlwaysOnTop(true);
	label->setMousePassThroughEnabled(true);
	label->setID(0);
}

void MainWindow::setupMinimap()
{
	Window* wnd = new MinimapWindow(m_document);
	m_sub_windows["Minimap"] = wnd;
	m_game_screen->addChildWindow(wnd->getCEGUIWindow());
	/*
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::Window* label;
 	label = win_mgr.createWindow("TaharezLook/StaticImage", "MinimapImage");
	m_game_screen->addChildWindow(label);
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.05)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.9f), cegui_reldim( 0.8f)));
	label->setProperty("Image", "set:TaharezLook image:CloseButtonNormal"); 
	label->setVisible(false);
	label->setMousePassThroughEnabled(true);
	label->setID(0);
	*/
	
	
}

bool MainWindow::setupObjectInfo()
{
	DEBUG4("setup object info");

	// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;

	// Leiste fuer Informationen
	
	CEGUI::ProgressBar* bar = static_cast<CEGUI::ProgressBar*>(win_mgr.createWindow("TaharezLook/ProgressBar", "MonsterHealthProgressBar"));
	m_game_screen->addChildWindow(bar);
	bar->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.02f)));
	bar->setSize(CEGUI::UVector2(cegui_reldim(0.6f), cegui_reldim( 0.07f)));
	bar->setWantsMultiClickEvents(false);
	bar->setProperty("MousePassThroughEnabled","true");
	bar->setProgress(1.0);
	
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "ObjectInfoLabel");
	m_game_screen->addChildWindow(label);
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setProperty("HorzFormatting", "HorzCentred");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.02f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.6f), cegui_reldim( 0.07f)));
	label->setAlpha(1.0);
	label->setProperty("MousePassThroughEnabled","true");
	
	

	return true;
}

bool MainWindow::setupItemInfo()
{
	DEBUG4("setup object info");

	// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;

	// Leiste fuer Informationen
	label = win_mgr.createWindow("TaharezLook/StaticText", "ItemInfoLabel");
	m_game_screen->addChildWindow(label);
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.02f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.08f), cegui_reldim( 0.03f)));
	label->setText("");
	label->setVisible(false);
	label->setAlpha(0.9);
	label->setProperty("MousePassThroughEnabled","true");

	return true;
}


bool MainWindow::setupPartyInfo()
{
	
	Window* wnd = new PartyInfo(m_document);
	m_sub_windows["PartyInfo"] = wnd;
	
	// PartyInfo anfangs ausblenden
	m_game_screen->addChildWindow(wnd->getCEGUIWindow());
	wnd->getCEGUIWindow()->setVisible(false);
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::Window* img;
	CEGUI::ProgressBar* bar;
	
	std::ostringstream stream;
	
	for (int i=0; i<7; i++)
	{
		stream.str("");
		stream << "PartyMemberImage";
		stream << i;
		
		img = win_mgr.createWindow("TaharezLook/StaticImage",stream.str());
		m_game_screen->addChildWindow(img);
		img->setProperty("FrameEnabled", "true");
		img->setProperty("BackgroundEnabled", "true");
		img->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim( 0.01f + 0.1f *i)));
		img->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.07f)));
		img->setID(i);
		img->setVisible(false);
		
		stream.str("");
		stream << "PartyMemberHealthBar";
		stream << i;
		
		bar = static_cast<CEGUI::ProgressBar*>(win_mgr.createWindow("TaharezLook/ProgressBar", stream.str()));
		m_game_screen->addChildWindow(bar);
		m_game_screen->addChildWindow(bar);
		bar->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim( 0.08f+ 0.1f *i)));
		bar->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.02f)));
		bar->setProgress(1.0);
		bar->setVisible(false);
	}
	
	return true;
}

void MainWindow::setupTrade()
{
	Window* wnd = new TradeWindow(m_document);
	m_sub_windows["Trade"] = wnd;
	
	
	// Inventar anfangs ausblenden
	m_game_screen->addChildWindow(wnd->getCEGUIWindow());
	wnd->getCEGUIWindow()->setVisible(false);
}


void  MainWindow::setWindowExtents(int width, int height){
	//Set Mouse Region.. if window resizes, we should alter this to reflect as well
	const OIS::MouseState &ms = m_mouse->getMouseState();
	ms.width = width;
	ms.height = height;
}


void  MainWindow::updateMainMenu()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* img;
	img  = win_mgr.getWindow("StartScreenImage");
	
	int wflags = m_document->getGUIState()->m_shown_windows;
	if (wflags & (Document::SAVEGAME_LIST | Document::CHAR_CREATE))
	{
		m_scene->changeViewportSize(Scene::VIEW_RIGHT);
		img->setVisible(false);
	}
	else
	{
		m_scene->changeViewportSize(Scene::VIEW_FULL);
		img->setVisible(true);
	}

}

void  MainWindow::updateCharCreate()
{
	m_sub_windows["CharCreate"]->update();

}


void MainWindow::updateCursorItemImage()
{
	Player* player = m_document->getLocalPlayer();
	Item* item = player->getEquipement()->getItem(Equipement::CURSOR_ITEM);
	
	
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label = win_mgr.getWindow("CursorItemImage");
	
	if (item ==0)
	{
		// Kein Item mehr am Cursor
		// Bild verstecken
		
		if (label->isVisible())
		{
			label->setVisible(false);
		}
	}
	else
	{
		// Es befindet sich ein Item am Cursor
		// Bild zeigen
		
		if (! label->isVisible())
		{
			label->setVisible(true);
		}	
		
		// Groesse des Items am Cursor ermitteln
		unsigned int size = item->m_size;
		
			
		if (size != label->getID())
		{
			// Groesse des Labels setzen
			if (size == Item::BIG)
			{
				label->setSize(CEGUI::UVector2(cegui_reldim(0.060f), cegui_reldim( 0.075f)));
			}
			else if (size == Item::MEDIUM)
			{
				label->setSize(CEGUI::UVector2(cegui_reldim(0.045f), cegui_reldim( 0.06f)));
			}
			else if (size == Item::SMALL)
			{
				label->setSize(CEGUI::UVector2(cegui_reldim(0.033f), cegui_reldim( 0.043f)));
			}
			
			label->setID(size);
		}
		
		std::ostringstream out_stream("");
		out_stream<< ItemWindow::getItemImage(item->m_subtype);
		
		if (label->getProperty("Image")!=out_stream.str())
		{
			label->setProperty("Image", out_stream.str());
			OIS::MouseEvent me(m_mouse,m_mouse->getMouseState ());
			
			mouseMoved(me);
		}
		
	}
}

void MainWindow::updateObjectInfo()
{
	Player* player = m_document->getLocalPlayer();
	short rid = player->getGridLocation()->m_region;
	Vector plpos = player->getShape()->m_center;

	// Ogre Name des Objektes auf das der Mauszeiger zeigt
	std::string objname = "";

	// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label = win_mgr.getWindow("ObjectInfoLabel");
	CEGUI::ProgressBar* bar = static_cast<CEGUI::ProgressBar*>(win_mgr.getWindow( "MonsterHealthProgressBar"));
	
	// Position der Maus
	float x = m_mouse->getMouseState().X.abs;
	float y = m_mouse->getMouseState().Y.abs;


	// Position des Mausklicks relativ zum Viewport
	Ogre::Viewport* viewport = m_scene->getViewport();
	float relx = x*1.0/(viewport->getActualWidth());
	float rely = y*1.0/(viewport->getActualHeight());

	// Strahl von der Kamera durch den angeklickten Punkt
	Ogre::Camera* camera = m_scene->getCamera();
	Ogre::Ray ray = camera->getCameraToViewportRay(relx,rely);

	Ogre::Entity* ent;
	
	
	Region* reg = World::getWorld()->getRegion(rid);
	if (reg ==0)
		return;


	// Ergebnispaar einer Intersection Abfrage
	std::pair<bool,Ogre::Real> isec;

	if (m_mouse->getMouseState().buttons !=0)
	{
		// fokussiertes Objekt nicht wechseln, wenn Maustaste gedrueckt
		objname = m_document->getGUIState()->m_cursor_object;
	}
	else
	{

		std::ostringstream out_stream;
		out_stream.str("");


		// Objekte in der Szene
		std::map<int,string>* objects = m_scene->getObjects();
		std::map<int,string>::iterator it;

		// minimale Distanz eines Objektes im Kamerastrahl
		float mindist = 1000000;
		float dist;
		
		for (it = objects->begin();it != objects->end();++it)
		{
			WorldObject* wo;
			
			if (m_document->getLocalPlayer()->getId() == it->first)
			{
				// Spieler selbst ueberspringen
				continue;
			}

			ent = m_scene_manager->getEntity(it->second);

			// Auf Ueberschneidung mit dem Kamerastrahl testen
			const Ogre::AxisAlignedBox& box = ent->getWorldBoundingBox();
			isec = ray.intersects(box);

			if (isec.first)
			{
				// nur aktive Objekte beruecksichtigen
				wo = reg->getObject(it->first);
				if (wo == 0)
				{
					continue;
				}
				
				if (wo->getState() != WorldObject::STATE_ACTIVE)
					continue;
						
				// Objekt wird vom Kamerastrahl geschnitten
				// Distance zum Spieler ausrechnen
				dist = plpos.distanceTo(wo->getShape()->m_center);
				if (dist<mindist)
				{
					// Objekt ist das bisher naechste
					objname = it->second;
					mindist = dist;
				}
			}

		}
		m_document->getGUIState()->m_cursor_object=objname;

		if (objname=="")
		{
			// aktuell kein Objekt unterm Mauszeiger
			m_document->getGUIState()->m_cursor_object_id =0;

			label->setVisible(false);
			bar->setVisible(false);
		}
	}

	std::string name;
	std::ostringstream string_stream;
	

	if (objname!="")
	{
		// es gibt ein ein Objekt unterm Mauszeiger
		m_document->getGUIState()->m_cursor_item_id =0;
		label->setVisible(true);
		bar->setVisible(true);
		
		// ID des Objektes ermitteln
		int pos = objname.find(":");
		std::string idstring = objname.substr(pos+1);
		name = objname.substr(0,pos);
		std::stringstream stream(idstring);
		int id;
		stream >> id;

		

		// zur ID gehoerendes Objekt
		WorldObject* cwo;
		cwo = World::getWorld()->getRegion(rid)->getObject(id);
		Creature* cr;

		if (cwo !=0)
		{
		// Objekt existiert
			m_document->getGUIState()->m_cursor_object_id = id;
			string_stream<<cwo->getName();
			if (cwo->getTypeInfo()->m_type != WorldObject::TypeInfo::TYPE_FIXED_OBJECT)
			{
				// Objekt ist ein Lebewesen
				// Lebenspunkte anfuegen
				cr = static_cast<Creature*>(cwo);
				//string_stream << " " << std::max(0,(int) (cr->getDynAttr()->m_health / cr->getBaseAttrMod()->m_max_health*100)) <<"%";
				float perc = cr->getDynAttr()->m_health / cr->getBaseAttrMod()->m_max_health;
				if (bar->getProgress() != perc)
				{
					bar->setProgress(perc);
				}
			}
			else
			{
				bar->setVisible(false);
			}
		}
		else
		{
			m_document->getGUIState()->m_cursor_object_id =0;
			m_document->getGUIState()->m_cursor_object="";
		}
	}


	name = string_stream.str();

	if (label->getText() != (CEGUI::utf8*) name.c_str())
	{

		label->setText((CEGUI::utf8*) name.c_str());
	}


}

void MainWindow::updateItemInfo()
{
	// Zaehler wie viele Labels fuer Items existieren
	static int lcount =0;
	
	// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label = win_mgr.getWindow("ItemInfoLabel");
	
	std::map<int,string>* itms = m_scene->getDropItems();
	std::map<int,string>::iterator it;
	
	DropItem* di;
	
	Player* player = m_document->getLocalPlayer();
	short rid = player->getGridLocation()->m_region;
	Vector plpos = player->getShape()->m_center;
	
	
	
	if (m_document->getGUIState()->m_cursor_object == "")
	{
		
		// Ogre Name des Objektes auf das der Mauszeiger zeigt
		std::string objname = "";
	
		// Position der Maus
		float x = m_mouse->getMouseState().X.abs;
		float y = m_mouse->getMouseState().Y.abs;
	
	
		// Position des Mausklicks relativ zum Viewport
		Ogre::Viewport* viewport = m_scene->getViewport();
		float relx = x*1.0/(viewport->getActualWidth());
		float rely = y*1.0/(viewport->getActualHeight());
	
		// Strahl von der Kamera durch den angeklickten Punkt
		Ogre::Camera* camera = m_scene->getCamera();
		Ogre::Ray ray = camera->getCameraToViewportRay(relx,rely);
		
		// Umrechnen in Spielkoordinaten
		// Ort an dem der Strahl den Boden beruehrt berechnen
		const Ogre::Vector3& orig = ray.getOrigin();
		const Ogre::Vector3& dir = ray.getDirection();
	
		// Schnittpunkt mit der Ebene y=0 ausrechnen
		Ogre::Vector3 p = orig + dir*(orig.y/(-dir.y));
		float gx = p.x/50;
		float gy = p.z/50;
	
		Ogre::Entity* ent;
		
		// Nach Items unterm Mauszeiger suchen
		// Ergebnispaar einer Intersection Abfrage
		std::pair<bool,Ogre::Real> isec;
			
		float mindist = 1000000;
		float dist;
		float ix,iy;
		float mix=0,miy=0;
		
		
		std::ostringstream out_stream;
		out_stream.str("");
			
		for (it = itms->begin();it != itms->end();++it)
		{
			ent = m_scene_manager->getEntity(it->second);
	
				// Auf Ueberschneidung mit dem Kamerastrahl testen
			const Ogre::AxisAlignedBox& box = ent->getWorldBoundingBox();
			isec = ray.intersects(box);
	
			if (isec.first)
			{
	
					// Objekt wird vom Kamerastrahl geschnitten
					// Das Objekt waehlen, dass am naechsten am Mauszeiger ist
				di = World::getWorld()->getRegion(rid)->getDropItem(it->first);
				if (di ==0)
				{
					continue;
				}
					
				ix = di->m_x/2.0;
				iy = di->m_y/2.0;
				dist = (gx-ix)*(gx-ix) + (gy-iy)*(gy-iy);
				if (dist < mindist)
				{
					mindist = dist;	
					objname = it->second;
					mix=ix;
					miy=iy;
						
				}
	
			}
	
		}
	
		if (objname!="")
		{
				// Item gefunden
				// ID des Objektes ermitteln
			int pos = objname.find(":");
			std::string idstring = objname.substr(pos+1);
			std::string name = objname.substr(0,pos);
			std::stringstream stream(idstring);
			int id;
			stream >> id;
	
			DEBUG5("item hover %s id %i",name.c_str(),id);
	
			Item* itm = player->getRegion()->getItem(id);
	
			if (itm !=0)
			{
				// Objekt existiert
				m_document->getGUIState()->m_cursor_item_id = id;
				
				if (!m_document->getGUIState()->m_alt_hold)
				{
					label->setVisible(true);
				}
				
				name = itm->getName();
				if (label->getText() != (CEGUI::utf8*) name.c_str())
				{

					label->setText((CEGUI::utf8*) name.c_str());
				}
				
				std::string propold = label->getProperty("TextColours").c_str();
				std::string propnew = "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF";
				if (itm->m_rarity == Item::MAGICAL)
				{
					propnew = "tl:FF5555AA tr:FF5555AA bl:FF5555AA br:FF5555AA";
				}
				if (propold != propnew)
				{
					label->setProperty("TextColours", propnew); 
				}
				
				
				std::pair<float,float> rpos = m_scene->getProjection(Vector(mix,miy));
				label->setPosition(CEGUI::UVector2(CEGUI::UDim(std::max(0.0,rpos.first-0.03),0), CEGUI::UDim(std::max(0.0,rpos.second-0.05),0)));
			}
			else
			{
				m_document->getGUIState()->m_cursor_item_id =0;
				label->setVisible(false);
			}
		}
		else
		{
			m_document->getGUIState()->m_cursor_item_id =0;
			label->setVisible(false);
		}
	}
	

	if (m_document->getGUIState()->m_alt_hold)
	{
		label->setVisible(false);
		
		// Alt gedrueckt
		// fuer alle Items ein Label erstellen
		std::ostringstream stream;
		int nr =0;
		std::string name;
		std::pair<float,float> rpos;
		
		float height = 0.030;
		int num = (int) (0.9/height);
		// fuer jede 0.025 hohe Spalte ein Element des Vektors
		// fuer jede Spalte eine Liste von Wertepaaren: Anfang, Ende
		std::vector< std::list < std::pair<float,float> > > itempos(num);
		
		// Lokales Koordinatensystem fuer die Items herstellen
		Vector pu,pl,pll,pmr,pml,ox,oy;
		pu = getIngamePos(0.5,0.1,true);
		pl = getIngamePos(0.5,0.9,true);
		pll = getIngamePos(0,0.9,true);
		pmr = getIngamePos(1.0,0.5,true);
		pml = getIngamePos(0,0.5,true);
		
		oy = pu - pl;
		ox = pmr - pml;
		
		Vector po = pll - pml;
		po.projectOn(ox);
		po = pll - po;
		
		static Vector oldoy = Vector(0,-1);
		
		if (fabs(oldoy.angle(oy)) < 0.01 && fabs(oy.getLength() - oldoy.getLength())<0.01 )
		{
			oy = oldoy;
		}
		else
		{
			oldoy = oy;
		}
		DEBUG5("oy %f %f",oy.m_x, oy.m_y);
		
		// Skalierung der Achsen
		float oxl = ox.getLength();
		float oyl = oy.getLength();
		
		float xscal = oxl;
		float yscal = oyl*height/0.9;
		
		ox.normalize();
		oy.normalize();

		DEBUG5("scal %f %f",xscal,yscal);
		
		// Ursprung des neuen Koordinatensystems
		Vector ypart = po;
		float ylen = ypart*oy;
		ypart.projectOn(oy);
		Vector xpart = po - ypart;
		
		DEBUG5("xpart %f %f",xpart.m_x, xpart.m_y);
		DEBUG5("ypart %f %f",ypart.m_x, ypart.m_y);
		float yoffs = ylen/yscal - roundf(ylen/yscal);
		
		
		Vector o = xpart + oy*(yscal*( roundf(ylen/yscal)));
		
		DEBUG5("pll %f %f %f",pll.m_x, pll.m_y,ylen);
		DEBUG5("o %f %f ", o.m_x, o.m_y);
		
		
		DEBUG5("\nitems\n");
		
		// Distanz zur wahren Position
		float optdist,dist;
		int optrow;
		float optcol;
		float len;
		// Begrenzung fuer den Schriftzug
		float lbound=0,rbound =1;
		int app;
		
		std::string propnew, propold;
		
		Vector tpos,pos;
		for (it = itms->begin();it != itms->end();++it)
		{
			
			di = World::getWorld()->getRegion(rid)->getDropItem(it->first);
			if (di ==0)
			{
				continue;
			}
			
			name = di->m_item->getName();
			DEBUG5(" ");
			DEBUG5(" %s %i",name.c_str(), di->getId());
			
			tpos = Vector(di->m_x/2.0,di->m_y/2.0);
			tpos -= o;
			
			pos.m_x = tpos*ox;
 			pos.m_y = tpos*oy;
			
			DEBUG5("tpos %f %f  %f",(tpos+o).m_x, (tpos+o).m_y,yscal);
			DEBUG5("test %f %f %f   oy %f %f",o*oy/yscal, (tpos+o)*oy/yscal, pos.m_y/yscal,oy.m_x, oy.m_y);
			
			// Position auf dem Bildschirm ermitteln
			DEBUG5("pos %f %f  -- %f",pos.m_x, pos.m_y,pos.m_y/yscal);
			rpos.first = pos.m_x / oxl;
			rpos.second = 0.9-pos.m_y / oyl*0.9;
			DEBUG5("rpos %f %f  - %f",rpos.first, rpos.second, rpos.second/height);
			
			// nur Items die wirklich sichtbar sind behandeln
			if (rpos.first<0 || rpos.first>1 || rpos.second<0 || rpos.second>0.9)
			{
				continue;
			}
			
			// optimale Position ermitteln
			// Distanz zur wahren Position
			optdist = 1000000;
			
			// beste gefundene Platzierung
			optrow= (int) (rpos.second/height);
			optcol= rpos.first;
			DEBUG5("optpos %i %f",optrow,optcol);
			
			// Laenge des Schriftzugs
			len = 0.009*name.length();
			app = 1;
			
			
			std::list < std::pair<float,float> >::iterator jt, optjt;
			
			
			// schleife ueber die Zeilen
			int row, centerrow = optrow;
			float col= rpos.second;
			int i;
			for (i=0; i<2*num; i++)
			{
				
				lbound=0;
				rbound =1;
				
				
				
				row = centerrow;
				if (i%2 == 0)
					row += (i+1)/2;
				else
					row -= (i+1)/2;
				
				if (row<0 || row >= num)
					continue;
				
				// Zeilen ueberspringen die so weit wegliegen, dass garantiert schon eine bessere Platzierung gefunden ist
				if ((rpos.second- row*height)*(rpos.second- row*height) > optdist)
					continue;
				
				// iterieren ueber die Liste der Gegenstaende, die bisher in der Zeile liegen
				// (genauer: die Luecken zwischen den Labels)
				for (jt = itempos[row].begin(); jt != itempos[row].end(); jt++)
				{
					rbound = jt->first;
					
					// Test ob das Label in die Luecke passt
					if (rbound -lbound < len)
					{
						lbound = jt->second;
						continue;
					}
					
					// Optimale Platzierung des Labels in der Luecke ermitteln
					if (rpos.first+len > rbound)
					{
						col = rbound-len;
					}
					else if (rpos.first<lbound)
					{
						col = lbound;
					}
					else
						col = rpos.first;
					
					dist = (rpos.first-col)*(rpos.first-col) + (centerrow-row)*(centerrow-row)*height*height;
					if (dist+0.0001 < optdist)
					{
						optdist = dist;
						optcol = col;
						optrow = row;
						optjt = jt;
						app = 0;
						
						DEBUG5("optdist %f optrow %i pos %i %f  app %i",optdist, optrow, row, col,app);
					}
					
					lbound = jt->second;
				}
				
				// Option, das Label hinter dem letzten bestehenden Label einzufuegen
				rbound = 1;
				if (rbound-lbound >= len)
				{
					if (rpos.first<lbound)
					{
						col = lbound;
					}
					else
						col = rpos.first;
					
					dist = (rpos.first-col)*(rpos.first-col) + (centerrow-row)*(centerrow-row)*height*height;
					
					if (dist+0.0001 < optdist)
					{
						app = 1;
						optdist = dist;
						optcol = col;
						optrow = row;
						
						DEBUG5("optdist %f optrow %i pos %i %f  app %i",optdist, optrow, row, col,app);
					}
					
				}
				
				
				
			}
			
			if (optdist > 10000)
				continue;
			
			DEBUG5("center %i rpos %f  %f",centerrow,rpos.second, rpos.second-centerrow*height);
			
			rpos.first=optcol-len/2;
			rpos.second=(optrow + yoffs) *height;
			
			
			
			DEBUG5("optdist %f optrow %i pos %f %f  app %i",optdist, optrow, rpos.second, rpos.first,app);
			
			if (app == 1)
			{
				itempos[optrow].push_back(std::make_pair(optcol,optcol+len));
			}
			else
			{
				DEBUG5("insert before %f %f",optjt->first, optjt->second);
				itempos[optrow].insert(optjt,std::make_pair(optcol,optcol+len));
			}
			
			if (rpos.first<0 || rpos.first+len>1 || rpos.second<0 || rpos.second+height>0.9)
				continue;
			
			stream.str("");
			stream << "ItemLabel";
			stream << nr;
			
			if (nr >= lcount)
			{
				lcount ++;
				label = win_mgr.createWindow("TaharezLook/StaticText", stream.str());
				m_game_screen->addChildWindow(label);
				label->setProperty("FrameEnabled", "false");
				label->setProperty("BackgroundEnabled", "false");
				label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.02f)));
				label->setSize(CEGUI::UVector2(cegui_reldim(0.08f), cegui_reldim( 0.03f)));
				label->setText("");
				label->setAlpha(0.9);
				label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onDropItemClicked, this));
				
			}
			else
			{
				label = win_mgr.getWindow(stream.str());
			}
			label->setVisible(true);
			
			
			if (label->getText() != (CEGUI::utf8*) name.c_str())
			{
	
				label->setText((CEGUI::utf8*) name.c_str());
			}
			
			label->setID(it->first);
			
			
			label->setPosition(CEGUI::UVector2(CEGUI::UDim(std::max(0.0f,rpos.first),0), CEGUI::UDim(std::max(0.0f,rpos.second),0)));
			
			propold = label->getProperty("TextColours").c_str();
			propnew = "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF";
			if (di->m_item->m_rarity == Item::MAGICAL)
			{
				propnew = "tl:FF5555AA tr:FF5555AA bl:FF5555AA br:FF5555AA";
			}
			if (propold != propnew)
			{
				label->setProperty("TextColours", propnew); 
			}
			nr++;

		}
		
		
		for (; nr<lcount; nr++)
		{
			stream.str("");
			stream << "ItemLabel";
			stream << nr;
			
			label = win_mgr.getWindow(stream.str());
			label->setVisible(false);
		}
		
	}
	else
	{
		std::ostringstream stream;
		for (int i=0; i<lcount; i++)
		{
			stream.str("");
			stream << "ItemLabel";
			stream << i;
			label = win_mgr.getWindow(stream.str());
			label->setVisible(false);
		}
	}
}

void MainWindow::updateSpeechBubbles()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// Zaehler wie viele Labels fuer Items existieren
	static int lcount =0;
	
	// Zaehler, wie viele Antwortmoeglichkeiten existieren
	static int acount =0;
	
	Player* player = m_document->getLocalPlayer();
	
	if (player ==0 || player->getRegion() ==0)
		return;
	
	CEGUI::Window* label;
	CEGUI::FrameWindow* ques;
	
	// Objekte im Umkreis von 20 Meter holen
	std::list<WorldObject*> objs;
	std::list<WorldObject*>::iterator it;
	
	Shape s;
	s.m_center = player->getShape()->m_center;
	s.m_type = Shape::RECT;
	s.m_extent = Vector(20,20);
	player->getRegion()->getObjectsInShape(&s,&objs);
	
	std::pair<float,float> pos;
	
	Creature* cr;
	
	int nr =0;
	std::string text;
	
	std::stringstream stream;
	
	CreatureSpeakText* question=0;
	
	for (it = objs.begin(); it != objs.end(); ++it)
	{
		// nur Kreaturen behandeln
		if (!(*it)->isCreature())
			continue;
		
		cr = static_cast<Creature*>(*it);
		pos = m_scene->getProjection(cr->getShape()->m_center);
		
		// nur Kreaturen behandeln, die wirklich zu sehen sind
		if (pos.first <0 || pos.first >1 || pos.second <0 || pos.second >1)
			continue;
		
		text = cr->getSpeakText().m_text;
		
		if (text == "")
			continue;
		
		// Fragen werden gesondert behandelt
		if (!cr->getSpeakText().m_answers.empty())
		{
			if (cr == player)
				question = &(cr->getSpeakText());
			continue;
		}
		
		stream.str("");
		stream << "SpeechLabel";
		stream << nr;
		
		if (nr >= lcount)
		{
			lcount ++;
			label = win_mgr.createWindow("TaharezLook/StaticText", stream.str());
			m_game_screen->addChildWindow(label);
			label->setProperty("FrameEnabled", "true");
			label->setProperty("BackgroundEnabled", "true");
			
			label->setText("");
			label->setAlpha(0.9);
			
		}
		else
		{
			label = win_mgr.getWindow(stream.str());
		}
		
			
			
		if (label->getText() != (CEGUI::utf8*) text.c_str())
		{
			float len = text.size()*0.01+0.03;
			label->setText((CEGUI::utf8*) text.c_str());
			
			label->setSize(CEGUI::UVector2(cegui_reldim(len), cegui_reldim( 0.06f)));
			label->setPosition(CEGUI::UVector2(CEGUI::UDim(std::max(0.0f,pos.first-len/2),0), CEGUI::UDim(std::max(0.0f,pos.second-0.1f),0)));
		}
		label->setVisible(true);
		
		nr++;
		
	}
	
	// restliche Label verstecken
	for (; nr<lcount; nr++)
	{
		stream.str("");
		stream << "SpeechLabel";
		stream << nr;
			
		label = win_mgr.getWindow(stream.str());
		label->setVisible(false);
	}
	
	// Fenster fuer eine Frage
	if (question !=0)
	{
		int wflags = m_document->getGUIState()->m_shown_windows;
		if (wflags != Document::QUESTIONBOX)
		{
			wflags = m_document->getGUIState()->m_shown_windows= Document::QUESTIONBOX;
			m_document->setModified(m_document->getModified() | Document::WINDOWS_MODIFIED);
		}
		
		
		if (acount ==0)
		{
			ques = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "QuestionWindow");
			ques->setProperty("FrameEnabled","false");
			ques->setProperty("TitlebarEnabled","false");
			ques->setProperty("CloseButtonEnabled","false");
			m_game_screen->addChildWindow(ques);
			ques->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::consumeEvent, this));
			ques->setVisible(false);
			
			label = win_mgr.createWindow("TaharezLook/StaticText", "QuestionLabel");
			ques->addChildWindow(label);
			label->setProperty("FrameEnabled", "false");
			label->setProperty("BackgroundEnabled", "false");

		}
		else
		{
			ques = (CEGUI::FrameWindow*)win_mgr.getWindow("QuestionWindow");
			label = (CEGUI::Window*) win_mgr.getWindow("QuestionLabel");
		}
		
		// Wenn das Frage Fenster schon sichtbar ist -> keine Aenderung
		if (ques->isVisible())
		{
			return;
		}
		nr =0;
		
		// Groesse des Fensters ermitteln
		float h = 0.07 + question->m_answers.size()*0.05;
		float len =0;
		float relh = 1/(1.5+question->m_answers.size());
		
		if (label->getText() != (CEGUI::utf8*) question->m_text.c_str())
		{
			label->setText((CEGUI::utf8*) question->m_text.c_str());
		}
		
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.1*relh)));
		label->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim(relh)));
		
		// Antworten einfuegen
		std::list < std::pair<std::string, std::string> >::iterator it;
		for (it = question->m_answers.begin(); it != question->m_answers.end(); ++it)
		{
			len = std::max(len, it->first.size()*0.01f);
			
			stream.str("");
			stream << "AnswerLabel";
			stream << nr;
			
			if (nr >= acount)
			{
				acount ++;
				label = win_mgr.createWindow("TaharezLook/StaticText", stream.str());
				ques->addChildWindow(label);
				label->setProperty("FrameEnabled", "false");
				label->setProperty("BackgroundEnabled", "false");
				label->setID(nr);
				label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onAnswerClicked, this));
			}
			else
			{
				label = win_mgr.getWindow(stream.str());
			}
			
			if (label->getText() != (CEGUI::utf8*) it->first.c_str())
			{
				label->setText((CEGUI::utf8*) it->first.c_str());
			}
			
			label->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim((nr+1.4)*relh)));
			label->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim(relh)));
			label->setVisible(true);
			
			nr++;
		}
		
		ques->setPosition(CEGUI::UVector2(cegui_reldim(0.5f-len/2), cegui_reldim(0.2)));
		ques->setSize(CEGUI::UVector2(cegui_reldim(len), cegui_reldim( h)));
		ques->setVisible(true);
		
		// restliche Antwortlabels ausblenden
		for (; nr<acount; nr++)
		{
			stream.str("");
			stream << "AnswerLabel";
			stream << nr;
			
			label = win_mgr.getWindow(stream.str());
			label->setVisible(false);
		}
	}
	else if (acount !=0)
	{
		m_document->getGUIState()->m_shown_windows &= ~Document::QUESTIONBOX;
		ques = (CEGUI::FrameWindow*) win_mgr.getWindow("QuestionWindow");
		ques->setVisible(false);
		
	}
}

void MainWindow::updatePartyInfo()
{
	int windows = m_document->getGUIState()->m_shown_windows;
	if (windows & Document::PARTY)
	{
		// Fenster PartyInfo aktualisieren
		m_sub_windows["PartyInfo"]->update();
	}
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::Window* img;
	CEGUI::ProgressBar* bar;
	
	std::ostringstream stream;
	
	int i=0;
	if (!( windows & Document::PARTY || windows & Document::CHARINFO)) 
	{
		float hperc;
		Player* player = m_document->getLocalPlayer();
		Player* pl;
		Party* party = World::getWorld()->getPartyFrac(player->getFraction());
		
		std::set<int>::iterator it;
		// Schleife ueber die Mitglieder der Party des Spielers
		for (it =  party->getMembers().begin(); it!= party->getMembers().end(); it++)
		{
			// eigenen Spieler ueberspringen
			if (*it == player->getId())
				continue;
			
			pl =  static_cast<Player*>(World::getWorld()->getPlayer(*it));
			
			stream.str("");
			stream << "PartyMemberImage";
			stream << i;
			img = win_mgr.getWindow(stream.str());
			
			// Bild setzen und anzeigen
			if (!img->isVisible())
			{
				img->setVisible(true);
			}
			
			stream.str("");
			stream << "PartyMemberHealthBar";
			stream << i;
			bar = static_cast<CEGUI::ProgressBar*>(win_mgr.getWindow(stream.str()));
			
			// Fortschrittsbalken setzen und anzeigen
			hperc = pl->getDynAttr()->m_health / pl->getBaseAttrMod()->m_max_health;
			if (bar->getProgress() != hperc)
			{
				bar->setProgress(hperc);
			}
			
			if (!bar->isVisible())
			{
				bar->setVisible(true);
			}
			
			i++;
		}
	}
	
	
	// restliche Bilder verstecken
	for (;i<7; i++)
	{
		stream.str("");
		stream << "PartyMemberImage";
		stream << i;
		img = win_mgr.getWindow(stream.str());
		if (img->isVisible())
		{
			img->setVisible(false);
		}
		
		stream.str("");
		stream << "PartyMemberHealthBar";
		stream << i;
		bar = static_cast<CEGUI::ProgressBar*>(win_mgr.getWindow(stream.str()));
		if (bar->isVisible())
		{
			bar->setVisible(false);
		}

	}
}

Vector MainWindow::getIngamePos(float screenx, float screeny, bool relative)
{
	// Position des Mausklicks relativ zum Viewport
	Ogre::Viewport* viewport = m_scene->getViewport();
	float relx,rely;
	if (relative)
	{
		relx = screenx;
		rely = screeny;
	}
	else
	{
		relx = screenx*1.0/(viewport->getActualWidth());
		rely = screeny*1.0/(viewport->getActualHeight());
	}
	DEBUG5("relative Koordinaten %f %f",relx,rely);

	// Strahl von der Kamera durch den angeklickten Punkt
	Ogre::Camera* camera = m_scene->getCamera();
	Ogre::Ray ray = camera->getCameraToViewportRay(relx,rely);

	// Ursprung und Richtung des Strahles
	const Ogre::Vector3& orig = ray.getOrigin();
	DEBUG5("ray orig %f %f %f",orig.x,orig.y,orig.z);
	const Ogre::Vector3& dir = ray.getDirection();
	DEBUG5("ray dir %f %f %f",dir.x,dir.y,dir.z);

	// Schnittpunkt mit der Ebene y=0 ausrechnen
	
	Vector ret;
	float dy = dir.y;
	if (dir.y>=0)
	{
		dy = -0.001;
	}
	
	
	// Durchstosspunkt durch die Ebene y=0
	Ogre::Vector3 p = orig + dir*(orig.y/(-dy));
	DEBUG5("schnittpunkt %f %f %f",p.x,p.y,p.z);

	// Umrechnen in Spielkoordinaten
	ret.m_x = p.x/50;
	ret.m_y = p.z/50;
	DEBUG5("Punkt in Spielkoordinaten %f %f",ret.m_x,ret.m_y);
	
	return ret;

}

// MouseListener
bool MainWindow::mouseMoved(const OIS::MouseEvent &evt) {
	m_cegui_system->injectMouseWheelChange(evt.state.Z.rel);
	
	
	//return m_cegui_system->injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
	//DEBUG("injection position %i %i",evt.state.X.abs,evt.state.Y.abs);
	m_document->onMouseMove(evt.state.X.rel, evt.state.Y.rel,evt.state.Z.rel);
	
	if (m_document->getGUIState()->m_left_mouse_pressed || m_document->getGUIState()->m_right_mouse_pressed)
	{
		Vector pos = getIngamePos(evt.state.X.abs,evt.state.Y.abs);
		m_document->getGUIState()->m_clicked = pos;
	}
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label = win_mgr.getWindow("CursorItemImage");
	
	int off = 0;
	if (label->getID() == Item::BIG)
		off = 24;
	if (label->getID() == Item::MEDIUM)
		off = 16;
	if (label->getID() == Item::SMALL)
		off = 12;
	
	
	label->setPosition(CEGUI::UVector2(CEGUI::UDim(0,std::max(0,evt.state.X.abs-off)),CEGUI::UDim(0,std::max(0,evt.state.Y.abs- off))));
	
	
	return m_cegui_system->injectMousePosition(evt.state.X.abs,evt.state.Y.abs);
}

bool MainWindow::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID btn)
{
	m_gui_hit = false;

	CEGUI::MouseButton button = CEGUI::NoButton;

	if (btn == OIS::MB_Left)
		button = CEGUI::LeftButton;

	if (btn == OIS::MB_Middle)
		button = CEGUI::MiddleButton;

	if (btn == OIS::MB_Right)
		button = CEGUI::RightButton;

	bool ret = m_cegui_system->injectMouseButtonDown(button);
	
	if (m_document->getGUIState()->m_sheet ==  Document::MAIN_MENU)
	{
		m_document->onStartScreenClicked();
		return true;
	}

	// Koordinaten an denen der Mausklick stattfand
	int x =evt.state.X.abs;
	int y =evt.state.Y.abs;
	DEBUG5("maus %i %i",x,y);

	Player* player = m_document->getLocalPlayer();
	if (player!=0)
	{
		// Spiel ist mit Server verbunden
		if (btn == OIS::MB_Middle)
		{
			m_document->getGUIState()->m_middle_mouse_pressed=true;
		}

		// Testet, dass man nicht auf die untere Steuerleiste geklickt hat
		if (not ret)
		{
			if (player->getEquipement()->getItem(Equipement::CURSOR_ITEM)!=0)
			{
				// Spieler hat Item in der Hand, fallen lassen
				m_document->dropCursorItem();
			}
			else
			{

				Vector pos = getIngamePos(x,y);

				if (btn == OIS::MB_Left)
				{
					m_document->getGUIState()->m_left_mouse_pressed=true;
					m_document->onLeftMouseButtonClick(pos);
				}
				else if (btn == OIS::MB_Right)
				{
					m_document->getGUIState()->m_right_mouse_pressed=true;
					m_document->onRightMouseButtonClick(pos);
				}


				
			}
		}

	}

	return ret;
}

bool MainWindow::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID btn) {
	CEGUI::MouseButton button = CEGUI::NoButton;

	if (btn==OIS::MB_Left)
	{
		DEBUG5("Button release");
		m_document->getGUIState()->m_left_mouse_pressed=false;
		m_document->getGUIState()->m_clicked_object_id=0;
	}

	if (btn==OIS::MB_Right)
	{
		DEBUG5("Right Button release");
		m_document->getGUIState()->m_right_mouse_pressed=false;
		m_document->getGUIState()->m_clicked_object_id=0;
	}
	
	if (btn == OIS::MB_Middle)
	{
		m_document->getGUIState()->m_middle_mouse_pressed=false;
	}

	if (btn == OIS::MB_Left)
		button = CEGUI::LeftButton;

	if (btn == OIS::MB_Middle)
		button = CEGUI::MiddleButton;

	if (btn == OIS::MB_Right)
		button = CEGUI::RightButton;

	return m_cegui_system->injectMouseButtonUp(button);
}


bool MainWindow::keyPressed(const OIS::KeyEvent &evt) {
	unsigned int ch = evt.text;
	
	DEBUG5("keycode %x",evt.key);
	if (m_document->getGUIState()->m_shown_windows & Document::OPTIONS)
	{
		if (static_cast<OptionsWindow*>(m_sub_windows["Options"])->requestsForKey())
		{
			static_cast<OptionsWindow*>(m_sub_windows["Options"])->setKeyCode(evt.key);
			return true;
		}
	}
	
	bool ret =m_cegui_system->injectKeyDown(evt.key);
	
	ret |= m_cegui_system->injectChar(ch);
	
	if (ch != 0 &&  (m_document->getGUIState()->m_shown_windows & Document::CHAT))
	{
		m_key = ch;
		m_key_repeat_timer.start();
		m_key_repeat = false;
	}
	
	
	if (evt.key == OIS::KC_RSHIFT || evt.key == OIS::KC_LSHIFT)
	{
		m_document->getGUIState()->m_shift_hold = true;
	}
	
	if (evt.key == OIS::KC_LMENU)
	{
		m_document->getGUIState()->m_alt_hold = true;
	}
	
	if (not ret)
	{
		ret = m_document->onKeyPress(evt.key);
	}
	return ret;
}

bool MainWindow::keyReleased(const OIS::KeyEvent &evt)
{

	m_key =0;
	m_key_repeat = false;
	
	bool ret = m_cegui_system->injectKeyUp(evt.key);
	
	if (evt.key == OIS::KC_RSHIFT || evt.key == OIS::KC_LSHIFT)
	{
		m_document->getGUIState()->m_shift_hold = false;
	}

	if (evt.key == OIS::KC_LMENU)
	{
		m_document->getGUIState()->m_alt_hold = false;
	}
	
	if (not ret)
	{
		ret = m_document->onKeyRelease(evt.key);
	}

	return ret;
}

bool MainWindow::onDropItemClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	
	DEBUG5("pick up item %i",id);
	m_document->onDropItemClick(id);
	return true;
}


bool MainWindow::onAnswerClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	
	m_document->onAnswerClick(id);
	return true;
}

bool MainWindow::consumeEvent(const CEGUI::EventArgs& evt)
{
	return true;
}










