#include "region.h"
#include "world.h"
#include "player.h"
#include "eventsystem.h"

RegionData::~RegionData()
{
	std::multimap<TriggerType, Event*>::iterator it;
	for (it = m_events.begin(); it != m_events.end(); ++it)
	{
		delete it->second;
	}
	
}

void RegionData::addObjectGroupTemplate(ObjectGroupTemplateName group_name, int prio, int number,float probability)
{
	ObjectGroupTemplateSet newgroup;
	newgroup.m_group_name = group_name;
	newgroup.m_probability = probability;
	newgroup.m_number = number;
	newgroup.m_probability = probability;

	m_object_groups.insert(std::make_pair(prio,newgroup));
}

void RegionData::addEvent(TriggerType trigger, Event* event)
{
	m_events.insert(std::make_pair(trigger,event));
}

void RegionCamera::update(float time)
{
	if (m_next_positions.empty())
		return;
	
	Position& pos = m_next_positions.front().first;
	float& rtime = m_next_positions.front().second;
	
	if (rtime <= time)
	{
		// naechstes Etappenziel wurde erreicht
		m_position  = pos;
		m_next_positions.pop_front();
	}
	else
	{
		// Phi so anpassen, dass jeweils der kuerzere Teilkreis genutzt wird
		if (m_position.m_phi - pos.m_phi > 180)
			m_position.m_phi -= 360;
	
		if (m_position.m_phi - pos.m_phi < -180)
			m_position.m_phi += 360;
		
		float frac = time/rtime;
		m_position.m_distance = (1-frac)*m_position.m_distance + frac* pos.m_distance;
		m_position.m_phi = (1-frac)*m_position.m_phi + frac* pos.m_phi;
		m_position.m_theta = (1-frac)*m_position.m_theta + frac* pos.m_theta;
		m_position.m_focus = m_position.m_focus*(1-frac) + pos.m_focus*frac;
		
		rtime -= time;
	}
}

Region::Region(short dimx, short dimy, short id, std::string name)
{
	DEBUG5("creating region");
	m_data_grid = new Matrix2d<Gridunit>(dimx,dimy);
	m_dimx = dimx;
	m_dimy = dimy;
	m_name = name;

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

	m_netevents = new NetEventList;

	m_cutscene_mode = false;
	
	m_revive_location = "";
	
	Trigger* tr = new Trigger("create_region");
	insertTrigger(tr);
	
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
		delete k->second->m_item;
		delete k->second;
	}
	
	std::list<Trigger*>::iterator l;
	for (l = m_triggers.begin(); l != m_triggers.end(); ++l)
	{
		delete *l;
	}
	
	std::multimap<TriggerType, Event*>::iterator m;
	for (m= m_events.begin(); m!= m_events.end(); ++m)
	{
		delete m->second;
	}
	
	delete m_objects;
	delete m_static_objects;
	delete m_players;
	delete m_projectiles;

	delete m_data_grid;
	delete m_tiles;
	delete m_drop_items;
	delete m_drop_item_locations;
	delete m_height;
	delete m_netevents;

}

WorldObject* Region::getObject ( int id)
{
	WorldObjectMap::iterator iter;

	// Objekt im Binärbaum suchen

	iter = m_objects->find(id);

	// Testen ob ein Objekt gefunden wurde
	if (iter == m_objects->end())
	{
		// unter den statischen Objekten suchen
		iter = m_static_objects->find(id);

		// Testen ob ein Objekt gefunden wurde
		if (iter == m_static_objects->end())
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

bool Region::getFreePlace(Shape* shape, short layer, Vector & pos, WorldObject* omit)
{
	// Menge der bereits getesteten Felder
	std::set<int> tfields;

	// Schlange mit zu testenden Feldern
	std::queue<int> fields;

	float c = 1.1;
	// Position in 0.5 x 0.5 Feldern
	int sx = (int) ((pos.m_x/c) + 0.5);
	int sy = (int) ((pos.m_y/c) + 0.5);
	int i;
	fields.push(sx*10000+sy);

	// Kopie der Form
	Shape s;
	memcpy(&s,shape,sizeof(Shape));

	WorldObjectList res;
	res.clear();

    // true, solange alle Felder von festen Objekten blockiert sind
    bool fixblock = true;

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
		getObjectsInShape(&s,&res,layer,WorldObject::FIXED,omit,true);

		if (!res.empty())
		{
			// Objekt im Weg
			res.clear();

			// wenn alle bisherigen Plaetze von festen Objekten blockiert warten trotzdem weitersuchen
			if (!fixblock)
                continue;
		}
		else
		{
		    fixblock = false;
		}

		DEBUG5("no fixed obstacle");
		// Testen, ob dort nicht gerade eine Kreatur steht
		getObjectsInShape(&s,&res,layer,WorldObject::CREATURE,omit,true);
		if (res.empty() && !fixblock)
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
		DEBUG5("testing obj %i layer %i",wo->getId(),wo->getLayer());

		s = wo->getShape();

		if (wo->getLayer() & layer)
		{
			DEBUG5("adding obj %i layer %i",wo->getId(), wo->getLayer());
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

		WorldObjectMap::iterator it;
		for (it = m_players->begin(); it != m_players->end(); ++it)
		{
 			if (shape->intersects(*(it->second->getShape()) ) && it->second != omit)
			{
				result->push_back(it->second);
			}
		}
	}
	else
	{
		int i,j;
		Gridunit* gu=0;
		// durchmustere alle 4*4 Felder die von dem Suchrechteck plus 4 Felder bedeckt werden

		// Maximale Ausdehnung der Flaeche in Richtung der Koordinatenaxen
		Vector ext = shape->getAxisExtent();
		Vector d1 = shape->m_center +ext;
		Vector d2 = shape->m_center -ext;


		int xmin = (int) floor(0.25*(d2.m_x-4));
		int ymin = (int) floor(0.25*(d2.m_y-4));
		int xmax = (int) floor(0.25*(d1.m_x+4));
		int ymax = (int) floor(0.25*(d1.m_y+4));
		// Pruefen ob die Suchanfrage nicht aus der Region herauslaeuft
		bool ret = false;
		int is = std::max (xmin,0);
		int ie = std::min(xmax,m_dimx-1);
		int js = std::max(ymin,0);
		int je = std::min(ymax,m_dimy-1);

		DEBUG5("searching square (%i %i) (%i %i)",is,ie,js,je);

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


bool Region::insertObject(WorldObject* object, Vector pos, float angle, bool collision_test)
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
		Player* pl = dynamic_cast<Player*>(object);
		DEBUG5("player entered Region");
		result &= (m_players->insert(std::make_pair(object->getId(),object))).second;
		
		if (m_revive_location != "")
		{
			pl->setRevivePosition(std::make_pair(getName(),m_revive_location));
			DEBUG("player will now revive at %s",m_revive_location.c_str());
		}
		
		Trigger* tr = new Trigger("enter_region");
		tr->addVariable("player", object->getId());
		insertTrigger(tr);

	}
	else
	{
		// fuer Nicht Spieler NetEvent erzeugen, dass das Objekt erschaffen wurde
		NetEvent event;
		event.m_type = NetEvent::OBJECT_CREATED;
		event.m_id = object->getId();
		insertNetEvent(event);
		DEBUG5("insert object %i",event.m_id);
		
		if (object->getGroup() == WorldObject::CREATURE)
		{
			Trigger* tr = new Trigger("create_unit");
			tr->addVariable("unit", object->getId());
			insertTrigger(tr);
		}
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
		 return false;
	}
	else
	{
		if (object->getLayer() != WorldObject::LAYER_SPECIAL)
		{
			Gridunit *gu = (m_data_grid->ind(x_g,y_g));

			result = gu->insertObject(object);
		}

	}

	//DEBUG("object inserted %s at %f %f",object->getTypeInfo()->m_subtype.c_str(),
	return result;
}

int Region::createObject(ObjectTemplateType generictype, Vector pos, float angle, WorldObject::State state)
{
	// Umgebung erfahren
	EnvironmentName env = getEnvironment(pos);

	// genauen Subtyp ermitteln
	WorldObject::TypeInfo::ObjectSubtype subtype = ObjectFactory::getObjectType(generictype, env);
	if (subtype == "")
	{
		DEBUG("no subtype found for generictype %s",generictype.c_str());
		return 0;
	}
	
	bool collision_test=true;
	
	// Basistyp ermitteln
	WorldObject::TypeInfo::ObjectType type = ObjectFactory::getObjectBaseType(subtype);
	if (type == WorldObject::TypeInfo::TYPE_NONE)
	{
		DEBUG("no base type for subtype %s",subtype.c_str());
		return 0;
	}

	// Objekt erstellen
	WorldObject* object = ObjectFactory::createObject(type,subtype);

	if (object ==0)
	{
		DEBUG("could not create object for generictype %s",generictype.c_str());
		return 0;
	}

	if (state != WorldObject::STATE_NONE)
	{
		object->setState(state);
	}
	
	if (state == WorldObject::STATE_AUTO)
	{
		if (object->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_FIXED_OBJECT)
		{
			object->setState(WorldObject::STATE_INACTIVE);
		}
		else
		{
			object->setState(WorldObject::STATE_ACTIVE);
		}
	}
	
	if (object->getState() == WorldObject::STATE_STATIC)
	{
		collision_test= false;
	}
	
	
	// Objekt einfuegen
	bool ret = insertObject(object,pos,angle,collision_test);
	if (!ret)
	{
		DEBUG("insertion failed");
	}

	return object->getId();

}


void Region::createObjectGroup(ObjectGroupTemplateName templname, Vector position, float angle)
{
	// Template aus der Datenbank heraussuchen
	std::map<ObjectGroupTemplateName, ObjectGroupTemplate*>::iterator it;

	ObjectGroupTemplate* templ = ObjectFactory::getObjectGroupTemplate(templname);

	if (templ != 0)
	{
		// Template wurde gefunden
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


				int id = createObject(gt->m_type, pos, angle+gt->m_angle);
				DEBUG5("inserting object %s at %f %f with id %i",gt->m_type.c_str(),pos.m_x, pos.m_y,id);

			}
		}

		// Orte einfuegen
		std::map<LocationName, Vector>::iterator lt;
		for (lt = templ->getLocations()->begin(); lt != templ->getLocations()->end(); ++lt)
		{
			pos = lt->second;
			pos.rotate(angle);
			pos += position;

			addLocation(lt->first,pos);
		}
	}
	else
	{
		ERRORMSG("object group template %s not found",templname.c_str());
	}
}


void Region::createMonsterGroup(MonsterGroupName mgname, Vector position)
{
	MonsterGroup * mgroup = ObjectFactory::getMonsterGroup(mgname);

	if (mgroup == 0)
	{
		DEBUG("monster group %s ot found",mgname.c_str());
		return;
	}

	std::list<MonsterGroup::SubGroup>::iterator mt;
	Vector pos;

	for (mt = mgroup->m_monsters.begin(); mt != mgroup->m_monsters.end(); ++mt)
	{
		int count =0;
		for (int i=0; i< mt->m_number; i++)
		{
			pos = position;

			if (Random::random() < mt->m_prob)
			{
				count ++;
				int id = createObject(mt->m_subtype, pos);
				DEBUG5("inserting monster %s at %f %f with id %i",mt->m_subtype.c_str(),pos.m_x, pos.m_y,id);
			}
		}
		DEBUG5("monstergroup %s : %i x %s prob %f",mgname.c_str(), count, mt->m_subtype.c_str(), mt->m_prob);
		
	}
}


bool  Region::insertProjectile(Projectile* object, Vector pos)
{
	m_projectiles->insert(std::make_pair(object->getId(),object));
	object->getShape()->m_center = pos;
	object->setRegion( m_id);

	// NetEvent erzeugen: neues Projektil in der Region
	NetEvent event;
	event.m_type = NetEvent::PROJECTILE_CREATED;
	event.m_id = object->getId();
	insertNetEvent(event);

	return true;
}

bool  Region::deleteObject (WorldObject* object)
{
	bool result = true;

	if (object == 0)
		return false;
	
	if (m_objects->count(object->getId()) == 0 && m_static_objects->count(object->getId()) == 0)
	{
		// Objekt nicht gefunden
		return false;
	}
	
	if (object->getTypeInfo()->m_type != WorldObject::TypeInfo::TYPE_PLAYER)
	{
		NetEvent event;
		event.m_type = NetEvent::OBJECT_DESTROYED;
		event.m_id = object->getId();
		insertNetEvent(event);
	}
	
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
		DEBUG5("Player deleted");
		m_players->erase(object->getId());

	}

	return result;
}

bool Region::deleteObject(int id)
{
	WorldObject* wo = getObject(id);
	if (wo !=0)
	{
		return deleteObject(wo);
	}
	else
	{
		DEBUG("no object with id %i",id);
	}
	return false;
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
		return false;
	}
	else
	{
		Gridunit *gu = &(*m_data_grid)[x_old][y_old];
		result =gu->deleteObject(object);
		if (result == false)
		{
			ERRORMSG("failed to remove object %i from gridunit %i %i",object->getId(),x_old, y_old);
		}

		gu = &(*m_data_grid)[x_new][y_new];
		result = gu->insertObject(object);
		if (result == false)
		{
			ERRORMSG("failed to insert object %i into gridunit %i %i",object->getId(),x_old, y_old);
		}


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
	if (proj ==0)
		return;
	
	int id = proj->getId();

	if (m_projectiles->count(id)!=0)
	{
		m_projectiles->erase(m_projectiles->find(id));
	}
}

void Region::update(float time)
{
	DEBUG5("update region %i",getId());

	DEBUG5("\nUpdate aller WeltObjekte starten\n");
	//DEBUG("m_players %p",m_players);
	// Iterator zum durchmustern einer solchen Liste
	WorldObjectMap::iterator iter;
	WorldObject* object;
	ProjectileMap::iterator it3;

	// Durchmustern aller WorldObjects
	for (iter =m_objects->begin(); iter!=m_objects->end(); )
	{
		object = iter->second;

		if (object->getDestroyed()==true)
		{
			// Objekte selbststaendig loeschen darf nur der Server
			if (World::getWorld()->isServer())
			{
				// nur Nichtspieler Objekte loeschen
				if (object->getTypeInfo()->m_type != WorldObject::TypeInfo::TYPE_PLAYER)
				{
					DEBUG5("Objekt gelöscht: %i \n",object->getId());
					
					++iter;
					object->destroy();
					deleteObject(object);
					delete object;
					continue;
				}
				else
					++iter;
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
			if (World::getWorld()->isServer())
			{
				NetEvent event;
				event.m_type = NetEvent::PROJECTILE_DESTROYED;
				event.m_id = pr->getId();
				insertNetEvent(event);

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

	// DropItems updaten
	DropItemMap::iterator it4;
	for (it4 = m_drop_items->begin(); it4 != m_drop_items->end(); ++it4)
	{
		it4->second->update(time);
	}

	if (World::getWorld()->isServer())
	{
		// NetEvents fuer geaenderte Objekte / Projektile erzeugen
		for (iter =m_objects->begin(); iter!=m_objects->end(); ++iter)
		{
			object = iter->second;

			// NetEvents durch Spieler werden global behandelt, daher hier nicht beruecksichtigen
			if (object->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
			{
				continue;
			}

			if (object->getNetEventMask() !=0)
			{
				DEBUG5("object %i has event mask %i",object->getId(), object->getNetEventMask());
				NetEvent event;
				event.m_type = NetEvent::OBJECT_STAT_CHANGED;
				event.m_data = object->getNetEventMask();
				event.m_id = object->getId();
				insertNetEvent(event);
				
				object->clearNetEventMask();
			}
		}

		for (it3 = m_projectiles->begin(); it3 !=m_projectiles->end();++it3)
		{
			pr = (it3->second);
			if (pr->getNetEventMask() !=0)
			{
				NetEvent event;
				event.m_type = NetEvent::PROJECTILE_STAT_CHANGED;
				event.m_data = pr->getNetEventMask();
				event.m_id = pr->getId();
				insertNetEvent(event);
				
				pr->clearNetEventMask();
			}
		}
	}

	// pruefen ob ein Spieler die Region verlassen hat
	bool del = false;
	for (iter = m_players->begin(); iter != m_players->end(); )
	{
		// Schleife ueber die Ausgaenge
		std::list<RegionExit>::iterator eit;
		for (eit = m_exits.begin(); eit != m_exits.end(); ++eit)
		{
			if (iter->second->getShape()->intersects(eit->m_shape))
			{
				// Spieler befindet sich im Ausgang
				// ID der neuen Region
				int id = World::getWorld()->getRegionId(eit->m_destination_region);

				WorldObjectMap::iterator iter2 = iter;
				iter ++;
				del = true;

				// Spieler aus der Region entfernen
				WorldObject* pl = iter2->second;
				deleteObject(pl);

				// Spieler in die neue Region einfuegen
				World::getWorld()->insertPlayerIntoRegion(pl, id, eit->m_destination_location);

				break;
			}
		}


		if (del == false)
		{

			// Pruefen ob der Spieler getoetet wurde
			if (iter->second->getState() == WorldObject::STATE_DEAD)
			{
				// Lebenspunkte wieder fuellen etc
				Player* pl = static_cast<Player*>(iter->second);
				pl->revive();

				// Zielpunkt ermitteln
				RegionLocation regloc = pl->getRevivePosition();
				int id = World::getWorld()->getRegionId(regloc.first);

				if (id == getId())
				{
					DEBUG5("revive in current region");
					// Spieler bleibt in der aktuellen Region
					Vector pos = getLocation(regloc.second);
					getFreePlace(pl->getShape(), pl->getLayer(), pos, pl);

					pl->moveTo(pos);
				}
				else
				{
					DEBUG5("revive in other region %i ", id);

					// Spieler verlaesst die Region
					WorldObjectMap::iterator iter2 = iter;
					iter ++;
					del = true;

					// Spieler aus der Region entfernen
					deleteObject(pl);

					// Spieler in die neue Region einfuegen
					World::getWorld()->insertPlayerIntoRegion(pl, id, regloc.second);
				}
			}

			if (del == false)
				++iter;
		}
		del = false;
	}
	
	// Trigger & Events abarbeiten
	// Zeitverzoegerte Trigger
	std::list<std::pair<float, Trigger*> > ::iterator tit, tjt;
	for (tit = tjt = m_timed_trigger.begin(); tjt != m_timed_trigger.end(); )
	{
		tit = tjt;
		++tjt;
		
		tit->first -= time;
		if (tit->first<=0)
		{
			insertTrigger(tit->second);
			m_timed_trigger.erase(tit);
		}
	}
	
	// Schleife ueber die Trigger
	EventSystem::setRegion(this);
	
	std::multimap<TriggerType, Event*>::iterator it, itend, jt;
	
	while (!m_triggers.empty())
	{
		TriggerType type;
		type = m_triggers.front()->getType();
		
		DEBUG5("trigger: %s",type.c_str());
		
		// Schleife ueber die ausgeloesten Events
		it = m_events.lower_bound(type);
		itend = m_events.upper_bound(type);
		while (it != itend)
		{
			jt = it;
			++it;
			
			// vom Trigger definierte Variablen einfuegen
			EventSystem::doString((char*) m_triggers.front()->getLuaVariables().c_str());
			DEBUG5("lua code \n %s",m_triggers.front()->getLuaVariables().c_str());
			
			// Event ausfuehren
			bool ret = EventSystem::executeEvent(jt->second);
			
			if (ret)
				DEBUG5("event on trigger: %s",type.c_str());
			
			// einmalige Ereignisse loeschen, wenn erfolgreich ausgefuehrt
			if (jt->second->getOnce() &&  ret)
			{
				delete jt->second;
				m_events.erase(jt);
			}
			
		}
		
		delete m_triggers.front();
		m_triggers.pop_front();
	}
	
	// update der Dialoge
	std::map<int, Dialogue*>::iterator it5;
	for (it5 = m_dialogues.begin(); it5!= m_dialogues.end(); )
	{
		EventSystem::setDialogue(it5->second);
		
		it5->second->update(time);
		
		if (it5->second->isFinished())
		{
			delete it5->second;
			m_dialogues.erase(it5++);
		}
		else
		{
			++it5;
		}
	}
	
	m_camera.update(time);
}

void Region::getRegionData(CharConv* cv)
{
	// Dimension des Feldes angeben
	cv->toBuffer((short) m_dimx);
	cv->toBuffer((short) m_dimy);

	// Name der Region
	char stmp[21];
	stmp[20] = '\0';
	strncpy(stmp,m_name.c_str(),20);
	cv->toBuffer(stmp,20);

	cv->toBuffer(m_ground_material);
	
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
	DEBUG5("static objects: %i",m_static_objects->size());
	cv->toBuffer<short>((short) m_static_objects->size());

	// statische Objekte in den Puffer eintragen
	WorldObjectMap::iterator it;
	for (it = m_static_objects->begin();it!=m_static_objects->end();++it)
	{
		(it->second)->toString(cv);
		DEBUG5("static object: %s",(it->second)->getNameId().c_str());
	}


	// Anzahl der nicht  statischen Objekte eintragen
	WorldObjectMap::iterator jt;
	short nr=0;
	for (jt = m_objects->begin();jt!=m_objects->end();++jt)
	{
		if (jt->second->getLayer() != WorldObject::LAYER_SPECIAL)
			nr++;
	}
	DEBUG5("nonstatic objects: %i",nr);
	cv->toBuffer<short>((short) nr);

	// nicht statische Objekte in den Puffer eintragen

	for (jt = m_objects->begin();jt!=m_objects->end();++jt)
	{
		if (jt->second->getLayer() != WorldObject::LAYER_SPECIAL)
		{
			DEBUG5("write offset: %i",cv->getBitStream()->GetNumberOfBitsUsed());
			(jt->second)->toString(cv);

			DEBUG5("object: %s",(jt->second)->getNameId().c_str());
		}
	}

	// Anzahl der Projektile eintragen
	cv->toBuffer<short>((short) m_projectiles->size());
	DEBUG5("projectiles: %i",m_projectiles->size());

	// Projektile in den Puffer eintragen
	ProjectileMap::iterator kt;
	for (kt = m_projectiles->begin(); kt != m_projectiles->end(); ++kt)
	{
		kt->second->toString(cv);
	}

	cv->toBuffer<short>((short) m_drop_items->size());
	DEBUG5("dropped items: %i",m_drop_items->size());

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

	if (obj==0)
	{
		ERRORMSG("failed to create Object %s",subt);
	}
	
	obj->fromString(cv);


	insertObject(obj,obj->getShape()->m_center,obj->getShape()->m_angle);
}


void Region::createProjectileFromString(CharConv* cv)
{

	char type,frac;
	int id;
	Projectile* proj;

	cv->fromBuffer(type);
	cv->fromBuffer(id);

	DEBUG5("new projectile %i frac %i id %i",type,frac,id);

	proj = new Projectile((Projectile::ProjectileType) type,0, id);

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
	DEBUG5("dropped item %i at %i %i", id,sx,sy);
	di->m_time = 0;

	m_drop_items->insert(std::make_pair(id,di));
	m_drop_item_locations->insert(std::make_pair(sx*10000+sy,di));

}


void Region::setRegionData(CharConv* cv,WorldObjectMap* players)
{
	// Groesse der Region wird schon vorher eingelesen

	// Name der Region
	char stmp[21];
	stmp[20] ='\0';
	cv->fromBuffer(stmp,20);
	m_name = stmp;
	DEBUG5("name of region: %s",stmp);
	cv->fromBuffer(m_ground_material);

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
	DEBUG5("static objects: %i",nr_stat);

	for (int i=0; i<nr_stat;i++)
	{
		createObjectFromString(cv,players);
	}



	// neue Objekte einlesen
	short nr_nonstat;
	cv->fromBuffer<short>(nr_nonstat);
	DEBUG5("nonstatic objects: %i",nr_nonstat);

	for (int i=0; i<nr_nonstat;i++)
	{
		createObjectFromString(cv,players);
	}


	// Anzahl der Projektile einlesen
	short nr_proj;
	cv->fromBuffer<short>(nr_proj);
	DEBUG5("projectiles: %i",nr_proj);
	// Projektile einlesen
	for (int i=0; i<nr_proj;i++)
	{
		createProjectileFromString(cv);
	}

	// Anzahl Gegenstaende einlesen
	short nr_items;
	cv->fromBuffer<short>(nr_items);
	DEBUG5("items: %i",nr_items);
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

  	// true, solange alle Felder von festen Objekten blockiert sind
	bool fixblock = true;
	
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

			// wenn alle bisherigen Plaetze von festen Objekten blockiert warten trotzdem weitersuchen
			if (!fixblock)
				continue;
		}
		else
		{
			fixblock = false;
		}

		// Testen, ob dort nicht schon ein Item liegt
		if (!fixblock && m_drop_item_locations->find(i) == m_drop_item_locations->end())
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

			if (World::getWorld()->isServer())
			{
				NetEvent event;
				event.m_type = NetEvent::ITEM_DROPPED;
				event.m_id = di->m_item->m_id;

				insertNetEvent(event);
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

bool Region::dropItem(Item::Subtype subtype, Vector pos, int magic_power)
{
		
	Item::Type type = ItemFactory::getBaseType(subtype);
	if (type == Item::NOITEM)
	{
		return false;
	}
	
	Item* item= ItemFactory::createItem(type,subtype,0,magic_power);
	DEBUG5("drop item %s at %f %f %p",subtype.c_str(),pos.m_x,pos.m_y,item);
	
	if (item !=0)
	{
		return dropItem(item, pos);
	}
	
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

		NetEvent event;
		event.m_type = NetEvent::ITEM_REMOVED;
		event.m_id = it->second->m_item->m_id;
		insertNetEvent(event);

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
	if (m_environments.empty())
	{
		ERRORMSG("no environments defined");
	}
	// Hoehe an der angegebenen Stelle
	float height = *(m_height->ind(int(pos.m_x/4),int(pos.m_y/4)));

	std::map<float, EnvironmentName>::iterator it;

	for (it = m_environments.begin(); it !=m_environments.end();++it)
	{
		if (height < it->first)
		{
			DEBUG5("environment %s", it->second.c_str());
			return it->second;
		}
	}

	return m_environments.rbegin()->second;
}

Vector Region::getLocation(LocationName name)
{
	std::map<LocationName, Vector>::iterator it;
	it = m_locations.find(name);

	if (it == m_locations.end())
	{
		return Vector(-1,-1);
	}

	return it->second;
}

Shape Region::getArea(AreaName name)
{
	std::map<AreaName, Shape>::iterator it;
	it = m_areas.find(name);

	if (it == m_areas.end())
	{
		return Shape();
	}

	return it->second;
}


void Region::addExit(RegionExit exit)
{
	exit.m_shape.m_center = getLocation(exit.m_exit_name);
	m_exits.push_back(exit);
}

void Region::insertTrigger(Trigger* trigger)
{
	m_triggers.push_back(trigger);
}

void Region::insertTimedTrigger(Trigger* trigger, float time)
{
	m_timed_trigger.push_back(std::make_pair(time,trigger));
}

void Region::addEvent(TriggerType trigger, Event* event)
{
	m_events.insert(std::make_pair(trigger,event));
}

void Region::setCutsceneMode(bool mode)
{
	NetEvent event;
	event.m_type = NetEvent::REGION_CUTSCENE;
	event.m_id = getId();
	event.m_data = (int) mode;
	insertNetEvent(event);
	
	m_cutscene_mode = mode;
}


Dialogue* Region::getDialogue(int id)
{
	std::map<int, Dialogue*>::iterator it;
	it = m_dialogues.find(id);
	
	if (it == m_dialogues.end())
		return 0;
	
	return it->second;
}

void Region::insertDialogue(Dialogue* dia)
{
	m_dialogues.insert(std::make_pair(dia->getId(),dia));
}

