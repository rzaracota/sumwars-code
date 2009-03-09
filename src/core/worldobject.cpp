#include "worldobject.h"
#include "world.h"
#include "eventsystem.h"
#include "objectfactory.h"

WorldObject::WorldObject( int id)
{
	m_id = id;
	m_state = STATE_ACTIVE;
	init();
	m_event_mask=0;
	
	m_grid_location.m_region = -1;
	m_fraction = NOFRACTION;
	m_category = NOCATEGORY;
}

bool WorldObject::isCreature()
{
	return m_type_info.m_type != TypeInfo::TYPE_FIXED_OBJECT;
}

bool WorldObject::moveTo(Vector newpos)
{
	if (World::getWorld()==0 || getRegion()==0)
	{
		m_shape.m_center = newpos;
		
		return true;
	}
	else
	{
		return getRegion()->moveObject(this, newpos);
	}
}

Region* WorldObject::getRegion()
{
	if (World::getWorld() == 0)
		return 0;
	
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
#ifndef WIN32
	return ObjectFactory::getObjectName(m_type_info.m_subtype);
#else
	return m_type_info.m_subtype;
#endif
	
}


string WorldObject::getNameId()
{
	std::ostringstream out_stream;

	out_stream.str("");
	out_stream << m_subtype << ":" << getId();
	return out_stream.str();
}


int WorldObject::getValue(std::string valname)
{
		if (valname =="id")
		{
			lua_pushinteger(EventSystem::getLuaState() , m_id );
			return 1;
		}
		else if (valname == "subtype")
		{
			lua_pushstring(EventSystem::getLuaState() ,m_type_info.m_subtype.c_str() );
			return 1;
		}
		else if (valname == "position")
		{
			EventSystem::pushVector(EventSystem::getLuaState(),getShape()->m_center);
			return 1;
		}
		else if (valname == "type")
		{
			if (m_type_info.m_type == TypeInfo::TYPE_MONSTER)
				lua_pushstring(EventSystem::getLuaState(),"monster");
			
			if (m_type_info.m_type == TypeInfo::TYPE_PLAYER)
				lua_pushstring(EventSystem::getLuaState(),"player");
			
			if (m_type_info.m_type == TypeInfo::TYPE_FIXED_OBJECT)
				lua_pushstring(EventSystem::getLuaState(),"fixed_object");
			
			return 1;
		}

		return 0;
}


bool WorldObject::setValue(std::string valname)
{
	return false;
}
