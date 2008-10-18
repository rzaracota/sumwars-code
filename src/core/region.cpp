#include "region.h"
#include "world.h"

void RegionData::addObjectGroupTemplate(ObjectGroupTemplateName group_name, int prio, int number,float probability)
{
	ObjectGroupTemplateSet newgroup;
	newgroup.m_group_name = group_name;
	newgroup.m_probability = probability;
	newgroup.m_number = number;
	newgroup.m_probability = probability;
	
	
}


Region::Region(short dimx, short dimy, short id, World* world)
{
	DEBUG5("creating region");
	m_data_grid = new Matrix2d<Gridunit>(dimx,dimy);
	m_dimx = dimx;
	m_dimy = dimy;

	m_height = new Matrix2d<float>(dimx,dimy);
	m_height->clear();
	
	m_tiles = new Matrix2d<char>(dimx*2,dimy*2);
	m_tiles->clear();

	// Binärbaum fuer WorldObjects anlegen
	m_objects = new WorldObjectMap;
	m_static_objects = new WorldObjectMap;

	m_players = new WorldObjectMap;

	// Baum fuer Projektile anlegen
	m_projectiles = new ProjectileMap;

	// Liste der Gegenstaende
	m_drop_items = new DropItemMap;
	m_drop_item_locations = new DropItemMap;

	m_id = id;

	m_events = new EventList;

	
	m_world = world;
}

Region::~Region()
{
	WorldObjectMap::iterator i;
	for (i=m_objects->begin(); i!=m_objects->end();i++)
	{
		delete i->second;
	}

	for (i=m_static_objects->begin(); i!=m_static_objects->end();i++)
	{
		delete i->second;
	}

	ProjectileMap::iterator j;
	for (j =  m_projectiles->begin(); j != m_projectiles->end(); j++)
	{
		delete (j->second);
	}


	DropItemMap::iterator k;
			for (k =  m_drop_items->begin(); k != m_drop_items->end(); k++)
	{
		delete k->second;
	}

	delete m_objects;
	delete m_static_objects;
	delete m_players;
	delete m_projectiles;

	delete m_data_grid;
	delete m_tiles;
	delete m_drop_items;
	delete m_drop_item_locations;

	delete m_events;

}

WorldObject* Region::getObject ( int id)
{
	WorldObjectMap::iterator iter;

	// Objekt im Binärbaum suchen
	iter = m_objects->find(id);

	// Testen ob ein Objekt gefunden wurde
	if (iter == m_objects->end())
	{
		// keins gefunden, NULL ausgeben
		return 0;
	}
	else
	{
		// Zeiger auf Objekt ausgeben
		return iter->second;
	}
}

Projectile* Region::getProjectile(int id)
{
	ProjectileMap::iterator iter;

	// Objekt im Binärbaum suchen
	iter = m_projectiles->find(id);

	// Testen ob ein Objekt gefunden wurde
	if (iter == m_projectiles->end())
	{
		// keins gefunden, NULL ausgeben
		return 0;
	}
	else
	{
		// Zeiger auf Objekt ausgeben
		return iter->second;
	}
}

bool Region::getFreePlace(Shape* shape, short layer, Vector & pos)
{
	// Menge der bereits getesteten Felder
	std::set<int> tfields;

	// Schlange mit zu testenden Feldern
	std::queue<int> fields;

	float c = 1.1;
	// Position in 0.5 x 0.5 Feldern
	int sx = (int) (pos.m_x/c);
	int sy = (int) (pos.m_y/c);
	int i;
	fields.push(sx*10000+sy);

	// Kopie der Form
	Shape s;
	memcpy(&s,shape,sizeof(Shape));

	WorldObjectList res;
	res.clear();

	// eine Stelle suchen an der das Objekt passt
	while (!fields.empty())
	{
		// erstes Feld aus der Queue nehmen
		i = fields.front();
		sx = i /10000;
		sy = i %10000;
		fields.pop();

		DEBUG5("testing field %f %f",sx*c,sy*c);

		// Testen ob dort keine festen Objekte sind
		s.m_center = Vector(sx*c,sy*c);

		res.clear();
		getObjectsInShape(&s,&res,layer,WorldObject::FIXED,0,true);

		if (!res.empty())
		{
			// Objekt im Weg
			res.clear();
			continue;
		}

		DEBUG5("no fixed obstacle");
		// Testen, ob dort nicht gerade eine Kreatur steht
		getObjectsInShape(&s,&res,layer,WorldObject::CREATURE,0,true);
		if (res.empty())
		{
			DEBUG5("field is free");
			// Stelle ist frei
			pos = s.m_center;

			return true;

		}
		else
		{
			WorldObject* obs = *(res.begin());
			DEBUG5("obstacle is %s at %f %f",obs->getNameId().c_str(), obs->getShape()->m_center.m_x,obs->getShape()->m_center.m_y);
			// Stelle ist besetzt
			tfields.insert(i);

			// Nachbarfelder in die Liste der zu untersuchenden Felder einfuegen
			if (sx>0)
			{
				i = (sx-1)*10000+sy;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sy>0)
			{
				i = (sx)*10000+sy-1;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sx<m_dimx*4-1)
			{
				i = (sx+1)*10000+sy;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sy<m_dimy*4-1)
			{
				i = (sx)*10000+sy+1;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

		}

	}

	// keine Stelle gefunden um das Item zu droppen
	return false;

}

bool  Region::addObjectsInShapeFromGridunit(Shape* shape, Gridunit* gu, WorldObjectList* result, short layer, short group,WorldObject* omit, bool empty_test )
{
	WorldObject* wo=0;
	WorldObject** arr=0;
	Shape* s=0;
	
	
	arr = gu->getObjects((WorldObject::Group) group);

	int n = gu->getObjectsNr((WorldObject::Group) group);

	DEBUG5("%i objects in layer %i, group %i",n,layer,group);
	
	// Schleife ueber alle Objekte
	// geprueft wird: Ebene, Schnitt mit der Flaeche
	for (int k=0;k< n;k++)
	{

		wo = arr[k];
		
		s = wo->getShape();

		if (wo->getLayer() & layer)
		{
			if ((shape ==0) || shape->intersects(*s))
			{
				if (wo != omit)
				{
					result->push_back(wo);

					if (empty_test)
						return true;
				}
			}
		}
	}
		
	return true;
}

bool Region::addObjectsOnLineFromGridunit(Line& line, Gridunit* gu, WorldObjectList* result, short layer, short group ,WorldObject* omit, bool empty_test )
{
	WorldObject* wo=0;
	WorldObject** arr=0;
	Shape* s=0;
	short groups[3] = {WorldObject::CREATURE, group == WorldObject::FIXED, group == WorldObject::DEAD};
	
	
	arr = gu->getObjects((WorldObject::Group) group);

	int n = gu->getObjectsNr((WorldObject::Group) group);

	DEBUG5("%i objects in layer %i, group %i",n,layer,group);
	// Schleife ueber alle Objekte
	// geprueft wird: Ebene, Schnitt mit der Linie
	for (int k=0;k< n;k++)
	{

		wo = arr[k];
		DEBUG5("testing %s %p",wo->getNameId().c_str(),wo->getNameId().c_str())
		DEBUG5("testing %p",wo);
		s = wo->getShape();
		
		if (wo->getLayer() & layer)
		{
			if (s->intersects(line))
			{
				if (wo != omit)
				{
					result->push_back(wo);

					if (empty_test)
						return true;
				}
			}
		}
	}
		
	return true;
}


bool Region::getObjectsInShape( Shape* shape,  WorldObjectList* result,short layer, short group, WorldObject* omit, bool empty_test)
{

	DEBUG5("shape %f %f %f",shape->m_center.m_x,shape->m_center.m_y,shape->m_radius);

	 // Wenn der Resultat Zeiger 0 ist: Fehler ausgeben
	if (result == 0)
	{
		ERRORMSG("no result list given");
		return false;
	}

	if (shape == 0)
	{
		ERRORMSG("no shape given");
		return false;
	}


	DEBUG5("layer %i group %i",layer,group);


	if (group == WorldObject::PLAYER)
	{
		// Wenn nur nach Spielern gesucht ist, dann nur die Liste der Spieler durchsuchen
		
		// TODO: einbauen
		
	}
	else
	{
		int i,j;
		Gridunit* gu=0;
		// durchmustere alle 4*4 Felder die von dem Suchrechteck plus 4 Felder bedeckt werden
		float xe,ye;
		
		// Maximale Ausdehnung der Flaeche in Richtung der Koordinatenaxen
		Vector ext = shape->getAxisExtent();
		Vector d1 = shape->m_center +ext;
		Vector d2 = shape->m_center -ext;
			

		int xmin = (int) floor(0.25*(d2.m_x-4));
		int ymin = (int) floor(0.25*(d2.m_y-4));
		int xmax = (int) floor(0.25*(d1.m_x+4));
		int ymax = (int) floor(0.25*(d1.m_y+4));
		DEBUG5("searching square (%i %i) (%i %i)",xmin,ymin,xmax,ymax);
		// Pruefen ob die Suchanfrage nicht aus der Region herauslaeuft
		bool ret = false;
		int is = std::max (xmin,0);
		int ie = std::min(xmax,m_dimx-1);
		int js = std::max(ymin,0);
		int je = std::min(ymax,m_dimy-1);

		// Alle 4x4 Felder durchmustern
		for (i = is;i<=ie;i++)
		{
			for (j=js;j<=je;j++)
			{

				DEBUG5("searching in Grid Tile %i %i",i,j);
				// Durchmustern der Listen im 4x4-Feld
				gu = &(*m_data_grid)[i][j];

				// Totenebene
				if (group & WorldObject::DEAD)
				{
					DEBUG5("searching dead layer");

					ret =  addObjectsInShapeFromGridunit(shape, gu, result, layer,WorldObject::DEAD, omit, empty_test);
					if (!result->empty() && empty_test)
						return true;

				}

				// feste Objekte
				if (group & WorldObject::FIXED )
				{
					DEBUG5("searching fixed layer");

					ret =  addObjectsInShapeFromGridunit(shape, gu, result, layer,WorldObject::FIXED, omit, empty_test);
					if (!result->empty() && empty_test)
						return true;

				}

				// lebende Objekte
				if (group & WorldObject::CREATURE)
				{

					ret =  addObjectsInShapeFromGridunit(shape, gu, result, layer,WorldObject::CREATURE, omit, empty_test);
					if (!result->empty() && empty_test)
						return true;

				}

			}

		}
	}
	return true;
}


WorldObject* Region::getObjectAt(Vector pos, short layer, short group)
{
	Shape s;
	s.m_type = Shape::CIRCLE;
	s.m_radius =0;
	s.m_center = pos;

	WorldObjectList l;
	l.clear();
	getObjectsInShape ( &s, &l,layer,group );
	if (l.empty())
	{
		return 0;
	}
	else
	{
		return *(l.begin());
	}
}



void Region::getObjectsOnLine(Line& line,  WorldObjectList* result,short layer, short group , WorldObject* omit)
{
	Gridunit* gu=0;

	int xmin = (short) floor(0.25*(std::min(line.m_start.m_x,line.m_end.m_x)-4));
	int ymin = (short) floor(0.25*(std::min(line.m_start.m_y,line.m_end.m_y)-4));
	int xmax = (short) floor(0.25*(std::max(line.m_start.m_x,line.m_end.m_x)+4));
	int ymax = (short) floor(0.25*(std::max(line.m_start.m_y,line.m_end.m_y)+4));
	int i,j;
	float d;
	
	// Richtung der Linie
	Vector dir = line.getDirection();
	dir.normalize();
	Vector p;

	for (i = std::max (xmin,0);i<=std::min(xmax,m_dimx-1);i++)
	{
		for (j=std::max(ymin,0);j<=std::min(ymax,m_dimy-1);j++)
		{
			DEBUG5("searching in Grid Tile %i %i",i,j);

			// Herausfiltern jener Felder die zu weit von der Linie entfernt sind
			p.m_x = i*4.0+2;
			p.m_y = j*4.0+2;
			p -= line.m_start;

			p.normalPartTo(dir);

			d = p.getLength();
			if (d>32)
			{
				DEBUG5("aborted");
				continue;
			}

			// Durchmustern der Listen im 4x4-Feld
			gu = &(*m_data_grid)[i][j];

			// Totenebene
			if (group & WorldObject::DEAD)
			{
				DEBUG5("searching dead layer");

				addObjectsOnLineFromGridunit(line, gu, result, layer,WorldObject::DEAD, omit);
			}

				// feste Objekte
			if (group & WorldObject::FIXED )
			{
				DEBUG5("searching fixed layer");


				addObjectsOnLineFromGridunit(line, gu, result, layer,WorldObject::FIXED, omit);
			}

				// lebende Objekte
			if (group & WorldObject::CREATURE)
			{


				addObjectsOnLineFromGridunit(line, gu, result, layer,WorldObject::CREATURE, omit);
			}


		}

	}
}


bool Region::insertObject (WorldObject* object, Vector pos, float angle, bool collision_test)
{
	bool result = true;


	object->getGridLocation()->m_region = m_id;

	 // Einfügen in den Binärbaum
	if (object->getState() != WorldObject::STATE_STATIC)
	{
		result &= (m_objects->insert(std::make_pair(object->getId(),object))).second;
	}
	else
	{
		result &= (m_static_objects->insert(std::make_pair(object->getId(),object))).second;
	}


	 // Wenn das Element bereits existiert ist die Antwort false
	if (result==false)
	{
		return result;
	}

	if (object->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
	{
		DEBUG5("player entered Region");
		result &= (m_players->insert(std::make_pair(object->getId(),object))).second;

	}
	else
	{
		// fuer Nicht Spieler Event erzeugen, dass das Objekt erschaffen wurde
		Event event;
		event.m_type = Event::OBJECT_CREATED;
		event.m_id = object->getId();
		insertEvent(event);
	}

	object->getShape()->m_angle = angle;
	
	// Test auf Kollisionen und eventuelle Verschiebung
	if (collision_test)
	{
		getFreePlace(object->getShape(), object->getLayer(), pos);
	}
	
	 // Koordinaten setzen
	object->getShape()->m_center = pos;

	 // Einfuegen in das Grid
	int x_g = (int) floor(0.25*pos.m_x);
	int y_g = (int) floor(0.25*pos.m_y);
	object->getGridLocation()->m_grid_x = x_g;
	object->getGridLocation()->m_grid_y = y_g;


	 // Testen ob das Objekt in der Region liegt
	if (x_g<0 || y_g<0 || x_g>=m_dimx || y_g>=m_dimy)
	{
		 // TODO
		 // Region neu setzen
	}
	else
	{
		if (object->getLayer() != WorldObject::LAYER_SPECIAL)
		{
			Gridunit *gu = (m_data_grid->ind(x_g,y_g));

			result = gu->insertObject(object);
		}

	}

	return result;
}

int Region::createObject(WorldObject::TypeInfo::ObjectType type, ObjectTemplateType generictype, Vector pos, float angle, bool collision_test)
{
	// Umgebung erfahren
	EnvironmentName env = getEnvironment(pos);
	
	// genauen Subtyp ermitteln
	WorldObject::TypeInfo::ObjectSubtype subtype = ObjectFactory::getObjectType(generictype, env);
	if (subtype == "")
	{
		return 0;
	}
	
	// Objekt erstellen
	WorldObject* object = ObjectFactory::createObject(type,subtype);
	if (object ==0)
	{
		return 0;
	}
	
	// Objekt einfuegen
	insertObject(object,pos,angle,false);
	
	return object->getId();
	
}


int Region::createObjectGroup(ObjectGroupTemplateName templname, Vector position, float angle)
{
	// Template aus der Datenbank heraussuchen
	std::map<ObjectGroupTemplateName, ObjectGroupTemplate*>::iterator it;
	
	ObjectGroupTemplate* templ = ObjectFactory::getObjectGroupTemplate(templname);
	
	if (templ != 0)
	{
		// Template wurde gefunden
		DEBUG("found template");
		// Objekte platzieren
		std::list<ObjectGroupTemplate::GroupObject>::iterator gt;
		Vector pos;
		
		for (gt = templ->getObjects()->begin(); gt != templ->getObjects()->end(); ++gt)
		{
			if (Random::random() < gt->m_probability)
			{
				
				pos = gt->m_center;
				pos.rotate(angle);
				pos += position;
				
				DEBUG("inserting object %s at %f %f",gt->m_type.c_str(),pos.m_x, pos.m_y);
				
				createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT,gt->m_type, pos, angle+gt->m_angle);
			}
		}
		
		// Orte einfuegen
	}
	else
	{
		ERRORMSG("object group template %s not found",templname.c_str());
	}
}


bool  Region::insertProjectile(Projectile* object, Vector pos)
{
	m_projectiles->insert(std::make_pair(object->getId(),object));
	object->getShape()->m_center = pos;
	object->setRegion( m_id);

	// Event erzeugen: neues Projektil in der Region
	Event event;
	event.m_type = Event::PROJECTILE_CREATED;
	event.m_id = object->getId();
	insertEvent(event);

	return true;
}

bool  Region::deleteObject (WorldObject* object)
{
	bool result = true;

	 // aus dem Grid loeschen
	int x = object->getGridLocation()->m_grid_x;
	int y = object->getGridLocation()->m_grid_y;
	DEBUG5("deleting object in grid tile %i %i",x,y);

	if (object->getLayer() != WorldObject::LAYER_SPECIAL)
	{
		Gridunit *gu = (m_data_grid->ind(x,y));
		result = gu->deleteObject(object);
	}

	if (result==false)
	{
		return result;
	}

	 // Aus dem Binärbaum loeschen
	if (object->getState() != WorldObject::STATE_STATIC)
	{
		m_objects->erase(object->getId());
	}
	else
	{
		m_static_objects->erase(object->getId());
	}

	if (object->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
	{
		DEBUG("Player deleted");
		m_players->erase(object->getId());

	}

	return result;
}

bool Region::moveObject(WorldObject* object, Vector pos)
{
	bool result = true;
	// Wenn NULL Zeiger übergeben -> Fehler anzeigen
	if (object == 0)
		return false;

	// Testen ob das Objekt innerhalb des 4*4 Grid in ein neues Feld kommt
	int x_old = object->getGridLocation()->m_grid_x;
	int y_old = object->getGridLocation()->m_grid_y;
	int x_new = (int) floor(0.25*pos.m_x);
	int y_new = (int) floor(0.25*pos.m_y);

	if (x_old == x_new && y_old == y_new)
	{
		object->getShape()->m_center = pos;
		return true;
	}
	// Testen ob das Objekt in der Region liegt
	if (x_new<0 || y_new<0 || x_new>=m_dimx || y_new>=m_dimy)
	{
		 // TODO
		 // Region neu setzen
	}
	else
	{
		Gridunit *gu = &(*m_data_grid)[x_old][y_old];
		result =gu->deleteObject(object);
		gu = &(*m_data_grid)[x_new][y_new];
		result = gu->insertObject(object);

		object->getGridLocation()->m_grid_x=x_new;
		object->getGridLocation()->m_grid_y=y_new;

		object->getShape()->m_center = pos;
	}

	return result;
}


bool Region::changeObjectGroup(WorldObject* object,WorldObject::Group group )
{
	bool result = true;

	int x = object->getGridLocation()->m_grid_x;
	int y = object->getGridLocation()->m_grid_y;
	DEBUG5("changing object in grid tile %i %i",x,y);

	Gridunit *gu = (m_data_grid->ind(x,y));
	result = gu->moveObject(object,group);


	return result;

}

void Region::deleteProjectile(Projectile* proj)
{
	int id = proj->getId();

	if (m_projectiles->count(id)!=0)
	{
		m_projectiles->erase(m_projectiles->find(id));
	}
}

void Region::update(float time)
{

	DEBUG5("\nUpdate aller WeltObjekte starten\n");
	//DEBUG("m_players %p",m_players);
	// Iterator zum durchmustern einer solchen Liste
	WorldObjectMap::iterator iter;
	WorldObject* object;
	ProjectileMap::iterator it3;


	// alle Eventsmasken loeschen
	for (iter =m_objects->begin(); iter!=m_objects->end(); ++iter)
	{
		iter->second->clearEventMask();
	}

	for (it3 = m_projectiles->begin(); it3 !=m_projectiles->end();++it3)
	{
		it3->second->clearEventMask();
	}

	// Durchmustern aller WorldObjects
	for (iter =m_objects->begin(); iter!=m_objects->end(); )
	{
		object = iter->second;
		
		if (object->getDestroyed()==true)
		{
			// Objekte selbststaendig loeschen darf nur der Server
			if (m_world->isServer())
			{
				DEBUG5("Objekt gelöscht: %i \n",object->getId());
				Event event;
				event.m_type = Event::OBJECT_DESTROYED;
				event.m_id = object->getId();
				insertEvent(event);

				++iter;
				object->destroy();
				deleteObject(object);
				delete object;
				continue;
			}
			else
			{
				++iter;
			}
		}
		else
		{
			// Polymorpher Funktionsaufruf
			object->update(time);
			++iter;

		}

	}
	DEBUG5("Update aller WeltObjekte abgeschlossen\n\n");

	// alle Projektile updaten
	Projectile* pr =0;

	for (it3 = m_projectiles->begin(); it3 !=m_projectiles->end();)
	{
		pr = (it3->second);
		if (pr->getState() == Projectile::DESTROYED)
		{
			// Projektile selbststaendig loeschen darf nur der Server
			if (m_world->isServer())
			{
				Event event;
				event.m_type = Event::PROJECTILE_DESTROYED;
				event.m_id = pr->getId();
				insertEvent(event);

				DEBUG5("deleting projectile %p",pr);
				m_projectiles->erase(it3++);
				delete pr;
				DEBUG5("loesche projektil");
			}
			else
			{
			 	++it3;
			}

		}
		else
		{
			pr->update(time);
			++it3;
		}
	}
	DEBUG5("update projektile abgeschlossen");

	if (m_world->isServer())
	{
		// Events fuer geaenderte Objekte / Projektile erzeugen
		for (iter =m_objects->begin(); iter!=m_objects->end(); ++iter)
		{
			object = iter->second;

			// Events durch Spieler werden global behandelt, daher hier nicht beruecksichtigen
			if (object->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
			{
				continue;
			}

			if (object->getEventMask() !=0)
			{
				Event event;
				event.m_type = Event::OBJECT_STAT_CHANGED;
				event.m_data = object->getEventMask();
				event.m_id = object->getId();
				insertEvent(event);
			}
		}

		for (it3 = m_projectiles->begin(); it3 !=m_projectiles->end();++it3)
		{
			pr = (it3->second);
			if (pr->getEventMask() !=0)
			{
				Event event;
				event.m_type = Event::PROJECTILE_STAT_CHANGED;
				event.m_data = pr->getEventMask();
				event.m_id = pr->getId();
				insertEvent(event);
			}
		}
	}
}

void Region::getRegionData(CharConv* cv)
{
	// Dimension des Feldes angeben
	cv->toBuffer((short) m_dimx);
	cv->toBuffer((short) m_dimy);

	// Tiles eintragen
	int i,j;
	for (i =0;i<m_dimx*2;i++)
	{
		for (j=0;j<m_dimy*2;j++)
		{
			cv->toBuffer(*(m_tiles->ind(i,j)));
		}
	}

	// Anzahl der statischen Objekte eintragen
	DEBUG("static objects: %i",m_static_objects->size());
	cv->toBuffer<short>((short) m_static_objects->size());

	// statische Objekte in den Puffer eintragen
	WorldObjectMap::iterator it;
	for (it = m_static_objects->begin();it!=m_static_objects->end();++it)
	{
		(it->second)->toString(cv);
		DEBUG5("static object: %s",(it->second)->getNameId().c_str());
	}


	// Anzahl der nicht  statischen Objekte eintragen
	DEBUG("nonstatic objects: %i",m_objects->size());
	cv->toBuffer<short>((short) m_objects->size());

	// nicht statische Objekte in den Puffer eintragen
	WorldObjectMap::iterator jt;
	for (jt = m_objects->begin();jt!=m_objects->end();++jt)
	{
		DEBUG5("write offset: %i",cv->getBitStream()->GetNumberOfBitsUsed());
		(jt->second)->toString(cv);

		DEBUG5("object: %s",(jt->second)->getNameId().c_str());
	}

	// Anzahl der Projektile eintragen
	cv->toBuffer<short>((short) m_projectiles->size());
	DEBUG("projectiles: %i",m_projectiles->size());

	// Projektile in den Puffer eintragen
	ProjectileMap::iterator kt;
	for (kt = m_projectiles->begin(); kt != m_projectiles->end(); ++kt)
	{
		kt->second->toString(cv);
	}

	cv->toBuffer<short>((short) m_drop_items->size());
	DEBUG("dropped items: %i",m_drop_items->size());

	//  Items in den Puffer eintragen
	DropItemMap::iterator lt;
	for (lt = m_drop_items->begin(); lt != m_drop_items->end(); ++lt)
	{
		cv->toBuffer(lt->second->m_x);
		cv->toBuffer(lt->second->m_y);
		lt->second->m_item->toString(cv);
	}
}


void Region::createObjectFromString(CharConv* cv, WorldObjectMap* players)
{
	char type;
	char subt[11];
	subt[10] ='\0';
	int id;

	WorldObject* obj;
	float x,y;


	DEBUG5("read offset: %i",cv->getBitStream()->GetReadOffset());

	cv->fromBuffer(type);
	cv->fromBuffer(subt,10);
	cv->fromBuffer(id);

	DEBUG5("object %s id %i",subt,id);

		// alle Objekte ausser den Spielern werden neu angelegt
		// die Spieler existieren schon
	if (type != WorldObject::TypeInfo::TYPE_PLAYER)
	{
		obj = ObjectFactory::createObject((WorldObject::TypeInfo::ObjectType) type, std::string(subt),id);
	}
	else
	{
		if (players->count(id) ==0)
		{
			ERRORMSG("player (%s) with id %i does not exist",subt,id);
		}
		obj = (*players)[id];
	}

	obj->fromString(cv);

	
	insertObject(obj,obj->getShape()->m_center,obj->getShape()->m_angle);
}


void Region::createProjectileFromString(CharConv* cv)
{

	char type,frac;
	int id;
	Projectile* proj;
	float x,y;

	cv->fromBuffer(type);
	cv->fromBuffer(frac);
	cv->fromBuffer(id);

	DEBUG5("new projectile %i frac %i id %i",type,frac,id);

	proj = new Projectile(m_world, (Projectile::ProjectileType) type, (WorldObject::Fraction) frac, id);

	proj->fromString(cv);

	insertProjectile(proj,proj->getShape()->m_center);
}

void Region::createItemFromString(CharConv* cv)
{
	char type;
	char subtype[11];
	subtype[10] ='\0';
	Item* item;
	int id;
	short sx,sy;
	cv->fromBuffer(sx);
	cv->fromBuffer(sy);

	cv->fromBuffer<char>(type);
	cv->fromBuffer(subtype,10);
	cv->fromBuffer(id);

	item = ItemFactory::createItem((Item::Type) type, std::string(subtype),id);
	item->fromString(cv);

	DropItem* di = new DropItem;
	di->m_item = item;
	di->m_x = sx;
	di->m_y = sy;
	DEBUG("dropped item %i at %i %i", id,sx,sy);
	di->m_time = 0;

	m_drop_items->insert(std::make_pair(id,di));
	m_drop_item_locations->insert(std::make_pair(sx*10000+sy,di));

}


void Region::setRegionData(CharConv* cv,WorldObjectMap* players)
{
	// Groesse der Region wird schon vorher eingelesen
	// Tiles eintragen
	int i,j;
	for (i =0;i<m_dimx*2;i++)
	{
		for (j=0;j<m_dimy*2;j++)
		{
			cv->fromBuffer(*(m_tiles->ind(i,j)));
		}
	}


	// alle bisherigen statischen Objekte entfernen
	WorldObjectMap::iterator it;
	for (it = m_static_objects->begin();it!=m_static_objects->end();it++)
	{
		it->second->destroy();
		deleteObject(it->second);
		delete it->second;
	}
	m_static_objects->clear();

	// alle bisherigen nichtstatischen Objekte entfernen
	// die SpielerObjekte bleiben erhalten, alle anderen werden geloescht
	WorldObjectMap::iterator jt;
	for (jt = m_objects->begin();jt!=m_objects->end();jt++)
	{
		if (jt->second->getTypeInfo()->m_type != WorldObject::TypeInfo::TYPE_PLAYER)
		{
			jt->second->destroy();
			deleteObject(jt->second);
			delete jt->second;
		}
	}
	m_objects->clear();
	m_players->clear();

	// statische Objekte einlesen
	short nr_stat;
	cv->fromBuffer<short>(nr_stat);
	DEBUG("static objects: %i",nr_stat);

	for (int i=0; i<nr_stat;i++)
	{
		createObjectFromString(cv,players);
	}



	// neue Objekte einlesen
	short nr_nonstat;
	cv->fromBuffer<short>(nr_nonstat);
	DEBUG("nonstatic objects: %i",nr_nonstat);

	for (int i=0; i<nr_nonstat;i++)
	{
		createObjectFromString(cv,players);
	}


	// Anzahl der Projektile einlesen
	short nr_proj;
	cv->fromBuffer<short>(nr_proj);
	DEBUG("projectiles: %i",nr_proj);
	// Projektile einlesen
	for (int i=0; i<nr_proj;i++)
	{
		createProjectileFromString(cv);
	}

	// Anzahl Gegenstaende einlesen
	short nr_items;
	cv->fromBuffer<short>(nr_items);
	DEBUG("items: %i",nr_items);
	// Gegenstaende einlesen
	for (int i=0; i<nr_items;i++)
	{
		createItemFromString(cv);

	}

}



void Region::setTile(Tile tile,short x, short y)
{
	*(m_tiles->ind(x,y)) = (char) tile;
}

bool  Region::dropItem(Item* item, Vector pos)
{
	// Menge der bereits getesteten Felder
	std::set<int> tfields;

	// Schlange mit zu testenden Feldern
	std::queue<int> fields;

	// Position in 0.5 x 0.5 Feldern
	int sx = (int) (pos.m_x*2);
	int sy = (int) (pos.m_y*2);
	int i;
	fields.push(sx*10000+sy);


	// Flaeche auf die das Item gedroppt wird
	Shape s;
	s.m_type= Shape::RECT;
	s.m_extent = Vector(0.5,0.5);
	

	WorldObjectList res;

	// eine Stelle suchen an der das Item fallen gelassen werden kann
	while (!fields.empty())
	{
		// erstes Feld aus der Queue nehmen
		i = fields.front();
		sx = i /10000;
		sy = i %10000;
		fields.pop();

		DEBUG5("testing field %i %i",sx,sy);

		// Testen ob dort keine festen Objekte sind
		s.m_center = Vector(sx*0.5, sy*0.5);
		

		res.clear();
		getObjectsInShape(&s,&res,WorldObject::LAYER_BASE,WorldObject::FIXED,0,true);

		if (!res.empty())
		{
			// Objekt im Weg
			res.clear();
			continue;
		}

		// Testen, ob dort nicht schon ein Item liegt
		if (m_drop_item_locations->find(i) == m_drop_item_locations->end())
		{
			DEBUG5("field is free");
			// Stelle ist frei
			// Item einfuegen
			DropItem* di = new DropItem;
			di->m_item = item;
			di->m_x = (short) sx;
			di->m_y = (short) sy;
			DEBUG5("dropped item %i", sx*10000+sy);
			di->m_time = 0;

			m_drop_items->insert(std::make_pair(di->m_item->m_id,di));
			m_drop_item_locations->insert(std::make_pair(i,di));


			DEBUG5("items dropped at %i %i",sx,sy);

			if (m_world->isServer())
			{
				Event event;
				event.m_type = Event::ITEM_DROPPED;
				event.m_id = di->m_item->m_id;

				insertEvent(event);
			}

			return true;

		}
		else
		{
			// an der Stelle liegt bereits ein Item
			tfields.insert(i);

			// Nachbarfelder in die Liste der zu untersuchenden Felder einfuegen
			if (sx>0)
			{
				i = (sx-1)*10000+sy;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sy>0)
			{
				i = (sx)*10000+sy-1;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sx<m_dimx*8-1)
			{
				i = (sx+1)*10000+sy;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sy<m_dimy*8-1)
			{
				i = (sx)*10000+sy+1;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

		}

	}

	// keine Stelle gefunden um das Item zu droppen
	return false;

}

Item*  Region::getItemAt(Vector pos)
{
	DropItemMap::iterator it;
	short sx = (int) (pos.m_x*2);
	short sy = (int) (pos.m_y*2);
	int id = sx*10000 + sy;

	it = m_drop_item_locations->find(id);
	if (it == m_drop_items->end())
	{
		return 0;
	}
	else
	{
		return it->second->m_item;
	}
}

Item* Region::getItem(int id)
{
	DropItemMap::iterator it;
	it = m_drop_items->find(id);
	if (it == m_drop_items->end())
	{
		return 0;
	}
	else
	{
		return it->second->m_item;
	}
}

DropItem* Region::getDropItem(int id)
{
	DropItemMap::iterator it;
	it = m_drop_items->find(id);
	if (it == m_drop_items->end())
	{
		return 0;
	}
	else
	{
		return it->second;
	}
}


bool Region::deleteItem(int id, bool delitem)
{

	DropItemMap::iterator it;
	DropItemMap::iterator it2;
	it = m_drop_items->find(id);
	if (it == m_drop_items->end())
	{
		return false;
	}
	else
	{
		// Item Wrapper loeschen
		int pos = 10000* it->second->m_x + it->second->m_y;
		it2 = m_drop_item_locations->find(pos);

		Event event;
		event.m_type = Event::ITEM_REMOVED;
		event.m_id = it->second->m_item->m_id;
		insertEvent(event);

		if (delitem)
		{
			delete it->second->m_item;
		}
		delete (it->second);

		m_drop_items->erase(it);
		m_drop_item_locations->erase(it2);



		return true;
	}
}

EnvironmentName Region::getEnvironment(Vector pos)
{
	// Hoehe an der angegebenen Stelle
	float height = *(m_height->ind(int(pos.m_x/4),int(pos.m_y/4)));
	
	std::map<float, EnvironmentName>::iterator it;
	
	for (it = m_environments.begin(); it !=m_environments.end();++it)
	{
		if (height < it->first)
			return it->second;
	}
	
	return m_environments.rbegin()->second;
}



