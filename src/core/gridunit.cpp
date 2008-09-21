#include "gridunit.h"

Gridunit::Gridunit()
	: m_items()
{
	DEBUG5("initialising Gridunit at %p",this);
	// Anzahl Objekte aller Sorten auf 0 setzen
	m_nr_dead=0;
	m_nr_fixed=0;
	m_nr_creature=0;

}

bool Gridunit::insertObject(ServerWObject* object)
{
	// Ebene in der das Objekt eingeordnet wird
	WorldObject::Group g = getObjectGroup(object);

	// Zeiger auf Array in das eingefuegt wird
	ServerWObject ** arr = getObjects(g);
	
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
	
	/*
	if ((layer & WorldObject::Geometry::LAYER_FIXED) == layer)
	{
		// Objekt wird in Ebene FIXED eingeordnet
		DEBUG5("inserting into fixed layer");

		i = m_nr_fixed;
		if (i== MAX_GRIDUNIT_OBJ)
		{
			// Maximale Anzahl in der Ebene erreicht, Fehler anzeigen
			return false;
		}
		else
		{
			// Element einfuegen, Zahler erhoehen
			m_fixed[i] = object;
			m_nr_fixed++;
		}
	}
	else if ((layer & WorldObject::Geometry::LAYER_DEAD) == layer)
	{
		DEBUG5("inserting into dead layer");
		// Objekt wird in Ebene DEAD eingeordnet
		i = m_nr_dead;
		if (i== MAX_GRIDUNIT_OBJ)
		{
			// Maximale Anzahl in der Ebene erreicht, Fehler anzeigen
			return false;
		}
		else
		{
			// Element einfuegen, Zahler erhoehen
			m_dead[i] = object;
			m_nr_dead++;
		}
	}
	else if (layer == WorldObject::Geometry::LAYER_SPECIAL)
	{
		return true;
	}
	else
	{
		DEBUG5("inserting into creature layer");
		// Objekt wird in Ebene CREATURE eingeordnet
		i = m_nr_creature;
		if (i== MAX_GRIDUNIT_OBJ)
		{
			// Maximale Anzahl in der Ebene erreicht, Fehler anzeigen
			return false;
		}
		else
		{
			// Maximale Anzahl in der Ebene erreicht, Fehler anzeigen
			m_creature[i] = object;
			m_nr_creature++;
		}
	}
	*/
}

bool  Gridunit::moveObject(ServerWObject* object,WorldObject::Group group )
{
	bool ret = deleteObject(object);
	if (ret==false)
		return false;
	
	// Zeiger auf Array in das eingefuegt wird
	ServerWObject ** arr = getObjects(group);
	
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
}

bool Gridunit::deleteObject(ServerWObject* object, short index)
{
	// Ebene aus der geloescht werden soll
	WorldObject::Group g = getObjectGroup(object);
	
	// Zeiger auf Array aus dem geloescht wird
	ServerWObject ** arr = getObjects(g);
	
	// Zeiger auf Anzahl der Objekte in der Ebene
	short &  np = getObjectsNr(g);
	/*
	if ((layer & WorldObject::Geometry::LAYER_FIXED )== layer)
	{
		// Loeschen aus FIXED Ebene
		arr = m_fixed;
		np = &m_nr_fixed;
		DEBUG5("deleting from fixed layer");


	}
	else if ((layer & WorldObject::Geometry::LAYER_DEAD )== layer)
	{
		// Loeschen aus DEAD Ebene
		arr = m_dead;
		np = &m_nr_dead;
		DEBUG5("deleting from dead layer");

	}
	else
	{
		// Loeschen aus CREATURE ebene
		arr = m_creature;
		np = &m_nr_creature;
		DEBUG5("deleting from creature layer");

	}
	*/
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

bool Gridunit::insertItem(DropItem* item)
{
	m_items.push_back(item);
	return true;
}

bool Gridunit::deleteItem(DropItem* item)
{
	list<DropItem*>::iterator it;
	for (it = m_items.begin();it != m_items.end();++it)
	{
		if ((*it)==item)
		{
			m_items.erase(it);
			return true;
		}
	}
	return false;
}

short&  Gridunit::getObjectsNr(WorldObject::Group group)
{
	if (group == WorldObject::CREATURE) return m_nr_creature;
	else if (group == WorldObject::FIXED) return m_nr_fixed;
	else return m_nr_dead;
}


