#include "event.h"
#include "debug.h"
#include "eventsystem.h"

Trigger::Trigger(TriggerType type)
{
	m_type = type,
	m_lua_variables = "trigger = {} \n";
	m_object_id =0;
}

void Trigger::addVariable(std::string name, int value)
{
	std::stringstream stream;
	stream <<"trigger."<< name << " = " << value << "\n";
	m_lua_variables += stream.str();
	
	if (name=="_id")
	{
		m_object_id = value;
	}
}

void Trigger::addVariable(std::string name, bool value)
{
	std::stringstream stream;
	stream <<"trigger."<< name << " = ";
	if (value)
	{
		 stream << "true"; 
	}
	else
	{
		 stream << "false"; 
	}
	
	stream<< "\n";
	m_lua_variables += stream.str();
}

void Trigger::addVariable(std::string name, float value)
{
	std::stringstream stream;
	stream <<"trigger."<< name << " = " << value << "\n";
	m_lua_variables += stream.str();
	
	if (name=="_id")
	{
		m_object_id = int(value);
	}
}

void Trigger::addVariable(std::string name, std::string value)
{
	std::stringstream stream;
	stream <<"trigger."<< name << " = \"" << value << "\"\n";
	m_lua_variables += stream.str();
}

void Trigger::addVariable(std::string name, Vector value)
{
	std::stringstream stream;
	stream <<"trigger."<< name << " = {" << value.m_x<<","<<value.m_y << "}\n";
	m_lua_variables += stream.str();
}


Event::~Event()
{
	if (!m_copy)
	{
		if (m_condition != LUA_NOREF)
		{
			luaL_unref(EventSystem::getLuaState(), LUA_REGISTRYINDEX, m_condition);
		}
		
		if (m_effect != LUA_NOREF)
		{
			luaL_unref(EventSystem::getLuaState(), LUA_REGISTRYINDEX, m_effect);
		}
	}
}

void Event::setEffect(const char * effect)
{
	EventSystem::clearCodeReference(m_effect);
	
	m_effect = EventSystem::createCodeReference(effect);
}

void Event::setCondition(const char * cond)
{
	EventSystem::clearCodeReference(m_condition);
	
	m_condition = EventSystem::createCodeReference(cond);
}


bool Event::checkCondition()
{
	// keine Bedingung gibt true
	if (m_condition == LUA_NOREF)
	{
		return true;
	}
	
	
	bool succ = EventSystem::executeCodeReference(m_condition);
	if (succ == false)
		return false;
	
	if (lua_gettop(EventSystem::getLuaState()) >0)
	{
		bool ret = lua_toboolean(EventSystem::getLuaState(), -1);
		lua_pop(EventSystem::getLuaState(), 1);
		return ret;
	}
	else
	{
		ERRORMSG("condition lua code must return bool");
		return false;
	}
}

void Event::doEffect()
{
	EventSystem::executeCodeReference(m_effect);
}

