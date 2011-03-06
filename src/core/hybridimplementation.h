#ifndef HYBRIDIMPLEMENTATION_H
#define HYBRIDIMPLEMENTATION_H


extern "C"
{
	
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "eventsystem.h"

/**
 * \var struct HybridImplementation
 * \brief Struktur fuer die Beschreibung einer Implementation ind C++ und/oder Lua
 */
struct HybridImplementation
{
	/**
	 * \var std::string m_cpp_impl
	 * \brief Kennstrings anhand denen ein oder mehrere Stuecke Quellcode selektiert werden
	 */
	std::list<std::string> m_cpp_impl;
	
	/**
	 * \var int m_lua_impl
	 * \brief Handle fuer Lua-Code
	 */
	int m_lua_impl;
	
	HybridImplementation()
	{
		m_lua_impl = LUA_NOREF;
	}
	
	/**
	 * \brief Destructor
	 */
	~HybridImplementation()
	{
		EventSystem::clearCodeReference(m_lua_impl);
	}
};

#endif

