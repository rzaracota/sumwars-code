
#ifndef APPLICATION_H
#define APPLICATION_H



#include "Ogre.h"
#include <OgreTimer.h>

// needed to be able to create the CEGUI renderer interface
#include "OgreCEGUIRenderer.h"


// CEGUI includes
#include "CEGUISystem.h"
#include "CEGUIInputEvent.h"
#include "CEGUIWindow.h"
#include "CEGUIWindowManager.h"
#include "CEGUISchemeManager.h"
#include "CEGUIFontManager.h"
#include "elements/CEGUIFrameWindow.h"

#include <stdio.h>

#include "mainwindow.h"



/**
 * \class Application
 * \brief Basisklasse der Anwendung
 */

class Application
{

	public:
		/**
		 * \fn Application()
		 * \brief Konstruktor
		 */
		Application();

		/**
		 * \fn ~Application()
		 * \brief Destruktor
		 */
		virtual ~Application();

		/**
		 * \fn run()
		 * \brief Startet die Applikation
		 */
		void run();

		// nur zum debuggen
		Document* getDocument()
		{
			return m_document;
		}

	private:
		// Funktionen
		/**
		* \fn init()
		* \brief Initialisiert die Anwendung
		 */
		bool init();

		/**
		 * \fn initOgre()
		 * \brief Initialisiert die Ogre Rendering Engine
		 */
		bool initOgre();

		/**
		 * \fn configureOgre()
		 * \brief Konfiguriert die Ogre Rendering Engine
		 */
		bool configureOgre();

		/**
		 * \fn setupResources()
		 * \brief Stellt die Ressourcen fuer Ogre und CEGUI ein
		 */
		bool setupResources();

		/**
		 * \fn initGettext()
		 * \brief Initialisiert Gettext
		 */
		bool initGettext();

		/**
		 * \fn initCEGUI()
		 * \brief Initialisiert CEGUI
		 */
		bool initCEGUI();

		/**
		 * \fn bool createView()
		 * \brief Erzeugt die Ansichten
		 */
		bool createView();

		/**
		 * \fn bool createDocument()
		 * \brief Erzeugt das Document
		 */
		bool createDocument();

		/**
		 * \fn virtual void update()
		 * \brief Aktualisierungen
		 */
		virtual void update();

		// Member
		/**
		 * \var Ogre::Root *m_ogre_root
		 * \brief Basisobjekt der Ogre Engine
		 */
		Ogre::Root *m_ogre_root;

		/**
		 * \var CEGUI::System* m_cegui_system
		 * \brief Basisobjekt von CEGUI
		 */
		CEGUI::System* m_cegui_system;

		/**
		 * \var CEGUI::OgreCEGUIRenderer* m_ogre_cegui_renderer
		 * \brief Objekt zum Rendern von CEGUI mittels Ogre
		 */
		CEGUI::OgreCEGUIRenderer* m_ogre_cegui_renderer;

		/**
		 * \var Ogre::RenderWindow *window
		 * \brief Fenster der Applikation
		 */
		Ogre::RenderWindow *m_window;

		/**
		 * \var Ogre::SceneManager *m_scene_manager
		 * \brief Objekt fuer das Management der Szene
		 */
 		Ogre::SceneManager *m_scene_manager;

		// Kamera
		// temporaer, wird spaeter View zugeordnet
		Ogre::Camera *m_camera;

		/**
		 * \var bool m_shutdown
		 * \brief true, wenn die Anwendung beendet werden soll
		 */
		bool m_shutdown;

		// zum testen

		//CEGUI::Window* pLayout;		// pseudo view

	protected:
		/**
		 * \var Document* m_document
		 * \brief Dokument, enthaelt alle Daten fuer die Darstellung
		 */
		Document* m_document;

		/**
		 * \var MainWindow* m_main_window
		 * \brief Hauptfenster der Applikation
		 */
		MainWindow* m_main_window;
};




















#endif

