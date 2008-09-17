#include "worldobject.h"

void WorldObject::toString(CharConv* cv)
{
	DEBUG5("worldobject::tostring");

	cv->toBuffer((char) m_type_info.m_type);
	char stmp[11];
	stmp[10] = '\0';
	strncpy(stmp,m_type_info.m_subtype.c_str(),10);
	cv->toBuffer(stmp,10);
	cv->toBuffer(m_id);
	cv->toBuffer(m_geometry.m_shape.m_coordinate_x);
	cv->toBuffer(m_geometry.m_shape.m_coordinate_y);
	cv->toBuffer((char) m_geometry.m_shape.m_type);
	if (m_geometry.m_shape.m_type==Shape::RECT)
	{
		cv->toBuffer(m_geometry.m_shape.m_extent_x);
		cv->toBuffer(m_geometry.m_shape.m_extent_y);
	}
	else
	{
		cv->toBuffer(m_geometry.m_shape.m_radius);
		cv->toBuffer(m_geometry.m_shape.m_radius);
	}
	cv->toBuffer((char) m_geometry.m_layer);
	cv->toBuffer(m_geometry.m_angle);

	
}

void WorldObject::fromString(CharConv* cv)
{
	short tmp;
	char ctmp;
	cv->fromBuffer<char>(ctmp);
	m_type_info.m_type = (TypeInfo::ObjectType) ctmp;
	char stmp[11];
	stmp[10] ='\0';
	cv->fromBuffer(stmp,10);
	m_type_info.m_subtype = stmp;	
	cv->fromBuffer<int>(m_id);
	cv->fromBuffer<float>(m_geometry.m_shape.m_coordinate_x) ;
	cv->fromBuffer<float>(m_geometry.m_shape.m_coordinate_y);
	cv->fromBuffer<char>(ctmp);
	m_geometry.m_shape.m_type = (Shape::ShapeType) ctmp;
	if (m_geometry.m_shape.m_type==Shape::RECT)
	{
		cv->fromBuffer<float>(m_geometry.m_shape.m_extent_x);	
		cv->fromBuffer<float>(m_geometry.m_shape.m_extent_y);
	}
	else
	{
		cv->fromBuffer<float>(m_geometry.m_shape.m_radius);
		cv->fromBuffer<float>(m_geometry.m_shape.m_radius);
	}
	cv->fromBuffer<char>(ctmp);
	m_geometry.m_layer  = (Geometry::Layer) ctmp;
	cv->fromBuffer<float>(m_geometry.m_angle);
	
}

string WorldObject::getName()
{
	/*
	if (m_type_info.m_type==TypeInfo::TYPE_MONSTER)
	{
		switch (m_type_info.m_subtype)
		{
			case TypeInfo::SUBTYPE_GOBLIN:
				return "Goblin";
				break;
		}
		return "Monster";
	}
	else if (m_type_info.m_type==TypeInfo::TYPE_PLAYER)
	{
		return "Player";
	}
	else if (m_type_info.m_type==TypeInfo::TYPE_FIXED_OBJECT)
	{
		return "Fixed";
	}
	*/
	
	return m_type_info.m_subtype;
}


string WorldObject::getNameId()
{
	ostringstream out_stream;
	
	out_stream.str("");
	out_stream << getName() << ":" << getId();
	return out_stream.str();
}



