#ifndef GRIDUNIT_H
#define GRIDUNIT_H

#include "serverwobject.h"
#include <list>
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
	ServerWObject* m_dead[MAX_GRIDUNIT_OBJ];

	/**
	 * \var m_nr_dead
	 * \brief Anzahl der toten Spieler auf dem Feld
	 */
	short m_nr_dead;

	/**
	 * \var m_fixed[20]
	 * \brief Liste der festen Objekte auf dem Feld
	 */

	ServerWObject* m_fixed[MAX_GRIDUNIT_OBJ];

	/**
	 * \var m_nr_fixed
	 * \brief Anzahl der festen Objekte auf dem Feld
	 */
	short m_nr_fixed;

	/**
	 * \var m_creature
	 * \brief Liste der Kreaturen auf dem Feld
	 */
	ServerWObject* m_creature[MAX_GRIDUNIT_OBJ];

	/**
	 * \var m_nr_creature
	 * \brief Anzahl der Kreaturen auf dem Feld
	 */
	short m_nr_creature;

	/**
	 * \var list<ServerItem*>* m_items
	 * \brief Liste der Items auf dem Feld
	 */
	list<DropItem*> m_items;


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
		 * \fn short getObjectNr(WorldObject::Geometry::Layer layer)
		 * \brief Gibt die Anzahl der Objekte in einer der Schichten aus.
		 * \param layer Schicht, deren Objektanzahl ausgegeben werden soll. Muss eines der folgenden sein: LAYER_FIXED, LAYER_CREATURE,LAYER_DEAD
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
		 * \fn getObjects(short layer)
		 * \brief Gibt die Objekte einer Schicht als Array aus
		 * \param layer Schicht, deren Objekte ausgegeben werden sollen. Muss eines der folgenden sein: LAYER_FIXED, LAYER_CREATURE,LAYER_DEAD
		 * \return Array mit Zeigern auf die Objekte
		 */
		ServerWObject** getObjects(WorldObject::Group group)
		{
			if (group == WorldObject::CREATURE) return m_creature;
			else if (group == WorldObject::FIXED) return m_fixed;
			else if (group == WorldObject::DEAD) return m_dead;
			return 0;
		}

		/**
		 * \fn bool insertObject(ServerWObject* object)
		 * \brief Fuegt das angegebenen Objekt in das Feld ein
		 * \param object einzufuegendes Objekt
		 * return gibt an, ob die Operation erfolgreich war
		 */
		bool insertObject(ServerWObject* object);

		/**
		 * \fn deleteObject(ServerWObject* object, short index=-1)
		 * \brief Loescht das angegebene Objekt aus dem Feld
		 * \param object zu loeschendes Objekt
		 * \param index Index des Objektes im Array. Angabe beschleunigt die Funktion, ist aber optional
		 * \return gibt an, ob die Operation erfolgreich war
		 */
		bool deleteObject(ServerWObject* object, short index=-1);

		/**
		 * \fn bool moveObject(ServerWObject* object,WorldObject::Group group )
		 * \brief verschiebt das Objekt in die angegebene Gruppe
		 * \param object das zu verschiebende Objekt
		 * \param group Gruppe in die das Objekt verschoben wird
		 */
		bool moveObject(ServerWObject* object,WorldObject::Group group );
		
		/**
		 * \fn void insertItem(DropItem* item)
		 * \brief Fuegt Gegenstand ein
		 * \param item einzufuegender Gegenstand
		 */
		bool insertItem(DropItem* item);

		/**
		 * \fn void deleteItem(short x, short y)
		 * \brief Loescht einen Gegenstand aus dem Feld
		 * \param item zu loeschender Gegenstand
		 */
		bool deleteItem(DropItem* item);

		/**
		 * \fn list<DropItem*>* getItems()
		 * \brief Gibt die Liste der Items aus
		 */
		list<DropItem*>* getItems()
		{
			return &m_items;
		}

};


#endif
