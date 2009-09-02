#include "eventsystem.h"
#include "region.h"
#include "player.h"
#include "item.h"
#include "dialogue.h"
#include "scriptobject.h"

#ifdef DEBUG_DATABASE
		std::map<int, std::string> EventSystem::m_code_fragments;
#endif

lua_State * EventSystem::m_lua=0;

Region* EventSystem::m_region =0;

Trigger*  EventSystem::m_trigger=0;

Dialogue*  EventSystem::m_dialogue=0;

Damage* EventSystem::m_damage=0;

CreatureBaseAttrMod* EventSystem::m_base_mod=0;

CreatureDynAttrMod* EventSystem::m_dyn_mod=0;

CharConv* EventSystem::m_charconv =0;

Event* EventSystem::m_event =0;

std::map<int, std::string> EventSystem::m_player_varupdates;

void EventSystem::init()
{
	if (m_lua !=0)
	{
		cleanup();
	}

	m_lua = lua_open();

	luaL_openlibs(m_lua);

	lua_register(m_lua, "getRegion", getRegion);
	lua_register(m_lua, "getObjectValue", getObjectValue);
	lua_register(m_lua, "get", getObjectValue);
	lua_register(m_lua, "setObjectValue", setObjectValue);
	lua_register(m_lua, "set", setObjectValue);
	lua_register(m_lua, "objectIsInRegion", objectIsInRegion);
	lua_register(m_lua, "pointIsInArea", pointIsInArea);
	lua_register(m_lua, "unitIsInArea", unitIsInArea);
	lua_register(m_lua, "objectIsInArea", unitIsInArea);
	
	lua_register(m_lua, "createObject", createObject);
	lua_register(m_lua, "createScriptObject", createScriptObject);
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
	lua_register(m_lua, "setBaseModValue", setBaseModValue);
	lua_register(m_lua, "getBaseModValue", getBaseModValue);
	lua_register(m_lua, "setDynModValue", setDynModValue);
	lua_register(m_lua, "getDynModValue", getDynModValue);
	
	lua_register(m_lua, "createProjectile", createProjectile);
	lua_register(m_lua, "createMonsterGroup", createMonsterGroup);
	lua_register(m_lua, "getObjectAt", getObjectAt);
	lua_register(m_lua, "getObjectsInArea", getObjectsInArea);
	lua_register(m_lua, "addUnitCommand", addUnitCommand);
	lua_register(m_lua, "clearUnitCommands",clearUnitCommands); 
	lua_register(m_lua, "setUnitAction", setUnitAction);
	lua_register(m_lua, "setScriptObjectAnimation",setScriptObjectAnimation);
	lua_register(m_lua, "setCutsceneMode", setCutsceneMode);
	lua_register(m_lua, "addCameraPosition", addCameraPosition);
	
	lua_register(m_lua, "speak", speak);
	lua_register(m_lua, "unitSpeak", unitSpeak);
	lua_register(m_lua, "setSpeakerEmotion", setSpeakerEmotion);
	lua_register(m_lua, "setSpeakerPosition", setSpeakerPosition);
	lua_register(m_lua, "addQuestion", addQuestion);
	lua_register(m_lua, "addAnswer", addAnswer);
	lua_register(m_lua, "changeTopic", changeTopic);
	lua_register(m_lua, "jumpTopic", jumpTopic);
	lua_register(m_lua, "createDialog", createDialogue);
	lua_register(m_lua, "createDialogue", createDialogue);
	lua_register(m_lua, "setTopicBase",setTopicBase );
	lua_register(m_lua, "addSpeaker", addSpeaker);
	lua_register(m_lua, "getSpeaker", getSpeaker);
	lua_register(m_lua, "setDialogueActive", setDialogueActive);
	lua_register(m_lua, "setCurrentDialogue", setCurrentDialogue);
	lua_register(m_lua, "executeInDialog",executeInDialog);
	
	lua_register(m_lua, "setRefName", setRefName);
	lua_register(m_lua, "getRolePlayers", getRolePlayers);
	lua_register(m_lua, "teleportPlayer", teleportPlayer);
	
	lua_register(m_lua, "createEvent", createEvent);
	lua_register(m_lua, "addCondition", addCondition);
	lua_register(m_lua, "addEffect", addEffect);
	lua_register(m_lua, "timedExecute",timedExecute);
	
	lua_register(m_lua, "getRelation",getRelation);
	lua_register(m_lua, "setRelation",setRelation);
	
	lua_register(m_lua, "writeString", writeString);
	lua_register(m_lua, "writeNewline", writeNewline);
	lua_register(m_lua, "writeUpdateString", writeUpdateString);
	lua_register(m_lua, "gettext", luagettext);
	lua_register(m_lua, "_", luagettext);


	m_region =0;
	m_trigger =0;
	m_dialogue =0;

	doString("quests = {} ; playervars = {}; scriptobjectvar = {}");
}

void  EventSystem::cleanup()
{
	lua_close(m_lua);
	m_lua =0;
}


void EventSystem::doString(const char* instructions)
{
	int err  = luaL_dostring(m_lua, instructions);

	if (err!=0)
	{
		reportErrors(m_lua, err,instructions);
	}
}

void EventSystem::doFile(const char* file)
{
	int err  = luaL_dofile(m_lua, file);

	if (err!=0)
	{
		std::string expl = "error in file of ";
		expl += file;
		reportErrors(m_lua, err,expl.c_str());
	}
}

void EventSystem::clearCodeReference(int& coderef)
{
	if (coderef != LUA_NOREF)
	{
		luaL_unref(EventSystem::getLuaState(), LUA_REGISTRYINDEX, coderef);
		coderef=LUA_NOREF;
	}
}

bool EventSystem::executeCodeReference(int coderef)
{
	if (coderef ==LUA_NOREF)
		return true;
	
	lua_rawgeti(EventSystem::getLuaState(),LUA_REGISTRYINDEX , coderef);
	int err = lua_pcall(EventSystem::getLuaState(), 0, LUA_MULTRET, 0);
	if (err !=0)
	{	
		
#ifdef DEBUG_DATABASE
		const char* instr = m_code_fragments[coderef].c_str();
#else
		char* instr =0;
#endif
		EventSystem::reportErrors(EventSystem::getLuaState(), err,instr);
		return false;
	}
	return true;
}

int EventSystem::createCodeReference(const char* code)
{
	int err = luaL_loadstring(EventSystem::getLuaState(),code);
	int ref;
	if (err ==0)
	{
		ref = luaL_ref(EventSystem::getLuaState(),LUA_REGISTRYINDEX);
#ifdef DEBUG_DATABASE
		m_code_fragments[ref] = code;
#endif
		return ref;
	}
	else
	{
		EventSystem::reportErrors(EventSystem::getLuaState(), err,code);
		return LUA_NOREF;
	}
}


void EventSystem::reportErrors(lua_State *L, int status, const char* instr)
{
	if ( status!=0 )
	{
		DEBUG("lua error %s \n", lua_tostring(L, -1));
		lua_pop(L, 1); // remove error message
		if (instr != 0)
		{
			DEBUG("lua code: \n%s",instr);
		}
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
	DEBUG5("execute Event %p",event)
	bool ret = event->checkCondition();
	if (ret ==false)
	{
		return false;
	}
	
	event->doEffect();

	return true;
}

int EventSystem::timedExecute(lua_State *L)
{
	// einzigartiger Name wird durch den counter sichergestellt
	static int counter=1;
	
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1) && lua_isnumber(L,2))
	{
		std::string code = lua_tostring(L,1);
		float time  = lua_tonumber(L, 2);
		
		Region* reg = m_region;
		if (argc>=3 && lua_isstring(L,3))
		{
			reg = World::getWorld()->getRegion(lua_tostring(L,3));
		}
		if (reg ==0)
		{
			return 0;
			DEBUG("region for timedExecute does not exist");
		}
		
		m_event = new Event();
		m_event->setOnce(true);
		m_event->setEffect(code.c_str());
		
		std::stringstream stream;
		stream << counter << "_event";
		
		reg ->addEvent(stream.str(),m_event);
		Trigger* trigger = new Trigger(stream.str());
		m_region->insertTimedTrigger(trigger,time);
		
		counter++;
	}
	else
	{
		ERRORMSG("Syntax: timedExecute( string luacode, float time, [string regionname])");
	}
	return 0;
}

Vector EventSystem::getVector(lua_State *L, int index)
{
	if (lua_istable(L,index))
	{
		float x,y;

		int idx = index;
		if (index <0)
			idx --;
		
		lua_pushinteger(L, 1);
		lua_gettable(L, idx);
		x = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushinteger(L, 2);
		lua_gettable(L, idx);
		y = lua_tonumber(L, -1);
		lua_pop(L, 1);

		return Vector(x,y);
	}
	else if (lua_isstring(L,index))
	{
		LocationName loc = lua_tostring(L, index);
		return m_region->getLocation(loc);
	}
	return Vector(0,0);
}

Shape EventSystem::getArea(lua_State *L, int& index)
{
	Shape s;
	if (lua_isstring(L,index))
	{
		std::string name = lua_tostring(L, index);
		if (name =="circle" || name =="rect")
		{
			s.m_center = getVector(L,index+1);
			if (name =="circle")
			{
				s.m_type = Shape::CIRCLE;
				s.m_radius = lua_tonumber(L,index+2);
			}
			else
			{
				s.m_type = Shape::RECT;
				s.m_extent = getVector(L,index+2);
			}
			index +=2;
		}
		else
		{
			return m_region->getArea(name);
		}
	}
	
	
	return s;
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

int EventSystem::getRegion(lua_State *L)
{
	if (m_region != 0)
	{
		lua_pushstring(L,m_region->getName().c_str());
		return 1;
	}
	return 0;
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

		if (wo ==0)
		{
			wo = World::getWorld()->getPlayer(id);
		}
		
		if (wo !=0)
		{
			ret = wo->getValue(valname);
		}
		else
		{
			ERRORMSG("getObjectValue: Object doesnt exist %i",id);
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
		
		
		if (wo ==0)
		{
			wo = World::getWorld()->getPlayer(id);
		}

		if (wo !=0)
		{
			ret = wo->setValue(valname);
		}
		else
		{
			ERRORMSG("setObjectValue: Object doesnt exist");
		}
	}
	else
	{
		ERRORMSG("Syntax: setObjectValue( int id, string valname, value)");
	}

	return 0;
}

int EventSystem::objectIsInRegion(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isnumber(L,1))
	{
		int id = lua_tointeger(L, 1);
		
		WorldObject* wo =0;
		if (m_region !=0)
		{
			wo = m_region->getObject(id);
			lua_pushboolean(EventSystem::getLuaState() , (wo!=0) );
			return 1;
		}
	}
	else
	{
		ERRORMSG("Syntax: objectIsInRegion( int id)");
	}
	return 0;
}


int EventSystem::getDamageValue(lua_State *L)
{
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string valname = lua_tostring(L, 1);

		if (m_damage != 0)
		{
			ret = m_damage->getValue(valname);
		}
	}
	else
	{
		ERRORMSG("Syntax: getDamageValue(string valname)");
	}

	return ret;
}

int EventSystem::setDamageValue(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1))
	{
		std::string valname = lua_tostring(L, 1);

		if (m_damage != 0)
		{
			m_damage->setValue(valname);
		}
	}
	else
	{
		ERRORMSG("Syntax: setDamageValue(string valname, value)");
	}

	return 0;
}

int EventSystem::getBaseModValue(lua_State *L)
{
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string valname = lua_tostring(L, 1);

		if (m_base_mod != 0)
		{
			ret = m_base_mod->getValue(valname);
		}
	}
	else
	{
		ERRORMSG("Syntax: getBaseModValue(string valname)");
	}

	return ret;
}

int EventSystem::setBaseModValue(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1))
	{
		std::string valname = lua_tostring(L, 1);

		if (m_base_mod != 0)
		{
			m_base_mod->setValue(valname);
		}
	}
	else
	{
		ERRORMSG("Syntax: setBaseModValue(string valname, value)");
	}

	return 0;
}

int EventSystem::getDynModValue(lua_State *L)
{
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string valname = lua_tostring(L, 1);

		if (m_dyn_mod != 0)
		{
			ret = m_dyn_mod->getValue(valname);
		}
	}
	else
	{
		ERRORMSG("Syntax: getDynModValue(string valname)");
	}

	return ret;
}

int EventSystem::setDynModValue(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1))
	{
		std::string valname = lua_tostring(L, 1);

		if (m_dyn_mod != 0)
		{
			m_dyn_mod->setValue(valname);
		}
	}
	else
	{
		ERRORMSG("Syntax: setDynModValue(string valname, value)");
	}

	return 0;
}

int EventSystem::createProjectile(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc >= 3 && lua_isstring(L,1)  && (lua_istable(L,2) || lua_isstring(L,2)))
	{
		if (m_region !=0 && m_damage != 0)
		{
			std::string tname = lua_tostring(L, 1);
			Vector pos = getVector(L,2);

			float speed = 10.0;
			
			// Schaden
			// Projektil erzeugen
			Projectile* pr = ObjectFactory::createProjectile(tname);
			if (pr ==0)
				return 0;
			
			pr->setDamage(m_damage);

			// Richtung, Geschwindigkeit ermitteln
			if (argc>=3 && (lua_istable(L,3) || lua_isstring(L,3)))
			{

				Vector goal = getVector(L,3);
				Vector dir = goal - pos;
				dir.normalize();

				if (argc>=4  && lua_isnumber(L,4))
				{
					speed = lua_tonumber(L, 4);
				}

				// Geschwindigkeit wird in m/ms gemessen
				pr->setSpeed(dir *speed/1000);

				if (argc>=5 && lua_isnumber(L,5))
				{
					float dist = lua_tonumber(L, 5);
					pos += dir*dist;
				}
			}

			m_region->insertProjectile(pr,pos);
		}

	}
	else
	{
		ERRORMSG("Syntax: createProjectile( string type, {startx, starty}, [{goalx, goaly}] , [speed], [dist])");
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


		if (m_region !=0)
		{
			WorldObject* wo = m_region->getObject(id);
			if (wo !=0)
			{
				int idx=2;
				ret = getArea(L,idx).intersects(*(wo->getShape()));
			}
			else
			{
				ERRORMSG("unitIsInArea: Unit does not exist");
			}
		}

	}
	else
	{
		ERRORMSG("Syntax: unitIsInArea( int unitid, string areaname)");
	}

	lua_pushboolean(EventSystem::getLuaState() , ret);
	return 1;
}

int EventSystem::pointIsInArea(lua_State *L)
{
	bool ret =false;
	int argc = lua_gettop(L);
	if (argc>=2 && (lua_istable(L,1) || lua_isstring(L,1)) && lua_isstring(L,2))
	{
		Vector c = getVector(L,1);

		Shape s;
		s.m_center = c;
		s.m_type = Shape::CIRCLE;
		s.m_radius=0;

		if (m_region !=0)
		{
			int idx=2;
			ret = getArea(L,idx).intersects(s);
		}

	}
	else
	{
		ERRORMSG("Syntax: pointIsInArea( {float x, float y}, string areaname)");
	}

	lua_pushboolean(EventSystem::getLuaState() , ret);
	return 1;
}

int EventSystem::createObject(lua_State *L)
{
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=2 && (lua_istable(L,2) || lua_isstring(L,2)) && lua_isstring(L,1))
	{


		WorldObject::Subtype subtype = lua_tostring(L, 1);
		Vector pos = getVector(L,2);

		if (m_region!=0)
		{
			float angle =0,height=0;
			if (argc>=3 && lua_isnumber(L,3))
			{
				angle = lua_tonumber(L, 3);
				angle *= 3.14159 / 180;
			}
			if (argc>=4 && lua_isnumber(L,4))
			{
				height = lua_tonumber(L, 4);
			}

			ret = m_region->createObject(subtype, pos,angle, height,WorldObject::STATE_AUTO);
		}

	}
	else
	{
		ERRORMSG("Syntax: createObject( string subtype, {float x, float y}, [angle],[height])");
	}

	lua_pushinteger(EventSystem::getLuaState() , ret);
	return 1;
}

int  EventSystem::createScriptObject(lua_State *L)
{
	int ret =0;
	int argc = lua_gettop(L);
	if (argc>=4)
	{
		WorldObject::Subtype subtype = lua_tostring(L, 1);
		ScriptObject* wo = new ScriptObject(0);
		
		std::string render_info = lua_tostring(L, 2);
		wo->setRenderInfo(render_info);
		
		Shape* ps = wo->getShape();
		int idx=3;
		*ps = getArea(L,idx);
		
		short layer = WorldObject::LAYER_NOCOLLISION;
		if (argc>=6 && lua_isstring(L,6))
		{
			std::string lstr = lua_tostring(L,6);
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
		
		wo->setLayer(layer);
		wo->setSubtype(subtype);
		m_region->insertObject(wo, wo->getPosition(),0, true);
		ret = wo->getId();
		
	}
	else
	{
		ERRORMSG("Syntax: createScriptObject( string subtype, string renderinfo, 'circle' | 'rect',{float posx, float posx}, {float extentx, float extentx} | radius), [layer]");
	}
	lua_pushinteger(EventSystem::getLuaState() , ret);
	return 1;
	
}

int EventSystem::createMonsterGroup(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && (lua_istable(L,2) || lua_isstring(L,2)) && lua_isstring(L,1))
	{


		MonsterGroupName group= lua_tostring(L, 1);
		Vector pos = getVector(L,2);

		if (m_region!=0)
		{

			m_region->createMonsterGroup(group, pos);
		}

	}
	else
	{
		ERRORMSG("Syntax: createMonsterGroup( string monstergroup, {float x, float y}])");
	}
	return 0;
}

int EventSystem::deleteObject(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isnumber(L,1))
	{
		int id = lua_tointeger(L, 1);
		if (m_region !=0)
		{
			WorldObject* wo = m_region->getObject(id);
			if (wo !=0)
			{
				bool success = m_region->deleteObject(wo);
				if (success)
				{
					delete wo;
				}
			}
			
		}
	}
	else
	{
		ERRORMSG("Syntax: deleteObject( int objectID)");
	}
	return 0;
}

int EventSystem::addUnitCommand(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isnumber(L,1) && lua_isstring(L,2) )
	{
		if (m_region ==0)
			return 0;

		int id = int (lua_tonumber(L,1));
		std::string actstr = lua_tostring(L,2);

		WorldObject* wo = m_region->getObject(id);
		if (wo !=0)
		{
			if (wo->getType() != "FIXED_OBJECT")
			{
				Creature* cr = static_cast<Creature*>(wo);

				Action::ActionType act = Action::getActionType(actstr);
				if (act != "noaction")
				{
					Command com;
					com.m_type = act;
					com.m_range = cr->getBaseAttrMod()->m_attack_range;

					if (act == "use")
						com.m_range = 0.5;

					if (argc >=3 && lua_isnumber(L,3))
					{
						com.m_goal_object_id = lua_tointeger(L,3);
						com.m_goal = cr->getPosition();
					}
					else if (argc >=3 && (lua_istable(L,3) || lua_isstring(L,3)))
					{
						com.m_goal = getVector(L,3);
						com.m_goal_object_id =0;
					}
					
					if (argc>=4)
					{
						std::string flags = lua_tostring(L,4);
						char flg =0;
						
						if (flags.find("repeat") != std::string::npos)
						{
							flg |= Command::REPEAT;
							com.m_flags = flg;
							DEBUG5("flags: %s",flags.c_str());
						}
					}
					
					float time = 50000;
					if (argc>=5 && lua_isnumber(L,5))
						time = lua_tonumber(L,5);
					cr->insertScriptCommand(com,time);
				}
			}
		}
	}
	else
	{
		ERRORMSG("Syntax: :addUnitCommand(int unitid, string command,  [goal_unitid | {goal_x,goal_y}], [flags], [float time])");
	}
	return 0;

}

int EventSystem::clearUnitCommands(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isnumber(L,1))
	{
		if (m_region ==0)
			return 0;
		
		int id = int (lua_tonumber(L,1));
		Creature* cr = dynamic_cast<Creature*>(m_region->getObject(id));
		if (cr != 0)
		{
			cr->clearScriptCommands();
		}
	}
	return 0;
}

int  EventSystem::setUnitAction(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isnumber(L,1) && lua_isstring(L,2) )
	{
		if (m_region ==0)
			return 0;

		int id = int (lua_tonumber(L,1));
		std::string actstr = lua_tostring(L,2);

		WorldObject* wo = m_region->getObject(id);
		if (wo !=0)
		{
			if (wo->getType() != "FIXED_OBJECT")
			{
				Creature* cr = static_cast<Creature*>(wo);

				Action::ActionType act = Action::getActionType(actstr);
				if (act != "noaction")
				{
					Action action;
					action.m_action_equip = Action::NO_WEAPON;
					action.m_type = act;
					
					if (argc >=3 && lua_isnumber(L,3))
					{
						action.m_goal_object_id = lua_tointeger(L,3);
						action.m_goal = cr->getPosition();
					}
					else if (argc >=3 && (lua_istable(L,3) || lua_isstring(L,3)))
					{
						action.m_goal = getVector(L,3);
						action.m_goal_object_id =0;
					}
					else
					{
						action.m_goal_object_id =0;
						action.m_goal = cr->getPosition();
					}
					
					
					action.m_elapsed_time=0;
					action.m_time = 1000;
					if (argc>=4 && lua_isnumber(L,4))
						action.m_time = lua_tonumber(L,4);
					
					cr->setAction(action);
				}
			}
		}
	}
	else
	{
		ERRORMSG("Syntax: :setUnitAction(int unitid, string command,  [goal_unitid | {goal_x,goal_y}], [float time])");
	}
	return 0;

}

int EventSystem::setScriptObjectAnimation(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=3 && lua_isnumber(L,1) && lua_isstring(L,2) && lua_isnumber(L,3))
	{
		if (m_region ==0)
			return 0;

		int id = int (lua_tonumber(L,1));
		std::string actstr = lua_tostring(L,2);

		
		ScriptObject* wo = dynamic_cast<ScriptObject*> (m_region->getObject(id));
		if (wo !=0)
		{
			
			float time = lua_tonumber(L,3);
			bool repeat = false;
			if (argc>=4 && lua_isstring(L,4))
			{
				std::string repstr = lua_tostring(L,4);
				repeat = (repstr == "true");
			}
			wo->setAnimation(actstr,time,repeat);
			DEBUG("action %s perc %f",wo->getActionString().c_str(),wo->getActionPercent());
			
		}
	}
	else
	{
		ERRORMSG("Syntax: setScriptObjectAnimation(int objid, string action, float time, [bool repeat])");
	}
	return 0;
}

int EventSystem::getObjectAt(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && (lua_istable(L,1) || lua_isstring(L,1)))
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
		ERRORMSG("Syntax: getObjectAt({float x, float y})");
	}

	return 0;
}

int EventSystem::getObjectsInArea(lua_State *L)
{
	int argc = lua_gettop(L);
	lua_newtable(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		if (m_region !=0)
		{
			WorldObjectList obj;
			WorldObjectList::iterator it;
			int idx = 1;
			Shape s = getArea(L,idx);

			short layer = WorldObject::WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
			if (argc>=idx+1 && lua_isstring(L,idx+1))
			{
				std::string lstr = lua_tostring(L,idx+1);
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
			if (argc>=idx+2 && lua_isstring(L,idx+2))
			{

				std::string gstr = lua_tostring(L,idx+2);
				if (gstr == "unit")
				{
					group = WorldObject::CREATURE;
				}
				else if (gstr == "player")
				{
					group = WorldObject::PLAYER;
				}
				else if (gstr == "monster")
				{
					group = WorldObject::CREATURE_ONLY;
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
	if (argc>=2 && (lua_istable(L,2) || lua_isstring(L,2)) && lua_isstring(L,1))
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
			m_region->dropItem(subtype, pos, int (magic_power));
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
	if (argc>=2 && (lua_istable(L,2) || lua_isstring(L,2)) && lua_isstring(L,1))
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
	if (argc>=4 && lua_isstring(L,1) && lua_isstring(L,2) && (lua_istable(L,3) || lua_isstring(L,3)) )
	{
		AreaName area = lua_tostring(L, 1);
		std::string type = lua_tostring(L, 2);

		int idx =2;
		Shape s=getArea(L,idx);

		if (m_region !=0)
		{
			m_region->addArea(area,s);
		}
	}
	else
	{
		ERRORMSG("Syntax: addArea(string areaname, 'circle' , {mx, my} ,r) \n \
				addArea(string areaname, 'rect' , {cx, cy} ,{ex, ey}) ");
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
			else if (lua_istable(L,2))
			{
				Vector v = getVector(L,2);
				m_trigger->addVariable(name,v);
			}

		}
	}
	else
	{
		ERRORMSG("Syntax: addTriggerVariable(string varname, value)");
	}
	return 0;
}

int EventSystem::setCutsceneMode(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1  && lua_isboolean(L,1))
	{
		bool mode = lua_toboolean(L,1);
		if (m_region!=0)
		{
			m_region->setCutsceneMode(mode);
		}
	}
	else
	{
		ERRORMSG("Syntax: setCutsceneMode(bool mode) ");
	}

	return 0;
}


int EventSystem::addCameraPosition(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1)
	{
		if (m_region !=0)
		{
			RegionCamera& cam = m_region->getCamera();
			RegionCamera::Position pos;
			if (cam.m_next_positions.empty())
			{
				pos = cam.m_position;
			}
			else
			{
				pos = cam.m_next_positions.back().first;
			}

			float time =0;
			if (lua_isnumber(L,1))
			{
				time = lua_tonumber(L,1);
			}

			if (lua_istable(L,2) || lua_isstring(L,2))
			{
				pos.m_focus = getVector(L,2);
			}

			if (lua_isnumber(L,3))
			{
				pos.m_phi = lua_tonumber(L,3);
			}

			if (lua_isnumber(L,4))
			{
				pos.m_theta = lua_tonumber(L,4);
			}

			if (lua_isnumber(L,5))
			{
				pos.m_distance = lua_tonumber(L,5);
			}

			cam.addPosition(pos,time);
		}
	}
	else
	{
		ERRORMSG("Syntax: setCameraPosition(float time, {float x, float y}, float phi, float theta, float dist");
	}

	return 0;
}


int EventSystem::speak(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;

	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1) && lua_isstring(L,2))
	{
		std::string refname = lua_tostring(L, 1);
		std::string text = lua_tostring(L, 2);

		std::string emotion = "";
		if (argc>=3 && lua_isstring(L,3))
		{
			emotion = lua_tostring(L,3);
		}
		
		float time = 0;
		if (argc>=4 && lua_isnumber(L,4))
		{
			time = lua_tonumber(L,4);
		}
		if (time ==0)
		{
			time = text.size()*50+800;
		}

		m_dialogue->speak(refname,text,emotion,time);

	}
	else
	{
		ERRORMSG("Syntax: speak(string refname, string text [,string emotion [,float time]])");
	}

	return 0;
}

int EventSystem::unitSpeak(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && (lua_isnumber(L,1) || lua_isstring(L,1)) && lua_isstring(L,2))
	{
		int id =0;
		if (lua_isnumber(L,1))
		{
			id = lua_tointeger(L, 1);
		}
		else
		{
			if (m_dialogue !=0)
			{
				id = m_dialogue->getSpeaker(lua_tostring(L, 1));
			}
		}
		
		if (id ==0)
			return 0;
		
		CreatureSpeakText text;
		text.m_text = lua_tostring(L, 2);

		WorldObject* wo =0;
		if (m_region !=0)
		{
			wo = m_region->getObject(id);
		}
		
		Creature* cr = dynamic_cast<Creature*>(wo);
		
		text.m_emotion = "";
		if (argc>=3 && lua_isstring(L,3))
		{
			text.m_emotion = lua_tostring(L,3);
		}
		
		text.m_time = 0;
		if (argc>=4 && lua_isnumber(L,4))
		{
			text.m_time = lua_tonumber(L,4);
		}
		if (text.m_time ==0)
		{
			text.m_time = text.m_text.size()*50+800;
		}

		if (cr!=0)
		{
			cr->speakText(text);
		}
		else
		{
			ERRORMSG("getObjectValue: Object doesnt exist %i",id);
		}
	}
	else
	{
		ERRORMSG("Syntax: unitSpeak(( int id, string text [,string emotion [,float time]])");
	}
	return 0;
	

}

int EventSystem::addQuestion(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;

	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string text = lua_tostring(L, 1);
		m_dialogue->addQuestion(text);
	}
	else
	{
		ERRORMSG("Syntax: addQuestion(string text)");
	}

	return 0;
}


int EventSystem::addAnswer(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;

	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1) && lua_isstring(L,2))
	{
		std::string text = lua_tostring(L, 1);
		std::string topic = lua_tostring(L, 2);

		m_dialogue->addAnswer(text,topic);
	}
	else
	{
		ERRORMSG("Syntax: addAnswer(string text, string topic)")
	}

	return 0;
}

int EventSystem::setSpeakerEmotion(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;
	
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1) && lua_isstring(L,2))
	{
		std::string refname = lua_tostring(L, 1);
		std::string emotion = lua_tostring(L, 2);
		m_dialogue->speak(refname,"#emotion#",emotion,0);
	}
	else
	{
		ERRORMSG("Syntax: setSpeakerEmotion(string refname, string emotion)");
	}

	return 0;
}

int EventSystem::setSpeakerPosition(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;
	
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1) && lua_isstring(L,2))
	{
		std::string refname = lua_tostring(L, 1);
		std::string position = lua_tostring(L, 2);
		m_dialogue->speak(refname,"#position#",position,0);
	}
	else
	{
		ERRORMSG("Syntax: setSpeakerPosition(string refname, string position)");
	}

	return 0;
}

int EventSystem::changeTopic(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;

	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string topic = lua_tostring(L, 1);
		m_dialogue->addQuestion("#change_topic#");
		m_dialogue->addAnswer("",topic);
	}
	else
	{
		ERRORMSG("Syntax: addQuestion(string text)");
	}

	return 0;
}

int EventSystem::jumpTopic(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;

	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string topic = lua_tostring(L, 1);
		m_dialogue->addQuestion("#jump_topic#");
		m_dialogue->addAnswer("",topic);
	}
	else
	{
		ERRORMSG("Syntax: addQuestion(string text)");
	}

	return 0;
}

int EventSystem::executeInDialog(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;
	
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string code = lua_tostring(L, 1);
		m_dialogue->speak("","#execute#",code);
	}
	else
	{
		ERRORMSG("Syntax: executeInDialog(luacode)");
	}

	return 0;
}


int EventSystem::createDialogue(lua_State *L)
{
	if (m_region ==0)
		return 0;

	m_dialogue = new Dialogue(m_region, "global");
	m_region->insertDialogue(m_dialogue);

	return 0;
}

int EventSystem::addSpeaker(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;

	int argc = lua_gettop(L);
	if (lua_isnil(L,1))
	{
		return 0;
	}
	
	if (argc>=1 && lua_isnumber(L,1))
	{
		
		int speaker = int (lua_tonumber(L, 1));

		std::string refname ="";
		if (argc>=1 && lua_isstring(L,1))
		{
			refname = lua_tostring(L,2);
		}

		m_dialogue->addSpeaker(speaker,refname);
	}
	else
	{
		ERRORMSG("Syntax: addSpeaker(int speaker, string refname)");
	}

	return 0;
}

int EventSystem::getSpeaker(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;

	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string refname = lua_tostring(L,1);
		int id = m_dialogue->getSpeaker(refname);
		lua_pushnumber(L,id);
	}
	else
	{
		ERRORMSG("Syntax: int getSpeaker(string refname)");
		return 0;
	}

	return 1;
}


int EventSystem::setTopicBase(lua_State *L)
{
	if (m_dialogue ==0)
		return 0;

	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string base = lua_tostring(L, 1);
		m_dialogue->setTopicBase(base);
	}
	else
	{
		ERRORMSG("Syntax: setTopicBase(string text)");
	}

	return 0;
}

int EventSystem::setRefName(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isnumber(L,1) && lua_isstring(L,2))
	{
		int id = int (lua_tonumber(L,1));
		std::string refname = lua_tostring(L,2);

		WorldObject* wo = m_region->getObject(id);
		if (wo!=0 && wo->isCreature())
		{
			static_cast<Creature*>(wo)->setRefName(refname);
		}
	}
	else
	{
		ERRORMSG("Syntax: setRefName(int id, string name)");
	}

	return 0;
}

int EventSystem::setDialogueActive(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 &&  lua_isboolean(L,1))
	{
		if (m_dialogue ==0)
			return 0;
		
		bool act = lua_toboolean(L,1);
		m_dialogue->setActive(act);
	}
	else
	{
		ERRORMSG("Syntax: setDialogueActive(true | false)");
	}
	return 0;
}

int EventSystem::setCurrentDialogue(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 &&  lua_isnumber(L,1))
	{
		int id = int (lua_tonumber(L,1));
		WorldObject* wo = m_region->getObject(id);
		if (wo!=0 && wo->isCreature())
		{
			m_dialogue = static_cast<Creature*>(wo)->getDialogue();
		}
	}
	else
	{
		ERRORMSG("Syntax: setCurrentDialogue(int objectid)");
	}
	return 0;
}

int EventSystem::getRolePlayers(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		lua_newtable(L);

		std::set< int >& members = World::getWorld()->getParty(Fraction::PLAYER)->getMembers();
		std::set< int >::iterator it;

		WorldObject* wo;
		Player* pl;

		std::string role = lua_tostring(L,1);

		int cnt =1;
		for (it = members.begin(); it != members.end(); ++it)
		{
			wo = World::getWorld()->getPlayer(*it);
			pl = dynamic_cast<Player*>(wo);
			if (pl !=0 && pl->checkRole(role))
			{
				lua_pushnumber(L,(*it));
				lua_rawseti (L, -2, cnt);
				cnt++;
			}
		}


	}
	else
	{
		ERRORMSG("Syntax: getRolePlayers(string role)");
		return 0;
	}

	return 1;
}

int EventSystem::teleportPlayer(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=3 && lua_isnumber(L,1) && lua_isstring(L,2) && lua_isstring(L,3))
	{
		int id = lua_tointeger(L, 1);
		std::string regname = lua_tostring(L, 2);
		std::string locname = lua_tostring(L, 3);
		
		RegionLocation regloc;
		regloc.first = regname;
		regloc.second = locname;

		WorldObject* wo =World::getWorld()->getPlayer(id);
		if (wo !=0 && wo->getRegion() !=0)
		{
			if (wo->getRegion()->getName() == regname)
			{
				// Spieler ist schon in der Richtigen Region
				Vector pos = wo->getRegion()->getLocation(locname);
				wo->getRegion()->getFreePlace (wo->getShape(), wo->getLayer(), pos);
				wo->moveTo(pos);
			}
			else
			{
				wo->getRegion()->insertPlayerTeleport(wo->getId(),regloc);
			}
		}
	}
	else
	{
		ERRORMSG("Syntax: objectIsInRegion( int playerid, string regionname, string locationname)");
	}
	return 0;
}

int EventSystem::createEvent(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		std::string trigger = lua_tostring(L,1);
		bool once = false;
		if (argc>=2 && lua_isboolean(L,2))
		{
			once = lua_toboolean(L,2);
		}
		
		Region* reg = m_region;
		if (argc>=3 && lua_isstring(L,3))
		{
			reg = World::getWorld()->getRegion(lua_tostring(L,3));
		}
		if (reg ==0)
		{
			return 0;
			DEBUG("region for createEvent does not exist");
		}
		
		m_event = new Event();
		m_event->setOnce(once);
		reg ->addEvent(trigger,m_event);
	}
	else
	{
		ERRORMSG("Syntax: createEvent(string triggername [,bool once [,string regionname]])");
	}
	return 0;
}

int EventSystem::addCondition(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		if (m_event !=0)
		{
			std::string cond = lua_tostring(L,1);
			m_event->setCondition(cond.c_str());
		}
	}
	else
	{
		ERRORMSG("Syntax: addCondition(string luacode)");		
	}
	return 0;
}

int EventSystem::addEffect(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=1 && lua_isstring(L,1))
	{
		if (m_event !=0)
		{
			std::string effect = lua_tostring(L,1);
			m_event->setEffect(effect.c_str());
		}
	}
	else
	{
		ERRORMSG("Syntax: addEffect(string luacode)");		
	}
	return 0;
}

int EventSystem::getRelation(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=2 && lua_isstring(L,1) && lua_isstring(L,2))
	{
		std::string frac1 = lua_tostring(L,1);
		std::string frac2 = lua_tostring(L,2);
		
		Fraction::Relation rel = World::getWorld()->getRelation(frac1,frac2);
		std::string result="neutral";
		if (rel == Fraction::ALLIED)
			result="allied";
		if (rel == Fraction::HOSTILE)
			result="hostile";
		
		lua_pushstring(L,result.c_str());
		return 1;
	}
	else
	{
		ERRORMSG("Syntax: getRelation(string fraction1, string fraction2)");		
	}
	return 0;
}

int EventSystem::setRelation(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc>=3 && lua_isstring(L,1) && lua_isstring(L,2) && lua_isstring(L,3))
	{
		std::string frac1 = lua_tostring(L,1);
		std::string frac2 = lua_tostring(L,2);
		std::string relation = lua_tostring(L,3);
		
		Fraction::Relation rel = Fraction::NEUTRAL;
		if (relation == "allied" || relation =="ALLIED")
			rel = Fraction::ALLIED;
		if (relation == "hostile"  || relation == "HOSTILE")
			rel = Fraction::HOSTILE;
		
		World::getWorld()->setRelation(frac1,frac2,rel);
	}
	else
	{
		ERRORMSG("Syntax: setRelation(string fraction1, string fraction2, 'neutral' | 'allied' | 'hostile')");		
	}
	return 0;
}

int EventSystem::writeString(lua_State *L)
{
	std::string s;
	s = lua_tostring (L,1);
	m_charconv->toBuffer<char>(1);
	m_charconv->toBuffer(s);
	return 0;
}

int EventSystem::writeNewline(lua_State *L)
{
	m_charconv->printNewline();
	return 0;
}

void EventSystem::writeSavegame(CharConv* savegame)
{
	m_charconv = savegame;
	if (m_lua != 0)
	{
		EventSystem::doString("writeSavegame();");
		m_charconv->toBuffer<char>(0);
	}
}

void EventSystem::readSavegame(CharConv* savegame, int playerid, bool local_player)
{
	std::stringstream stream;
	stream << "addPlayer(" <<playerid <<");";
	doString(stream.str().c_str());

	stream.str("");
	stream << "playervars["<<playerid<<"].";
	std::string prefix = stream.str();

	std::string instr,instr_pr;
	std::string tablename;
	DEBUG5("prefix %s",prefix.c_str());

	char c=1;
	while (c==1)
	{
		c=0;
		savegame->fromBuffer(c);
		if (c==0)
			break;

		savegame->fromBuffer(instr);
		if (local_player)
		{
			doString(instr.c_str());

			tablename = instr.substr(0,instr.find_first_of ('='));
			DEBUG5("table name %s",tablename.c_str());
			stream.str("");
			stream << "quests."<<tablename <<" = "<<tablename;
			DEBUG5("instr %s",stream.str().c_str());
			doString(stream.str().c_str());
		}

		if (World::getWorld() !=0 && World::getWorld()->isServer())
		{
			instr_pr = prefix;
			instr_pr += instr;
			doString(instr_pr.c_str());
		}
	}

	if (not local_player)
	{
		stream.str("");
		stream << "initPlayerVars("<<playerid<<");";
		doString(stream.str().c_str());
	}
}

void EventSystem::clearPlayerVarString(int id)
{
	m_player_varupdates.erase(id);
}

std::string EventSystem::getPlayerVarString(int id)
{
	if (m_player_varupdates.count(id) ==0)
	{
		return "";
	}
	return m_player_varupdates[id];
}

int EventSystem::writeUpdateString(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc <2)
		return 0;

	int id = (int) lua_tonumber(L,1);
	std::string instr =  lua_tostring(L,2);

	m_player_varupdates[id] += instr;
	return 0;
}

int EventSystem::luagettext(lua_State *L)
{
	int argc = lua_gettop(L);

	if (argc<1)
	{
		lua_pushstring(L,"");
		return 1;
	}

	DEBUG5("to translate: %s",lua_tostring(L,1));
	std::string text="return ";
	std::string transl = dgettext("event",lua_tostring(L,1));
	
	// testen, ob es zusammengesetzter String ist
	bool complex = false;
	
	if (transl.find("\'..") != std::string::npos || transl.find("\"..") != std::string::npos || transl.find("]]..") != std::string::npos)
	{
		complex = true;
	}
	
	if (!complex)
		text += "[[";

	text += transl;

	if (!complex)
		text += "]];";

	DEBUG5("return string %s",text.c_str());
	doString(text.c_str());
	return 1;

}


