#include "mainwindow.h"

#include <string>
#include <sstream>

using namespace std;


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

		DEBUG4("setup main menu");
		// GUI Elemente erzeugen

		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

		// Oberstes Fenster der Hauptmenue Schicht
		m_main_menu = win_mgr.createWindow("DefaultWindow", "MainMenu");

		// Rahmen fuer das Menue Savegame auswaehlen
		CEGUI::FrameWindow* save_menu = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "SavegameMenu");
		m_main_menu->addChildWindow(save_menu);
		save_menu->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f))); //0.0/0.8
		save_menu->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 1.0f))); //1.0/0.2
		save_menu->setProperty("FrameEnabled","false");
		save_menu->setProperty("TitlebarEnabled","false");
		save_menu->setProperty("CloseButtonEnabled","false");

		// Bestandteile der Kontrollleiste hinzufuegen
		CEGUI::PushButton* btn;
		CEGUI::Window* label;

		// Label Savegame waehlen
		label = win_mgr.createWindow("TaharezLook/StaticText", "SavegameChooseLabel");
		save_menu->addChildWindow(label);
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.1f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.1f)));
		label->setText("Savegame auswaehlen");

		CEGUI::MultiColumnList* savelist = (CEGUI::MultiColumnList*) win_mgr.createWindow("TaharezLook/MultiColumnList", "SavegameList");
		save_menu->addChildWindow(savelist);
		savelist->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.2f)));
		savelist->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.6f)));

		savelist->setSelectionMode(CEGUI::MultiColumnList::RowSingle);
		savelist->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged, CEGUI::Event::Subscriber(&MainWindow::onSavegameChosen, this));

		savelist->addColumn("Name",0,CEGUI::UDim(0.5,0));
		savelist->addColumn("Klasse",1,CEGUI::UDim(0.3,0));
		savelist->addColumn("Level",2,CEGUI::UDim(0.2,0));

		// Button Savegame akzeptieren
		btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SelectSavegameButton"));
		save_menu->addChildWindow(btn);
 		btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.85f)));
		btn->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.10f)));
		btn->setText("Ok");
		btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onSavegameSelected, this));
		btn->setWantsMultiClickEvents(false);

		// Rahmen fuer das Menue Savegame auswaehlen
		CEGUI::FrameWindow* start_menu = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "StartMenu");
		m_main_menu->addChildWindow(start_menu);
		start_menu->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.25f))); //0.0/0.8
		start_menu->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.5f))); //1.0/0.2
		start_menu->setProperty("FrameEnabled","false");
		start_menu->setProperty("TitlebarEnabled","false");
		start_menu->setProperty("CloseButtonEnabled","false");

		// Button Einzelspieler
		btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SinglePlayerButton"));
		start_menu->addChildWindow(btn);
		btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.1f)));
		btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.2f)));
		btn->setText("Einzelspieler");
		btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onStartSinglePlayer, this));
		btn->setWantsMultiClickEvents(false);

		// Button Server beitreten
		btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "ServerJoinButton"));
		start_menu->addChildWindow(btn);
		btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.4f)));
		btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.2f)));
		btn->setText("Server beitreten");
		btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onStartMultiPlayer, this));
		btn->setWantsMultiClickEvents(false);

		// Button Server aufsetzen
		btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "ServerHostButton"));
		start_menu->addChildWindow(btn);
		btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.7f)));
		btn->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.2f)));
		btn->setText("Server aufsetzen");
		btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onStartMultiPlayerHost, this));
		btn->setWantsMultiClickEvents(false);

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

		}

		if (m_document->getGUIState()->m_sheet ==  Document::GAME_SCREEN)
		{
			m_cegui_system->setGUISheet(m_game_screen);
		}
		m_document->setModified(m_document->getModified() & (~Document::GUISHEET_MODIFIED));
	}

	// Testen ob Anzeige der Subfenster geaendert werden muss
	if (m_document->getModified() & Document::WINDOWS_MODIFIED)
	{

		DEBUG5("new shown windows %i",wflags);

		// Auswahlliste Savegames  anzeigen wenn entsprechendes Flag gesetzt
		CEGUI::FrameWindow* savelist = (CEGUI::FrameWindow*) win_mgr.getWindow("SavegameMenu");
		if (wflags & Document::SAVEGAME_LIST)
		{
			savelist->setVisible(true);
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
		m_document->setModified(m_document->getModified() & ~Document::WINDOWS_MODIFIED);
	}


	// Objekte aus dem Dokument darstellen
	if (m_document->getLocalPlayer()!=0)
	{
		
		
		// Szene aktualisieren
		m_scene->update();
		
		

		// ObjectInfo aktualisieren
		updateObjectInfo();

		if (wflags & Document::CHARINFO)
		{
			// Fenster CharacterInfo aktualisieren
			updateCharInfo();
		}


		// Steuerleiste aktualisieren
		updateControlPanel();


		if (wflags & Document::INVENTORY)
		{
			// Fenster Inventar aktualisieren
			updateInventory();
		}
		if (wflags & Document::SKILLTREE)
		{
			// Skilltree aktualisieren
			updateSkilltree();
		}
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

		// Leiste fuer Objekt-Info anlegen
		setupObjectInfo();
	}
	catch (CEGUI::Exception e)
	{
		ERRORMSG("Error message: %s",e.getMessage().c_str());
	}

	return true;
}

void MainWindow::setupControlPanel()
{
	DEBUG4("setup control panel");

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	std::string name;

	// Rahmen fuer die untere Kontrollleiste
	CEGUI::FrameWindow* ctrl_panel = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "ControlPanel");
	m_game_screen->addChildWindow(ctrl_panel);
	ctrl_panel->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.88f))); //0.0/0.8
	ctrl_panel->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 0.12f))); //1.0/0.2
	ctrl_panel->setProperty("FrameEnabled","false");
	ctrl_panel->setProperty("TitlebarEnabled","false");
	ctrl_panel->setProperty("CloseButtonEnabled","false");
	ctrl_panel->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::consumeEvent, this));
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
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onButtonInventoryClicked, this));
	btn->setWantsMultiClickEvents(false);

	// Button Charakterinfo
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "CharInfoButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.3675f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.30f)));
	btn->setText("C");
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onButtonCharInfoClicked, this));

	// Button Chat oeffnen
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "ChatOpenButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.64f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.30f)));
	btn->setText("M");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onButtonOpenChatClicked, this));
	btn->setWantsMultiClickEvents(false);

	// Button SkillTree
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SkillTreeButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.5275f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.30f)));
	btn->setText("T");
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onButtonSkilltreeClicked, this));

	// Button Party
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "PartyButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.255f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.30f)));
	btn->setText("P");
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onButtonPartyClicked, this));

	// Button Speichern und Beenden
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SaveExitButton"));
	ctrl_panel->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.835f), cegui_reldim( 0.30f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.155f), cegui_reldim( 0.30f)));
	btn->setText(gettext("main_saveexit"));
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onButtonSaveExitClicked, this));

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
	ostringstream outStream;
	for (i=0;i<10;i++)
	{
		outStream.str("");
		outStream << "InventoryItem" << i;
		label = win_mgr.createWindow("TaharezLook/StaticText", outStream.str());
		ctrl_panel->addChildWindow(label);
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.225f+i*0.05f), cegui_reldim( 0.05f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.045f), cegui_reldim( 0.5f)));
		outStream.str("");
		outStream << "I" << i;
		label->setText(outStream.str());
		label->setID(Equipement::SMALL_ITEMS+i);
		label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
		label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));
	}

	DEBUG("end");

	// Verbinden mit dem Document
}

void MainWindow::setupCharInfo()
{
	DEBUG4("setup charinfo");

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();


	// Rahmen fuer CharInfo Fenster
	CEGUI::FrameWindow* char_info = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "CharInfo");
	m_game_screen->addChildWindow(char_info);
	char_info->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
	char_info->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	char_info->setProperty("FrameEnabled","false");
	char_info->setProperty("TitlebarEnabled","false");
	char_info->setProperty("CloseButtonEnabled","false");
	char_info->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::consumeEvent, this));
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
	label->setText("Name");

	// Label Klasse
	label = win_mgr.createWindow("TaharezLook/StaticText", "ClassLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.0f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.1f)));
	label->setText("Klasse");

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
	label->setText(gettext("main_power"));

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
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onIncreaseAttributeButtonClicked, this));


	// Label Geschick
	label = win_mgr.createWindow("TaharezLook/StaticText", "DexterityLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.23f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_dexterity"));

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
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onIncreaseAttributeButtonClicked, this));

	// Label Willenskraft
	label = win_mgr.createWindow("TaharezLook/StaticText", "WillpowerLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.31f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_willpower"));

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
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onIncreaseAttributeButtonClicked, this));

	// Label Zauberkraft
	label = win_mgr.createWindow("TaharezLook/StaticText", "MagicpowerLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.39f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_magic_power"));

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
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onIncreaseAttributeButtonClicked, this));

	// Label Attributpunkte
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttrPointsLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.47f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_credits"));

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
	label->setText(gettext("main_experience"));

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
	label->setText(gettext("main_healthpoints"));

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
	label->setText(gettext("main_attack"));

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
	label->setText(gettext("main_attacks_per_second"));

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
	label->setText(gettext("main_range"));

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
	label->setText(gettext("main_penetration_power"));

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
	label->setText(gettext("main_armour"));

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
	label->setText(gettext("main_block"));

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
	label->setText(gettext("main_base_damage"));

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
	label->setText(gettext("main_physical_resistance"));

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
	label->setText(gettext("main_fire_resistance"));

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
	label->setText(gettext("main_air_resistance"));

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
	label->setText(gettext("main_ice_resistance"));

	// Label Eisresistenz (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistIceValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.90f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0");

	// Charakterinfo anfangs ausblenden
	char_info->setVisible(false);
}

void MainWindow::setupInventory()
{
	DEBUG4("setup inventory");

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	// Rahmen fuer Inventar Fenster
	CEGUI::FrameWindow* inventory = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "Inventory");
	m_game_screen->addChildWindow(inventory);
	inventory->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	inventory->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.78f)));
	inventory->setProperty("FrameEnabled","false");
	inventory->setProperty("TitlebarEnabled","false");
	inventory->setProperty("CloseButtonEnabled","false");
	inventory->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::consumeEvent, this));
	inventory->setWantsMultiClickEvents(false);
	inventory->setAlpha(0.7);

	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;

	int i,j;

	// Label fuer grosse Items
	ostringstream outStream;
	for (i=0;i<5;i++)
	{
		outStream.str("");
		outStream << "BigItem" << i<< "Label";
		label = win_mgr.createWindow("TaharezLook/StaticText", outStream.str());
		inventory->addChildWindow(label);
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f+i*0.18f), cegui_reldim( 0.5f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		outStream.str("");
		outStream << "B" << i;
		label->setText(outStream.str());
		label->setID(Equipement::BIG_ITEMS+i);
		label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
		label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));
	}

	// Label fuer mittlere Items
	for (j=0;j<2;j++)
	{
		for (i=0;i<7;i++)
		{
			outStream.str("");
			outStream << "MediumItem" << j*7+i<< "Label";
			label = win_mgr.createWindow("TaharezLook/StaticText", outStream.str());
			inventory->addChildWindow(label);
			label->setProperty("FrameEnabled", "true");
			label->setProperty("BackgroundEnabled", "true");
			label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f+i*0.13f), cegui_reldim( 0.61f+0.09*j)));
			label->setSize(CEGUI::UVector2(cegui_reldim(0.115f), cegui_reldim( 0.08f)));
			outStream.str("");
			outStream << "M" << j*7+i;
			label->setText(outStream.str());
			label->setID(Equipement::MEDIUM_ITEMS+j*7+i);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
			label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));
		}
	}
	// Label fuer kleine Items
	for (j=0;j<3;j++)
	{
		for (i=0;i<10;i++)
		{
			outStream.str("");
			outStream << "SmallItem" << j*10+i<< "Label";
			label = win_mgr.createWindow("TaharezLook/StaticText", outStream.str());
			inventory->addChildWindow(label);
			label->setProperty("FrameEnabled", "true");
			label->setProperty("BackgroundEnabled", "true");
			label->setPosition(CEGUI::UVector2(cegui_reldim(0.02f+i*0.096f), cegui_reldim( 0.79f+0.07*j)));
			label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.06f)));
			outStream.str("");
			outStream << "S" << j*10+i;
			label->setText(outStream.str());
			label->setID(Equipement::SMALL_ITEMS+j*10+i);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
			label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));
		}
	}

	// Label Ruestung
	label = win_mgr.createWindow("TaharezLook/StaticText", "ArmorItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.35f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setText("Ruestung");
	label->setID(Equipement::ARMOR);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));



	/*
	try
	{
		numlabel = (NumberedWindow*) win_mgr.createWindow("Taharez/NumberedStaticText", "ArmorItemLabel");
		inventory->addChildWindow(numlabel);
		numlabel->setProperty("FrameEnabled", "true");
		numlabel->setProperty("BackgroundEnabled", "true");
		numlabel->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.35f)));
		numlabel->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		numlabel->setText("Ruestung");
		DEBUG("NumberedWindow2 created");
	}
	catch (CEGUI::UnknownObjectException e)
	{
		ERRORMSG("Error message: %s",e.getMessage().c_str());
	}
	*/

	// Label Waffe
	label = win_mgr.createWindow("TaharezLook/StaticText", "WeaponItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.14f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setText("Waffe");
	label->setID(Equipement::WEAPON);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));

	// Label Helm
	label = win_mgr.createWindow("TaharezLook/StaticText", "HelmetItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.07f)));
	label->setText("Helm");
	label->setID(Equipement::HELMET);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));

	// Label Schild
	label = win_mgr.createWindow("TaharezLook/StaticText", "ShieldItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.14f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setText("Schild");
	label->setID(Equipement::SHIELD);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));

	// Label Handschuhe
	label = win_mgr.createWindow("TaharezLook/StaticText", "GlovesItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.83f), cegui_reldim( 0.35f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.07f)));
	label->setText("Handschuhe");
	label->setID(Equipement::GLOVES);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));

	// Ring links
	label = win_mgr.createWindow("TaharezLook/StaticText", "RingLeftItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.85f), cegui_reldim( 0.26f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.06f)));
	label->setText("Ring1");
	label->setID(Equipement::RING_LEFT);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));

	// Ring rechts
	label = win_mgr.createWindow("TaharezLook/StaticText", "RingRightItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.26f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.06f)));
	label->setText("Ring2");
	label->setID(Equipement::RING_RIGHT);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));

	// Amulett
	label = win_mgr.createWindow("TaharezLook/StaticText", "AmuletItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.85f), cegui_reldim( 0.06f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.06f)));
	label->setText("Amulett");
	label->setID(Equipement::AMULET);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonPressed, this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&MainWindow::onItemMouseButtonReleased, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onItemHover, this));

	// Button Ausruestung umschalten
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SwapEquipButton"));
	inventory->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.06f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.07f)));
	btn->setText("1");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainWindow::onSwapEquipClicked, this));

	// Inventar anfangs ausblenden
	inventory->setVisible(false);

}


void MainWindow::setupSkilltree()
{
	DEBUG4("setup skilltree");

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();


	// Rahmen fuer Skilltree Fenster

	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	CEGUI::DefaultWindow* tab;

	ostringstream outStream;
	std::string name;
	std::string lname;
	unsigned int act;

	// Skilltree Magier
	CEGUI::TabControl* skilltree_mage = (CEGUI::TabControl*) win_mgr.createWindow("TaharezLook/TabControl", "SkilltreeMage");
	m_game_screen->addChildWindow(skilltree_mage);
	skilltree_mage->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	skilltree_mage->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	skilltree_mage->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::consumeEvent, this));
	skilltree_mage->setWantsMultiClickEvents(false);
	skilltree_mage->setAlpha(0.7);

	// Tab Feuermagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeMage_1");
	tab->setText(gettext("main_firemagic"));
	skilltree_mage->addTab(tab);

	name =Action::getActionInfo(Action::MAGIC_ATTACK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Action::MAGIC_ATTACK);
	//label->setText(Action::getName(Action::MAGIC_ATTACK));
	label->setProperty("Image", "set:skills image:magic_attack");
	label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&MainWindow::onSkillMouseClicked, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onAbilityHover, this));


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
	tab->setText(gettext("main_icemagic"));

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
	tab->setText(gettext("main_airmagic"));

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
		//label->setText(lname);
		label->setProperty("Image", "set:skills image:" + name);
		label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&MainWindow::onSkillMouseClicked, this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onAbilityHover, this));

	}


	skilltree_mage->setVisible(false);


	// Skilltree Krieger
	CEGUI::TabControl* skilltree_warrior = (CEGUI::TabControl*) win_mgr.createWindow("TaharezLook/TabControl", "SkilltreeWarrior");
	m_game_screen->addChildWindow(skilltree_warrior);
	skilltree_warrior->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	skilltree_warrior->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	skilltree_warrior->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::consumeEvent, this));
	skilltree_warrior->setWantsMultiClickEvents(false);
	skilltree_warrior->setAlpha(0.7);


	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeWarrior_1");
	tab->setText("Kampftechniken");
	skilltree_warrior->addTab(tab);


	/*
	name =Action::getActionInfo(Action::ATTACK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticText", name.append( "Label") );
	tab->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Action::ATTACK);
	label->setText(Action::getName(Action::ATTACK));
	label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&MainWindow::onSkillMouseClicked, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onAbilityHover, this));
	*/


	name =Action::getActionInfo(Action::ATTACK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.05f)));
 	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Action::ATTACK);
//	label->setText(Action::getName(Action::ATTACK));
	label->setProperty("Image", "set:skills image:attack");
	label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&MainWindow::onSkillMouseClicked, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onAbilityHover, this));


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
	tab->setText("Passive Faehigkeiten");

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
	tab->setText("??????");

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
		//label->setText(lname);
		label->setProperty("Image", "set:skills image:" + name);
		label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&MainWindow::onSkillMouseClicked, this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onAbilityHover, this));

	}

	skilltree_warrior->setVisible(false);


	// Skilltree Schuetze
	CEGUI::TabControl* skilltree_archer = (CEGUI::TabControl*) win_mgr.createWindow("TaharezLook/TabControl", "SkilltreeArcher");
	m_game_screen->addChildWindow(skilltree_archer);
	skilltree_archer->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	skilltree_archer->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	skilltree_archer->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::consumeEvent, this));
	skilltree_archer->setWantsMultiClickEvents(false);
	skilltree_archer->setAlpha(0.7);


	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreeArcher_1");
	tab->setText("Kampffertigkeiten");
	skilltree_archer->addTab(tab);

	name =Action::getActionInfo(Action::RANGE_ATTACK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Action::RANGE_ATTACK);
	//label->setText(Action::getName(Action::RANGE_ATTACK));
	label->setProperty("Image", "set:skills image:range_attack");
	label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&MainWindow::onSkillMouseClicked, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onAbilityHover, this));

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
	tab->setText("passive Faehigkeiten");

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
	tab->setText("hmm was hier");

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
		//label->setText(lname);
		label->setProperty("Image", "set:skills image:" + name);
		label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&MainWindow::onSkillMouseClicked, this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onAbilityHover, this));

	}

	skilltree_archer->setVisible(false);


	// Skilltree Priester
	CEGUI::TabControl* skilltree_priest = (CEGUI::TabControl*) win_mgr.createWindow("TaharezLook/TabControl", "SkilltreePriest");
	m_game_screen->addChildWindow(skilltree_priest);
	skilltree_priest->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	skilltree_priest->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	skilltree_priest->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainWindow::consumeEvent, this));
	skilltree_priest->setWantsMultiClickEvents(false);
	skilltree_priest->setAlpha(0.7);


	// Tab Lichtmagie
	tab = (CEGUI::DefaultWindow*) win_mgr.createWindow("TaharezLook/TabContentPane", "SkilltreePriest_1");
	tab->setText(gettext("main_lightmagic"));
	skilltree_priest->addTab(tab);

	name =Action::getActionInfo(Action::HOLY_ATTACK)->m_enum_name;
	label = win_mgr.createWindow("TaharezLook/StaticImage", name.append( "Label") );
	tab->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim(0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Action::HOLY_ATTACK);
	//label->setText(Action::getName(Action::HOLY_ATTACK));
	label->setProperty("Image", "set:skills image:holy_attack");
	label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&MainWindow::onSkillMouseClicked, this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onAbilityHover, this));

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
	tab->setText(gettext("main_watermagic"));

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
	tab->setText(gettext("main_windmagic"));

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
		//label->setText(lname);
		label->setProperty("Image", "set:skills image:" + name);
		label->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&MainWindow::onSkillMouseClicked, this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&MainWindow::onAbilityHover, this));

	}

	skilltree_priest->setVisible(false);
}


bool MainWindow::setupObjectInfo()
{
	DEBUG4("setup object info");

	// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;

	// Leiste fuer Informationen
	label = win_mgr.createWindow("TaharezLook/StaticText", "ObjectInfoLabel");
	m_game_screen->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.02f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.6f), cegui_reldim( 0.07f)));
	label->setText("test");
	label->setAlpha(0.5);
	label->setProperty("MousePassThroughEnabled","true");

	return true;
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

	CEGUI::MultiColumnList* savelist = (CEGUI::MultiColumnList*) win_mgr.getWindow("SavegameList");
	savelist->resetList();

	// Liste aller Files im Save Ordner der Form *.sav
	Ogre::StringVectorPtr files = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Savegame","*.sav");

	ifstream file;
	char head[50];
	int len;
	int n=0;
	string name;
	char lev;
	std::string cl;
	string classname;
	std::ostringstream stream;
	char* bp;

	// iterieren ueber die Files
	for (Ogre::StringVector::iterator it = files->begin(); it!= files->end();++it)
	{
		DEBUG("file found %s",it->c_str());
		//File oeffnen

		file.open(("../../save/"+(*it)).c_str(),ios::in| ios::binary);
		if (file.is_open())
		{
			savelist->addRow();

			// Daten einlesen
			for (int i=0;i<50;i++)
			{
				file.get(head[i]);
			}

			// nicht benoetigte Headerdaten ueberspringen
			bp = head+7;
			
			char ctmp[11];
			ctmp[10] = '\0';
			strncpy(ctmp,bp,10);
			cl = ctmp;

			// Level einlesen
			bp +=10;
			DEBUG5("bp pointer: %i",bp-head);
            lev = *(bp+32);
			DEBUG5("level: %i",lev);
			//hexwrite(bp,33);
			stream.str("");
			stream << (int) lev;
			savelist->setItem(new ListItem(stream.str()),2,n);

			// Klasse einlesen
			if (cl=="warrior")
				classname = "Krieger";
			if (cl=="mage")
				classname = "Magier";
			if (cl=="archer")
				classname = "Bogenschuetze";
			if (cl=="priest")
				classname = "Priester";
			savelist->setItem(new ListItem(classname),1,n);

			name = bp;
			savelist->setItem(new SaveListItem(name,(*it)),0,n);
			n++;

			file.close();
		}
	}

}

void MainWindow::updateCharInfo()
{
	// Spielerobjekt
	Player* player = m_document->getLocalPlayer();

	std::string tooltip;

	// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	ostringstream out_stream;

	// Label Name
	label =  win_mgr.getWindow("NameLabel");
	out_stream.str("");
	out_stream.str(player->getName());
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Klasse
	label =  win_mgr.getWindow("ClassLabel");
	out_stream.str("");
	if (player->getTypeInfo()->m_subtype == "warrior")
	{
		out_stream.str(gettext("main_warrior"));
	}
	else if (player->getTypeInfo()->m_subtype == "mage")
	{
		out_stream.str(gettext("main_magician"));
	}
	else if (player->getTypeInfo()->m_subtype == "archer")
	{
		out_stream.str(gettext("main_archer"));
	}
	else if (player->getTypeInfo()->m_subtype == "priest")
	{
		out_stream.str(gettext("main_priest"));
	}
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Level
	label =  win_mgr.getWindow("LevelLabel");
	out_stream.str("");
	out_stream << "Level "<<(int) player->getBaseAttr()->m_level;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Staerke
	label =  win_mgr.getWindow("StrengthValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_strength;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Zauberkraft
	label =  win_mgr.getWindow("MagicpowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_magic_power;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Willenskraft
	label =  win_mgr.getWindow("WillpowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_willpower;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Geschick
	label =  win_mgr.getWindow("DexterityValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_dexterity;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
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
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Ruestung
	label =  win_mgr.getWindow("ArmorValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_armor;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Attacke
	label =  win_mgr.getWindow("AttackValueLabel");
	out_stream.str("");
	out_stream << player->getBaseDamage().m_attack;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Block
	label =  win_mgr.getWindow("BlockValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_block;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label HP
	label =  win_mgr.getWindow("HitpointsValueLabel");
	out_stream.str("");
	out_stream << (int) player->getDynAttr()->m_health <<"/" <<(int) player->getBaseAttrMod()->m_max_health;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Exp
	label =  win_mgr.getWindow("ExperienceValueLabel");
	out_stream.str("");
	out_stream << (int) player->getDynAttr()->m_experience <<"/" <<(int) player->getBaseAttr()->m_max_experience;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Phys Resistenz
	label =  win_mgr.getWindow("ResistPhysValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::PHYSICAL];
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Feuer Resistenz
	label =  win_mgr.getWindow("ResistFireValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::FIRE];
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Eis Resistenz
	label =  win_mgr.getWindow("ResistIceValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::ICE];
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Luft Resistenz
	label =  win_mgr.getWindow("ResistAirValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::AIR];
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Angriffsgeschwindigkeit
	label =  win_mgr.getWindow("AttackSpeedValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_attack_speed;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Reichweite
	label =  win_mgr.getWindow("RangeValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_attack_range;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Durchschlagskraft
	label =  win_mgr.getWindow("PowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseDamage().m_power;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Schaden Basisattacke
	float minb=player->getBaseDamage().getSumMinDamage();
	float maxb=player->getBaseDamage().getSumMaxDamage();
	// Schaden Attacke links
	float minl=player->getLeftDamage().getSumMinDamage();
	float maxl=player->getLeftDamage().getSumMaxDamage();
	// Schaden Attacke rechts
	float minr=player->getRightDamage().getSumMinDamage();
	float maxr=player->getRightDamage().getSumMaxDamage();



	// Label Schaden Basisattacke
	label =  win_mgr.getWindow( "BaseDmgLabel");
	out_stream.str("");
	if (player->getTypeInfo()->m_subtype == "mage")
		out_stream << Action::getName(Action::MAGIC_ATTACK);
	if (player->getTypeInfo()->m_subtype == "archer")
		out_stream << Action::getName(Action::RANGE_ATTACK);
	if (player->getTypeInfo()->m_subtype == "warrior")
		out_stream << Action::getName(Action::ATTACK);
	if (player->getTypeInfo()->m_subtype == "priest")
		out_stream << Action::getName(Action::HOLY_ATTACK);

	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Basisschaden
	label =  win_mgr.getWindow("BaseDmgValueLabel");
	out_stream.str("");
	out_stream << (int) minb << "-" << (int) maxb;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}
	tooltip = player->getBaseDamage().getDamageString();
	if (tooltip != label->getTooltipText())
	{
		label->setTooltipText(tooltip);
	}

	// Label Attacke links
	label =  win_mgr.getWindow( "Skill1DmgLabel");
	out_stream.str("");
	out_stream << Action::getName(player->getLeftAction());
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());

	}

	// Label Schaden Attacke links
	label =  win_mgr.getWindow( "Skill1DmgValueLabel");
	out_stream.str("");
	out_stream << (int) minl << "-" << (int) maxl;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}
	tooltip = player->getLeftDamage().getDamageString();
	if (tooltip != label->getTooltipText())
	{
		label->setTooltipText(tooltip);
	}



	// Label Attacke rechts
	label =  win_mgr.getWindow( "Skill2DmgLabel");
	out_stream.str("");
	out_stream << Action::getName(player->getRightAction());
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());

	}

	// Label Schaden Attacke rechts
	label =  win_mgr.getWindow( "Skill2DmgValueLabel");
	out_stream.str("");
	out_stream << (int) minr << "-" << (int) maxr;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}
	tooltip = player->getRightDamage().getDamageString();
	if (tooltip != label->getTooltipText())
	{
		label->setTooltipText(tooltip);
	}
}

void MainWindow::updateControlPanel()
{
	// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	ostringstream out_stream;
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
		label =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::SMALL_ITEMS+i);
		out_stream.str("");
		if (it!=0)
		{
			out_stream<< (it->getName());
		}
		if (label->getText()!=out_stream.str())
		{
			label->setText(out_stream.str());
		}
	}

}

void MainWindow::updateInventory()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	ostringstream out_stream;
	
	Player* player = m_document->getLocalPlayer();

	// Label Ruestung
	out_stream.str("");
	Equipement* equ = player->getEquipement();
	if (equ->getItem(Equipement::ARMOR)!=0)
	{
		out_stream << equ->getItem(Equipement::ARMOR)->getName();
	}
	label =  win_mgr.getWindow("ArmorItemLabel");
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Waffe
	out_stream.str("");
	if (player->getWeapon()!=0)
	{
		out_stream << player->getWeapon()->getName();
	}
	label =  win_mgr.getWindow("WeaponItemLabel");
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Helm
	out_stream.str("");
	if (equ->getItem(Equipement::HELMET)!=0)
	{
		out_stream << equ->getItem(Equipement::HELMET)->getName();
	}
	label =  win_mgr.getWindow("HelmetItemLabel");
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Schild
	out_stream.str("");
	if (player->getShield()!=0)
	{
		out_stream << player->getShield()->getName();
	}
	label =  win_mgr.getWindow("ShieldItemLabel");
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Handschuhe
	out_stream.str("");
	if (equ->getItem(Equipement::GLOVES)!=0)
	{
		out_stream << equ->getItem(Equipement::GLOVES)->getName();
	}
	label =  win_mgr.getWindow("GlovesItemLabel");
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Ring links
	out_stream.str("");
	if (equ->getItem(Equipement::RING_LEFT)!=0)
	{
		out_stream << equ->getItem(Equipement::RING_LEFT)->getName();
	}
	label =  win_mgr.getWindow("RingLeftItemLabel");
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Ring rechts
	out_stream.str("");
	if (equ->getItem(Equipement::RING_RIGHT)!=0)
	{
		out_stream << equ->getItem(Equipement::RING_RIGHT)->getName();
	}
	label =  win_mgr.getWindow("RingRightItemLabel");
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}
	
	// Label Amulet
	out_stream.str("");
	if (equ->getItem(Equipement::AMULET)!=0)
	{
		out_stream << equ->getItem(Equipement::AMULET)->getName();
	}
	label =  win_mgr.getWindow("AmuletItemLabel");
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	int i;
	Item* it;
	// Inventar: grosse Items
	for (i=0;i<5;i++)
	{
		out_stream.str("");
		out_stream << "BigItem" << i<< "Label";
		label =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::BIG_ITEMS+i);
		out_stream.str("");
		if (it!=0)
		{
			out_stream<< (it->getName());
		}
		if (label->getText()!=out_stream.str())
		{
			label->setText(out_stream.str());
		}
	}

	// Inventar: mittlere Items
	for (i=0;i<14;i++)
	{
		out_stream.str("");
		out_stream << "MediumItem" << i<< "Label";
		label =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::MEDIUM_ITEMS+i);
		out_stream.str("");
		if (it!=0)
		{
			out_stream<< (it->getName());
		}
		if (label->getText()!=out_stream.str())
		{
			label->setText(out_stream.str());
		}
	}

	// Inventar: kleine Items
	for (i=0;i<30;i++)
	{
		out_stream.str("");
		out_stream << "SmallItem" << i<< "Label";
		label =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::SMALL_ITEMS+i);
		out_stream.str("");
		if (it!=0)
		{
			out_stream<< (it->getName());
		}
		if (label->getText()!=out_stream.str())
		{
			label->setText(out_stream.str());
		}
	}


}

void MainWindow::updateSkilltree()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	ostringstream out_stream;

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

void MainWindow::updateItemTooltip(unsigned int pos)
{
	ServerItem* item = static_cast<ServerItem*>(m_document->getLocalPlayer()->getEquipement()->getItem(pos));
	if (item ==0)
		return;
	
	DEBUG5("setting tool tip for item at %i",pos);
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	ostringstream out_stream;
	out_stream.str("");
	if (pos == Equipement::ARMOR)
		out_stream << "ArmorItemLabel";
	if (pos == Equipement::WEAPON || pos == Equipement::WEAPON2)
		out_stream << "WeaponItemLabel";
	if (pos == Equipement::HELMET)
		out_stream << "HelmetItemLabel";
	if (pos == Equipement::SHIELD || pos == Equipement::SHIELD2)
		out_stream << "ShieldItemLabel";
	if (pos == Equipement::GLOVES)
		out_stream << "GlovesItemLabel";
	if (pos == Equipement::RING_LEFT)
		out_stream << "RingLeftLabel";
	if (pos == Equipement::RING_RIGHT)
		out_stream << "RingRightItemLabel";
	if (pos == Equipement::AMULET)
		out_stream << "AmuletItemLabel";
	if (pos>= Equipement::BIG_ITEMS && pos <  Equipement::MEDIUM_ITEMS)
	{
		out_stream << "BigItem"<<pos-Equipement::BIG_ITEMS<<"Label";
	}
	if (pos>= Equipement::MEDIUM_ITEMS && pos < Equipement::SMALL_ITEMS)
	{
		out_stream << "MediumItem"<<pos-Equipement::MEDIUM_ITEMS<<"Label";
	}
	if (pos>= Equipement::SMALL_ITEMS)
	{
		out_stream << "SmallItem"<<pos-Equipement::SMALL_ITEMS<<"Label";
	}
	label = win_mgr.getWindow(out_stream.str());

	

	std::string msg =item->getDescription();
	label->setTooltipText(msg);
	
	DEBUG5("Label: %s \ndescription: \n%s",out_stream.str().c_str(),msg.c_str());

	if (pos>= Equipement::SMALL_ITEMS && pos< Equipement::SMALL_ITEMS+10)
	{
		// Item befindet sich im Guertel
		out_stream.str("");
		out_stream << "InventoryItem"<<pos-Equipement::SMALL_ITEMS;
		label = win_mgr.getWindow(out_stream.str());
		label->setTooltipText(msg);
	}
	
}

void MainWindow::updateAbilityTooltip(unsigned int pos)
{
	
	if (pos!=Action::NOACTION)
	{
		DEBUG("update tooltip for %i", pos);

		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Window* label;
		ostringstream out_stream;
		out_stream.str("");
		out_stream << Action::getActionInfo((Action::ActionType) pos)->m_enum_name << "Label";

		label = win_mgr.getWindow(out_stream.str());

		std::string tooltip = m_document->getAbilityDescription((Action::ActionType) pos);

		label->setTooltipText(tooltip);

	}
	
}

void MainWindow::updateObjectInfo()
{
	Player* player = m_document->getLocalPlayer();
	
	// Ogre Name des Objektes auf das der Mauszeiger zeigt
	std::string objname = "";

	// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label = win_mgr.getWindow("ObjectInfoLabel");

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


	// Ergebnispaar einer Intersection Abfrage
	std::pair<bool,Ogre::Real> isec;

	if (m_mouse->getMouseState().buttons !=0)
	{
		// fokussiertes Objekt nicht wechseln, wenn Maustaste gedrueckt
		objname = m_document->getGUIState()->m_cursor_object;
	}
	else
	{

		ostringstream out_stream;
		out_stream.str("");


		// Objekte in der Szene
		map<int,string>* objects = m_scene->getObjects();
		map<int,string>::iterator it;

		// minimale Distanz eines Objektes im Kamerastrahl
		float mindist = 1000000;

		for (it = objects->begin();it != objects->end();++it)
		{
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

				// Objekt wird vom Kamerastrahl geschnitten
				if (isec.second<mindist)
				{
					// Objekt ist das bisher naechste
					objname = it->second;
					mindist = isec.second;
				}
			}

		}
		m_document->getGUIState()->m_cursor_object=objname;

		if (objname=="")
		{
			// aktuell kein Objekt unterm Mauszeiger
			m_document->getGUIState()->m_cursor_object_id =0;

			label->setVisible(false);
		}
	}

	std::string name;
	std::ostringstream string_stream;
	short rid = player->getGridLocation()->m_region;

	if (objname!="")
	{
		// es gibt ein ein Objekt unterm Mauszeiger
		m_document->getGUIState()->m_cursor_item_id =0;
		label->setVisible(true);

		// ID des Objektes ermitteln
		int pos = objname.find(":");
		std::string idstring = objname.substr(pos+1);
		name = objname.substr(0,pos);
		std::stringstream stream(idstring);
		int id;
		stream >> id;

		string_stream<<name;

		// zur ID gehoerendes Objekt
		ServerWObject* cwo;
		cwo = m_document->getWorld()->getSWObject(id,rid);
		Creature* cr;
		
		if (cwo !=0)
		{
		// Objekt existiert
			m_document->getGUIState()->m_cursor_object_id = id;

			if (cwo->getTypeInfo()->m_type != WorldObject::TypeInfo::TYPE_FIXED_OBJECT)
			{
				// Objekt ist ein Lebewesen
				// Lebenspunkte anfuegen
				cr = static_cast<Creature*>(cwo);
				string_stream << " " << max(0,(int) (cr->getDynAttr()->m_health / cr->getBaseAttrMod()->m_max_health*100)) <<"%";
			}
		}
		else
		{
			m_document->getGUIState()->m_cursor_object_id =0;
			m_document->getGUIState()->m_cursor_object="";
		}
	}
	else
	{
		// Nach Items unterm Mauszeiger suchen
		map<int,string>* itms = m_scene->getDropItems();
		map<int,string>::iterator it;

		int id=0;

		for (it = itms->begin();it != itms->end();++it)
		{
			ent = m_scene_manager->getEntity(it->second);

			// Auf Ueberschneidung mit dem Kamerastrahl testen
			const Ogre::AxisAlignedBox& box = ent->getWorldBoundingBox();
			isec = ray.intersects(box);

			if (isec.first)
			{

				// Objekt wird vom Kamerastrahl geschnitten
				objname = it->second;
				break;

			}

		}

		if (objname!="")
		{
			// Item gefunden
			// ID des Objektes ermitteln
			int pos = objname.find(":");
			std::string idstring = objname.substr(pos+1);
			name = objname.substr(0,pos);
			std::stringstream stream(idstring);
			int id;
			stream >> id;
			string_stream<<name;

			Item* itm = player->getRegion()->getItem(id);

			if (itm !=0)
			{
				// Objekt existiert
				m_document->getGUIState()->m_cursor_item_id = id;
				label->setVisible(true);
			}
			else
			{
				m_document->getGUIState()->m_cursor_object_id =0;
				m_document->getGUIState()->m_cursor_object="";
			}
		}


	}



	name = string_stream.str();

	if (label->getText() != name)
	{

		label->setText(name);
	}


}

// MouseListener
bool MainWindow::mouseMoved(const OIS::MouseEvent &evt) {
	m_cegui_system->injectMouseWheelChange(evt.state.Z.rel);
	//return m_cegui_system->injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
	//DEBUG("injection position %i %i",evt.state.X.abs,evt.state.Y.abs);
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

	// Koordinaten an denen der Mausklick stattfand
	int x =evt.state.X.abs;
	int y =evt.state.Y.abs;
	DEBUG5("maus %i %i",x,y);

	Player* player = m_document->getLocalPlayer();
	if (player!=0)
	{
		// Spiel ist mit Server verbunden

		// Testet, dass man nicht auf die untere Steuerleiste geklickt hat
		// TODO: vernuenftige Vorgehensweise um Klicks auf GUI Elemente zu erkennen
		if (not m_gui_hit && not ret)
		{
			if (player->getEquipement()->getItem(Equipement::CURSOR_ITEM)!=0)
			{
				// Spieler hat Item in der Hand, fallen lassen
				m_document->dropCursorItem();
			}
			else
			{

				// Koordinaten des Spielers
				float mpx=player->getGeometry()->m_shape.m_coordinate_x;
				float mpy=player->getGeometry()->m_shape.m_coordinate_y;

				// Position des Mausklicks relativ zum Viewport
				Ogre::Viewport* viewport = m_scene->getViewport();
				float relx = x*1.0/(viewport->getActualWidth());
				float rely = y*1.0/(viewport->getActualHeight());
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
				if (dir.y <0)
				{
					// Strahl zeigt *nach unten*

					// Durchstosspunkt durch die Ebene y=0
					Ogre::Vector3 p = orig + dir*(orig.y/(-dir.y));
					DEBUG5("schnittpunkt %f %f %f",p.x,p.y,p.z);

					// Umrechnen in Spielkoordinaten
					float gx = p.x/50;
					float gy = p.z/50;
					DEBUG5("Punkt in Spielkoordinaten %f %f",gx,gy);

					// Koordinaten relativ zum Spieler
					//gx -= mpx;
					//gy -= mpy;

					if (btn == OIS::MB_Left)
					{
						m_document->getGUIState()->m_left_mouse_hold=true;
						m_document->onLeftMouseButtonClick(gx, gy);
					}
					else if (btn == OIS::MB_Right)
					{
						m_document->getGUIState()->m_right_mouse_hold=true;
						m_document->onRightMouseButtonClick(gx, gy);
					}


				}
				else
				{
					// Strahl zeigt nach oben, ueber den Horizont
					// sollte eigentlich nicht passieren
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
		m_document->getGUIState()->m_left_mouse_hold=false;
		m_document->getGUIState()->m_clicked_object_id=0;
	}

	if (btn==OIS::MB_Right)
	{
		DEBUG5("Right Button release");
		m_document->getGUIState()->m_right_mouse_hold=false;
		m_document->getGUIState()->m_clicked_object_id=0;
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

	m_cegui_system->injectKeyDown(evt.key);
	DEBUG5("keycode %x",evt.key);
	bool ret = m_cegui_system->injectChar(ch);
	if (not ret)
	{
		ret = m_document->onKeyPress(evt.key);
	}
	return ret;
}

bool MainWindow::keyReleased(const OIS::KeyEvent &evt)
{

	bool ret = m_cegui_system->injectKeyUp(evt.key);

	if (not ret)
	{
		ret = m_document->onKeyRelease(evt.key);
	}

	return ret;
}


bool MainWindow::onButtonSaveExitClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonSaveExitClicked();
	return true;
}

bool MainWindow::onButtonInventoryClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonInventoryClicked();
	return true;
}

bool MainWindow::onButtonCharInfoClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonCharInfoClicked();
	return true;
}

bool MainWindow::onButtonSkilltreeClicked(const CEGUI::EventArgs& evt)
{
	m_document->onButtonSkilltreeClicked();
	return true;
}

bool MainWindow::onItemMouseButtonPressed(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();

	if (we.button == CEGUI::LeftButton)
	{
		m_document->onItemLeftClick((short) id);
	}

	if (we.button == CEGUI::RightButton)
	{
		m_document->onItemRightClick((short) id);
	}


}

bool MainWindow::onItemMouseButtonReleased(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	if (we.button == CEGUI::LeftButton)
	{
		DEBUG("left button released on Item %i",id);
	}

}

bool MainWindow::onSkillMouseClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();

	if (we.button == CEGUI::LeftButton)
	{
		DEBUG("left button pressed on skill %i",id);
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
		DEBUG("right button pressed on skill %i",id);
		m_document->setRightAction((Action::ActionType) id);
	}

}

bool MainWindow::onSwapEquipClicked(const CEGUI::EventArgs& evt)
{
	m_document->onSwapEquip();
}

bool MainWindow::onItemHover(const CEGUI::EventArgs& evt)
{
	
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	updateItemTooltip(id);
	
}

bool MainWindow::onAbilityHover(const CEGUI::EventArgs& evt)
{
	
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	DEBUG5("mouse entered Ability %i",id);
	updateAbilityTooltip(id);
	
}

bool MainWindow::onIncreaseAttributeButtonClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	DEBUG("left button pressed on skill %i",id);
	m_document->increaseAttribute((CreatureBaseAttr::Attribute) id);

}

bool MainWindow::onButtonPartyClicked(const CEGUI::EventArgs& evt)
{
	// Debugging
	m_document->emitDebugSignal(0);
}

bool MainWindow::onButtonOpenChatClicked(const CEGUI::EventArgs& evt)
{
	// Debugging
	m_document->emitDebugSignal(1);
}

bool MainWindow::onSavegameChosen(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::MultiColumnList* savelist = (CEGUI::MultiColumnList*) win_mgr.getWindow("SavegameList");

	CEGUI::ListboxItem * itm = savelist->getFirstSelectedItem();


	if (itm !=0)
	{
		SaveListItem * sitm = (SaveListItem *) itm;
		m_document->setSaveFile(sitm->m_filename);
		DEBUG("selected Savegame %s", sitm->m_filename.c_str());
	}

}

bool MainWindow::onSavegameSelected(const CEGUI::EventArgs& evt)
{
	if (m_document->getSaveFile() != "")
	{
		DEBUG("savegame accepted %s", m_document->getSaveFile().c_str());
		m_document->getGUIState()->m_shown_windows = Document::START_MENU;
		m_document->setModified(Document::WINDOWS_MODIFIED);
	}
}

bool MainWindow::onStartSinglePlayer(const CEGUI::EventArgs& evt)
{
	DEBUG("start single player game");
	// Spieler ist selbst der Host
	m_document->setServer(true);
	
	// Verbindung aufbauen
	m_document->setState(Document::START_GAME);
}

bool MainWindow::onStartMultiPlayer(const CEGUI::EventArgs& evt)
{
	DEBUG("start multi player game");

	m_document->setServer(false);


	// Verbindung aufbauen
	m_document->setState(Document::START_GAME);
}

bool MainWindow::onStartMultiPlayerHost(const CEGUI::EventArgs& evt)
{
	DEBUG("start single player game");
	// Spieler ist selbst der Host
	m_document->setServer(true);

	
	// Verbindung aufbauen
	m_document->setState(Document::START_GAME);
}












