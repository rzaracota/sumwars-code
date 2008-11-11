#ifndef PARTY_H
#define PARTY_H

#include "debug.h"
#include "charconv.h"
#include "worldobject.h"
#include <set>
#include <map>

/**
 * \class Party
 * \brief Eine Party ist eine Gruppe von Lebewesen welche kooperieren
 */
class Party
{

	public:



	/**
	 * \fn Party()
	 * \brief Konstruktor
	 */
	Party(){};


	/**
	 * \fn ~Party()
	 * \brief Destruktor
	 */
	virtual ~Party();

	/**
	 * \fn init( int id)
	 * \brief initialisiert das Objekt
	 * \param id ID
	 */
	void init( int id);


	/**
	 * \fn void clear()
	 * \brief Loescht alle aktuellen Mitglieder und Bewerber
	 */
	void clear();


	/**
	 * \fn std::set<int>& getMembers()
	 * \brief gibt Liste der Mitglieder zurueck
	 */
	std::set<int>& getMembers()
	{
		return m_members;
	}

	/**
	 * \fn std::set<int>&  getCandidates()
	 * \brief Gibt Liste der Kandidaten zurueck
	 */
	std::set<int>& getCandidates()
	{
		return m_candidates;
	}

	/**
	 * \fn std::map<int, WorldObject::Relation>& getRelations()
	 * \brief Gibt Beziehungen zu den anderen Parties aus
	 */
	std::map<int, WorldObject::Relation>& getRelations()
	{
		return m_relations;
	}

	/**
	 * \fn int getId()
	 * \brief Gibt die ID aus
	 * \return ID
	 */
	int getId()
	{
		return m_id;
	}

	/**
	 * \fn int getNrMembers()
	 * \brief Gibt Anzahl der Mitglieder aus
	 */
	int getNrMembers()
	{
		return m_members.size();
	}

	/**
	 * \fn int getNrCandidates()
	 * \brief Gibt Anzahl der Bewerber aus
	 */
	int getNrCandidates()
	{
		return m_candidates.size();
	}

	/**
	 * \fn void addMember(int id)
	 * \brief Fuegt Mitglied hinzu
	 * \param id ID des aufgenommenen Mitglieds
	 */
	void addMember(int id);

	/**
	 * \fn void removeMember(int id)
	 * \brief Entfernt Mitglied
	 * \param id ID des entfernten Mitglieds
	 */
	void removeMember(int id);

	/**
	 * \fn void addCandidate(int id)
	 * \brief Fuegt einen Bewerber hinzu
	 * \param id ID des neuen Bewerbers
	 */
	void addCandidate(int id);

	/**
	 * \fn void acceptCandidate(int id)
	 * \brief Nimmt Bewerber auf
	 * \param id ID des aufgenommenen Bewerbers
	 */
	void acceptCandidate(int id);

	/**
	 * \fn void removeCandidate(int id)
	 * \brief Entfernt einen Bewerber
	 * \param id ID des Bewerbers
	 */
	void removeCandidate(int id);

	
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	 */
	virtual void toString(CharConv* cv);


	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	virtual void fromString(CharConv* cv);



	private:

	/**
	 * \var int m_id
	 * \brief ID
	 */
	int m_id;
	
	/**
	 * \var int m_leader_id
	 * \brief ID des Party Leiters
	 */
	int m_leader_id;

	/**
	 * \var std::set<int> m_members
	 * \brief Menge der IDs der Mitglieder
	 */
	std::set<int> m_members;

	/**
	 * \var std::set<int> m_candidates
	 * \brief Menge der IDs der Bewerber
	 */
	std::set<int> m_candidates;

	/**
	 * \var std::map<int, WorldObject::Relation> m_relations
	 * \brief Beziehungen zu anderen Parties
	 */
	std::map<int, WorldObject::Relation> m_relations;

};


#endif

