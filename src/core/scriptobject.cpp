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
	
	std::multimap<TriggerType, Event*>::iterator it;
	for (it = m_events.begin(); it != m_events.end(); ++it)
	{
		delete it->second;
	}
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
	
	if (event->m_data & NetEvent::DATA_FLAGS)
	{
		cv->toBuffer<short>(m_flags.size());
		std::set<std::string>::iterator it;
		for (it = m_flags.begin(); it != m_flags.end(); ++it)
		{
			cv->toBuffer(*it);
		}
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
	
	if (event->m_data & NetEvent::DATA_FLAGS)
	{
		m_flags.clear();
		std::string flag;
		short nr;
		cv->fromBuffer(nr);
		for (int i=0; i<nr; i++)
		{
			cv->fromBuffer(flag);
			m_flags.insert(flag);
		}
	}
}

void ScriptObject::setFlag(std::string flag, bool set)
{
	addToNetEventMask(NetEvent::DATA_FLAGS);
	if (set == true)
	{
		m_flags.insert(flag);
	}
	else
	{
		m_flags.erase(flag);
	}
}

void ScriptObject::addEvent(TriggerType trigger, Event* event)
{
	m_events.insert(std::make_pair(trigger,event));
}

void ScriptObject::activateTrigger(Trigger* trigger)
{
	std::multimap<TriggerType, Event*>::iterator it, itend, jt;
	TriggerType type;
	type = trigger->getType();
		
	DEBUG5("trigger: %s",type.c_str());
		
	// Schleife ueber die ausgeloesten Events
	it = m_events.lower_bound(type);
	itend = m_events.upper_bound(type);
	while (it != itend)
	{
		jt = it;
		++it;
			
		// vom Trigger definierte Variablen einfuegen
		EventSystem::doString((char*) trigger->getLuaVariables().c_str());
		DEBUG5("lua code \n %s",trigger->getLuaVariables().c_str());
			
			// Event ausfuehren
		bool ret = EventSystem::executeEvent(jt->second);
			
		if (ret)
			DEBUG5("event on trigger: %s",type.c_str());
			
		// einmalige Ereignisse loeschen, wenn erfolgreich ausgefuehrt
		if (jt->second->getOnce() &&  ret)
		{
			delete jt->second;
			m_events.erase(jt);
		}
	}
}