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

void Projectile::toString(CharConv* cv)
{

	cv->toBuffer((char) m_type);
	cv->toBuffer((char) m_state);
	cv->toBuffer( m_id);
	cv->toBuffer(m_geometry.m_coordinate_x);
	cv->toBuffer(m_geometry.m_coordinate_y);
	cv->toBuffer(m_geometry.m_radius);
	cv->toBuffer(m_geometry.m_angle);
	cv->toBuffer(m_geometry.m_layer);
	cv->toBuffer(m_timer);
	cv->toBuffer(m_timer_limit);

}

void Projectile::fromString(CharConv* cv)
{
	
	char tmp;
	cv->fromBuffer<char>(tmp);
	m_type = (ProjectileType) tmp;
	cv->fromBuffer<char>(tmp);
	m_state = (ProjectileState) tmp;
	cv->fromBuffer<int>(m_id);
	cv->fromBuffer<float>(m_geometry.m_coordinate_x);
	cv->fromBuffer<float>(m_geometry.m_coordinate_y);
	cv->fromBuffer<float>(m_geometry.m_radius);
	cv->fromBuffer<float>(m_geometry.m_angle);
	cv->fromBuffer<short>(m_geometry.m_layer);
	cv->fromBuffer<float>(m_timer);
	cv->fromBuffer<float>(m_timer_limit);
		

}


