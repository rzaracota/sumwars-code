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
	lua_register(m_lua, "setDamageValue", setDamageValue);
	lua_register(m_lua, "getDamageValue", getDamageValue);
	lua_register(m_lua, "createProjectile", createProjectile);
	lua_register(m_lua, "getObjectAt", getObjectAt);
	lua_register(m_lua, "getObjectsInArea", getObjectsInArea);
	
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

Vector EventSystem::getVector(lua_State *L, int index)
{
	float x,y;
		
	lua_pushinteger(L, 1);
	lua_gettable(L, index);
	x = lua_tonumber(L, -1);
	lua_pop(L, 1);
			
	lua_pushinteger(L, 2);
	lua_gettable(L, index);
	y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	return Vector(x,y);
}

void EventSystem::pushVector(lua_State *L, Vector v)
{
	lua_newtable(L);
	lua_pushinteger(L, 1);
	lua_pushnumber(L, v.m_x);
	lua_settable(L, -3);
			
	lua_pushinteger(L, 2);
	lua_pushnumber(L,v.m_y);
	lua_settable(L, -3);
}

int EventSystem::getObjectValue(lua_State *L)
{
	
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isnumber(L,1) && lua_isstring(L,2))
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
		else
		{
			ERRORMSG("getObjectValue: Object doesnt exist");
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
	if (argc>=2 && lua_isnumber(L,1) && lua_isstring(L,2))
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
		else
		{
			ERRORMSG("getObjectValue: Object doesnt exist");
		}
	}
	else
	{
		ERRORMSG("Syntax: setObjectValue( int id, string valname, value)");
	}
	
	return 0;
}

int EventSystem::getDamageValue(lua_State *L)
{
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1) && lua_isstring(L,2))
	{
		std::string dmgname = lua_tostring(L, 1);
		std::string valname = lua_tostring(L, 2);
		
		if (m_region !=0)
		{
			ret = m_region->getDamageObject(dmgname).getValue(valname);
		}
	}
	else
	{
		ERRORMSG("Syntax: getDamageValue( string damagename, string valname)");
	}
	
	return ret;
}

int EventSystem::setDamageValue(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=3 && lua_isstring(L,1) && lua_isstring(L,2) &&  lua_isnumber(L,3))
	{
		std::string dmgname = lua_tostring(L, 1);
		std::string valname = lua_tostring(L, 2);
		
		if (m_region !=0)
		{
			m_region->getDamageObject(dmgname).setValue(valname);
		}
	}
	else
	{
		ERRORMSG("Syntax: setDamageValue( string damagename, string valname, value, [value])");
	}
	
	return 0;
}

int EventSystem::createProjectile(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc >= 3 && lua_isstring(L,1) && lua_isstring(L,2)  && lua_istable(L,3))
	{
		if (m_region !=0)
		{
			std::string tname = lua_tostring(L, 1);
			std::string dmgname = lua_tostring(L, 2);
			Vector pos = getVector(L,3);
		
			float speed = 10.0;
			DEBUG5("name %s dmg %s %f %f",tname.c_str(), dmgname.c_str(),pos.m_x,pos.m_y);
			
			// Typ ermitteln
			Projectile::ProjectileType type = Projectile::ARROW;
			if (tname =="arrow") type = Projectile::ARROW;
			else if (tname =="magic_arrow") type = Projectile::MAGIC_ARROW;
			else if (tname =="fire_bolt") type = Projectile::FIRE_BOLT;
			else if (tname =="fire_ball") type = Projectile::FIRE_BALL ;
			else if (tname =="fire_wall") type = Projectile::FIRE_WALL;
			else if (tname =="fire_wave") type = Projectile:: FIRE_WAVE;
			else if (tname =="ice_bolt") type = Projectile::ICE_BOLT;
			else if (tname =="blizzard") type = Projectile::BLIZZARD;
			else if (tname =="ice_ring") type = Projectile:: ICE_RING;
			else if (tname =="freeze") type = Projectile:: FREEZE;
			else if (tname =="lightning") type = Projectile::LIGHTNING;
			else if (tname =="thunderstorm") type = Projectile:: THUNDERSTORM ;
			else if (tname =="chain_lightning") type = Projectile::CHAIN_LIGHTNING;
			else if (tname =="static_shield") type = Projectile::STATIC_SHIELD;
			else if (tname =="fire_arrow") type = Projectile::FIRE_ARROW;
			else if (tname =="ice_arrow") type = Projectile::ICE_ARROW ;
			else if (tname =="wind_arrow") type = Projectile::WIND_ARROW;
			else if (tname =="guided_arrow") type = Projectile::GUIDED_ARROW;
			else if (tname =="explosion") type = Projectile::EXPLOSION;
			else if (tname =="fire_explosion") type = Projectile:: FIRE_EXPLOSION;
			else if (tname =="ice_explosion") type = Projectile::ICE_EXPLOSION;
			else if (tname =="wind_explosion") type = Projectile::WIND_EXPLOSION;
			else if (tname =="light_beam") type = Projectile:: LIGHT_BEAM ;
			else if (tname =="elem_explosion") type = Projectile::ELEM_EXPLOSION;
			else if (tname =="acid") type = Projectile:: ACID;
			else if (tname =="divine_beam") type = Projectile::DIVINE_BEAM;
			else if (tname =="hypnosis") type = Projectile::HYPNOSIS;
			
			// Schaden
			Damage* dmg = &(m_region->getDamageObject(dmgname));
			
			// Projektil erzeugen
			Projectile* pr = new Projectile(type, dmg, World::getWorld()->getValidProjectileId());
			
			// Richtung, Geschwindigkeit ermitteln
			if (argc>=4 && lua_istable(L,4))
			{

				Vector goal = getVector(L,4);
				Vector dir = goal - pos;
				dir.normalize();
				
				if (argc>=5  && lua_isnumber(L,5))
				{
					speed = lua_tonumber(L, 5);
				}
				
				// Geschwindigkeit wird in m/ms gemessen
				pr->setSpeed(dir *speed/1000);
				
				if (argc>=6 && lua_isnumber(L,6))
				{
					float dist = lua_tonumber(L, 6);
					pos += dir*dist;
				}
			}
			
			m_region->insertProjectile(pr,pos);
		}
		
	}
	else
	{
		ERRORMSG("Syntax: createProjectile( string type, string dmgname, {startx, starty}, [{goalx, goaly}] , [speed], [dist])");
	}
	return 0;
}

int EventSystem::unitIsInArea(lua_State *L)
{
	bool ret =false;
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isnumber(L,1) && lua_isstring(L,2) )
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
			else
			{
				ERRORMSG("unitIsInArea: Unit does not exist");
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
	if (argc>=2 && lua_istable(L,1) && lua_isstring(L,2))
	{
		Vector c = getVector(L,1);
		AreaName area = lua_tostring(L, 2);
		
		Shape s;
		s.m_center = c;
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
	if (argc>=2 && lua_istable(L,2) && lua_isstring(L,1))
	{
		
		
		WorldObject::TypeInfo::ObjectSubtype subtype = lua_tostring(L, 1);
		Vector pos = getVector(L,2);
		
		if (m_region!=0)
		{
			float angle =0;
			if (argc>=3 && lua_isnumber(L,3))
			{
				angle = lua_tonumber(L, 3);
			}
			
			ret = m_region->createObject(subtype, pos,angle, WorldObject::STATE_AUTO);
		}
		
	}
	else
	{
		ERRORMSG("Syntax: createObject( string subtype, {float x, float y}, [angle])");
	}
	
	lua_pushinteger(EventSystem::getLuaState() , ret);
	return 1;
}

int EventSystem::deleteObject(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isnumber(L,1))
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

int EventSystem::getObjectAt(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && lua_istable(L,1))
	{
		Vector pos = getVector(L,1);
		if (m_region !=0)
		{
			WorldObject* wo = m_region->getObjectAt(pos);
			DEBUG("pos %f %f wo %p",pos.m_x, pos.m_y,wo);
			if (wo !=0)
			{
				lua_pushnumber(L,wo->getId());
				return 1;
			}
		}
	}
	else
	{
		ERRORMSG("Syntax: getObjectAt({float x, float x})");
	}
	
	return 0;
}

int EventSystem::getObjectsInArea(lua_State *L)
{
	int argc = lua_gettop(L);
	lua_newtable(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		AreaName area = lua_tostring(L, 1);
		if (m_region !=0)
		{
			WorldObjectList obj;
			WorldObjectList::iterator it;
			Shape s = m_region->getArea(area);
			
			short layer = WorldObject::LAYER_ALL;
			if (argc>=2 && lua_isstring(L,2))
			{
				std::string lstr = lua_tostring(L,2);
				if (lstr == "base")
				{
					layer = WorldObject::LAYER_BASE;
				}
				else  if (lstr == "air")
				{
					layer = WorldObject::LAYER_AIR;
				}
				else  if (lstr == "dead")
				{
					layer = WorldObject::LAYER_DEAD;
				}
				else  if (lstr == "normal")
				{
					layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
				}
			}
			
			short group = WorldObject::GROUP_ALL;
			if (argc>=3 && lua_isstring(L,3))
			{
				
				std::string gstr = lua_tostring(L,3);
				if (gstr == "unit")
				{
					group = WorldObject::CREATURE;
				}
				else if (gstr == "player")
				{
					group = WorldObject::PLAYER;
				}
				else if (gstr == "fixed")
				{
					group = WorldObject::FIXED;
				}
			}
			m_region->getObjectsInShape(&s,&obj,layer,group);
			
			int cnt =1;
			for (it = obj.begin(); it!= obj.end(); ++it)
			{
				lua_pushnumber(L,(*it)->getId());
				lua_rawseti (L, -2, cnt);
				cnt++;
			}
		}
	}
	else
	{
		ERRORMSG("Syntax: getObjectsInArea(string areaname)")
	}
	
	
	return 1;
}

int EventSystem::dropItem(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && lua_istable(L,2) && lua_isstring(L,1))
	{
		Item::Subtype subtype = lua_tostring(L, 1);
		Vector pos = getVector(L,2);
		
		float magic_power=0;
		if (argc>=3 && lua_isnumber(L,3))
		{
			magic_power= lua_tonumber(L, 3);
		}
		
		if (m_region!=0)
		{
			m_region->dropItem(subtype, pos, magic_power);
		}
	}
	else
	{
		ERRORMSG("Syntax: :dropItem(string itemtype,{float x,float y}, [magic_power]");
	}
	return 0;
}
	
int EventSystem::addLocation(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && lua_istable(L,2) && lua_isstring(L,1))
	{
		LocationName loc = lua_tostring(L, 1);
		Vector v = getVector(L,2);
		
		
		if (m_region !=0)
		{
			m_region->addLocation(loc,v);
		}
	}
	else
	{
		ERRORMSG("Syntax: addLocation(string locname, {x, y})");
	}
	return 0;
}
	
int EventSystem::getLocation(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		LocationName loc = lua_tostring(L, 1);
		if (m_region !=0)
		{
			pushVector(L,m_region->getLocation(loc));
			
			return 1;
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
	if (argc>=4 && lua_isstring(L,1) && lua_isstring(L,2) && lua_istable(L,3) )
	{
		AreaName area = lua_tostring(L, 1);
		std::string type = lua_tostring(L, 2);
		
		Shape s;
		s.m_center = getVector(L,3);
		if (type == "rect" || type == "RECT")
		{
			s.m_type = Shape::RECT;
			s.m_extent = getVector(L,4);
		}
		else
		{
			float r = lua_tonumber(L, 4);
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
		ERRORMSG("Syntax: addLocation(string areaname, 'circle' , {mx, my} ,r) \n \
				addLocation(string areaname, 'rect' , {cx, cy} ,{ex, ey}) ");
	}
	return 0;
}

int EventSystem::startTimer(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1) && lua_isnumber(L,2))
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
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string type = lua_tostring(L, 1);
		Region* reg = m_region;
		
		if (argc >=2 && lua_isstring(L,2))
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
	if (argc>=2  && lua_isstring(L,1))
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
