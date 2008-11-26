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

/**
 * \class VariableRef
 * \brief Klasse fuer eine Referenz auf eine Variable
 */
class VariableRef
{	
	public:
		/**
		 * \fn VariableRef()
		 * \brief Standardkonstruktor
		 */
		VariableRef();
		
		/**
		 * \fn VariableRef(int &i)
		 * \brief erzeugt eine Referenz auf ein Integer
		 * \param i integer der referenziert wird
		 */
		VariableRef(int &i);
		
		/**
		 * \fn VariableRef(float &f)
		 * \brief erzeugt eine Referenz auf ein float
		 * \param f float der referenziert wird
		 */
		VariableRef(float &f);
		
		/**
		 * \fn VariableRef(short &s)
		 * \brief erzeugt eine Referenz auf ein short
		 * \param s short der referenziert wird
		 */
		VariableRef(short &s);
		
		/**
		 * \fn VariableRef(char &c)
		 * \brief erzeugt eine Referenz auf ein char
		 * \param c char der referenziert wird
		 */
		VariableRef(char &c);
		
		/**
		 * \fn VariableRef(std::string &s)
		 * \brief erzeugt eine Referenz auf einen string
		 * \param s string der referenziert wird
		 */
		VariableRef(std::string &s);
		
		/**
		 * \fn VariableRef(Variable &v)
		 * \brief erzeugt eine Referenz auf einen Scripting Variable
		 * \param v Variable die referenziert wird
		 */
		VariableRef(Variable &v);
		
		/**
		 * \fn void setVariable(int &i)
		 * \brief setzt die referenzierte Variable
		 * \param i referenzierte Variable
		 */
		void setVariable(int &i);
		
		/**
		 * \fn void setVariable(float &f)
		 * \brief setzt die referenzierte Variable
		 * \param f referenzierte Variable
		 */
		void setVariable(float &f);
		
		/**
		 * \fn void setVariable(short &s)
		 * \brief setzt die referenzierte Variable
		 * \param s referenzierte Variable
		 */
		void setVariable(short &s);
		
		/**
		 * \fn void setVariable(char &c)
		 * \brief setzt die referenzierte Variable
		 * \param c referenzierte Variable
		 */
		void setVariable(char &c);
		
		/**
		 * \fn void setVariable(std::string &s)
		 * \brief setzt die referenzierte Variable
		 * \param s referenzierte Variable
		 */
		void setVariable(std::string &s);
		
		/**
		 * \fn void setVariable(Variable &v)
		 * \brief setzt die referenzierte Variable
		 * \param v referenzierte Variable
		 */
		void setVariable(Variable &v);
		
		/**
		 * \fn bool isValid()
		 * \brief Gibt aus, ob die Referenz an eine Variable gebunden ist
		 */
		bool isValid()
		{
			return (m_data_ptr!=0);
		}
		
		/**
		 * \fn void operator=(std::string& s)
		 * \brief Weist der Variable den Wert zu, der in dem String steht
		 * \param s zugewiesener Wert
		 */
		void operator=(std::string& s);
		
		/**
		 * \fn std::string print()
		 * \brief Gibt die referenzierte Variable als String aus
		 */
		std::string print();
		
		/**
		 * \fn VarType getType()
		 * \brief Gibt den Typ der Referenz aus
		 */
		VarType getType()
		{
			return m_type;
		}
		
	private:
		/**
		 * \fn void* m_data_ptr
		 * \brief Zeiger auf die Daten
		 */
		void* m_data_ptr;
		
		/**
		 * \var VarType m_type
		 * \brief Typ der Variable
		 */
		VarType m_type;
};

#endif

