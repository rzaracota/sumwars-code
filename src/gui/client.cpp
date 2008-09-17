#include "client.h"

Client::Client()
{
	// Anwendung initialisieren
	bool ret = init();

	if (ret == false)
	{
		// ERRORMSG("Initialisation failed, stop");
		m_shutdown = true;
	}

}

bool Client::init()
{
	bool ret = true;

	// Ogre Root erzeugen
	// pluginfile: plugins.cfg
	// configfile: keines
	#ifdef WIN32
        m_ogre_root = new Ogre::Root("plugins_win.cfg", "ogre.cfg");
    #else
        m_ogre_root = new Ogre::Root("plugins.cfg", "ogre.cfg");
    #endif

	if (m_ogre_root ==0)
			return false;

	// Ogre configurieren
	ret = configureOgre();
	if (ret==false)
	{
		 ERRORMSG("Configuring Ogre failed");
		return false;
	}

	// Ogre initialisieren
	ret = initOgre();
	if (ret==false)
	{
		 ERRORMSG("Initialisation of Ogre failed");
		return false;
	}

	// Ressourcen festlegen
	ret = setupResources();
	if (ret==false)
	{
		 ERRORMSG("Setting up Resources failed");
		return false;
	}

	//Gettext initialisieren
	ret = initGettext();
	if (ret==false)
	{
		ERRORMSG("Initialisation of Gettext failed");
		return false;
	}


	//CEGUI initialisieren
	ret = initCEGUI();
	if (ret==false)
	{
		 ERRORMSG("Initialisation of CEGUI failed");
		return false;
	}

	// Document anlegen
	ret = createDocument();
	if (ret==false)
	{
		 ERRORMSG("cant create document");
		return false;
	}
	// View anlegen
	ret = createView();
	if (ret==false)
	{
		 ERRORMSG("cant create view");
		return false;
	}

	DEBUG("application initialized");
	// debugging
	//MyFrameListener* mfl = new MyFrameListener(m_main_window,m_document);
	//m_ogre_root->addFrameListener(mfl);

	return true;
}


Client::~Client()
{
	// dynamisch angelegte Objekte in umgekehrter Reihenfolge freigeben

	printf("deleting application\n");
	delete m_main_window;
	delete m_document;
	delete m_ogre_cegui_renderer;
	delete m_ogre_root;
}

void Client::run()
{
	printf("started\n");
	Ogre::Timer timer;
	unsigned long ltime;
	float time=0;
	float uptime =0;
	//m_ogre_root->startRendering();
	float timesum=0;
	int count=0;
    Ogre::Timer timer2;

	while (m_document->getState() != Document::SHUTDOWN)
	{

		ltime =timer.getMicroseconds ();
		time = ltime / ( 1000000.0);
		timer.reset();

        if (time>0.025)
       {
            DEBUG5("frame time was %f",time*1000);
       }

		update();

        ltime =timer.getMicroseconds ();
		uptime = ltime / ( 1000000.0);
        if (uptime>0.025)
        {
            DEBUG("update time was %f",time*1000);
        }


		// run the message pump
		Ogre::WindowEventUtilities::messagePump();

		// Document aktualisieren
		DEBUG5("document update");
		m_document->update(time*1000);
		DEBUG5("done");

		count ++;
		timesum += time;
		if (count ==100)
		{
			count =0;
			timesum *=10;
			DEBUG5("frame time is %f",timesum);
			timesum =0;
		}

		try
		{
			// View aktualisieren
			DEBUG5("main window update");
			m_main_window->update();
		}
		catch (CEGUI::Exception e)
		{
			ERRORMSG("Error message: %s",e.getMessage().c_str());
		}


		//DEBUG("frame time in s %f",time);
		m_cegui_system->injectTimePulse(time);


		// rendern
		DEBUG5("ogre frame");
		timer2.reset();

		m_ogre_root->renderOneFrame();

		ltime =timer.getMicroseconds ();
		time = ltime / ( 1000000.0);

        if (time>0.025)
        {
            DEBUG5("ogre frame time was %f",time*1000);
        }

		DEBUG5("ogre frame end");
	}

	

}

bool Client::initOgre()
{
	DEBUG("init ogre");
	// Fenster anlegen, Ogre initialisieren
	m_window = m_ogre_root->initialise(true);

	// Szenemanager anlegen
	m_scene_manager = m_ogre_root->createSceneManager(Ogre::ST_GENERIC,"DefaultSceneManager");

	return true;

}

bool Client::configureOgre()
{
	DEBUG("configure ogre");
	// Logging nur fuer Fehlermeldungen verwenden
	Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_LOW );
	// Rendering System waehlen
	// Liste aller verfuegbaren Systeme ausgeben lassen und das erste davon nehmen
	Ogre::RenderSystemList *renderSystems = NULL;
	Ogre::RenderSystemList::iterator r_it;
	renderSystems = m_ogre_root->getAvailableRenderers();
	if (renderSystems->empty())
	{
		// ERRORMSG("no rendering system available");
		return false;
	}
	r_it = renderSystems->begin();
	m_ogre_root->setRenderSystem(*r_it);

	// config Dialog anzeigen
	// TODO: Einstellungen direkt setzen oder aus einem File einlesen
	bool result = true;
	//result = m_ogre_root->showConfigDialog();
	m_ogre_root->restoreConfig();
	if (result == false)
	{
		// DEBUG("User pressed Cancel on config Dialog");
		return false;
	}


	return true;
}

bool Client::setupResources()
{
	DEBUG("initalizing resources");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/models", "FileSystem", "General");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/materials/scripts", "FileSystem", "General");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/materials/textures", "FileSystem", "General");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/particle", "FileSystem", "General");

	// CEGUI Resourcen laden
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/gui/configs", "FileSystem", "GUI");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/gui/fonts", "FileSystem", "GUI");

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/gui/imagesets", "FileSystem", "GUI");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/gui/layouts", "FileSystem", "GUI");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/gui/looknfeel", "FileSystem", "GUI");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/gui/schemes", "FileSystem", "GUI");

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/resources/gui/schemes", "FileSystem", "GUI");

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../client/save", "FileSystem", "Savegame");


#if defined(WIN32)
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("c:\\windows\\fonts", "FileSystem", "GUI");
#endif

	// Gruppen initialisieren
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Savegame");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");

	// Debugging: Meshes direkt anlegen

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	Ogre::MeshManager::getSingleton().createPlane("Tile", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,100,100,1,1,true,1,1,1,Ogre::Vector3::UNIT_X);


	Ogre::MeshManager::getSingleton().createPlane("Item", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,25,25,1,1,true,1,1,1,Ogre::Vector3::UNIT_X);



	return true;
}

bool Client::initGettext()
{
	DEBUG("initializing internationalisation");
	#ifndef WIN32
	bool result = false;
	char * language;
	language = getenv("LANG");

	if ((language[0] == 'd' && language[1] == 'e') ||
		(language[0] == 'e' && language[1] == 'n'))
	{
		result = setlocale (LC_MESSAGES, "");
		//result = setlocale (LC_MESSAGES, "de_DE");
	}
	else
	{
		result = setlocale (LC_MESSAGES, "en");

		if (!result)
			result = setlocale (LC_MESSAGES, "en_GB");
		if (!result)
			result = setlocale (LC_MESSAGES, "en_US");
		if (!result)
			result = setlocale (LC_MESSAGES, "en_NZ");
	}

	bindtextdomain ("sumwars","../shared/translation/");
	textdomain ("sumwars");
	return result;
	#endif
	return true;
}

bool Client::initCEGUI()
{
	DEBUG("init CEGUI\n");
	m_ogre_cegui_renderer = new CEGUI::OgreCEGUIRenderer(
	m_window,							// Fenster in das CEGUi rendert
	Ogre::RENDER_QUEUE_OVERLAY,	// Render Queue von CEGUI
	false,								// CEGUI in der Render Queue zuerst bearbeitet
	3000,									// max quads for the UI
 	m_scene_manager						// verwendeter Szenemanager
	);

	// Groesse festlegen
	m_ogre_cegui_renderer->setDisplaySize(CEGUI::Size((int) m_window->getWidth(),(int) m_window->getHeight()));

	// Basisklasse von CEGUI erzeugen
	m_cegui_system = new CEGUI::System(m_ogre_cegui_renderer);


	// Log level
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

	// Scheme laden
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLook.scheme", (CEGUI::utf8*)"GUI");

	// Imagesets laden
	//CEGUI::Imageset* imgset = CEGUI::ImagesetManager::getSingleton().createImageset("test.imageset");
	CEGUI::Imageset* imgset = CEGUI::ImagesetManager::getSingleton().createImageset("skills.imageset");

	// Mauscursor setzen (evtl eher in View auslagern ? )
	m_cegui_system->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");

	// Font setzen
	CEGUI::FontManager::getSingleton().createFont("DejaVuSerif-8.font", (CEGUI::utf8*)"GUI");
	m_cegui_system->setDefaultFont((CEGUI::utf8*)"DejaVuSerif-8");

	// default ToolTip erzeugen
	CEGUI::System::getSingleton().setDefaultTooltip( (CEGUI::utf8*)"TaharezLook/Tooltip" );
	CEGUI::Tooltip* ttip = CEGUI::System::getSingleton().getDefaultTooltip();
	ttip->setDisplayTime(0);

	// eigene Factorys einfuegen
	/*
	CEGUI::WindowFactoryManager& win_fact_man = CEGUI::WindowFactoryManager::getSingleton();
	NumberedWindowFactory* num_win_fac = new NumberedWindowFactory();
	win_fact_man.addFactory(num_win_fac);
	win_fact_man.addFalagardWindowMapping("Taharez/NumberedStaticText","NumberedWindow","TaharezLook/StaticText",win_fact_man.getMappedRendererForType ("TaharezLook/StaticText"));
	*/
	return true;
}

bool Client::createDocument()
{
	DEBUG("create document\n");
	m_document = new Document();


	return true;
}

bool Client::createView()
{
	DEBUG("create view\n");
	m_main_window = new MainWindow(m_ogre_root, m_cegui_system,m_window,m_document);

	DEBUG("done");
	return true;
}

void  Client::update()
{

}


