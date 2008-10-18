#include "spawnpoint.h"

Spawnpoint::Spawnpoint( int id) : WorldObject( id)
{
	Spawnpoint::init();

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

void Spawnpoint::addMonsterGroup(SpawnedMonsterGroup& mgroup)
{
	m_spawned_groups.push_back(mgroup);
}

void Spawnpoint::setRespawnTime(float t)
{
	m_respawn_time = t;
}

bool Spawnpoint::update (float time)
{
	m_time -= time;
	int i;
	WorldObject* wo;
	float x,y;
	bool ret;
	Vector pos;
	if (m_time <= 0)
	{
		// Zeit abgelaufen: Monster generieren

		// Schleife ueber die Monstergruppen
		std::list<SpawnedMonsterGroup>::iterator it;
		for (it = m_spawned_groups.begin(); it !=  m_spawned_groups.end(); it++)
		{
			// Schleife ueber die Monster
			for (i=0;i<it->m_number;i++)
			{
				if (Random::random()<it->m_prob)
				{
					DEBUG5("spawning %i monster of type %s at prob %f",it->m_number,it->m_subtype.c_str(),it->m_prob);

					// Monster erzeugen
					wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, it->m_subtype);
					pos = getShape()->m_center;
					

					// freien Platz suchen
					ret = getRegion()->getFreePlace(wo->getShape(),wo->getLayer() , pos);
					if (ret == false)
					{
						ERRORMSG("keinen Platz gefunden um das Monster %s zu platzieren",wo->getNameId().c_str());
						delete wo;
					}
					else
					{
						World::getWorld()->insertObject(wo, pos,getGridLocation()->m_region);
					}
				}
			}
		}

		m_time += m_respawn_time;
	}
	return true;
}

