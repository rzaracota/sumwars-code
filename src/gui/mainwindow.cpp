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
		
		// m_main_menu->setProperty("BackgroundEnabled", "true");
		//m_main_menu->setProperty("Image", "set:startscreen.png image:full_image");

		Window* wnd = new SavegameList(m_document);
		m_sub_windows["SavegameList"] = wnd;
		m_main_menu->addChildWindow(wnd->getCEGUIWindow());
		
		wnd = new MainMenu(m_document);
		m_sub_windows["MainMenu"] = wnd;
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
		
		// Chatfenster anlegen
		setupChatWindow();
		
		// Fenster fuer Partyinfos
		setupPartyInfo();
		
		// Bild fuer das Item am Cursor
		setupCursorItemImage();
		
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

bool MainWindow::setupPartyInfo()
{
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




void  MainWindow::setWindowExtents(int width, int height){
	//Set Mouse Region.. if window resizes, we should alter this to reflect as well
	const OIS::MouseState &ms = m_mouse->getMouseState();
	ms.width = width;
	ms.height = height;
}


void  MainWindow::updateMainMenu()
{
	m_sub_windows["SavegameList"]->update();

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
	
	// Ort an dem der Strahl den Boden beruehrt berechnen
	const Ogre::Vector3& orig = ray.getOrigin();
	const Ogre::Vector3& dir = ray.getDirection();
	
	// Schnittpunkt mit der Ebene y=0 ausrechnen
	Ogre::Vector3 p = orig + dir*(orig.y/(-dir.y));
	
	// Umrechnen in Spielkoordinaten
	float gx = p.x/50;
	float gy = p.z/50;


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
				wo = World::getWorld()->getRegion(rid)->getObject(it->first);
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
		}
	}

	std::string name;
	std::ostringstream string_stream;
	

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
		WorldObject* cwo;
		cwo = World::getWorld()->getRegion(rid)->getObject(id);
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
				string_stream << " " << std::max(0,(int) (cr->getDynAttr()->m_health / cr->getBaseAttrMod()->m_max_health*100)) <<"%";
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
		std::map<int,string>* itms = m_scene->getDropItems();
		std::map<int,string>::iterator it;
		
		float mindist = 1000000;
		float dist;
		float ix,iy;
		DropItem* di;
		
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
				}

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

			DEBUG5("item hover id %i",id);

			Item* itm = player->getRegion()->getItem(id);

			if (itm !=0)
			{
				// Objekt existiert
				m_document->getGUIState()->m_cursor_item_id = id;
				label->setVisible(true);
			}
			else
			{
				m_document->getGUIState()->m_cursor_item_id =0;
			}
		}
		else
		{
			m_document->getGUIState()->m_cursor_item_id =0;
		}


	}



	name = string_stream.str();

	if (label->getText() != name)
	{

		label->setText(name);
	}


}

void MainWindow::updatePartyInfo()
{
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::Window* img;
	CEGUI::ProgressBar* bar;
	
	std::ostringstream stream;
	
	int i=0;
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

// MouseListener
bool MainWindow::mouseMoved(const OIS::MouseEvent &evt) {
	m_cegui_system->injectMouseWheelChange(evt.state.Z.rel);
	//return m_cegui_system->injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
	//DEBUG("injection position %i %i",evt.state.X.abs,evt.state.Y.abs);
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












