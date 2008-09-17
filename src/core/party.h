#ifndef PARTY_H
#define PARTY_H

#include "debug.h"
#include "charconv.h"
#include "worldobject.h"

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
	 * \parm id ID der Party
	 */
	Party(){};


	/**
	 * \fn ~Party()
	 * \brief Destruktor
	 */
	virtual ~Party();

	/**
	 * \fn init(char nr_parties, char id)
	 * \brief initialisiert das Objekt
	 * \param nr_parties maximale Anzahl Parties
	 * \param id ID
	 */
	void init(char nr_parties, char id);


	/**
	 * \fn void clear()
	 * \brief Loescht alle aktuellen Mitglieder und Bewerber
	 */
	void clear();


	/**
	 * \fn int* getMembers()
	 * \brief gibt Liste der Mitglieder zurueck
	 */
	int* getMembers()
	{
		return m_members;
	}

	/**
	 * \fn int* getCandidates()
	 * \brief Gibt Liste der Kandidaten zurueck
	 */
	int* getCandidates()
	{
		return m_candidates;
	}

	/**
	 * \fn Relation* getRelations()
	 * \brief Gibt Beziehungen zu den anderen Parties aus
	 */
	WorldObject::Relation* getRelations()
	{
		return m_relations;
	}

	/**
	 * \fn char getId()
	 * \brief Gibt die ID aus
	 * \return ID
	 */
	char getId()
	{
		return m_id;
	}

	/**
	 * \fn char getNrMembers()
	 * \brief Gibt Anzahl der Mitglieder aus
	 */
	char getNrMembers()
	{
		return m_nr_members;
	}

	/**
	 * \fn char getNrCandidates()
	 * \brief Gibt Anzahl der Bewerber aus
	 */
	char getNrCandidates()
	{
		return m_nr_candidates;
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
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	virtual void toString(CharConv* cv);


	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param buf Objekt als String
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	virtual void fromString(CharConv* cv);



	private:

	/**
	 * \var char m_id
	 * \brief ID
	 */
	char m_id;

	/**
	 * \var char m_nr_parties
	 * \brief maximale Anzahl von Parties
	 */
	char m_nr_parties;

	/**
	 * \var int* m_members
	 * \brief Feld mit den IDs der Mitglieder
	 */
	int* m_members;

	/**
	 * \var char m_nr_members
	 * \brief Anzahl der Mitglieder
	 */
	char m_nr_members;

	/**
	 * \var char m_nr_candidates
	 * \brief Anzahl der Bewerber
	 */
	char m_nr_candidates;

	/**
	 * \var int* candidates
	 * \brief Feld mit den IDs der Bewerber
	 */
	int* m_candidates;

	/**
	 * \var Relation* m_relations
	 * \brief Beziehungen zu anderen Parties
	 */
	WorldObject::Relation* m_relations;

};


#endif

