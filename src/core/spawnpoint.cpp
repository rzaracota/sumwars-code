#include "spawnpoint.h"

Spawnpoint::Spawnpoint(World* world, int id) : ServerWObject(world, id)
{
	bool tmp=Spawnpoint::init();
	
}


//Methods
bool Spawnpoint::init()
{
	//eigene Initialisierung
	getTypeInfo()->m_type = TypeInfo::TYPE_FIXED_OBJECT;
	getTypeInfo()->m_subtype = "spawnpoint";
	getGeometry()->m_layer = Geometry::LAYER_SPECIAL;
	
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
	ServerWObject* wo;
	float x,y;
	bool ret;
	if (m_time <= 0)
	{
		// Zeit abgelaufen: Monster generieren
		
		// Schleife ueber die Monstergruppen
		list<SpawnedMonsterGroup>::iterator it;
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
					x= getGeometry()->m_shape.m_coordinate_x;
					y= getGeometry()->m_shape.m_coordinate_y;
					
					// freien Platz suchen
					ret = getRegion()->getFreePlace(&(wo->getGeometry()->m_shape),wo->getGeometry()->m_layer , x, y);
					if (ret == false)
					{
						ERRORMSG("keinen Platz gefunden um das Monster %s zu platzieren",wo->getNameId().c_str());
						delete wo;
					}
					else
					{
						getWorld()->insertSWObject(wo, x,y,getGridLocation()->m_region);
					}
				}
			}
		}
		
		m_time += m_respawn_time;
	}
}

