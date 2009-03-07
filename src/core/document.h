/*
	Summoning Wars - document.h
	Copyright (C) 2007  Hans Wulf

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * \file document.h
 * \defgroup Document \ Dokument
 * \brief Stellt das Dokumentkonzept dar
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse Document
 */

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Ogre.h"

#include <string>
#include <list>
#include <map>
#include <set>


#include "networkstruct.h"
#include "projectile.h"
#include "party.h"
#include "debug.h"
#include "damage.h"
#include "itemlist.h"
#include "random.h"
#include "item.h"
#include "matrix2d.h"
#include "tiles.h"
#include "dropitem.h"
#include "world.h"
#include "player.h"
#include "timer.h"
#include <pthread.h>

#include "gettext.h"



/**
 * \class Document
 * \brief Diese Klasse enthaelt alle Daten fuer Repraesentation der Spielwelt auf der Clientseite.
 *
 *
 */

/**
 * Code fuer eine Taste
 */
typedef int KeyCode;

class Document
{


	public:




	/**
	 * \enum State
	 * \brief zaehlt verschiedene Zustaende auf, in denen sich das Dokument befinden kann
	 */
	enum State
	{
		INACTIVE =0,

		START_GAME =1,

		LOAD_SAVEGAME =2,

		RUNNING =3,

		SHUTDOWN_REQUEST =4,

		
		SHUTDOWN =5,

		DISCONNECTED =6,

 		START_SERVER =7,
   
   		SHUTDOWN_WRITE_SAVEGAME=8,

   
   		END_GAME =9,
	};


	/**
	 * \enum Modifications
	 * \brief Zaehlt Inhalte auf, die veraendert werden koennen
	 */
	enum Modifications
	{
		NO_MODIFICATIONS=0,
		OBJECTS_MODIFIED=1,
		GUISHEET_MODIFIED=2,
		WINDOWS_MODIFIED=4,
		ITEM_MODIFIED=8,
		ABILITY_MODIFIED=16,
		REGION_MODIFIED = 32,
		SAVEGAME_MODIFIED = 64,
	};

	/**
	 * \enum GUISheets
	 * \brief Verschiedene Ebenen die angezeigt werden können
	 */
	enum GUISheets
	{
		MAIN_MENU=1,
		GAME_SCREEN=2,
	};

	/**
	 * \enum Windows
	 * \brief Fenster die waerend des Spieles angezeigt werden koennen
	 */
	enum Windows
	{
		NO_WINDOWS =0x0,
		INVENTORY =0x1,
		CHARINFO=0x2,
		SKILLTREE=0x4,
		MENU =0x8,
		CHAT = 0x10,
		PARTY = 0x20,
		SAVEGAME_LIST = 0x40,
		START_MENU = 0x80,
		CHAR_CREATE = 0x100,
  		QUEST_INFO = 0x200,
		MINIMAP = 0x400,
		QUESTIONBOX = 0x800,
		TRADE =0x1000,
	};

	/**
	 * \enum ShortkeyDestination
	 * \brief Listet Aktion auf, die ueber Kurztasten verfuegbar sind
	 */
	enum ShortkeyDestination
	{
		NO_KEY=0,
		SHOW_INVENTORY =1,
		SHOW_CHARINFO =2,
		SHOW_SKILLTREE =3,
		SHOW_PARTYMENU=4,
		SHOW_CHATBOX =5,
		SHOW_CHATBOX_NO_TOGGLE =6,
		SHOW_QUESTINFO =7,
  		SHOW_MINIMAP =8,
		CLOSE_ALL=9,
		SWAP_EQUIP=10,
		USE_POTION = 30,
		USE_SKILL_LEFT=100,
		USE_SKILL_RIGHT=300,
		CHEAT = 1000,
	};

	/**
	 * Bildet von Tastencodes auf eine Wirkung ab
	 */
	typedef std::map<KeyCode,ShortkeyDestination> ShortkeyMap;


	/**
	 * \struct GUIState
	 * \brief Informationen zum aktuellen Zustand der GUI
	 */
	struct GUIState
	{

		/**
		 * \var m_chat_window_content
		 * \brief Inhalt des Chat-Nachrichtenfensters
		 */
		string m_chat_window_content;


		/**
		 * \var m_chat_sendline_content
		 * \brief Inhalt der Chat-Eingabezeile
	 	 */
		string m_chat_sendline_content;

		 /**
		 * \var m_left_mouse_pressed
		 * \brief Gibt an, ob die linke Maustaste festgehalten wird
	 	 */
		bool m_left_mouse_pressed;

		 /**
		 * \var m_right_mouse_pressed
		 * \brief Gibt an, ob die rechte Maustaste festgehalten wird
	 	 */
		bool m_right_mouse_pressed;
		
		 /**
		 * \var m_middle_mouse_pressed
		 * \brief Gibt an, ob die mittlere Maustaste festgehalten wird
		 */
		bool m_middle_mouse_pressed;

		/**
		 * \var m_shift_hold
		 * \brief Gibt an, ob die Shift-Taste festgehalten wird
		 */
		bool m_shift_hold;
		
		/**
		 * \var bool m_alt_hold
		 * \brief Gibt an, ob die Alt-Taste festgehalten wird
		 */
		bool m_alt_hold;

		 /**
		 * \var m_clicked_object_id
		 * \brief ID des angeklickten Objekts
	 	 */
		int m_clicked_object_id;

		/**
		 * \var Vector m_clicked;
		 * \brief zuletzt angeclickter Punkt
		 */
		Vector m_clicked;
		
		/**
		 * \var float m_left_mouse_pressed_time
		 * \brief Zaehler der die Anzahl Frames zaehlt die die linke Maustaste festgehalten wurde
		 */
		float m_left_mouse_pressed_time;


		/**
		 * \var float m_right_mouse_pressed_time
		 * \brief Zaehler der die Anzahl Frames zaehlt die die rechte Maustaste festgehalten wurde
		 */
		float m_right_mouse_pressed_time;

		/**
		 * \var GUISheets* m_sheet
		 * \brief Aktuell angezeigte Ebene
		 */
		GUISheets m_sheet;

		/**
		 * \var int m_shown_windows
		 * \brief aktuell angezeigte Fenster als Bitmaske
		 */
		int m_shown_windows;

		/**
		 * \var KeyCode m_pressed_key
		 * \brief aktuell festgehaltene Taste
		 */
		KeyCode m_pressed_key;

		/**
		 * \var std::string m_cursor_object
		 * \brief Ogre Name des Objektes auf das gerade der Mauszeiger zeigt
		 */
		std::string m_cursor_object;

		/**
		 * \var int m_cursor_object_id
		 * \brief ID des Objektes auf das gerade der Mauszeiger zeigt
		 */
		int m_cursor_object_id;

		/**
		 * \var int m_cursor_item_id
		 * \brief ID des Gegenstandes auf den gerade der Mauszeiger zeigt
		 */
		int m_cursor_item_id;

	};



// Konstruktor / Destruktor
	/**
	 * \fn Document()
	 * \brief Konstruktor
	 *
	 * Legt ein neues Document-Objekt an
	 */
	Document();


	/**
	 * \fn ~Document()
	 * \brief Destruktor
	 *
	 * Gibt den allokierten Speicher wieder frei
	 */
	~Document();
// Setter und Getter

	/**
	 * \fn void setSaveFile(string s)
	 * \brief Setzt den Name des Savefiles
	 * \param s Name des Savefiles
	 */
	void setSaveFile(string s);
	

	/**
	 * \fn string getSaveFile()
	 * \brief Gibt den Name des Savefiles aus
	 */
	string getSaveFile()
	{
		return m_save_file;
	}

	/**
	 * \fn static void* writeSaveFile(void* doc_ptr)
	 * \param doc_ptr Zeiger auf das Dokument
	 * \brief Schreibt das Savefile
	 */
	static void* writeSaveFile(void* doc_ptr);


	/**
	 * \fn GUISTate* getGUIState()
	 * \brief Gibt Zustand der GUI Datenfelder zurueck
	 * \return Zustand der GUI
	 */
	GUIState* getGUIState()
	{
		return &m_gui_state;
	}


	/**
	 * \fn State getState()
	 * \brief Gibt aktuellen Status des Dokuments aus
	 */
	State getState()
	{
		return m_state;
	}

	/**
	 * \fn void setState(State s)
	 * \brief Setzt den Status des Dokuments
	 */
	void setState(State s)
	{
		m_state =s;
	}

	/**
	 * \fn bool getModified()
	 * \brief Gibt aus, ob Daten des Dokumentes geaendert wurden
	 * \return modifiziert
	 */
	int getModified()
	{
		return m_modified;
	}

	/**
	 * \fn void setModified(int mod)
	 * \brief Setzt den Zustand modifiziert
	 * \param mod neuer Zustand
	 */
	void setModified(int mod)
	{
		m_modified = mod;
	}


	/**
	 * \fn Party* getParty()
	 * \brief Gibt die Party aus der der Spieler angehoert
	 * \return Party
	 */
	Party* getParty()
	{
		return 0;
	}

	/**
	 * \fn void setLeftAction(Action::ActionType a)
	 * \brief Setzt die Aktion, die durch die linke Maustaste ausgeloest wird
	 */
	void setLeftAction(Action::ActionType a);



	/**
	 * \fn void setRightAction(Action::ActionType a)
	 * \brief Setzt die Aktion, die durch die rechte Maustaste ausgeloest wird
	 */
	void setRightAction(Action::ActionType a);

	

	/**
	 * \fn void setServer(bool server)
	 * \brief Stellt ein ob das Spiel der Server ist
	 */
	void setServer(bool server)
	{
		m_server = server;
	}

	/**
	 * \fn std:.string getAbilityDescription(Action::ActionType ability)
	 * \brief Gibt die Beschreibung der Faehigkeit, ueber der Mauszeiger ist, aus
	 */
	std::string getAbilityDescription(Action::ActionType ability);

	/**
	 * \fn KeyCode getMappedKey(ShortkeyDestination sd)
	 * \brief Gibt die Taste zurueck, die das angegebene Ereignis ausloest
	 */
	KeyCode getMappedKey(ShortkeyDestination sd);

	/**
	 * \fn void installShortkey(KeyCode key,ShortkeyDestination dest)
	 * \brief Bindet eine Taste an ein Ereignis
	 * \param key Taste
	 * \param dest Ereignis, das auf die Taste hin ausgeloest werden soll
	 */
	void installShortkey(KeyCode key,ShortkeyDestination dest);




	/**
	 * \fn void onButtonSendMessageClicked ( )
	 * \brief Button zum Absenden einer Nachricht wurde angeklickt
	 */
	void onButtonSendMessageClicked ( );

	/**
	 * \fn bool checkSubwindowsAllowed()
	 * \brief Prueft, ob aktuell das oeffnen von Unterfenstern (Inventar etc) erlaubt ist
	 */
	bool checkSubwindowsAllowed();


	/**
	 * \fn void onButtonSaveExitClicked ( )
	 * \brief Button zum Speichern und Beenden des Spiels wurde angeklickt
	 */
	void onButtonSaveExitClicked ( );

	/**
	 * \fn void onButtonMinimapClicked()
	 * \brief Behandelt Klick auf Inventar Button
	 */
	void onButtonMinimapClicked();

	/**
	 * \fn void onButtonInventoryClicked()
	 * \brief Behandelt Klick auf Inventar Button
	 */
	void onButtonInventoryClicked();

	/**
	 * \fn void onButtonCharInfoClicked()
	 * \brief Behandelt Klick auf CharakterInfo Button
	 */
	void onButtonCharInfoClicked();
	
	/**
	 * \fn void onButtonPartyInfoClicked()
	 * \brief Behandelt Klick auf CharakterInfo Button
	 */
	void onButtonPartyInfoClicked();


	/**
	 * \fn void onButtonSkilltreeClicked()
	 * \brief Behandelt Klick auf Skilltree Button
	 */
	void onButtonSkilltreeClicked();

	/**
	 * \fn void onButtonOpenChatClicked()
	 * \brief Behandelt Klick auf Chat oeffnen Button
	 */
	void onButtonOpenChatClicked();
	
	/**
	 * \fn void onButtonQuestInfoClicked()
	 * \brief Behandelt Klick auf Chat oeffnen Button
	 */
	void onButtonQuestInfoClicked();

	/**
	 * \fn void onSwapEquip()
	 * \brief Wechselt die Ausruestung
	 */
	void onSwapEquip();
	
	

	/**
	 * \fn void onLeftMouseButtonClick (Vector pos)
	 * \brief Behandelt die Reaktion auf einen Linksklick ins Spielfeld
	 * \param pos Position des Mausklicks
	 */
	void onLeftMouseButtonClick(Vector pos);


	/**
	 * \fn void onRightMouseButtonClick (Vector pos)
	 * \brief Behandelt die Reaktion auf einen Rechtsklick ins Spielfeld
	 * \param pos Position des Mausklicks
	 */
	void onRightMouseButtonClick(Vector pos);
	
	/**
	 * \fn void onMouseMove(float xrel, float yrel, float wheelrel)
	 * \brief Behandelt die Bewegung der Maus
	 * \param xrel relative Bewegung in x-Richtung
	 * \param yrel relative Bewegung in y-Richtung
	 * \param wheelrel Bewegung des Mausrades
	 */
	void onMouseMove(float xrel, float yrel, float wheelrel);

	/**
	 * \fn void onStartScreenClicked()
	 * \brief Behandelt die Reaktion auf einen Klick im Startbildschirm
	 */
	void onStartScreenClicked();
	
	
	/**
	 * \fn void onItemLeftClick(short pos)
	 * \brief Behandelt Linksklick auf ein Item
	 */
	void onItemLeftClick(short pos);

	/**
	 * \fn void onItemRightClick(short pos)
	 * \brief Behandelt Rechtsklick auf ein Item
	 */
	void onItemRightClick(short pos);
	
	/**
	 * \fn void onAnswerClick(int id)
	 * \brief Behandelt Klick auf eine Antwort bei einem Gespraech
	 * \param id Nummer der Antwort
	 */
	void onAnswerClick(int id);

	/**
	 * \fn void increaseAttribute(CreatureBaseAttr::Attribute attr)
	 * \brief Erhoeht das angegebene Attribut
	 */
	void increaseAttribute(CreatureBaseAttr::Attribute attr);

	/**
	 * \fn void learnAbility(Action::ActionType act)
	 * \brief Lernt die angegebene Faehigkeit
	 */
	void learnAbility(Action::ActionType act);

	/**
	 * \fn void dropCursorItem()
	 * \brief Laesst das Item fallen, das aktuell am Cursor haengt
	 */
	void dropCursorItem();

	/**
	 * \fn void onDropItemClick(int id)
	 * \brief Behandelt Linksklick auf ein Item
	 */
	void onDropItemClick(int id);
	
	/**
	 * \fn bool onKeyPress(KeyCode key)
	 * \brief Behandelt einen Tastendruck
	 * \param key Keycode der Taste
	 */
	bool onKeyPress(KeyCode key);

	/**
	 * \fn bool onKeyRelease(KeyCode key)
	 * \brief Behandelt loslassen einer Taste
	 */
	bool onKeyRelease(KeyCode key);

	/**
	 * \fn void onButtonKick(int id)
	 * \brief Behandelt Klick auf Button Spieler kicken
	 * \param id ID des Spielers
	 */
	void onButtonKick(int id);
	
	/**
	 * \fn void onButtonPartyApply(int id)
	 * \brief Behandelt Druck auf Button Bei einer Party bewerben
	 * \param id ID des Partyleiters
	 */
	void onButtonPartyApply(int id);

	/**
	 * \fn void onButtonPartyAccept(int id)
	 * \brief Behandelt Druck auf Button Partymitglied annehmen
	 * \param id ID des Bewerbers
	 */
	void onButtonPartyAccept(int id);
	
	/**
	 * \fn void onButtonPartyReject(int id)
	 * \brief Behandelt Druck auf Button Partymitglied ablehnen
	 * \param id ID des Bewerbers
	 */
	void onButtonPartyReject(int id);
	
	/**
	 * \fn void onButtonPartyWar(int id)
	 * \brief Behandelt Druck auf Button Krieg erklaeren
	 * \param id ID des Partyleiters
	 */
	void onButtonPartyWar(int id);
	
	/**
	 * \fn void onButtonPartyPeace(int id)
	 * \brief Behandelt Druck auf Button Frieden schliessen
	 * \param id ID des Partyleiters
	 */
	void onButtonPartyPeace(int id);
	
	/**
	 * \fn void onButtonPartyLeave()
	 * \brief Behandelt Druck auf Button Partymitglied ablehnen
	 */
	void onButtonPartyLeave();
	
	/**
	 * \fn void onButtonCloseTrade()
	 * \brief Behandelt Druck auf Button Handel beenden
	 */
	void onButtonCloseTrade();
	
	/**
	 * \fn void onTradeItemLeftClick(short pos)
	 * \brief Behandelt Linksklick auf ein Item beim Handel
	 */
	void onTradeItemLeftClick(short pos);

	/**
	 * \fn void onTradeItemRightClick(short pos)
	 * \brief Behandelt Rechtsklick auf ein Item beim Handel
	 */
	void onTradeItemRightClick(short pos);
	
	/**
	 * \fn void onTradeSellClicked();
	 * \brief Behandelt Klick auf die Flaeche des Handelsfensters (verkauft Item)
	 */
	void onTradeSellClicked();
	
	/**
	 * \fn void sendChatMessage(std::string msg)
	 * \brief sendet eine Chatnachricht an alle Spieler
	 */
	void sendChatMessage(std::string msg);
	
	/**
	 * \fn void setNewCharacter(WorldObject::TypeInfo::ObjectSubtype subtype, PlayerLook look)
	 * \brief setzt die Daten fuer neuen Spieler
	 * \param subtype Typ des Spielers
	 * \param look Aussehen des Spielers
	 */
	void setNewCharacter(WorldObject::TypeInfo::ObjectSubtype subtype, PlayerLook look);
	
	/**
	 * \fn void createNewCharacter()
	 * \brief erzeugt einen neuen Spieler
	 * \param name Name des Spielers
	 */
	void createNewCharacter(std::string name);
	
	/**
	 * \fn void onCreateNewCharButton()
	 * \brief Oeffnet das Fenster zur Charaktererstellung
	 */
	void onCreateNewCharButton();

	/**
	 * \fn void emitDebugSignal(int i=0)
	 * \brief Sendet ein Signal zu Debugging Zwecken
	 * \param i Nummer des Debugereignisses
	 */
	void emitDebugSignal(int i=0);

	/**
	 * \fn void update( float time=0)
	 * \brief Aktualisiert den Inhalt der GUI, holt alle Daten neu vom Netzwerk
	 */
	void update(float time=0);

	/**
	 * \fn void startGame(bool server)
	 * \brief Baut Verbindung zum Server auf
	 */
	void startGame(bool server);

	/**
	 * \fn Player* getLocalPlayer()
	 * \brief Gibt den Spieler aus, der lokal an dem Rechner ist
	 */
	Player* getLocalPlayer()
	{
		if (World::getWorld() ==0)
			return m_temp_player;

		return static_cast<Player*>(World::getWorld()->getLocalPlayer());
	}

	/**
	 * \fn World* getWorld()
	 * \brief gibt Zeiger auf die Spielwelt aus
	 */
	World* getWorld()
	{
		return World::getWorld();
	}


	// Private stuff
	private:
	//Fields
		/**
	 * \fn void sendCommand(ClientCommand* command)
	 * \param command Kommando, das gesendet werden soll
	 * \return Status des Netzwerkes nach der Sendeanfrage
	 * \brief sendet ein Kommando an den Server
		 **/
		void sendCommand(ClientCommand* comm);

		/**
		 * \fn void loadSavegame()
		 * \brief laedt ein Savegame
		 */
		void loadSavegame();

		/**
		 * \fn void updateContent(float time)
		 * \brief Aktualisiert den Inhalt des Dokuments
		 */
		void updateContent(float time);


		/**
		* \fn int getObjectAt(float x,float y)
		* \brief Ermittelt die ID des Objektes an den angegebenen Koordinaten
		 * \param x x-Koordinate
		 * \param y y-Koordinate
		* \return ID
		*/
		int getObjectAt(float x,float y);
	
	
	
	
		/**
		* \var GUIState m_gui_state
		* \brief Enthaelt alle Informationen zum Zustand der GUI
		*/
		GUIState m_gui_state;
	
	
		/**
		* \var int m_modified
		* \brief Bitmaske welche angibt welche Teile des Dokuments veraendert wurden
		*/
		int m_modified;
	
		/**
		* \var string m_save_file
		* \brief Dateiname des Savefiles
		*/
		string m_save_file;
	
		/**
		* \var State m_state
		* \brief Status des Dokuments
		*/
		State m_state;
	
		/**
		* \var ShortkeyMap m_shortkey_map
		* \brief Bildet Taste auf Ereignis, das per Shortkey ausgeloest werden kann ab
		*/
		ShortkeyMap m_shortkey_map;
	
		/**
		* \var std::set<KeyCode> m_special_keys
		* \brief Menge der Tasten, die eine besondere Bedeutung haben und die deswegen nicht frei zugewiesen werden koennen
		*/
		std::set<KeyCode> m_special_keys;
	
	
		/**
		* \var char m_server_ip[16]
		* \brief IP des Servers
		*/
		char m_server_ip[16];
	
	
		/**
		* \var bool m_server
		* \brief true, wenn der aktuelle Rechner der Server ist
		*/
		bool m_server;
		
		/**
		 * \var Player* m_temp_player
		 * \brief temporaer angelegtes Spielerobjekt zum rendern des ausgewaehlten Spielers
		 */
		Player* m_temp_player;
	
		/**
		* \var Timer m_timer
		* \brief Timer fuer die Updatezyklen der Spielwelt
		*/
		Timer m_timer;
	
		/**
		* \var m_shutdown_timer
		* \brief Timer fuer Shutdown
		*/
		float m_shutdown_timer;

};


#endif //DOCUMENT_H



