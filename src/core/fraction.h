#include "debug.h"
#include <string>
#include <map>

/**
 * \class Fraction
 * \brief Fraktion, der Spieler und Monster angehoeren koennen
 */
class Fraction
{
	public:
		/**
		 * \enum Relation
		 * \brief Listet die verschiedenen Beziehungen zwischen Fraktionen auf
		 */
		enum Relation
		{
			NEUTRAL = 0,
			ALLIED = 1,
			HOSTILE =2
		};
		
		typedef std::string Type;

		
		/**
		 * \fn Fraction()
		 * \brief Konstruktor
		 */
		Fraction(int id, Type type, bool human)
		{
			m_id = id;
			m_type = type;
		}
		
		
		/**
		 * \fn int getId()
		 * \brief Gibt die ID aus
		 */
		int getId()
		{
			return m_id;
		}
		
		/**
		 * \fn Type getType()
		 * \brief Gibt den Typ aus
		 */
		Type getType()
		{
			return m_type;
		}
		
		/**
		 * \fn Relation getRelation(int id)
		 * \brief Gibt Verhaeltnis zur Fraktion mit der ID id aus
		 * \param id ID einer anderen Fraktion
		 */
		Relation getRelation(int id);
	
		
	private:
		/**
		 * \var int m_id
		 * \brief Nummer der Fraktion
		 */
		int m_id;
		
		/**
		 * \var Type m_type
		 * \brief Type der Fraktion
		 */
		Type m_type;
		
		/**
		 * \var std::string m_name
		 * \brief Name der Fraktion
		 */
		std::string m_name;
		
		/**
		 * \var std::map<int, Relation> m_relations
		 * \brief Gibt Verhaeltnis zu anderen Fraktionen an
		 */
		std::map<int, Relation> m_relations;
		
		/**
		 * \var bool m_human_fraction
		 * \brief Gibt an, ob es sich um eine Fraktion von Spielern handelt
		 */
		bool m_human_fraction;
};
