#include "projectile.h"

Projectile::Projectile( ProjectileType type, int id)
{
	m_id = id;
	m_type = type;
	m_timer =0;
	m_timer_limit =0;
	m_counter =0;
	m_geometry.m_layer = WorldObject::Geometry::LAYER_AIR;

}

Projectile::Projectile(int id)
{
	m_id = id;
}



