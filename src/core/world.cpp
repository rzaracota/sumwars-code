/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "world.h"
#include "player.h"

/**
 * Constructors/Destructors
 */
/**
 *
 */
 World::World(bool server)
{
	m_server = server;
	
	// diverse Initialisierungen
	
	m_player_slots = new map<int,ServerWObject*>;
	m_players = new map<int,ServerWObject*>;;
	
	// Baum fuer die Handelsvorgaenge anlegen	
	m_trades = new map<int, Trade* >;
	m_trades->clear();
	

	m_max_nr_players =8;
	m_parties = new Party[m_max_nr_players];
	for (int i =0;i<m_max_nr_players;i++)
	{
		m_parties[i].init(m_max_nr_players,i);
	}
	
		 
	 for (int i=0;i<WORLD_MAX_REGIONS;i++)
		 m_regions[i]=0;
	 
	 m_local_player =0;
	
}


bool World::init()
{
	// Aktionen initialisieren
	Action::init();
		
	// Items initialisieren
	ItemFactory::init();
	
	ObjectFactory::init();
	ObjectFactory::m_world = this;
	
	if (m_server)
	{
		DEBUG("server");
		m_network = new ServerNetwork(m_max_nr_players);
	}
	else
	{
		DEBUG("client");
		m_network = new ClientNetwork();
	}
	
	long sec;
	time(&sec);
	srand(sec);
	
	if( m_network->init( REQ_PORT )!=NET_OK )
	{
		ERRORMSG( "Error occured in network" );
		return false;
	}
	
	
}


void World::createRegion(short region)
{
	DEBUG5("creating region %i",region);
	int type = 2;
	if (type==1)
	{

	}
	else if(type==2)
	{
		Region* reg = new Region(25,25,region);
		short rid = insertRegion(reg,region);




		// Objekte anlegen
		ServerWObject* wo=0,*wo2=0;
		WorldObject::Geometry* wob=0;
		Shape* sp=0;

		// Tiles Setzen
		for (int i=1;i<5;i++)
		{
			reg->setTile(TILE_GRASS,5,i);
		}

		for (int i=7;i<10;i++)
		{
			reg->setTile(TILE_WATER,i,6);
		}

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT, "fence");
		insertSWObject(wo, 4,6,rid);


		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT, "tree");
		insertSWObject(wo, 9,13,rid);


		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 13,8,rid);


		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "gob_dog");
		insertSWObject(wo, 14.2,8.2,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 14.5,6.7,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 14.8,5.7,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 14,10.9,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 17,8.2,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "lich");
		insertSWObject(wo, 7,5,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 18.5,7,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 17.2,6.6,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 19.9,6.9,rid);

/*
		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 16.2,3,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 19.2,3.1,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 6.5,6.4,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertSWObject(wo, 8.5,6.4,rid);
		*/
/*

		Spawnpoint::SpawnedMonsterGroup mg("goblin",1.0,1);
		spawn = new Spawnpoint(m_world,1);
		spawn->setRespawnTime(10000);
		spawn->addMonsterGroup(mg);
		insertSWObject(spawn,9,9,rid);
*/
/*
		int i,j;
		for (i=1;i<48;i++)
		{
		for (int j=1;j<48;j++)
		{
		reg->setTile(TILE_GRASS,i,j);
	}
	}

		for (i=0;i<49;i++)
		{
		reg->setTile(TILE_WATER,i,0);
		reg->setTile(TILE_WATER,i,49);
		reg->setTile(TILE_WATER,49,i);
		reg->setTile(TILE_WATER,0,i);

	}

		Spawnpoint::SpawnedMonsterGroup gob("goblin",0.6,6);
		Spawnpoint::SpawnedMonsterGroup gob2("goblin",0.5,3);
		Spawnpoint::SpawnedMonsterGroup lich("lich",0.7,1);
		Spawnpoint::SpawnedMonsterGroup dog("gob_dog",0.4,4);

		spawn = new Spawnpoint(m_world,1);
		spawn->setRespawnTime(100000);
		spawn->addMonsterGroup(gob);
		insertSWObject(spawn,9,9,rid);

		spawn = new Spawnpoint(m_world,2);
		spawn->setRespawnTime(100000);
		spawn->addMonsterGroup(gob);
		insertSWObject(spawn,6,18,rid);

		spawn = new Spawnpoint(m_world,3);
		spawn->setRespawnTime(100000);
		spawn->addMonsterGroup(gob2);
		spawn->addMonsterGroup(dog);
		insertSWObject(spawn,22,14,rid);

		spawn = new Spawnpoint(m_world,4);
		spawn->setRespawnTime(100000);
		spawn->addMonsterGroup(gob);
		spawn->addMonsterGroup(lich);
		insertSWObject(spawn,16,25,rid);

		spawn = new Spawnpoint(m_world,5);
		spawn->setRespawnTime(100000);
		spawn->addMonsterGroup(gob2);
		spawn->addMonsterGroup(lich);
		spawn->addMonsterGroup(dog);
		insertSWObject(spawn,30,27,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT, "tree");
		insertSWObject(wo, 24,26,rid);


		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT, "tree");
		insertSWObject(wo, 10,13,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT, "tree");
		insertSWObject(wo, 20,18,rid);
*/

	}
}


void World::acceptLogins()
{
	int login_slot;

	while((login_slot=static_cast<ServerNetwork*>(m_network)->popNewLoginSlot()) != -1 )
	{
		INFO( "user login detected: %i",  login_slot  );
		m_logins.push_back(login_slot);


	}
}


void World::updateLogins()
{
	list<int>::iterator i;
	Packet* data;
	int lend;
	PackageHeader header;
	DEBUG5("update logins");
	for (i=m_logins.begin();i!=m_logins.end();)
	{
		DEBUG5("testing slot %i",(*i));
		if (m_network->numberSlotMessages((*i) )>0)
		{
			DEBUG5("got Package");
			m_network->popSlotMessage(data,(*i));
			CharConv cv(data);
			header.fromString(&cv);
			if (header.m_content == PTYPE_C2S_SAVEGAME)
			{
				DEBUG5("got savegame from slot %i",(*i));
				handleSavegame(&cv,*i);
				i = m_logins.erase(i);
				
			}
			else
			{
				DEBUG("unknown type %i",header.m_content);
			}
			m_network->deallocatePacket(data);
		}
		else
		{
			i++;
		}

	}
	DEBUG5("done");

}



World::~World()
{

	map<int, Trade* >::iterator j;
	for (j=m_trades->begin(); j!=m_trades->end();j++)
		delete j->second;
	
	delete m_trades;
	
	for (int i=0;i<WORLD_MAX_REGIONS;i++)
	{
		if (m_regions[i]!=0)
			delete m_regions[i];
	}

	delete[] m_parties; 
	delete m_player_slots;
	delete m_players;
}	


 
short World::insertRegion(Region* region, int rnr)
{
	
	m_regions[rnr]=region;
	return rnr;
	
}

WorldObject::Relation World::getRelation(WorldObject::TypeInfo::Fraction frac, ServerWObject* wo)
{
	WorldObject::TypeInfo::Fraction f = wo->getTypeInfo()->m_fraction;
	
	DEBUG5("frac1 %i frac2 %i",frac,f);

	if (wo->getTypeInfo()->m_type > WorldObject::TypeInfo::TYPE_MONSTER)
	{
		return 	WorldObject::NEUTRAL;
	}
	
	if (frac == WorldObject::TypeInfo::NOFRACTION)
		return WorldObject::NEUTRAL;
	else if (frac <=  WorldObject::TypeInfo::FRAC_MONSTER)
	{
		if (frac ==  wo->getTypeInfo()->m_fraction)
			return WorldObject::ALLIED;
		else if (wo->getTypeInfo()->m_fraction==WorldObject::TypeInfo::NOFRACTION)
			return WorldObject::NEUTRAL;
		else
			return WorldObject::HOSTILE;
	}
	else if (frac ==WorldObject::TypeInfo::FRAC_HOSTILE_TO_ALL)
	{
		return WorldObject::HOSTILE;
	}
	else
	{
		
		if (f == WorldObject::TypeInfo::NOFRACTION)
			return WorldObject::NEUTRAL;
		else if (f <=  WorldObject::TypeInfo::FRAC_HOSTILE_TO_ALL)
			return WorldObject::HOSTILE;
		else
		{
			return (m_parties[frac - WorldObject::TypeInfo::FRAC_PLAYER_PARTY].getRelations())[f- WorldObject::TypeInfo::FRAC_PLAYER_PARTY];
		}
	}
}


Party* World::getEmptyParty()
{
	int i;
	for (i=0;i<m_max_nr_players;i++)
	{
		if (m_parties[i].getNrMembers()==0)
			return &(m_parties[i]);
	}
	return 0;
}


bool World::intersect(Shape* s1, Shape* s2)
{
	Shape* t=0;
	float x1 = s1->m_coordinate_x;
	float x2 = s2->m_coordinate_x;
	float y1 = s1->m_coordinate_y;
	float y2 = s2->m_coordinate_y;
	
	if (s1->m_type == Shape::CIRCLE && s2->m_type == Shape::CIRCLE)
	{
		DEBUG5("checking circle-circle");
		
		// Pruefen von Kreis gegen Kreis
		// Ueberschneiden sich, wenn  Abstand der Mittelpunkte geringer als Summe der Radien
		if (sqr(x1-x2) + sqr(y1-y2) < sqr(s1->m_radius + s2->m_radius))
		{
			DEBUG5("intersect %f %f %f %f",sqr(x1-x2),sqr(y1-y2),sqr(s1->m_radius + s2->m_radius),sqr(x1-x2) + sqr(y1-y2));
			return true;
		}
		else
			return false;
	}
	else if (s1->m_type == Shape::RECT && s2->m_type == Shape::RECT)
	{
		DEBUG5("checking rect-rect");
		
		// Pruefen Rechteck gegen Rechteck
		// Ueberschneiden sich, wenn Differenz der Mittelpunkt in beiden Koordinatenrichtungen geringer als die Summe der Ausdehnungen in den beiden Richtungen
		float xe1 = s1->m_extent_x;
		float xe2 = s2->m_extent_x;
		float ye1 = s1->m_extent_y;
		float ye2 = s2->m_extent_y;
		if ( fabs(x1-x2) < xe1+xe2 && fabs(y1-y2) < ye1+ye2 )
			return true;
		else
			return false;
	}
	else
	{
		// Pruefen Kreis gegen Rechteck
		DEBUG5("checking circle-rect");
		
		// So tauschen, dass s1 der Kreis ist
		if (s1->m_type == Shape::RECT)
		{
			t = s1;
			s1 = s2;
			s2 = t;
			x1 = s1->m_coordinate_x;
			x2 = s2->m_coordinate_x;
			y1 = s1->m_coordinate_y;
			y2 = s2->m_coordinate_y;
		}	
		float xe2 = s2->m_extent_x;
		float ye2 = s2->m_extent_y;
		float r1 = s1->m_radius;
		
		// zuerst ein Test gegen das den Kreis umgebende Rechteck
		if (fabs(x1-x2) > r1+xe2 || fabs(y1-y2) > r1+ye2 )
		{
			return false;
		}
		else
		{
			// Testen ob Kreismittelpunkt direkt im Rechteck liegt
			if ( fabs(x1-x2) < xe2 || fabs(y1-y2) < ye2 )
				return true;
			
			// Kreis liegt auf dem Reckteck, wenn Abstand zu einer der Ecken kleiner als r1
			float mx = min (fabs(x1-x2+xe2), fabs(x1-x2-xe2));
			float my = min (fabs(y1-y2+ye2), fabs(y1-y2-ye2));
			if (sqr(mx)+sqr(my) < sqr(r1))
				return true;
			else
				return false;
			
		}

	}

}
	
	
float World::getDistance(Shape& s1, Shape& s2)
{
	return (sqrt(sqr(s1.m_coordinate_x-s2.m_coordinate_x) +sqr(s1.m_coordinate_y-s2.m_coordinate_y)) - s1.m_radius - s2.m_radius);
}
	




ServerWObject* World::getSWObject ( int id,short rid) 
{
	return m_regions[rid]->getSWObject(id);
}


Trade* World::getTrade ( int id)
{
	map<int,Trade*>::iterator iter;
	
	//Handel suchen
	iter = m_trades->find(id);
	
	// Testen ob gefunden
	if (iter == m_trades->end())
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


int World::newTrade(int trader1_id, int trader2_id)
{
	// zufällige ID erzeugen;
	int id = 0;
	Trade* trade;
	while (id == 0)
	{
		id=rand();
		trade=getTrade(id);
		if (trade!=0)
		{
			id=0;
		}
	}
	
	// Neues Trade Objekt anlegen
	trade = new Trade(id, trader1_id,trader2_id);
	if (trade ==0)
	{
		return 0;
	}
	
	m_trades->insert(make_pair(id,trade));
	// ID ausgeben
	return id;
}


bool World:: getSWObjectsInShape( Shape* shape, short region, list<ServerWObject*>* result,short layer, short group, ServerWObject* omit )
{

	 
	// Region ermitteln, wenn gleich 0, Fehler ausgeben
	Region* r = m_regions[region];
	if (r == 0)
	{
		return false;
	}
	
	
	bool res = r->getSWObjectsInShape(shape,result,layer,group,omit);
	
	return res;
	 
}


ServerWObject* World::getSWObjectAt(float x_coordinate, float y_coordinate,  short region,short layer, short group )
{
	// Region ermitteln, wenn gleich 0, Fehler ausgeben
	Region* r = m_regions[region];
	if (r == 0)
	{
		ERRORMSG("cant find region %i",region);
		return false;
	}
	
	return r->getSWObjectAt(x_coordinate,y_coordinate,layer,group);
}

void World::getSWObjectsOnLine( float xstart, float ystart, float xend, float yend,  short region, list<ServerWObject*>* result,short layer, short group, ServerWObject* omit)
{
	// Region ermitteln, wenn gleich 0 beenden
	Region* r = m_regions[region];
	if (r == 0)
		return;
	
	r->getSWObjectsOnLine(xstart,ystart,xend,yend,result,layer,group,omit);
	
}


void World::getProjectilesOnScreen(float center_x,float center_y,short region, list<DmgProjectile*>* result)
{
	Region* r = m_regions[region];
	if (r==0)
		return ;
	
	r->getProjectilesOnScreen(center_x,center_y,result);
}


bool World::lineIntersect(float xstart, float ystart, float xend,float yend ,float dir[2],Shape* s)
{
	float p[2];
	float proj[2];
	float d;
	float l=sqr(xend-xstart) +sqr(yend-ystart);
	float cx = s->m_coordinate_x;
	float cy = s->m_coordinate_y;
	
	if (s->m_type == Shape::CIRCLE)
	{

		float r = s->m_radius;
		
		// testen des Abstandes zu den Endpunkten
		if (sqr(r)>sqr(cx-xstart)+sqr(cy-ystart))
				  return true;
		
		if (sqr(r)>sqr(cx-xend)+sqr(cy-yend))
				  return true;
		
		
		// Testen des Abstandes zur Linie
		p[0] = cx-xstart;
		p[1] = cy-ystart;
		d=p[0]*dir[0]+p[1]*dir[1];
		proj[0] = dir[0]*d;
		proj[1] = dir[1]*d;
		DEBUG5("proj on dir %f %f dist= %f %f",proj[0],proj[1],sqr(p[0]-proj[0])+sqr(p[1]-proj[1]),sqr(r));
		
		// Kreis liegt nicht auf der Linie, wenn der Abstand zu gross ist
		if (sqr(r)<sqr(p[0]-proj[0])+sqr(p[1]-proj[1]))
			return false;
		
		// Kreis muss ausserdem zwischen den zwei Punkten liegen
		// geringer Abstand zur Linie ist nicht ausreichend
		float l2 = sqr(proj[0])+sqr(proj[1]);
		if (l2>=0 && l2<=l)
			return true;
			
	}
	else
	{
		// testen ob Linie rechteck garantiert verfehlt
		if (xstart<cx-s->m_extent_x && xend<cx-s->m_extent_x || xstart>cx+s->m_extent_x && xend>cx+s->m_extent_x || ystart<cy-s->m_extent_y && yend<cy-s->m_extent_y || ystart>cy+s->m_extent_y && yend>cy+s->m_extent_y)
		{
			return false;
		}
		
		// Kreuzprodukte bilden zwischen der Richtung und Vektor zu zwei gegenueberliegenden Ecken
		// Auswahl des Eckenpaares so, dass die Diagonale gewaehlt wird, die einen grossen Winkel zur Linienrichtung hat
		// Kreuzprodukt muss einmal positiv, einmal negativ sein
		
		float p1,p2;
		if (dir[0]*dir[1]>0)
		{
			p1 = (cx-s->m_extent_x-xstart)*dir[1] - (cy+s->m_extent_y-ystart)*dir[0];
			p2 = (cx+s->m_extent_x-xstart)*dir[1] - (cy-s->m_extent_y-ystart)*dir[0];
			if (p1*p2<0)
				return true;
			
		}
		else
		{
			p1 = (cx+s->m_extent_x-xstart)*dir[1] - (cy+s->m_extent_y-ystart)*dir[0];
			p2 = (cx-s->m_extent_x-xstart)*dir[1] - (cy-s->m_extent_y-ystart)*dir[0];
			if (p1*p2<0)
				return true;
			
		}
	}
	return false;
}

/*
//TODO: neu implementieren
bool World::getClosestFreeSquare(float x_coordinate, float y_coordinate, float &x_ret_coordinate, float &y_ret_coordinate)
{
	bool result = true;
	float x =x_coordinate;
	float y =y_coordinate;
	
	// Startpunkt ist der eingegebene Punkt
	int px = 0;
	int py = 0;
	
	bool field[21][21];
	
	int i,j;
	for (i=0;i<=20;i++)
		for (j=0;j<=20;j++)
			field [i][j]=false;
	
	field[10][10]=true;
	
	// Queue für die durchsuchten Punkte
	queue<int>* qux = new queue<int>;
	queue<int>* quy = new queue<int>;
	
	// Liste für Objekte die im Weg sind
	list<ServerWObject*>* ret = new  list<ServerWObject*>;
	

	// Selektor für fixe Objekte
	ServerWObjectSelector* fixsel = new ServerWObjectSelector;
	fixsel->getObjectType().setObjectType(OBJECTTYPE_FIXED_OBJECT);
	fixsel->setSelectObjectType(true);
	
	
	// nach Objekten suchen die im Weg sind
	getServerWObjectsInRect(x+px-0.9999,y+py-0.9999,x+px+0.9999,y+py+0.99990,0, ret);
	
	qux->push(px);
	quy->push(py);	
	
	// Solange noch Punkte zum durchsuchen existieren und der aktuelle Punkt blockiert ist
	while (qux -> empty() != true && ret->empty()!=true)
	{
		ret->clear();
		px = qux->front();
		py = quy->front();
		qux->pop();
		quy->pop();
		
		// nach fixen Objekten suchen die im Weg sind
		getServerWObjectsInRect(x+px-0.9999,y+py-0.9999,x+px+0.9999,y+py+0.99990,fixsel, ret);
		
		DEBUG5("untersuche: %f , %f",x+px,y+py);

		// Wenn keine fixen Objekte im Weg
		if (ret->empty() || (px==0 && py ==0))
		{
			// allgemein nach Hindernissen suchen
			getServerWObjectsInRect(x+px-0.9999,y+py-0.9999,x+px+0.9999,y+py+0.99990,0, ret);
			
			// umliegende Felder in die Queue schieben, wenn sie noch nicht untersucht wurden
			if (px < 10 && field[px+11][py+10]==false)
			{
				field[px+11][py+10]=true;
				qux->push(px+1);
				quy->push(py);
			}
					
			if (px > -10 && field[px+9][py+10]==false)
			{
				field[px+9][py+10]=true;
				qux->push(px-1);
				quy->push(py);
			}
			
			if (py < 10 &&field[px+10][py+11]==false)
			{
				field[px+10][py+11]=true;
				qux->push(px);
				quy->push(py+1);
			}

			if (py >-10 && field[px+10][py+9]==false)
			{
				field[px+10][py+9]=true;
				qux->push(px);
				quy->push(py-1);
			}

			
		}	
	}
	
	if (qux->empty())
	{
		// keinen Punkt gefunden
		result=false;
	}
	else
	{
		// Ausgabekoordinaten setzen
		x_ret_coordinate = x+ px;
		y_ret_coordinate = y+ py;
	}
	
	delete ret;
	delete qux;
	delete quy;
	delete fixsel;
	
	return result;

}
*/


 bool World::insertSWObject (ServerWObject* object, float x, float y, short region) 
{
	DEBUG5("inserting Object at %f %f into region %i",x,y,region);
	 bool result=true;
	 
	 // Wenn NULL Zeiger übergeben -> Fehler anzeigen
	 if (object == 0)
		 return false;
	 
	 object->getGridLocation()->m_region = region;
	 object->getGeometry()->m_shape.m_coordinate_x=x;
	 object->getGeometry()->m_shape.m_coordinate_y=y;
	 
	 Region* r = m_regions[region];
	
	 if (r!=0)
	 {
	 	result &= r->insertSWObject(object,x,y);
	 }
	 else
	 {
		 return false;
	 }

	 return result;
}


bool World::insertPlayer(ServerWObject* player, int slot)
{
	if (slot != NOSLOT)
	{
		m_player_slots->insert(make_pair(slot,player));
	}
	m_players->insert(make_pair(player->getId(),player));
	
	/*
	DEBUG("all players: ");
	map<int,ServerWObject*>::iterator it;
	
	for (it = m_players->begin(); it != m_players->end(); ++it)
	{
		DEBUG("%s with id %i",it->second->getTypeInfo()->m_subtype.c_str(), it->second->getId());
	}
	*/
}


bool World::moveSWObject(ServerWObject* object, float x, float y)
{
	bool result;

	

	Region* r = m_regions[object->getGridLocation()->m_region];
	if (r==0)
	{
		return false;
	}
	
	result &= r->moveSWObject(object,x,y);
	
	return result;
}


 bool World::deleteSWObject (ServerWObject* object) {
	 
	 bool result=true;
	 
	 // Region ermitteln
	 Region* r = m_regions[object->getGridLocation()->m_region];
	 if (r==0)
		 return false;
	 
	 // Wenn NULL Zeiger übergeben -> Fehler anzeigen
	 if (object == 0)
		 return false;
	 
	 result &= r->deleteSWObject(object);
	 
	 /*
	 if (object->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
	 {
		 DEBUG("Player deleted");
		 m_players->erase(object->getId());
	 }
	 */
	
}


bool  World::insertProjectile(DmgProjectile* object, float x, float y, short region)
{
	m_regions[region]->insertProjectile(object,x,y);
	object->setRegion(region);
		
}

bool World::insertPlayerIntoRegion(ServerWObject* player, short region)
{
	Region* reg = m_regions[region];
	
	// Testen ob alle Daten vorhanden sind
	int data_missing =0;
	if (reg ==0)
	{
		data_missing =1;
	}
	

		 
	// Serverseite: Region erzeugen
	if (player->getState() != WorldObject::STATE_ENTER_REGION)
	{
		if (m_server)
		{
			if (data_missing !=0)
			{
				createRegion(region);
				reg = m_regions[region];
			}
			
			if (player == m_local_player)
			{
				// Spieler in die Region einfuegen
				player->setState(WorldObject::STATE_ENTER_REGION);
				DEBUG("player can enter region");
			}
			else 
			{
				// Auf Datenanfrage seitens des Client warten
				player->setState(WorldObject::STATE_REGION_DATA_WAITING);
				DEBUG("waiting for a client data request");
			}
			
			
		}
		else
		{
			// Clientseite
			if (player == m_local_player)
			{
				// Server nach den fehlenden Informationen fragen
				player->setState(WorldObject::STATE_REGION_DATA_REQUEST);
				
			}
			else
			{
				// anderer Spieler wurde in unbekannte Region eingefuegt, ignorieren
				player->setState(WorldObject::STATE_INACTIVE);
			}
		
		}
	}
	
	if (player->getState() == WorldObject::STATE_ENTER_REGION)
	{
		DEBUG("player %i entered region %i",player->getId(), region);
		// Daten sind vollständig
		float x,y;
		x = player->getGeometry()->m_shape.m_coordinate_x;
		y = player->getGeometry()->m_shape.m_coordinate_y;
		reg->getFreePlace(&(player->getGeometry()->m_shape),player->getGeometry()->m_layer , x, y);
		insertSWObject(player, x,y,region);
		player->setState(WorldObject::STATE_ACTIVE);
		
	}
}

void World::handleSavegame(CharConv *cv, int slot)
{
	DEBUG("got savegame from slot %i",slot);
	// Spieler aus dem Savegame erzeugen
	char binsave;
	cv->fromBuffer<char>(binsave);
	short version;
	cv->fromBuffer<short>(version);
	int len;
	cv->fromBuffer<int>(len);
	WorldObject::TypeInfo::ObjectSubtype ot;
	char tmp[11];
	tmp[10] = '\0';
	cv->fromBuffer(tmp,10);
	ot = tmp;
	ServerWObject* pl =0;

	DEBUG("type %s",tmp);	
	pl=ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_PLAYER, ot);
	
	// Spieler ist lokal
	if (slot == LOCAL_SLOT)
	{
		m_local_player = pl;
		
		if (!m_server)
		{
			// Savegame dem Server senden
			// Savegame an den Server senden
			PackageHeader header;
			header.m_content = PTYPE_C2S_SAVEGAME; 	// Savegame von Client zu Server
			header.m_number =1;
			CharConv save;
			header.toString(&save);
			
			save.toBuffer((char*) cv->getBitStream()->GetData(),len);
			m_network->pushSlotMessage(save.getBitStream());
		}
	}
	
	// Spieler zur Welt hinzufuegen
	if (pl!=0)
	{
		DEBUG("insert player");
		insertPlayer(pl,slot);
		// Daten aus dem Savegame laden
		
		pl->setState(WorldObject::STATE_ACTIVE);
		
		// Debugging: Region, Koordinaten setzen
		pl->getGridLocation()->m_region =0;
		pl->getGeometry()->m_shape.m_coordinate_x = 6;
		pl->getGeometry()->m_shape.m_coordinate_y = 11;
		
		insertPlayerIntoRegion(pl,pl->getGridLocation()->m_region);

		if (m_server)
		{
			map<int,ServerWObject*>::iterator it;
			
			if (slot != LOCAL_SLOT)
			{
				// Daten zur Initialisierung
				PackageHeader header3;
				header3.m_content =PTYPE_S2C_INITIALISATION;
				header3.m_number =1;
				
				CharConv msg2;
				header3.toString(&msg2);
				
				// die eigene ID auf Serverseite
				msg2.toBuffer(pl->getId());
				
				m_network->pushSlotMessage(msg2.getBitStream(),slot);
				
				// Dem Spieler Informationen ueber alle anderen Spieler in der Welt senden	
				PackageHeader header;
				header.m_content = PTYPE_S2C_PLAYER;		// Spielerdaten vom Server zum Client
				header.m_number = m_player_slots->size()-1;	// alle Spieler bis auf den eigenen
				
				CharConv msg;
				header.toString(&msg);
				
				// Informationen ueber die Spieler
				for (it = m_player_slots->begin(); it != m_player_slots->end(); ++it)
				{
					// Nur senden, wenn es nicht der eigene Spieler ist
					if (it->first != slot)
					{
						it->second->toString(&msg);
					}
				}
				
				// Nachricht an den Client senden
				if (header.m_number>0)
				{
					m_network->pushSlotMessage(msg.getBitStream(),slot);
				}
			}
			
			// Nachricht von dem neuen Spieler an alle anderen Spieler senden
			// ausser dem Spieler selbst und dem Server
			PackageHeader header2;
			header2.m_content = PTYPE_S2C_PLAYER;	// Spielerdaten vom Server zum Client
			header2.m_number = 1;					// der neue Spieler
			
			CharConv msg2;
			header2.toString(&msg2);
			pl->toString(&msg2);
			
			for (it = m_player_slots->begin(); it != m_player_slots->end(); ++it)
			{
				if (it->first != slot && it->first != LOCAL_SLOT)
				{
					m_network->pushSlotMessage(msg2.getBitStream(),it->first);
				}
			}
		}
	}
	

}


void World::handleCommand(ClientCommand* comm, int slot)
{
	DEBUG5("Kommando (%f %f) button: %i id: %i action: %i",comm->m_coordinate_x,comm->m_coordinate_y,comm->m_button, comm->m_id,comm->m_action);

	
	// Wenn man sich nicht auf Serverseite befindet
	if (!m_server)
	{
		// Kommando an den Server senden
		CharConv cv;

		// Header anlegen
		PackageHeader header;
		header.m_content = PTYPE_C2S_COMMAND; 	// Daten von Client zu Server
		header.m_number = 1;


		// Header in den Puffer schreiben
		header.toString(&cv);
		// Kommando in den Puffer schreiben
		comm->toString(&cv);
		


	 	// Datenpaket zum Server senden
		getNetwork()->pushSlotMessage(cv.getBitStream());
	}
	
	Player* pl = static_cast<Player*> ((*m_player_slots)[slot]);
	if (pl == 0)
	{
		ERRORMSG("no player in slot %i",slot);
	}
	else
	{
		pl->onClientCommand(comm);
	}
	
	
}

int World::getValidId()
{
	// zufällige ID erzeugen;
	static int j=1;
	return j++;
}


int World::getValidProjectileId()
{
	// zufällige ID erzeugen;
	return getValidId();
}


void World::update(float time)
{
	DEBUG5("update %f",time);
	for (int i=0;i<WORLD_MAX_REGIONS;i++)
	{
		if (m_regions[i]!=0)
		{
			m_regions[i]->update(time);
		}
	}
	// Durchmustern alle Handelsvorgänge
	
	map<int,Trade*>::iterator iter2;
	Trade* trade=0;
	
	for (iter2 =m_trades->begin(); iter2!=m_trades->end();)
	{
		trade = iter2->second;
		DEBUG5("Trades behandeln %p\n\n",trade);
		// Testen ob der Handel gelöscht werden kann
		
		if (trade->getFinished() == true)
		{
			if (trade->getSuccessful() == true)
			{
				// Handel war erfolgreich, löschen wenn beide Spieler ihre Gegenstaende erhalten haben
				if (trade->getGotItems(0)==true && trade->getGotItems(1)==true)
				{
					m_trades->erase(iter2++);
					delete trade;
					continue;
				}
			}
			else
			{
				// Handel war nicht erfolgreich, sofort löschen
				m_trades->erase(iter2++);
				delete trade;
				continue;
			}
		}
		++iter2;
	}
	DEBUG5("Trades behandeln abgeschlossen");
	
	m_network->update();
	
	
	if (m_server)
	{
		updateLogins();
		acceptLogins();
	}
	
	
	updatePlayers();
	
	m_network->update();
	
}

void World::updatePlayers()
{
	// Schleife ueber die Spieler
	map<int,ServerWObject*>::iterator it;
	Player* pl;
	int slot;
	for (it = m_player_slots->begin(); it != m_player_slots->end(); ++it)
	{
		slot = it->first;
		pl = static_cast<Player*>(it->second);
		
		if (pl->getState() == WorldObject::STATE_REGION_DATA_REQUEST)
		{
			DEBUG("send data request to server");
			// Client wartet auf Daten zur Region
			pl->setState(WorldObject::STATE_REGION_DATA_WAITING);
			
			// fehlende Daten zur Region anfordern
			PackageHeader header;
			header.m_content = PTYPE_C2S_DATA_REQUEST; 	// Data Request von Client zu Server
			header.m_number =1;
			
			ClientDataRequest datareq;
			datareq.m_data = ClientDataRequest::REGION_ALL;
			datareq.m_id = pl->getGridLocation()->m_region;
			
			CharConv msg;
			header.toString(&msg);
			datareq.toString(&msg);
			
			m_network->pushSlotMessage(msg.getBitStream());
		}
		
		if (pl->getState() == WorldObject::STATE_ENTER_REGION && pl->getRegion() !=0 )
		{
			insertPlayerIntoRegion(pl,pl->getGridLocation()->m_region);
			pl->setState(WorldObject::STATE_ACTIVE);
		}
		
		
		if (m_server && slot != -1)
		{
			// Nachrichten fuer die Spieler abholen und Verteilen
			PackageHeader headerp;
			Packet* data;
			CharConv* cv;
			
			if (m_network->getSlotStatus( slot )!=NET_CONNECTED)
			{
				// disconnect
			}
			else
			{
				while (m_network->numberSlotMessages( slot )>0)
				{
					m_network->popSlotMessage( data ,slot);
		
					cv = new CharConv(data);

					headerp.fromString(cv);
					
					if (headerp.m_content ==  PTYPE_C2S_COMMAND)
					{
						// Kommando Daten erhalten
						ClientCommand com;

						// Spielerobjekt die Daten senden
						com.fromString(cv);	
						
						handleCommand(&com,slot);
					}
					
					if (headerp.m_content == PTYPE_C2S_DATA_REQUEST)
					{
						// Datenanfrage erhalten
						ClientDataRequest req;
						req.fromString(cv);
							
						handleDataRequest(&req,slot);
					}
					
					delete cv;
				}
			}
		}
		
	}
	
	if (!m_server)
	{
		// Daten vom Server empfangen und verarbeiten
		if (m_network->getSlotStatus()!=NET_CONNECTED)
		{
			// disconnect
		}
		else
		{
			PackageHeader headerp;
			Packet* data;
			CharConv* cv;
			
			while (m_network->numberSlotMessages()>0)
			{
				m_network->popSlotMessage( data ,slot);
		
				cv = new CharConv(data);

				headerp.fromString(cv);
				
				if (headerp.m_content == PTYPE_S2C_PLAYER)
				{
					for (int n=0; n< headerp.m_number;n++)
					{
						// Daten zu Spielern erhalten
						// Typ Spieler (schon bekannt)
						char tmp;
						cv->fromBuffer(tmp);
						
						// Subtyp
						char subt[11];
						subt[10] ='\0';
						cv->fromBuffer(subt,10);
						
						int id;
						cv->fromBuffer(id);
						ServerWObject* player;
						
						DEBUG("got data for player %s id %i",subt,id);
						
						// Spieler entweder neu anlegen oder aus den existierenden herraussuchen
						if (m_players->count(id)==0)
						{
							// Spieler existiert noch nicht
							player = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_PLAYER, std::string(subt),id);
							insertPlayer(player);
						}
						else
						{
							player = (*m_players)[id];
						}
						
						// Daten aktualisieren
						player->fromString(cv);
						
					}
					
				}
				
				if (headerp.m_content == PTYPE_S2C_REGION)
				{
					// Daten zu einer Region erhalten
					DEBUG("got data for region %i",headerp.m_number);
					short dimx, dimy;
						
					// Groesse der Region
					cv->fromBuffer(dimx);
					cv->fromBuffer(dimy);
					
					// Region anlegen wenn sie noch nicht existiert
					if (m_regions[headerp.m_number] ==0)
					{
						m_regions[headerp.m_number] = new Region(dimx,dimy,headerp.m_number);	
					}
					
					// Daten schreiben
					m_regions[headerp.m_number]->setRegionData(cv,m_players);
					
					// lokalen Spieler fuer die Region freischalten
					m_local_player->setState(WorldObject::STATE_ENTER_REGION);
				}
				
				if (headerp.m_content == PTYPE_S2C_INITIALISATION)
				{
					int id;
					cv->fromBuffer(id);
					DEBUG("ID at server %i",id);
					m_players->clear();
					m_local_player->setId(id);
					insertPlayer(m_local_player, LOCAL_SLOT);
				}
				
				delete cv;
			}
		}
	}
}

void World::handleDataRequest(ClientDataRequest* request, int slot )
{
	// Spieler von dem die Anfrage ausging
	ServerWObject* player;

	// Spieler in dem betreffenden Slot aktivieren
	if (m_player_slots->count(slot)==0)
	{
		ERRORMSG("got request from an empty slot %i",slot);
		return;
	}
	else
	{
		player = (*m_player_slots)[slot];
	}
	
	if (request->m_data <= ClientDataRequest::REGION_ALL)
	{
		DEBUG("Daten zur Region %i gefordert",request->m_id);
		Region* region = m_regions[request->m_id];
		
		if (region!=0)
		{
			// Daten der Region senden
			PackageHeader header;
			header.m_content = PTYPE_S2C_REGION; 	
			header.m_number =request->m_id;
			
			CharConv msg;
			header.toString(&msg);
			
			region->getRegionData(&msg);
			
			m_network->pushSlotMessage(msg.getBitStream(),slot);
			
			player->setState(WorldObject::STATE_ENTER_REGION);
			
		}
		
	}
}

bool World::calcBlockmat(PathfindInfo * pathinfo)
{
	float sqs = pathinfo->m_base_size / pathinfo->m_quality;
	list<ServerWObject*> ret;
	list<ServerWObject*>::iterator it;
	int i,j,is,ie,js,je;
	Shape s;
	s.m_coordinate_x = pathinfo->m_center_x;
	s.m_coordinate_y = pathinfo->m_center_y;
	s.m_type = Shape::RECT;
	s.m_extent_y =s.m_extent_x = 0.5*sqs * pathinfo->m_dim;
	float c_x = pathinfo->m_center_x-s.m_extent_x;
	float c_y = pathinfo->m_center_y-s.m_extent_y;
	float e_x = pathinfo->m_center_x+s.m_extent_x;
	float e_y = pathinfo->m_center_y+s.m_extent_y;
	float rezsqs = 1/ sqs;
	float hb = pathinfo->m_base_size * 0.5;
	
	DEBUG5("Calc blockmat %f %f", pathinfo->m_center_x,pathinfo->m_center_y);
	
	Shape s2;
	s2.m_type = Shape::CIRCLE;
	s2.m_radius = hb;
	
	Shape* wos=0;
	
	DEBUG5("parameter %f %f sqs %f hb %f",c_x,c_y,sqs,hb);
	
	// Alle Objekte in dem Gebiet suchen
	getSWObjectsInShape(&s, pathinfo->m_region, &ret,pathinfo->m_layer);
	ServerWObject* wo=0;
	pathinfo->m_block->clear();
	
	// durchmustern der Objekte
	for (it = ret.begin(); it!=ret.end(); ++it)
	{
		wo = *it;
		// eigenes Objekt nicht mit einbeziehen
		if (wo->getId() != pathinfo->m_id)
		{
			DEBUG5("found obstacle %i",wo->getId());
			wos = &(wo->getGeometry()->m_shape);
			DEBUG5("shape %i %f %f %f",wos->m_type,wos->m_radius,wos->m_extent_x,wos->m_extent_y);
			

			
			// Rand des Objektes mit X zeichnen
			js = (int) floor((wos->m_coordinate_y-c_y)/sqs);
			is = (int) floor((wos->m_coordinate_x-c_x)/sqs);
			is = max(min(is,pathinfo->m_dim-1),0);
			js = max(min(js,pathinfo->m_dim-1),0);
			s2.m_coordinate_y = (js+0.5)*sqs+c_y;
			s2.m_coordinate_x = (is+0.5)*sqs+c_x;
			
			
			DEBUG5("start %i %i %f %f",is,js,s2.m_coordinate_x,s2.m_coordinate_y);
			float dir[12][2] = {{sqs,sqs},{sqs,0},{sqs,-sqs},{0,-sqs},{-sqs,-sqs},{-sqs,0},{-sqs,sqs},{0,sqs},{sqs,sqs},{sqs,0},{sqs,-sqs},{0,-sqs}};
			int idir[12][2] = {{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1}};
			
			float x,y;
			while (s2.m_coordinate_y< e_y && intersect(&s2, wos))
			{
				s2.m_coordinate_y += sqs;
				js++;
			}
			js--;
			s2.m_coordinate_y -= sqs;
			int d=1;
			char val = 9;
			if (wo->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_MONSTER || wo->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
			{
				if (sqr(wo->getMoveInfo()->m_speed_x) + sqr(wo->getMoveInfo()->m_speed_y) == 0)
				{
					// fest stehendes Lebewesen
					val = 6;
				}
				else
				{
					// sich bewegendes Lebewesen
					val = 4;
				}
			}
			
			
			 
			if (*(pathinfo->m_block->ind(is,js))<val)
				*(pathinfo->m_block->ind(is,js)) = val;
			
			DEBUG5("start2 %i %i %f %f",is,js,s2.m_coordinate_x,s2.m_coordinate_y);
			while (d<10)
			{
				x = s2.m_coordinate_x;
				y = s2.m_coordinate_y;

				s2.m_coordinate_x += dir[d][0];
				s2.m_coordinate_y += dir[d][1];
				while ((d<=10) && (s2.m_coordinate_x<c_x || s2.m_coordinate_y<c_y ||
				s2.m_coordinate_x>e_x || s2.m_coordinate_y>e_y))
				{
					d++;
					s2.m_coordinate_x = x+dir[d][0];
					s2.m_coordinate_y = y+dir[d][1];
				}
				
				if (d==10)
					break;
				
				if (intersect(&s2, wos))
				{
					DEBUG5("intersecting: %i %i %f %f",is+idir[d][0],js+idir[d][1],s2.m_coordinate_x,s2.m_coordinate_y);
					d--;
					s2.m_coordinate_x = x+dir[d][0];
					s2.m_coordinate_y = y+dir[d][1];
					if (s2.m_coordinate_x<c_x || s2.m_coordinate_y<c_y ||
						 s2.m_coordinate_x>e_x || s2.m_coordinate_y>e_y || !intersect(&s2, wos))
					{
						d++;
						s2.m_coordinate_x = x+dir[d][0];
						s2.m_coordinate_y = y+dir[d][1];
					}
				}
				else
				{
					DEBUG5("not intersecting: %i %i %f %f",is+idir[d][0],js+idir[d][1],s2.m_coordinate_x,s2.m_coordinate_y);
					do
					{
						d++;
						s2.m_coordinate_x = x+dir[d][0];
						s2.m_coordinate_y = y+dir[d][1];
					} while (d<=10 && (s2.m_coordinate_x<c_x || s2.m_coordinate_y<c_y ||
											 s2.m_coordinate_x>e_x || s2.m_coordinate_y>e_y || !intersect(&s2, wos)));

				}
				if (d<10)
				{
					DEBUG5("d: %i",d);
					is += idir[d][0];
					js += idir[d][1];
					DEBUG5("intersecting: %i %i %f %f",is,js,s2.m_coordinate_x,s2.m_coordinate_y);
					if (*(pathinfo->m_block->ind(is,js))<val)
						*(pathinfo->m_block->ind(is,js)) = val;
					
					/*
					cout << "  001122334455667788990011223344556677889900\n";
	
					for (j=pathinfo->m_dim-1;j>=0;j--)
					{
						cout << j%10 << j%10;
						for (i=0;i<pathinfo->m_dim;i++)
						{
			
							if (*(pathinfo->m_block->ind(i,j))!=0)
							{
								if (*(pathinfo->m_block->ind(i,j))=='X')
									cout << *(pathinfo->m_block->ind(i,j)) << *(pathinfo->m_block->ind(i,j));
								else
									cout << *(pathinfo->m_block->ind(i,j))+'0' << *(pathinfo->m_block->ind(i,j))+'0';
							}
							else
								cout << "  ";
						}
						cout << "\n";
					}
					cout << "---------------------\n";
	
					char c;
					cin >> c;
					*/
				}
			}
			// Die jenigen Felder alsblockiert markieren, auf deren Mittelpunkt kein Objekt der Grösse base_size Platz hat
			/*
			for (i=is; i<=ie;i++)
			{
				for (j=js;j<=je; j++)
				{
					s2.m_coordinate_x = (i+0.5) * sqs + c_x;
					s2.m_coordinate_y = (j+0.5) * sqs + c_y;
					if (intersect(&s2, wos ))
						*(pathinfo->m_block->ind(i,j)) = 'X';
					
				}
			}
			*/
		}
	}
	/*
	cout << "  001122334455667788990011223344556677889900\n";
	
	for (j=pathinfo->m_dim-1;j>=0;j--)
	{
		cout << j%10 << j%10;
		for (i=0;i<pathinfo->m_dim;i++)
		{
			
			if (*(pathinfo->m_block->ind(i,j))!=0)
			{
				if (*(pathinfo->m_block->ind(i,j))=='X')
					cout << *(pathinfo->m_block->ind(i,j)) << *(pathinfo->m_block->ind(i,j));
				else
					cout <<(char) ( *(pathinfo->m_block->ind(i,j))+'0') << (char) (*(pathinfo->m_block->ind(i,j))+'0');
			}
			else
				cout << "  ";
		}
		cout << "\n";
	}
	cout << "---------------------\n";
	*/
	return true;
}


bool World::calcPotential(PathfindInfo* pathinfo)
{
	//TODO: vordefinierte Felder benutzen
	
	float sqs = pathinfo->m_base_size / pathinfo->m_quality;
	float l = 0.5*sqs * pathinfo->m_dim;
	SearchField * sf = new SearchField(pathinfo->m_dim);
	sf->init(pathinfo->m_pot, pathinfo->m_block);
	int sx = (int) ((pathinfo->m_start_x - pathinfo->m_center_x + l) / sqs);
	int sy = (int) ((pathinfo->m_start_y - pathinfo->m_center_y + l) / sqs);
	DEBUG5("goal: %f %f",pathinfo->m_start_x,pathinfo->m_start_y);
	DEBUG5("Calc potential %i %i", sx,sy);
	
	sf->createPotential(sx,sy);
	delete sf;
	return true;
	
}



void World::calcPathDirection(PathfindInfo* pathinfo, float x_start, float y_start, float dir[2])
{
	float sqs = pathinfo->m_base_size / pathinfo->m_quality;
	float l = 0.5*sqs * pathinfo->m_dim;
	if (fabs(x_start-pathinfo->m_center_x) >l  || fabs(y_start-pathinfo->m_center_y) >l)
	{
		dir[0] = pathinfo->m_start_x -  x_start;
		dir[1] = pathinfo->m_start_y -  y_start;
	}
	else
	{
		int sx = (int) ((x_start - pathinfo->m_center_x + l) / sqs);
		int sy = (int) ((y_start - pathinfo->m_center_y + l) / sqs);
		DEBUG5("Calc direction from %i %i", sx,sy);
		SearchField::getGradient(pathinfo->m_pot,pathinfo->m_block,pathinfo->m_dim, dir, sx, sy);
	}
}




