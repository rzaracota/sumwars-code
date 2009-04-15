#include "spawnpoint.h"

Spawnpoint::Spawnpoint( MonsterGroupName name,int id) : WorldObject( id)
{
	Spawnpoint::init();
	m_spawned_monsters = name;
	
	m_respawn_time = 1e20;
}


//Methods
bool Spawnpoint::init()
{
	//eigene Initialisierung
	setType("FIXED_OBJECT");
	setSubtype("spawnpoint");
	setLayer(LAYER_SPECIAL);

	setState(STATE_ACTIVE,false);

	m_time =0;


	return true;
}

void Spawnpoint::setRespawnTime(float t)
{
	m_respawn_time = t;
}

bool Spawnpoint::update (float time)
{
	Shape s;
	s.m_center = getShape()->m_center;
	s.m_type = Shape::CIRCLE;
	s.m_radius = 40;
	
	WorldObjectList res;
	getRegion()->getObjectsInShape (&s, &res, LAYER_ALL, PLAYER);
	
	m_time -= time;
	
	if (!res.empty())
	{
		if (m_time <= 0)
		{
			// Zeit abgelaufen: Monster generieren
			getRegion()->createMonsterGroup(m_spawned_monsters, getShape()->m_center);
	
			m_time += m_respawn_time;
		}
	}
	return true;
}

