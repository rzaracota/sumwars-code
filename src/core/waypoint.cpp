#include "waypoint.h"
#include "world.h"
#include "player.h"

Waypoint::Waypoint( int id) : FixedObject( id)
{
	init();
}

bool Waypoint::init ()
{
	FixedObject::init();
	
	getShape()->m_type = Shape::CIRCLE;
	getShape()->m_radius = 0.5;
	setLayer(LAYER_BASE | LAYER_AIR);
	
	setState(STATE_ACTIVE,false);
	setSubtype("waypoint");
	return true;
}

bool  Waypoint::update (float time)
{
	if (World::getWorld()->isServer())
	{
		// alle Spieler im Umkreis von 12m suchen
		WorldObjectMap *players = getRegion()->getPlayers();
		WorldObjectMap::iterator it;
		
		Shape s;
		s.m_type = Shape::CIRCLE;
		s.m_center = getShape()->m_center;
		s.m_radius = 4;
		
		Player* pl;
		
		for (it = players->begin(); it != players->end(); ++it)
		{
			pl = dynamic_cast<Player*>(it->second);
			if (pl !=0 && pl->getShape()->intersects(s))
			{
				// Spieler ist in der Naehe des Wegpunktes, aktivieren
				pl->addWaypoint(getRegion()->getId(),true);
			}
		}
	}
	return true;
}


bool Waypoint::reactOnUse(int id)
{
	if (World::getWorld()->isServer())
	{
		Player* pl;
		pl = dynamic_cast<Player*>(World::getWorld()->getPlayer(id));
		
		pl->setUsingWaypoint(true);
	}
	return true;
}

