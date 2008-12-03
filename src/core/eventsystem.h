#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <string>
#include "debug.h"
#include "event.h"

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
		 * \fn static int pointIsInArea(lua_State *L)
		 * \brief prueft, ob ein Objekt sich in einem Gebiet befindet
		 */
		static int pointIsInArea(lua_State *L);
		
		/**
		 * \fn static int createObject(lua_State *L);
		 * \brief erzeugt ein neues Objekt
		 */
		static int createObject(lua_State *L);
		
		/**
		 * \fn static int deleteObject(lua_State *L)
		 * \brief Entfernt ein Objekt
		 */
		static int deleteObject(lua_State *L);
		
		/**
		 * \fn static int dropItem(lua_State *L)
		 * \brief Laesst einen Gegenstand fallen
		 */
		static int dropItem(lua_State *L);
		
		/**
		 * \fn static int getLocation(lua_State *L)
		 * \brief einen Ort aus
		 */
		static int getLocation(lua_State *L);
		
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
};


#endif
