#include "debug.h"
#include "eventsystem.h"

#include <string>
#include <sstream>

/**
 * \class Quest
 * \brief Klasse fuer eine Aufgabe, die die Spieler erledigen koennen
 */
class Quest
{
	public:
		/**
		* \enum State
		* \brief Aufzaehlung von Zustaenden fuer ein Quest
		*/
		enum State
		{
			NOT_STARTED =0,
			STARTED = 1,
			FINISHED = 2,
			FAILED = 3
		};
		
		/**
		 * \fn Quest(std::string name, std::string table_name)
		 * \brief Konstruktor
		 * \param name Name des Quests
		 * \param table_name Name der Lua Tabelle in der die Variablen zu dem Quest gespeichert werden
		 */
		Quest(std::string name, std::string table_name);
		
		/**
		 * \fn ~Quest()
		 * \brief Destruktor
		 */
		~Quest();
		
		/**
		 * \fn void setInit(const char* init)
		 * \brief Setzt die Anweisungen fuer die Initialisierung
		 * \param init Zeiger auf Lua Code
		 */
		void setInit(const char* init);
		
		/**
		 * \fn void setDescription(const char*)
		 * \brief Setzt die Beschreibung des Quests
		 * \param descr Zeiger auf den Lua Code, der die Beschreibung zurueck gibt
		 */
		void setDescription(const char* descr);
		
		/**
		 * \fn void init()
		 * \brief Initialisiert das Quest
		 */
		void init();
		
		/**
		 * \fn State getState()
		 * \brief Gibt den Status des Quests aus
		 */
		State getState();
		
		/**
		 * \fn std::string getDescription()
		 * \brief Gibt die Beschreibung des Quests aus
		 */
		std::string getDescription();
	
	private:
		/**
	 	 * \var std::string m_name
		 * \brief Name des Quests
		 */
		std::string m_name;
		
		/**
		 * \var std::string m_table_name
		 * \brief Name der Lua Tabelle in der die Variablen zu dem Quest gespeichert werden
		 */
		std::string m_table_name;
		
		
		/**
		 * \var int m_initialisation
		 * \brief Initialisierung des Quest (Handle auf eine Folge von Lua Anweisungen)
		 */
		int m_initialisation;
		
		
		/**
		 * \var int m_description
		 * \brief Handle auf eine Folge von Lua Anweisungen, die die Beschreibung ausgibt
		 */
		int m_description;
		
};
