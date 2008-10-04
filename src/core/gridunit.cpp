#include "gridunit.h"

Gridunit::Gridunit()
{
	DEBUG5("initialising Gridunit at %p",this);
	// Anzahl Objekte aller Sorten auf 0 setzen
	m_nr_dead=0;
	m_nr_fixed=0;
	m_nr_creature=0;

}

bool Gridunit::insertObject(WorldObject* object)
{
	// Ebene in der das Objekt eingeordnet wird
	WorldObject::Group g = getObjectGroup(object);

	// Zeiger auf Array in das eingefuegt wird
	WorldObject ** arr = getObjects(g);

	// Zeiger auf Anzahl der Objekte in der Ebene
	short &  np = getObjectsNr(g);

	if (np== MAX_GRIDUNIT_OBJ)
	{
		// Maximale Anzahl in der Ebene erreicht, Fehler anzeigen
		return false;
	}
	else
	{
		// Element einfuegen, Zahler erhoehen
		arr[np] = object;
		np++;
		DEBUG5("inserted object %p, as number %i in group %i",object,np,g);
	}

    return true;
}

bool  Gridunit::moveObject(WorldObject* object,WorldObject::Group group )
{
	bool ret = deleteObject(object);
	if (ret==false)
		return false;

	// Zeiger auf Array in das eingefuegt wird
	WorldObject ** arr = getObjects(group);

	// Zeiger auf Anzahl der Objekte in der Ebene
	short &  np = getObjectsNr(group);

	if (np== MAX_GRIDUNIT_OBJ)
	{
		// Maximale Anzahl in der Ebene erreicht, Fehler anzeigen
		return false;
	}
	else
	{
		// Element einfuegen, Zahler erhoehen
		arr[np] = object;
		np++;
		DEBUG5("inserted object %p, as number %i in group %i",object,np,group);
	}
    return true;
}

bool Gridunit::deleteObject(WorldObject* object, short index)
{
	// Ebene aus der geloescht werden soll
	WorldObject::Group g = getObjectGroup(object);

	// Zeiger auf Array aus dem geloescht wird
	WorldObject ** arr = getObjects(g);

	// Zeiger auf Anzahl der Objekte in der Ebene
	short &  np = getObjectsNr(g);

	DEBUG5("deleting obj %p from group %i",object,g);
	if (index != -1)
	{
		// Stelle an der geloescht werden soll ist explizit vorgegeben
		if (arr[index]==object)
		{
			// Letztes Objekt an die Stelle des geloeschten kopieren
			arr[index] = arr[m_nr_fixed-1];
			// Anzahl Objekte in dem Array reduzieren
			np--;
			return true;
		}
		else
		{
			// Objekt befindet sich nicht an der Stelle, Fehler ausgeben
//			ERROR("Object not found at index %i",index);

			return false;
		}
	}
	else
	{
		// Suchen an welcher Stelle sich das Objekt befindet
		for (int i=0;i<np;i++)
		{
		DEBUG5(" testing object %i",i);
			if (arr[i]==object)
			{
				// Objekt gefunden
				// Anzahl Objekte in dem Array reduzieren
				np--;

				// Letztes Objekt an die Stelle des geloeschten kopieren
				arr[i] = arr[np];
				return true;
			}
		}
	}

	// Objekt nicht gefunden, Fehler anzeigen
	ERRORMSG("Object %s not found in group %i",object->getNameId().c_str(),g);
	return false;

}


short&  Gridunit::getObjectsNr(WorldObject::Group group)
{
	if (group == WorldObject::CREATURE) return m_nr_creature;
	else if (group == WorldObject::FIXED) return m_nr_fixed;
	else return m_nr_dead;
}


