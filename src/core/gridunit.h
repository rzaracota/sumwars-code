#ifndef GRIDUNIT_H
#define GRIDUNIT_H

#include "worldobject.h"
#include <list>
#include <map>
#include "dropitem.h"
#include "debug.h"

#define MAX_GRIDUNIT_OBJ 20

/**
 * \struct Gridunit
 * \brief Struktur fuer ein 4x4 Feld der Welt#
 */
struct Gridunit
{
	
	
	private:

		/**
		* \var m_dead[20]
		* \brief Liste der toten Spieler auf dem Feld
		*/
		WorldObject* m_dead[MAX_GRIDUNIT_OBJ];
	
		/**
		* \var m_nr_dead
		* \brief Anzahl der toten Spieler auf dem Feld
		*/
		short m_nr_dead;
	
		/**
		* \var m_fixed[20]
		* \brief Liste der festen Objekte auf dem Feld
		*/
	
		WorldObject* m_fixed[MAX_GRIDUNIT_OBJ];
	
		/**
		* \var m_nr_fixed
		* \brief Anzahl der festen Objekte auf dem Feld
		*/
		short m_nr_fixed;
	
		/**
		* \var m_creature
		* \brief Liste der Kreaturen auf dem Feld
		*/
		WorldObject* m_creature[MAX_GRIDUNIT_OBJ];
	
		/**
		* \var m_nr_creature
		* \brief Anzahl der Kreaturen auf dem Feld
		*/
		short m_nr_creature;



	public:

		/**
	 	 * \fn Gridunit()
		 * \brief Konstruktor
		 */
		Gridunit();

		/**
		 * \fn ~Gridunit()
		 * \brief Destruktor
		 */
		~Gridunit()
		{
		}



		/**
		 * \fn short&  getObjectsNr(WorldObject::Group group)
		 * \brief Gibt die Anzahl der Objekte in einer der Gruppen aus.
		 * \param group Gruppe deren Objekte ausgegeben werden
		 */
		short&  getObjectsNr(WorldObject::Group group);
		
		
		/**
		 * \fn WorldObject::Group getObjectGroup(WorldObject* object)
		 * \brief Gibt aus zu welcher Gruppierung ein Objekt gehoert
		 */
		WorldObject::Group getObjectGroup(WorldObject* object)
		{
			if (object->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_FIXED_OBJECT)
				return WorldObject::FIXED;
			if (object->getState() == WorldObject::STATE_DEAD || object->getState() == WorldObject::STATE_DIEING )
				return WorldObject::DEAD;
			return WorldObject::CREATURE;
			
		}

		/**
		 * \fn WorldObject** getObjects(WorldObject::Group group)
		 * \brief Gibt die Objekte einer Gruppe als Array aus
		 * \param group Gruppe deren Objekte ausgegeben werden
		 * \return Array mit Zeigern auf die Objekte
		 */
		WorldObject** getObjects(WorldObject::Group group)
		{
			if (group == WorldObject::CREATURE) return m_creature;
			else if (group == WorldObject::FIXED) return m_fixed;
			else if (group == WorldObject::DEAD) return m_dead;
			return 0;
		}

		/**
		 * \fn bool insertObject(WorldObject* object)
		 * \brief Fuegt das angegebenen Objekt in das Feld ein
		 * \param object einzufuegendes Objekt
		 * return gibt an, ob die Operation erfolgreich war
		 */
		bool insertObject(WorldObject* object);

		/**
		 * \fn deleteObject(WorldObject* object, short index=-1)
		 * \brief Loescht das angegebene Objekt aus dem Feld
		 * \param object zu loeschendes Objekt
		 * \param index Index des Objektes im Array. Angabe beschleunigt die Funktion, ist aber optional
		 * \return gibt an, ob die Operation erfolgreich war
		 */
		bool deleteObject(WorldObject* object, short index=-1);

		/**
		 * \fn bool moveObject(WorldObject* object,WorldObject::Group group )
		 * \brief verschiebt das Objekt in die angegebene Gruppe
		 * \param object das zu verschiebende Objekt
		 * \param group Gruppe in die das Objekt verschoben wird
		 */
		bool moveObject(WorldObject* object,WorldObject::Group group );
		

};


#endif
