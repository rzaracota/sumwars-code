#include "event.h"
#include "debug.h"
#include "eventsystem.h"

Trigger::Trigger(TriggerType type)
{
	m_type = type,
	m_lua_variables = "trigger = {} \n";
}

void Trigger::addVariable(std::string name, int value)
{
	std::stringstream stream;
	stream <<"trigger."<< name << " = " << value << "\n";
	m_lua_variables += stream.str();
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
	if (m_effect != LUA_NOREF)
	{
		luaL_unref(EventSystem::getLuaState(), LUA_REGISTRYINDEX, m_effect);
	}
	
	int err = luaL_loadstring(EventSystem::getLuaState(),effect);
	if (err ==0)
	{
		m_effect = luaL_ref(EventSystem::getLuaState(),LUA_REGISTRYINDEX);
	}
	else
	{
		EventSystem::reportErrors(EventSystem::getLuaState(), err);
	}
}

void Event::setCondition(const char * cond)
{
	if (m_condition != LUA_NOREF)
	{
		luaL_unref(EventSystem::getLuaState(), LUA_REGISTRYINDEX, m_condition);
	}
	
	int err = luaL_loadstring(EventSystem::getLuaState(),cond);
	if (err ==0)
	{
		m_condition = luaL_ref(EventSystem::getLuaState(),LUA_REGISTRYINDEX);
		DEBUG5("condition reference %i",m_condition);
	}
	else
	{
		EventSystem::reportErrors(EventSystem::getLuaState(), err);
	}
}


bool Event::checkCondition()
{
	// keine Bedingung gibt true
	if (m_condition == LUA_NOREF)
	{
		return true;
	}
	
	
	lua_rawgeti(EventSystem::getLuaState(),LUA_REGISTRYINDEX , m_condition);
	int err = lua_pcall(EventSystem::getLuaState(), 0, LUA_MULTRET, 0);
	if (err !=0)
	{
		EventSystem::reportErrors(EventSystem::getLuaState(), err);
		return false;
	}
	
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
	if (m_effect == LUA_NOREF)
	{
		return;
	}
	
	lua_rawgeti(EventSystem::getLuaState(),LUA_REGISTRYINDEX , m_effect);
	int err = lua_pcall(EventSystem::getLuaState(), 0, LUA_MULTRET, 0);
	
	if (err !=0)
	{
		EventSystem::reportErrors(EventSystem::getLuaState(), err);
	}
}

