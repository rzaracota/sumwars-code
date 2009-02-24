#include "quest.h"


Quest::Quest(std::string name, std::string table_name)
{
	m_name = name;
	m_table_name = table_name;
	
	m_initialisation = LUA_NOREF;
	m_description = LUA_NOREF;
	
}

Quest::~Quest()
{
	if (m_initialisation != LUA_NOREF)
	{
		luaL_unref(EventSystem::getLuaState(), LUA_REGISTRYINDEX, m_initialisation);
	}
	
	if (m_description != LUA_NOREF)
	{
		luaL_unref(EventSystem::getLuaState(), LUA_REGISTRYINDEX,m_description);
	}
}

void Quest::setInit(const char* init)
{
	if (m_initialisation != LUA_NOREF)
	{
		luaL_unref(EventSystem::getLuaState(), LUA_REGISTRYINDEX, m_initialisation);
	}
	
	int err = luaL_loadstring(EventSystem::getLuaState(),init);
	if (err ==0)
	{
		m_initialisation = luaL_ref(EventSystem::getLuaState(),LUA_REGISTRYINDEX);
	}
	else
	{
		EventSystem::reportErrors(EventSystem::getLuaState(), err);
	}	
}


void Quest::setDescription(const char* descr)
{
	if (m_description != LUA_NOREF)
	{
		luaL_unref(EventSystem::getLuaState(), LUA_REGISTRYINDEX, m_description);
	}
	
	int err = luaL_loadstring(EventSystem::getLuaState(),descr);
	if (err ==0)
	{
		m_description = luaL_ref(EventSystem::getLuaState(),LUA_REGISTRYINDEX);
	}
	else
	{
		EventSystem::reportErrors(EventSystem::getLuaState(), err);
	}	
}

void Quest::init()
{
	std::string luainit = m_table_name;
	luainit += " = {}; \n";
	luainit += m_table_name;
	luainit += "_tmp = {}; \n";
	luainit += m_table_name;
	luainit += ".started = false; \n";
	luainit += m_table_name;
	luainit += ".finished = false; \n";
	luainit += m_table_name;
	luainit += ".failed = false; \n";
	luainit += "quests.";
	luainit += m_table_name;
	luainit += " = ";
	luainit += m_table_name;
	DEBUG5("lua init %s",luainit.c_str());
	EventSystem::doString(luainit.c_str());
	
	if (m_initialisation != LUA_NOREF)
	{
		lua_rawgeti(EventSystem::getLuaState(),LUA_REGISTRYINDEX ,  m_initialisation);
		int err = lua_pcall(EventSystem::getLuaState(), 0, LUA_MULTRET, 0);
		if (err !=0)
		{
			EventSystem::reportErrors(EventSystem::getLuaState(), err);
		}
	}
}

Quest::State Quest::getState()
{
	std::stringstream stream;
	stream << "if "<< m_table_name <<".failed then return " << FAILED << "\n ";
	stream << "else if "<< m_table_name <<".finished then return " << FINISHED << "\n ";
	stream << "else if "<< m_table_name <<".started then return " << STARTED << "\n ";
	stream << "else return " << NOT_STARTED << " end end end\n ";
	
	
	EventSystem::doString((char*) stream.str().c_str());
	std::string ret = EventSystem::getReturnValue();
	stream.str(ret);
	int r;
	stream >> r;
	return (State) r;
}

std::string Quest::getDescription()
{
	lua_rawgeti(EventSystem::getLuaState(),LUA_REGISTRYINDEX ,  m_description);
	int err = lua_pcall(EventSystem::getLuaState(), 0, LUA_MULTRET, 0);
	if (err !=0)
	{
		EventSystem::reportErrors(EventSystem::getLuaState(), err);
		return "";
	}
	
	return EventSystem::getReturnValue();
}


