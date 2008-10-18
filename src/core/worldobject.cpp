#include "worldobject.h"
#include "world.h"

WorldObject::WorldObject( int id)
{
	m_id = id;
	m_state = STATE_ACTIVE;
	init();
}

bool WorldObject::moveTo(Vector newpos)
{
	if (World::getWorld()==0)
	{
		m_shape.m_center = newpos;
		
		return true;
	}
	else
	{
		return World::getWorld()->moveObject(this, newpos);
	}
}

Region* WorldObject::getRegion()
{
	return World::getWorld()->getRegion(m_grid_location.m_region);
}

bool  WorldObject::destroy()
{
	return true;
}

void WorldObject::toString(CharConv* cv)
{
	DEBUG5("worldobject::tostring");

	cv->toBuffer((char) m_type_info.m_type);
	char stmp[11];
	stmp[10] = '\0';
	strncpy(stmp,m_type_info.m_subtype.c_str(),10);
	cv->toBuffer(stmp,10);
	cv->toBuffer(m_id);
	cv->toBuffer(m_shape.m_center.m_x);
	cv->toBuffer(m_shape.m_center.m_y);
	cv->toBuffer((char) m_shape.m_type);
	if (m_shape.m_type==Shape::RECT)
	{
		cv->toBuffer(m_shape.m_extent.m_x);
		cv->toBuffer(m_shape.m_extent.m_y);
	}
	else
	{
		cv->toBuffer(m_shape.m_radius);
		cv->toBuffer(m_shape.m_radius);
	}
	cv->toBuffer((char) m_layer);
	cv->toBuffer(m_shape.m_angle);

	cv->toBuffer((char) m_state);

}

void WorldObject::fromString(CharConv* cv)
{
	char ctmp;

	// Typ, Subtyp und ID werden extern gelesen
	/*
	cv->fromBuffer<char>(ctmp);
	m_type_info.m_type = (TypeInfo::ObjectType) ctmp;

	char stmp[11];
	stmp[10] ='\0';
	cv->fromBuffer(stmp,10);
	m_type_info.m_subtype = stmp;


	cv->fromBuffer<int>(m_id);

	*/


	cv->fromBuffer<float>(m_shape.m_center.m_x) ;
	cv->fromBuffer<float>(m_shape.m_center.m_y);
	cv->fromBuffer<char>(ctmp);
	m_shape.m_type = (Shape::ShapeType) ctmp;
	if (m_shape.m_type==Shape::RECT)
	{
		cv->fromBuffer<float>(m_shape.m_extent.m_x);
		cv->fromBuffer<float>(m_shape.m_extent.m_y);
	}
	else
	{
		cv->fromBuffer<float>(m_shape.m_radius);
		cv->fromBuffer<float>(m_shape.m_radius);
	}
	cv->fromBuffer<char>(ctmp);
	m_layer  = (Layer) ctmp;
	cv->fromBuffer<float>(m_shape.m_angle);

	cv->fromBuffer<char>(ctmp);
	m_state = (State) ctmp;
}

string WorldObject::getName()
{

	return m_type_info.m_subtype;
}


string WorldObject::getNameId()
{
	std::ostringstream out_stream;

	out_stream.str("");
	out_stream << getName() << ":" << getId();
	return out_stream.str();
}



