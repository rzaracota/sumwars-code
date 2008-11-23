
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include "Ogre.h"

// needed to be able to create the CEGUI renderer interface
#include "OgreCEGUIRenderer.h"

// CEGUI includes
#include "CEGUI.h"


// OIS
#include "OISEvents.h"
#include "OISInputManager.h"
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "OISJoyStick.h"

#include "document.h"
#include "scene.h"
#include "gettext.h"
#include "player.h"

#include "charinfo.h"
#include "controlpanel.h"
#include "inventory.h"
#include "skilltree.h"
#include "mainmenu.h"
#include "savegamelist.h"
#include "chatline.h"
#include "partyinfo.h"

/**
 * \class MainWindow
 * \brief Implementiert das Hauptfenster der Applikation
 */
class MainWindow :
	public OIS::MouseListener,
	public OIS::KeyListener
{

	public:


	/**
	 * \fn MainWindow(Ogre::Root* ogreroot, CEGUI::System* ceguisystem,Ogre::RenderWindow* window,Document* doc)
	 * \param ogreroot Zeiger auf das Basisobjekt von Ogre
	 * \param ceguisystem Zeiger auf das Basisobjekt von CEGUI
	 * \param window Fenster in die Applikation gestartet wird
	 * \param doc Zeiger auf das Dokument
	 */
	MainWindow(Ogre::Root* ogreroot, CEGUI::System* ceguisystem,Ogre::RenderWindow* window,Document* doc);

	/**
	 * \fn ~MainWindow()
	 * \brief Destruktor
	 */
	~MainWindow();

	/**
	 * \fn init()
	 * \brief Initialisiert das Objekt
	 * \return true, wenn erfolgreich, sonst false
	 */
	bool init();

	/**
	 * \fn void update()
	 * \brief aktualisiert die Anzeige
	 */
	void update();

	/**
	 * \fn bool setupGameScreen()
	 * \brief Erstellt den Hauptspielbildschirm
	 * \return true, wenn erfolgreich, sonst false
	 */
	bool setupGameScreen();

	/**
	 * \fn void setupControlPanel()
	 * \brief Erzeugt die Kontrollleiste
	 */
	void setupControlPanel();

	/**
	 * \fn void setupInventory()
	 * \brief Erzeugt das Fenster Inventar
	 */
	void setupInventory();

	/**
	 * \fn void setupCharInfo()
	 * \brief Erzeugt das Fenster CharakterInfo
	 */
	void setupCharInfo();

	/**
	 * \fn void setupSkilltree()
	 * \brief Erzeugt das Fenster Skilltree
	 */
	void setupSkilltree();

	/**
	 * \fn void setupChatWindow()
	 * \brief Erzeugt das Fenster Chat
	 */
	void setupChatWindow();
	
	/**
	 * \fn void setupCursorItemImage()
	 * \brief Erzeugt das Bild fuer am Cursor befestigte Items
	 */
	void setupCursorItemImage();

	/**
	 * \fn bool setupMainMenu()
	 * \brief Erstellt das Hauptmenue
	 * \return true, wenn erfolgreich, sonst false
	 */
	bool setupMainMenu();


	/**
	 * \fn bool setupObjectInfo()
	 * \brief Erstellt die Leiste mit Informationen zu einem Objekt
	 */
	bool setupObjectInfo();
	
	/**
	 * \fn bool setupObjectInfo()
	 * \brief Erstellt die Fenster mit Informationen zu Gegenstaenden
	 */
	bool setupItemInfo();
	
	/**
	 * \fn bool setupPartyInfo()
	 * \brief erstellt die Fenster mit Informationen zur Party
	 */
	bool setupPartyInfo();

	/**
	 * \fn bool initInputs()
	 * \brief Initialisiert die Eingabegeraete
	 */
	bool initInputs();

	/**
	 * \fn void updateMainMenu()
	 * \brief Aktualisiert das Hauptmenue
	 */
	void updateMainMenu();

	/**
	 * \fn void updateCursorItemImage()
	 * \brief aktualisiert das Bild fuer den am Cursor befestigten Gegenstand
	 */
	void updateCursorItemImage();

	/**
	 * \fn void updateObjectInfo()
	 * \brief Aktualisiert die Leiste mit Informationen zu einem Objekt
	 */
	void updateObjectInfo();
	
	/**
	 * \fn void updateObjectInfo()
	 * \brief Aktualisiert die Fenster mit Informationen zu Gegenstaenden
	 */
	void updateItemInfo();

	/**
	 * \fn void updatePartyInfo()
	 * \brief Aktualisiert die Informationen zu Partymitgliedern
	 */
	void updatePartyInfo();

	/**
	 * \fn bool mouseMoved(const OIS::MouseEvent &evt)
	 * \brief Behandelt Mausbewegung
	 */
	bool mouseMoved(const OIS::MouseEvent &evt);

	/**
	 * \fn bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID)
	 * \brief Behandelt Druecken von Maustasten
	 */
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID);

	/**
	 * \fn bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID)
	 * \brief Behandelt Loslassen von Maustasten
	 */
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID);
	
	/**
	 * \fn bool onDropItemClicked(const CEGUI::EventArgs& evt)
	 * \brief Behandelt das Senden einer Nachricht
	 * \param evt CEGUI Event Parameter
	 */
	bool onDropItemClicked(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool keyPressed(const OIS::KeyEvent &evt)
	 * \brief Behandelt Druecken von Tasten auf der Tastatur
	 */
	bool keyPressed(const OIS::KeyEvent &evt);

	/**
	 * \fn bool keyReleased(const OIS::KeyEvent &evt)
	 * \brief Behandelt Loslassen von Tasten auf der Tastatur
	 */
	bool keyReleased(const OIS::KeyEvent &evt);

	/**
	 * \fn void setWindowExtents(int width, int height)
	 * \brief Setzt die Groesse des Fensters
	 * \param width Breite
	 * \param height Hoehe
	 */
	void setWindowExtents(int width, int height);


	private:

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
	 * \var Ogre::RenderWindow *m_window
	 * \brief Fenster der Applikation
	 */
	Ogre::RenderWindow *m_window;

	/**
	 * \var Ogre::SceneManager* m_scene_manager
	 * \brief aktueller Szenemanager
	 */
	Ogre::SceneManager* m_scene_manager;


	/**
	 * \var OIS::InputManager *m_ois
	 * \brief Basisobjekt von OIS
	 */
	OIS::InputManager *m_ois;

	/**
	 * \var OIS::Mouse *m_mouse
	 * \brief Repraesentation der Maus
	 */
	OIS::Mouse *m_mouse;

	/**
	 * \var OIS::Keyboard *m_keyboard
	 * \brief Repraesentation der Tastatur
	 */
	OIS::Keyboard *m_keyboard;


	/**
	 * \var Document* m_document
	 *  \brief Dokument
	 */
	Document* m_document;

	/**
	 * \fn CEGUI::Window* m_main_menu
	 * \brief Hauptmenue
	 */
	CEGUI::Window* m_main_menu;

	/**
	 * \var CEGUI::Window* m_game_screen
	 * \brief Fenster, in dem das eigentliche Spiel laeuft
	 */
	CEGUI::Window* m_game_screen;

	/**
	 * \var Scene* m_scene
	 * \brief Szene die dargestellt wird
	 */
	Scene* m_scene;

	/**
	 * \var bool m_gui_hit
	 * \brief wird auf true gesetzt, wenn bei einem Mausklick ein GUI Element getroffen wird
	 */
	bool m_gui_hit;
	
	/**
	 * \var std::map<std::string, Window*> m_sub_windows
	 * \brief Unterfenster sortiert nach Name
	 */
	std::map<std::string, Window*> m_sub_windows;
	
	

};

#endif
