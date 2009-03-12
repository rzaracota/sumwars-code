#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <string>
#include "debug.h"
#include "event.h"
#include "geometry.h"
#include "charconv.h"

extern "C"
{
	
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class Region;
class Dialogue;

/**
 * \class EventSystem
 * \brief Klasse fuer das Ereignissystem
 */
class EventSystem
{
	public:
	
		/**
		* \fn static void init()
		* \brief initialisiert das Eventsystem
		*/
		static void init();
		
		/**
		* \fn static void cleanup()
		* \brief gibt allokierten Speicher frei
		*/
		static void cleanup();
		
		/**
		 * \fn static void doString(const char* instructions)
		 * \brief Fuehrt die Anweisungen aus, die in dem String stehen
		 * \param instructions String mit Lua Befehlen
		 */
		static void doString(const char* instructions);
		
		/**
		 * \fn static void doFile(const char* file)
		 * \brief Fuehrt die Anweisungen aus, die in der Datei
		 * \param file Datei mit Lua Befehlen
		 */
		static void doFile(const char* file);
		
		/**
		 * \fn static void reportErrors(lua_State *L, int status)
		 * \brief Funktion um Fehler bei der Ausfuehrung von Skripten zu melden
		 * \param L Zustandsobjekt von Lua
		 * \param status von Lua gemeldeter Fehlerzustand
		 */
		static void reportErrors(lua_State *L, int status);
		
		/**
		 * \fn static void setRegion(Region* region)
		 * \brief Setzt die Region in der aktuell alle Events ablaufen
		 * \param region Region
		 */
		static void setRegion(Region* region)
		{
			m_region = region;
			m_trigger =0;
		}
		
		/**
		 * \fn static void setDialogue(Dialogue* dialogue)
		 * \brief Setzt den Dialog, dem die weiteren Antworten hinzugefuegt werden
		 * \param dialogue Dialog
		 */
		static void setDialogue(Dialogue* dialogue)
		{
			m_dialogue = dialogue;
		}
		

		/**
		 * \fn static std::string getReturnValue()
		 * \brief Gibt den Rueckgabewert des Luascripts als String aus
		 */
		static std::string getReturnValue();
		
		/**
		 * \fn lua_State* getLuaState()
		 * \brief Gibt das Lua Statusobjekt aus
		 */
		static lua_State* getLuaState()
		{
			return m_lua;
		}
		

		/**
		 * \fn static bool executeEvent(Event* event)
		 * \brief Fuehrt das Event durch.
		 * \return true, wenn das Event erfolgreich ausgefuehrt wurde
		 */
		static bool executeEvent(Event* event);
		
		/**
		 * \fn static int getObjectValue(lua_State *L)
		 * \brief Schiebt den gewuenschten Attributwert eines Objektes auf den Lua Stack
		 * \param L Lua Status
		 */
		static int getObjectValue(lua_State *L);
		
		/**
		 * \fn static int getObjectValue(lua_State *L)
		 * \brief Setzt den gewuenschten Attributwert eines Objektes
		 * \param L Lua Status
		 */
		static int setObjectValue(lua_State *L);
		
		/**
		 * \fn static int getDamageValue(lua_State *L)
		 * \brief Setzt einen Wert des Schadensobjekts
		 * \param L Lua Status
		 */
		static int getDamageValue(lua_State *L);
		
		/**
		 * \fn static int getDamageValue(lua_State *L)
		 * \brief Schiebt einen Wert des Schadensobjekts auf den Stack
		 * \param L Lua Status
		 */
		static int setDamageValue(lua_State *L);
		
		/**
		 * \fn static int createProjectile(lua_State *L)
		 * \brief Erzeugt ein Projektil
		 * \param L Lua Status
		 */
		static int createProjectile(lua_State *L);
		
		/**
		 * \fn static int pointIsInArea(lua_State *L)
		 * \brief prueft, ob ein Objekt sich in einem Gebiet befindet
		 * \param L Lua Status
		 */
		static int pointIsInArea(lua_State *L);
		
		/**
		 * \fn static int unitIsInArea(lua_State *L)
		 * \brief prueft, ob ein Objekt sich in einem Gebiet befindet
		 * \param L Lua Status
		 */
		static int unitIsInArea(lua_State *L);
		
		/**
		 * \fn static int createObject(lua_State *L);
		 * \brief erzeugt ein neues Objekt
		 * \param L Lua Status
		 */
		static int createObject(lua_State *L);
		
		/**
		 * \fn static int deleteObject(lua_State *L)
		 * \brief Entfernt ein Objekt
		 * \param L Lua Status
		 */
		static int deleteObject(lua_State *L);
		
		/**
		 * \fn static int createMonsterGroup(lua_State *L)
		 * \brief Erstellt eine neue Monstergruppe
		 * \param L Lua Status
		 */
		static int createMonsterGroup(lua_State *L);
		
		/**
		 * \fn static int addUnitCommand(lua_State *L)
		 * \brief Fuegt zu einer Einheit ein Kommando hinzu
		 * \param L Lua Status
		 */
		static int addUnitCommand(lua_State *L);
		
		/**
		 * \fn static int getObjectAt(lua_State *L)
		 * \brief Gibt das Objekt an der angegebenen Stelle aus
		 * \param L Lua Status
		 **/
		static int getObjectAt(lua_State *L);
		
		/**
		 * \fn static int getObjectsInArea(lua_State *L)
		 * \brief Gibt die Objekte in der angegebenen Flaeche aus
		 */
		static int getObjectsInArea(lua_State *L);
		
		/**
		 * \fn static int dropItem(lua_State *L)
		 * \brief Laesst einen Gegenstand fallen
		 * \param L Lua Status
		 */
		static int dropItem(lua_State *L);
		
		/**
		 * \fn static int addLocation(lua_State *L)
		 * \brief Fuegt einen neuen Ort hinzu
		 * \param L Lua Status
		 */
		static int addLocation(lua_State *L);
		
		/**
		 * \fn static int getLocation(lua_State *L)
		 * \brief einen Ort aus
		 * \param L Lua Status
		 */
		static int getLocation(lua_State *L);
		
		/**
		 * \fn static int addArea(lua_State *L)
		 * \brief Fuegt einen neuen Bereich hinzu
		 * \param L Lua Status
		 */
		static int addArea(lua_State *L);
		
		/**
		 * \fn static int startTimer(lua_State *L)
		 * \brief startet einen Timer
		 * \param L Lua Status
		 */
		static int startTimer(lua_State *L);
		
		/**
		 * \fn static int insertTrigger(lua_State *L)
		 * \brief Fuegt einen Trigger ein
		 * \param L Lua Status
		 */
		static int insertTrigger(lua_State *L);
		
		/**
		 * \fn static addTriggerVariable(lua_State *L)
		 * \brief Fuegt zum zuletzt erzeugten Trigger eine Variable hinzu
		 * \param L Lua Status
		 */
		static int addTriggerVariable(lua_State *L);
		
		/**
		 * \fn static int setCutsceneMode(lua_State *L)
		 * \brief Setzt den Cutscene Modus der Region
		 * \param L Lua Status
		 */
		static int setCutsceneMode(lua_State *L);
		
		/**
		 * \fn static int addCameraPosition(lua_State *L)
		 * \brief Fuegt der Kamerafahrt eine Position hinzu
		 * \param L Lua Status
		 */
		static int addCameraPosition(lua_State *L);
		
		/**
		 * \fn static int speak(lua_State *L)
		 * \brief Fuegt dem aktuellen Dialog einen Text hinzu
		 * \param L Lua Status
		 */
		static int speak(lua_State *L);
		
		/**
		 * \fn static int addQuestion(lua_State *L)
		 * \brief Fuegt dem aktuellen Dialog eine Frage
		 * \param L Lua Status
		 */
		static int addQuestion(lua_State *L);
		
		/**
		 * \fn static int addAnswer(lua_State *L)
		 * \brief Fuegt dem aktuellen Dialog eine Antwort auf die Frage hinzu
		 * \param L Lua Status
		 */
		static int addAnswer(lua_State *L);
		
		/**
		 * \fn static int changeTopic(lua_State *L)
		 * \brief Wechselt das Thema des aktuelle Dialogs
		 * \param L Lua Status
		 */
		static int changeTopic(lua_State *L);
		
		/**
		 * \fn static int createDialogue(lua_State *L)
		 * \brief Erzeugt einen neuen Dialog
		 * \param L Lua Status
		 */
		static int createDialogue(lua_State *L);
		
		/**
		 * \fn static int addSpeaker(lua_State *L)
		 * \brief fuegt dem Dialog einen Sprecher hinzu
		 * \param L Lua Status
		 */
		static int addSpeaker(lua_State *L);
		
		/**
		 * \fn static int getSpeaker(lua_State *L)
		 * \brief Gibt zu einem Sprecher das Objekt aus
		 * \param L Lua Status
		 */
		static int getSpeaker(lua_State *L);
		
		/**
		 * \fn static int setTopicBase(lua_State *L)
		 * \brief Setzt den Basisbereich der Topics
		 * \param L Lua Status
		 */
		static int setTopicBase(lua_State *L);
		
		/**
		 * \fn static int setRefName(lua_State *L)
		 * \brief Setzt den Referenznamen eines Objekts
		 * \param L Lua Status
		 */
		static int setRefName(lua_State *L);
		
		/**
		 * \fn static Vector getVector(lua_State *L, int index)
		 * \brief Liest einen Vector vom Lua Stack
		 * \param L Lua Status
		 * \param index Index des Vektors auf dem Stack
		 */
		static Vector getVector(lua_State *L, int index);
		
		/**
		 * \fn static void pushVector(lua_State *L, Vector v)
		 * \brief Schiebt einen Vektor auf den Lua Stack
		 * \param v Vector
		 * \param L Lua Status
		 **/
		static void pushVector(lua_State *L, Vector v);
		
		/**
		 * \fn static int getRolePlayers(lua_State *L)
		 * \brief Gibt eine Liste von Spielerd aus, die eine bestimmte Rolle erfuellen
		 * \param L Lua Status
		 **/
		static int getRolePlayers(lua_State *L);
		
		/**
		 * \fn static int writeString(lua_State *L)
		 * \brief schreibt einen String in den Stream
		 * \param L Lua Status
		 */
		static int writeString(lua_State *L);

		/**
		 * \fn static int writeNewline(lua_State *L)
		 * \brief Schreibt ein Newline in den Stream
		 * \param L Lua Status
		 */
		static int writeNewline(lua_State *L);
				
		/**
		 * \fn static void writeSavegame(CharConv* savegame, int playerid, bool local_player)
		 * \brief Schreibt die Daten fuer ein Savegame
		 * \param savegame Streaming Objekt fuer das Savegame
		 */
		static void writeSavegame(CharConv* savegame);
		
		/**
		 * \fn static void readSavegame(CharConv* savegame, int playerid, bool local_player)
		 * \brief Liest die Daten aus einem Savegame
		 * \param savegame Streaming Objekt fuer das Savegame
		 * \param playerid ID des Spielers
		 * \param local_player Gibt an, ob es sich um den lokalen Spieler handelt
		 */
		static void readSavegame(CharConv* savegame, int playerid, bool local_player);
		
		/**
		 * \fn static void clearPlayerVarString(int id)
		 * \brief Loescht den String mit Lua Befehlen
		 * \param id ID des Spielers
		 */
		static void clearPlayerVarString(int id);
		
		/**
		 * \fn static std::string getPlayerVarString(int id)
		 * \brief Gibt den String mit Lua Befehlen zum aktualisieren der Variablen aus
		 * \param id ID des Spielers
		 */
		static std::string getPlayerVarString(int id);
		
		/**
		 * \fn static int writeUpdateString(lua_State *L)
		 * \brief Schreibt einen Updatestring fuer einen Spieler
		 */
		static int writeUpdateString(lua_State *L);
		
		
		/**
		 * \fn static int luagettext(lua_State *L)
		 * \brief gettext fuer lua
		 */
		static int luagettext(lua_State *L);
	private:
		/**
		 * \var static lua_State * m_lua
		 * \brief Zustandsobjekt von Lua
		 */
		static lua_State * m_lua;
		
		/**
		 * \var static  Region m_region
		 * \brief Region in der der aktuell alle Events ausgefuehrt werden
		 */
		static Region* m_region;
		
		/**
		 * \var static Dialogue* m_dialogue
		 * \brief aktueller Dialog
		 */
		static Dialogue* m_dialogue;
		
		/**
		 * \var static Trigger* m_trigger
		 * \brief der zuletzt vom Script eingefuegte Trigger
		 */
		static Trigger* m_trigger;
		
		/**
		 * \var static CharConv* m_charconv
		 * \brief Stream Objekt in das die Lua Variablen geschrieben werden
		 */
		static CharConv* m_charconv;
		
		/**
		 * \var static std::map<int, std::string> m_player_varupdates
		 * \brief Liste mit Befehlen mit denen lokale Variablen der Spieler angepasst werden
		 */
		static std::map<int, std::string> m_player_varupdates;
};


#endif
