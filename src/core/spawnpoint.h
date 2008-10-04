#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H
#include <string>

#include "worldobject.h"
#include "random.h"
#include "objectfactory.h"
#include <list>

/**
 * \class Spawnpoint
 * \brief Dieses Objekt stellt einen Ort dar, an dem Monster die Welt betreten
 */
class Spawnpoint : public WorldObject {
/**
	 * Public stuff
 */
	public:
	
	/**
	 * \struct SpawnedMonsterGroup
	 * \brief Struktur fuer eine Gruppe von Monstern gleichen Typs die vom Spawnpoint erzeugt werden kann
	 */
	struct SpawnedMonsterGroup
	{
		/**
		 * \fn SpawnedMonsterGroup(TypeInfo::ObjectSubtype s, float p, int n)
		 * \brief Konstruktor
		 * \param s Subtyp der zu erzeugenden Monster
		 * \param p Wahrscheinlichkeit pro Monster, dass es erzeugt wird
		 * \param n Anzahl der maximal erzeugten Monster
		 */
		SpawnedMonsterGroup(TypeInfo::ObjectSubtype s, float p, int n)
		{
			m_subtype =s;
			m_prob = p;
			m_number = n;
		}
		
		
		/**
		 * \var TypeInfo::ObjectSubtype m_subtype
		 * \brief Subtype der erzeugten Monster
		 */
		TypeInfo::ObjectSubtype m_subtype;
		
		/**
		 * \var float m_prob
		 * \brief Wahrscheinlichkeit mit der ein Monster erzeugt wird
		 */
		float m_prob;
		
		/**
		 * \var int m_number
		 * \brief Anzahl der Monster die maximal erzeugt werden. Jedes Monster wird gemaess der Wahrscheinlichkeit prob erzeugt. Im Erwartungswert also prob * number Monster
		 */
		int m_number;
		
		/**
		 * \fn void operator=(SpawnedMonsterGroup & other)
		 * \brief Zuweisungsoperator
		 */
		void operator=(SpawnedMonsterGroup & other)
		{
			m_subtype = other.m_subtype;
			m_prob = other.m_prob;
			m_number = other.m_number;
		}
	};
	
	/**
	 * \fn Spawnpoint(World* world, unsigned int id)
	 * \brief Konstruktor
	 */
	Spawnpoint(World* world, int id);
	
	/**
	 * \fn virtual bool init()
	 * \brief Initialisierung von internen Daten
	 * \return bool Gibt an, ob die Initialisierung erfolgreich war
	 */
	virtual bool init ();
	
	/**
	 * \fn virtual bool  update (float time)
	 * \brief aktualisiert das Objekt nachdem time Millisekunden vergangen sind
	 * \param time vergangene Zeit in Millisekunden
	 */
	virtual bool  update (float time);
	
	/**
	 * \fn void addMonsterGroup(SpawnedMonsterGroup& mgroup)
	 * \brief Fuegt zum Spawnpunkt eine Monstergruppe hinzu
	 */
	void addMonsterGroup(SpawnedMonsterGroup& mgroup);
	
	/**
	 * \fn void setRespawnTime(float t)
	 * \brief Setzt die Zeit bis zum Neuerscheinen der Monster
	 */
	void setRespawnTime(float t);


	private:
		/**
	 * \var std::list<SpawnedMonsterGroup> m_spawned_groups
		 * \brief Liste der verschiedenen Monstergruppen die der Spawnpoint erzeugen kann
		 */
		std::list<SpawnedMonsterGroup> m_spawned_groups;
		
		/**
		 * \var float m_time
		 * \brief interner Timer, nach Ablaufen werden die Monster neu erzeugt
		 */
		float m_time;
		
		/**
		 * \var float m_respawn_time
		 * \brief Zeitdauer bis zum neu Spawnen der Monster
		 */
		float m_respawn_time;
	
};
#endif

