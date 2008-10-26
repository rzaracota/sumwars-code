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
	getTypeInfo()->m_type = TypeInfo::TYPE_FIXED_OBJECT;
	getTypeInfo()->m_subtype = "spawnpoint";
	m_layer = LAYER_SPECIAL;

	setState(STATE_ACTIVE);

	m_time =0;


	return true;
}

void Spawnpoint::setRespawnTime(float t)
{
	m_respawn_time = t;
}

bool Spawnpoint::update (float time)
{
	m_time -= time;
	
	if (m_time <= 0)
	{
		// Zeit abgelaufen: Monster generieren
		getRegion()->createMonsterGroup(m_spawned_monsters, getShape()->m_center);

		m_time += m_respawn_time;
	}
	return true;
}

