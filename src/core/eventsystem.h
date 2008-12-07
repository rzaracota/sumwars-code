#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <string>
#include "debug.h"
#include "event.h"
#include "geometry.h"

extern "C"
{
	
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class Region;

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
		 * \fn static void doString(char* instructions)
		 * \brief Fuehrt die Anweisungen aus, die in dem String stehen
		 * \param instructions String mit Lua Befehlen
		 */
		static void doString(char* instructions);
		
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
		 * \var static Trigger* m_trigger
		 * \brief der zuletzt vom Script eingefuegte Trigger
		 */
		static Trigger* m_trigger;
};


#endif
