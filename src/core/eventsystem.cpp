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

int EventSystem::getDamageValue(lua_State *L)
{
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=2)
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
	if (argc>=3)
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
	if (argc >= 4)
	{
		if (m_region !=0)
		{
			std::string tname = lua_tostring(L, 1);
			std::string dmgname = lua_tostring(L, 2);
			float sx = lua_tonumber(L, 3);
			float sy = lua_tonumber(L, 4);
		
			float speed = 10.0;
			Vector pos(sx,sy);
			DEBUG("number of arguments %i",argc);
			
			DEBUG("name %s dmg %s %f %f",tname.c_str(), dmgname.c_str(),sx,sy);
			
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
			
			// Fraktion ermitteln
			WorldObject::Fraction fr = WorldObject::FRAC_HOSTILE_TO_ALL;
			WorldObject* wo = m_region->getObject(dmg->m_attacker_id);
			if (wo !=0)
			{
				fr = wo->getFraction();
			}
			
			// Projektil erzeugen
			Projectile* pr = new Projectile(type, fr, World::getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),dmg,sizeof(Damage));
			
			// Richtung, Geschwindigkeit ermitteln
			if (argc>=6)
			{
				float gx = lua_tonumber(L, 5);
				float gy = lua_tonumber(L, 6);
				
				Vector goal(gx,gy);
				Vector dir = goal - pos;
				dir.normalize();
				
				if (argc>=7)
				{
					speed = lua_tonumber(L, 7);
				}
				
				// Geschwindigkeit wird in m/ms gemessen
				pr->setSpeed(dir *speed/1000);
				
				if (argc>=8)
				{
					float dist = lua_tonumber(L, 8);
					pos += dir*dist;
				}
			}
			
			m_region->insertProjectile(pr,pos);
		}
		
	}
	else
	{
		ERRORMSG("Syntax: createProjectile( string type, string dmgname, startx, starty, [goalx, goaly] , [speed], [dist])");
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
