#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <sstream>
#include "debug.h"

/**
 * Type der Variable
 **/
typedef std::string VarType;

/**
 * \class Variable
 * \brief Klasse fuer eine Variable im Skriptingsystem
 */
class Variable
{
	public:
		
		
		/**
		 * \fn Variable()
		 * \brief Standardkonstruktor
		 */
		Variable()
		{
			m_data="";
			m_type = "";
		}
		
		/**
		 * \fn Variable(int i)
		 * \brief erzeugt eine Variable vom Typ int und initialisiert sie mit dem Wert i
		 * \param i Initwert
		 */
		Variable(int i);
		
		
		
		/**
		 * \fn Variable(float f)
		 * \brief erzeugt eine Variable vom Typ float und initialisiert sie mit dem Wert f
		 * \param f Initwert
		 */
		Variable(float f);
		
		
		/**
		 * \fn Variable(std::string s)
		 * \brief erzeugt eine Variable vom Typ string und initialisiert sie mit dem Wert s
		 * \param s Initwert
		 */
		Variable(std::string);
		
		/**
		 * \fn setType(VarType type)
		 * \brief Setzt den Typ der Variable
		 * \param type Typ der Variable
		 */
		void setType(VarType type)
		{
			m_type = type;
		}
		
		/**
		 * \fn void setData(int i)
		 * \brief setzt die  Variable
		 * \param i Wert
		 */
		void setData(int i);
		
		/**
		 * \fn void setData(float f)
		 * \brief setzt die  Variable
		 * \param f Wert
		 */
		void setData(float f);
		
		/**
		 * \fn void setData(std::string data)
		 * \brief setzt die Daten
		 * \param data Daten
		 */
		void setData(std::string data)
		{
			m_data = data;
		}
		
		/**
		 * \fn  void operator=(std::string data)
		 * \brief Zuweisungsoperator
		 * \param data zugewiesene Daten
		 */
		void operator=(std::string data)
		{
			m_data = data;
		}
		
		/**
		 * \fn std::string& getData()
		 * \brief Gibt eine Referenz auf die Daten zurueck
		 **/
		std::string& getData()
		{
			return m_data;
		}
	
	private:
		/**
		 * \var std::string m_data
		 * \brief Daten der Variable. Der Typ bestimmt die Interpretation der Daten
		 */
		std::string m_data;	
		
		/**
		 * \var VarType m_type
		 * \brief Typ der Variable
		 */
		VarType m_type;
};

#endif

