#include "eventsystem.h"
#include "region.h"
#include "player.h"
#include "item.h"

lua_State * EventSystem::m_lua;

Region* EventSystem::m_region;
 
Trigger*  EventSystem::m_trigger;

void EventSystem::init()
{
	m_lua = lua_open();

	luaL_openlibs(m_lua);
	
	lua_register(m_lua, "getObjectValue", getObjectValue);
	lua_register(m_lua, "get", getObjectValue);	
	lua_register(m_lua, "setObjectValue", setObjectValue);
	lua_register(m_lua, "set", setObjectValue);
	lua_register(m_lua, "pointIsInArea", pointIsInArea);
	lua_register(m_lua, "unitIsInArea", unitIsInArea);
	lua_register(m_lua, "createObject", createObject);
	lua_register(m_lua, "deleteObject", deleteObject);
	lua_register(m_lua, "dropItem", dropItem);
	lua_register(m_lua, "getLocation", getLocation);
	lua_register(m_lua, "addLocation", addLocation);
	lua_register(m_lua, "addArea", addArea);
	lua_register(m_lua, "startTimer",startTimer);
	lua_register(m_lua, "insertTrigger",insertTrigger);
	lua_register(m_lua, "addTriggerVariable", addTriggerVariable);
	
	m_region =0;
	m_trigger =0;
}

void  EventSystem::cleanup()
{
	lua_close(m_lua);
}


void EventSystem::doString(char* instructions)
{
	int err  = luaL_dostring(m_lua, instructions);
	
	if (err!=0)
	{
		reportErrors(m_lua, err);
	}
}

void EventSystem::reportErrors(lua_State *L, int status)
{
	if ( status!=0 )
	{
		DEBUG("lua error %s \n", lua_tostring(L, -1));
		lua_pop(L, 1); // remove error message
	}
}

std::string EventSystem::getReturnValue()
{
	if (lua_gettop(m_lua) >0)
	{
		std::string ret = lua_tostring(m_lua, -1);
		lua_pop(m_lua, 1);
		return ret;
	}
	return "";
}

bool EventSystem::executeEvent(Event* event)
{
	// Lua Code zum Bedingung pruefen ausfuehren
	if (event->getCondition() != "")
	{
		doString((char*) event->getCondition().c_str());
	
		// Pruefen, ob true zurueck gegeben wurde
		if (lua_gettop(m_lua) >0)
		{
			bool ret = lua_toboolean(m_lua, -1);
			lua_pop(m_lua, 1);
			
			if (ret == false)
			{
				// Bedingung nicht erfuellt
				return false;
			}
		}
		else
		{
			ERRORMSG("condition lua code must return bool");
			return false;
		}
	}
	
	if (event->getEffect() != "")
	{
		doString((char*) event->getEffect().c_str());
	}
	
	return true;
}

int EventSystem::getObjectValue(lua_State *L)
{
	
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=2)
	{
		int id = lua_tointeger(L, 1);
		std::string valname = lua_tostring(L, 2);
		
		WorldObject* wo =0; 
		if (m_region !=0)
		{
			wo = m_region->getObject(id);
		}
		
		if (wo !=0)
		{
			ret = wo->getValue(valname);
		}
	}
	else
	{
		ERRORMSG("Syntax: getObjectValue( int id, string valname)");
	}
	
	return ret;
}

int EventSystem::setObjectValue(lua_State *L)
{
	
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=3)
	{
		int id = lua_tointeger(L, 1);
		std::string valname = lua_tostring(L, 2);
		
		WorldObject* wo =0; 
		if (m_region !=0)
		{
			wo = m_region->getObject(id);
		}
		
		if (wo !=0)
		{
			ret = wo->setValue(valname);
		}
	}
	else
	{
		ERRORMSG("Syntax: setObjectValue( int id, string valname, value)");
	}
	
	return 0;
}

int EventSystem::unitIsInArea(lua_State *L)
{
	bool ret =false;
	int argc = lua_gettop(L);
	if (argc>=2)
	{
		int id = lua_tointeger(L, 1);
		AreaName area = lua_tostring(L, 2);
		
		
		if (m_region !=0)
		{
			WorldObject* wo = m_region->getObject(id);
			if (wo !=0)
			{
				ret = m_region->getArea(area).intersects(*(wo->getShape()));
			}
		}
		
	}
	else
	{
		ERRORMSG("Syntax: pointIsInArea( int unitid, string areaname)");
	}
	
	lua_pushboolean(EventSystem::getLuaState() , ret);
	return 1;
}

int EventSystem::pointIsInArea(lua_State *L)
{
	bool ret =false;
	int argc = lua_gettop(L);
	if (argc>=3)
	{
		float x = lua_tonumber(L, 1);	
		float y = lua_tonumber(L, 2);	
		AreaName area = lua_tostring(L, 3);
		
		Shape s;
		s.m_center = Vector(x,y);
		s.m_type = Shape::CIRCLE;
		s.m_radius=0;
		
		if (m_region !=0)
		{
			ret = m_region->getArea(area).intersects(s);
		}
		
	}
	else
	{
		ERRORMSG("Syntax: pointIsInArea( float x, float y, string areaname)");
	}
	
	lua_pushboolean(EventSystem::getLuaState() , ret);
	return 1;
}

int EventSystem::createObject(lua_State *L)
{
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=3)
	{
		
		
		WorldObject::TypeInfo::ObjectSubtype subtype = lua_tostring(L, 1);
		float x = lua_tonumber(L, 2);	
		float y = lua_tonumber(L, 3);
		
		if (m_region!=0)
		{
			float angle =0;
			if (argc>=4)
			{
				angle = lua_tonumber(L, 4);
			}
			
			ret = m_region->createObject(subtype, Vector(x,y),angle, WorldObject::STATE_AUTO);
		}
		
	}
	else
	{
		ERRORMSG("Syntax: createObject( string subtype, float x, float y)");
	}
	
	lua_pushinteger(EventSystem::getLuaState() , ret);
	return 1;
}

int EventSystem::deleteObject(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1)
	{
		int id = lua_tointeger(L, 1);
		if (m_region !=0)
		{
			m_region->deleteObject(id);
		}
	}
	else
	{
		ERRORMSG("Syntax: deleteObject( int objectID)");
	}
	return 0;
}
	
int EventSystem::dropItem(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=3)
	{
		Item::Subtype subtype = lua_tostring(L, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		
		float magic_power=0;
		if (argc>=4)
		{
			magic_power= lua_tonumber(L, 4);
		}
		
		if (m_region!=0)
		{
			m_region->dropItem(subtype, Vector(x,y), magic_power);
		}
	}
	else
	{
		ERRORMSG("Syntax: :dropItem(string itemtype,float x,float y, [magic_power]");
	}
	return 0;
}
	
int EventSystem::addLocation(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=3)
	{
		LocationName loc = lua_tostring(L, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		
		if (m_region !=0)
		{
			m_region->addLocation(loc,Vector(x,y));
		}
	}
	else
	{
		ERRORMSG("Syntax: addLocation(string locname, x, y)");
	}
	return 0;
}
	
int EventSystem::getLocation(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1)
	{
		LocationName loc = lua_tostring(L, 1);
		if (m_region !=0)
		{
			Vector v = m_region->getLocation(loc);
			lua_pushnumber(EventSystem::getLuaState() , v.m_x);
			lua_pushnumber(EventSystem::getLuaState() , v.m_y);
			
			return 2;
		}
	}
	else
	{
		ERRORMSG("Syntax: getLocation( string locationname");
	}
	return 0;
}

int EventSystem::addArea(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=5)
	{
		AreaName area = lua_tostring(L, 1);
		std::string type = lua_tostring(L, 2);
		float x = lua_tonumber(L, 3);
		float y = lua_tonumber(L, 4);
		
		Shape s;
		s.m_center = Vector(x,y);
		if (type == "rect" || type == "RECT")
		{
			float ex = lua_tonumber(L, 5);
			float ey = lua_tonumber(L, 6);
			s.m_type = Shape::RECT;
			s.m_extent = Vector(ex,ey);
		}
		else
		{
			float r = lua_tonumber(L, 5);
			s.m_type = Shape::CIRCLE;
			s.m_radius = r;
		}
		
		if (m_region !=0)
		{
			m_region->addArea(area,s);
		}
	}
	else
	{
		ERRORMSG("Syntax: addLocation(string areaname, 'circle' , mx, my ,r) \n \
				addLocation(string areaname, 'rect' , cx, cy ,ex, ey) ");
	}
	return 0;
}

int EventSystem::startTimer(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2)
	{
		std::string type = lua_tostring(L, 1);
		float time = lua_tonumber(L, 2);
		
		if (m_region !=0)
		{
			m_trigger = new Trigger(type);
			m_region->insertTimedTrigger(m_trigger,time);
		}
	}
	else
	{
		ERRORMSG("Syntax: startTimer(triggername, time");
	}
	return 0;
}

int EventSystem::insertTrigger(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1)
	{
		std::string type = lua_tostring(L, 1);
		Region* reg = m_region;
		
		if (argc >=2)
		{
			std::string regname = lua_tostring(L, 2);
			reg = World::getWorld()->getRegion(regname);
		}
		
		if (reg !=0)
		{
			m_trigger = new Trigger(type);
			reg->insertTrigger(m_trigger);
		}
	}
	else
	{
		ERRORMSG("Syntax: insertTrigger(triggername, [regionname]");
	}
	return 0;	
}

int EventSystem::addTriggerVariable(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2)
	{
		if (m_trigger !=0)
		{
			std::string name = lua_tostring(L, 1);
			
			if (lua_isnumber(L,2))
			{
				float f = lua_tonumber(L, 2);
				m_trigger->addVariable(name,f);
			}
			else if (lua_isstring(L,2))
			{
				std::string s = lua_tostring(L, 2);
				m_trigger->addVariable(name, s);
			}
			
		}
	}
	else
	{
		ERRORMSG("Syntax: addTriggerVariable(string varname, value)");
	}
	return 0;
}
