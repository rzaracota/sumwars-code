#include "scriptobject.h"
#include "eventsystem.h"

ScriptObject::ScriptObject(int id)
	: WorldObject(id)
{
	std::stringstream stream;
	stream << "scriptobjectvar["<<getId()<<"]={}";
	EventSystem::doString(stream.str().c_str());
	
	setState(STATE_ACTIVE);
	setType("SCRIPTOBJECT");
	
	m_animation ="";
	m_animation_time =1;
	m_animation_elapsed_time = 0;
	m_animation_repeat =false;
	
}

ScriptObject::~ScriptObject()
{
	std::stringstream stream;
	stream << "scriptobjectvar["<<getId()<<"]=nil";
	EventSystem::doString(stream.str().c_str());
}

bool ScriptObject::takeDamage(Damage* damage)
{
	WorldObject::takeDamage(damage);
	return true;
}

int ScriptObject::getValue(std::string valname)
{
	
	int ret = WorldObject::getValue(valname);
	if (ret ==0)
	{
		std::stringstream stream;
		stream << "return scriptobjectvar["<<getId()<<"]['"<<valname<<"']";
		EventSystem::doString(stream.str().c_str());
		return 1;
	}
	
	return ret;
}

bool ScriptObject::setValue(std::string valname)
{
	bool ret = false;
	if (valname == "speed")
	{
		Vector speed = EventSystem::getVector(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		DEBUG("speed %f %f",speed.m_x,speed.m_y);
		setSpeed(speed);
		return 1;
	}
	else
	{
	 	ret = WorldObject::setValue(valname);
	}
	
	if (ret ==false)
	{
		std::string value  = lua_tostring(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		std::stringstream stream;
		stream << "scriptobjectvar["<<getId()<<"]['"<<valname<<"'] = "<<value;
		EventSystem::doString(stream.str().c_str());
		return true;
	}
	
	return ret;
}

bool  ScriptObject::update ( float time)
{
	getShape()->m_center += getSpeed()*time;
	
	if (m_animation != "")
	{
		m_animation_elapsed_time +=time;
		
		if (m_animation_elapsed_time > m_animation_time)
		{
			if (m_animation_repeat)
			{
				m_animation_elapsed_time -= m_animation_time;
			}
			else
			{
				m_animation ="";
				m_animation_time =1;
				m_animation_elapsed_time = 0;
			}
		}
	}
	
	return true;
}

void ScriptObject::setAnimation(std::string anim, float time, bool repeat)
{
	m_animation = anim; 
	m_animation_elapsed_time =0;
	m_animation_time = time;
	m_animation_repeat = repeat;
	
	addToNetEventMask(NetEvent::DATA_ACTION);
}


void ScriptObject::writeNetEvent(NetEvent* event, CharConv* cv)
{
	if (event->m_data & NetEvent::DATA_ACTION)
	{
		cv->toBuffer(m_animation);
		cv->toBuffer(m_animation_time);
		cv->toBuffer(m_animation_repeat);
	}
}


void ScriptObject::processNetEvent(NetEvent* event, CharConv* cv)
{
	if (event->m_data & NetEvent::DATA_ACTION)
	{
		cv->fromBuffer(m_animation);
		cv->fromBuffer(m_animation_time);
		cv->fromBuffer(m_animation_repeat);
		m_animation_elapsed_time = 0;
	}
}

