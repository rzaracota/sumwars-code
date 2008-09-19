
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include "Ogre.h"

// needed to be able to create the CEGUI renderer interface
#include "OgreCEGUIRenderer.h"

// CEGUI includes
#include "CEGUI.h"
#include "numberedwindow.h"


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

// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class ListItem : public CEGUI::ListboxTextItem
{
	public:
		ListItem(const CEGUI::String& text) : ListboxTextItem(text)
		{
			setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
		}
};


class SaveListItem : public ListItem
{
	public:
		SaveListItem(const CEGUI::String& text, std::string file) : ListItem(text)
		{
			setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
			m_filename = file;
		}

		std::string m_filename;
};



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
	 * \fn bool setupMainScreen()
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
	 * \fn void updateCharInfo()
	 * \brief Aktualisiert die Anzeigen des CharakterInfo Fensters
	 */
	void updateCharInfo();

	/**
	 * \fn void updateControlPanel()
	 * \brief Aktualisiert die Kontrollleiste
	 */
	void updateControlPanel();

	/**
	 * \fn void updateInventory()
	 * \brief Aktualisiert die Inventaranzeige
	 */
	void updateInventory();

	/**
	 * \fn void updateSkilltree()
	 * \brief Aktualisiert Skilltree
	 */
	void updateSkilltree();

	/**
	 * \fn void updateItemTooltip(unsigned int pos)
	 * \brief Aktualisiert den Tooltip fuer das Item ueber dem die Maus ist
	 * \param pos Nummer des zu aktualisierenden Items
	 */
	void updateItemTooltip(unsigned int pos);

	/**
	 * \fn void updateAbilityTooltip(unsigned int pos)
	 * \brief Aktualisiert den Tooltip fuer die Faehigkeit ueber der die Maus ist
	 */
	void updateAbilityTooltip(unsigned int pos);

	/**
	 * \fn void updateObjectInfo()
	 * \brief Aktualisiert die Leiste mit Informationen zu einem Objekt
	 */
	void updateObjectInfo();


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

	/**
	 * \fn bool onSavegameChosen(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Auswahl eines Savegames in der Liste
	 */
	bool onSavegameChosen(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool onSavegameSelected(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Auswahl eines Savegames
	 */
	bool onSavegameSelected(const CEGUI::EventArgs& evt);


	/**
	 * \fn bool onStartSinglePlayer(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Start eines Einzelspielerspieles
	 */
	bool onStartSinglePlayer(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool onStartMultiPlayer(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Start eines Einzelspielerspieles
	 */
	bool onStartMultiPlayer(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool onStartMultiPlayerHost(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Start eines Einzelspielerspieles
	 */
	bool onStartMultiPlayerHost(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool onButtonSaveExitClicked(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Klick auf Speichern/Beenden
	 */
	bool onButtonSaveExitClicked(const CEGUI::EventArgs& evt);

	/**
	 * \fn void onButtonInventoryClicked()
	 * \brief Behandelt Klick auf Inventar Button
	 */
	bool onButtonInventoryClicked(const CEGUI::EventArgs& evt);

	/**
	 * \fn void onButtonCharInfoClicked()
	 * \brief Behandelt Klick auf CharakterInfo Button
	 */
	bool onButtonCharInfoClicked(const CEGUI::EventArgs& evt);

	/**
	 *\fn bool onButtonPartyClicked(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Klick auf Party Button
	 */
	bool onButtonPartyClicked(const CEGUI::EventArgs& evt);

	/**
	 * \fn void onButtonSkilltreeClicked()
	 * \brief Behandelt Klick auf Skilltree Button
	 */
	bool onButtonSkilltreeClicked(const CEGUI::EventArgs& evt);

	/**
	 * \fn void onButtonOpenChatClicked()
	 * \brief Behandelt Klick auf Chat oeffnen Button
	 */
	bool onButtonOpenChatClicked(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool onItemMouseButtonPressed(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Druecken der linken Maustaste ueber einen Item
	 */
	bool onItemMouseButtonPressed(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool onItemMouseButtonReleased(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Loslassen der linken Maustaste ueber einen Item
	 */
	bool onItemMouseButtonReleased(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool onSkillMouseClicked(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Mausklick auf eine Faehigkeit im Skilltree
	 */
	bool onSkillMouseClicked(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool onSwapEquipClicked(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Mausklick auf Ausruestung wechseln
	 */
	bool onSwapEquipClicked(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool onItemHover(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Eintritt der Maus in Bereich ueber einem Item
	 */
	bool onItemHover(const CEGUI::EventArgs& evt);


	/**
	 * \fn bool onAbilityHover(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Eintritt der Maus in Bereich ueber einer Faehigkeit
	 */
	bool onAbilityHover(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool onIncreaseAttributeButtonClicked(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Klick auf Button Attribut erhoehen
	 */
	bool onIncreaseAttributeButtonClicked(const CEGUI::EventArgs& evt);

	/**
	 * \fn bool consumeEvent(const CEGUI::EventArgs& evt)
	 * \brief Funktion die Events verbraucht
	 * \fn gibt immer true zurueck
	 */
	bool consumeEvent(const CEGUI::EventArgs& evt)
	{
		DEBUG("consumed");
		m_gui_hit = true;
		return true;
	}




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
	 * \var Ogre::RenderWindow *window
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

};

#endif
