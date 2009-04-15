#include "gameobject.h"
#include "world.h"
#include "region.h"

GameObject::GameObject( int id)
{
	if (id !=0)
	{
		m_id = id;
	}
	else
	{
		if (World::getWorld() !=0)
		{
			m_id = World::getWorld()->getValidId();
		}
		else
			m_id =0;
	}
	
	m_state = STATE_ACTIVE;
	m_event_mask=0;
	m_region_id = -1;
}


void GameObject::setState(State s, bool throwevent)
{
	m_state = s;
	
	// einige Zustaende werden nicht ueber das Netzwerk angeglichen
	if (throwevent)
	{
		addToNetEventMask(NetEvent::DATA_STATE);
	}
}

void  GameObject::setType(Type type)
{
	m_type = type;
}

void GameObject::setSubtype(Subtype subtype)
{
	m_subtype = subtype;
}

void GameObject::setLayer(short layer)
{
	m_layer = layer;
}

Region* GameObject::getRegion()
{
	if (World::getWorld() == 0)
		return 0;

	return World::getWorld()->getRegion(m_region_id);
}

void GameObject::setSpeed(Vector speed, bool throwevent)
{
	m_speed =speed;
	if (m_speed.getLength() >0.001f)
	{
		getShape()->m_angle = m_speed.angle();
	}
	if (throwevent)
	{
		m_event_mask |= NetEvent::DATA_SPEED;
	}
}

void GameObject::setAngle(float angle)
{
	m_shape.m_angle = angle;
}

void  GameObject::setPosition(Vector pos)
{
	m_shape.m_center = pos;
}

void GameObject::toString(CharConv* cv)
{
	cv->toBuffer(m_type);
	cv->toBuffer(m_subtype);
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

void GameObject::fromString(CharConv* cv)
{
	char ctmp;

	// Typ, Subtyp und ID werden extern gelesen
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

string GameObject::getName()
{
	return ObjectFactory::getObjectName(m_subtype);
}


string GameObject::getNameId()
{
	std::ostringstream out_stream;

	out_stream.str("");
	out_stream << m_subtype << ":" << getId();
	return out_stream.str();
}

void GameObject::writeNetEvent(NetEvent* event, CharConv* cv)
{
	
	if (event->m_data & NetEvent::DATA_TYPE)
	{
		cv->toBuffer(m_type);
		cv->toBuffer(m_subtype);
	}
	
	if (event->m_data & NetEvent::DATA_SHAPE)
	{
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
	}
	
	/*
	if (event->m_data & NetEvent::DATA_STATE)
	{
		cv->toBuffer((char) getState());
	}
	
	if (event->m_data & NetEvent::DATA_SPEED)
	{
		cv->toBuffer(getSpeed().m_x);
		cv->toBuffer(getSpeed().m_y);
	}
*/
}

void GameObject::processNetEvent(NetEvent* event, CharConv* cv)
{
	
	if (event->m_data & NetEvent::DATA_TYPE)
	{
		cv->fromBuffer(m_type);
		cv->fromBuffer(m_subtype);
	}
	
	if (event->m_data & NetEvent::DATA_SHAPE)
	{
		
		char ctmp;

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
	}
	
	/*
	if (event->m_data & NetEvent::DATA_STATE)
	{
		char ctmp;
		cv->fromBuffer(ctmp);
		setState((State) ctmp);
		DEBUG5("object %i changed state to %i",getId(),ctmp);
	}
	
	if (event->m_data & NetEvent::DATA_SPEED)
	{
		cv->fromBuffer(m_speed.m_x);
		cv->fromBuffer(m_speed.m_y);

	}
	*/
}


void GameObject::addToNetEventMask(int mask)
{
	m_event_mask |= mask;
}
