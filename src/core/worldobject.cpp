#include "worldobject.h"

#include "world.h"
#include "eventsystem.h"
#include "objectfactory.h"
#include "networkstruct.h"


WorldObject::WorldObject( int id)
	: GameObject(id)
{
	setBaseType(WORLDOBJECT);
	init();
	
	m_fraction = NOFRACTION;
	m_category = NOCATEGORY;
}

bool WorldObject::isCreature()
{
	return (getBaseType() == WORLDOBJECT && getType() != "FIXED_OBJECT");
}

bool WorldObject::isLarge()
{
	if (getState() == WorldObject::STATE_STATIC)
	{
		// statische Objekte duerfen in jeder Richtung maximal 3 Gridunits ueberdecken
		Vector pos = getShape()->m_center;
		Vector ext = getShape()->getAxisExtent();
		Vector corner1 = pos - ext;
		Vector corner2 = pos + ext;
		
		int x1 = (int) floor(0.25*corner1.m_x+0.01);
		int y1 = (int) floor(0.25*corner1.m_y+0.01);
		
		int x2 = (int) floor(0.25*corner2.m_x-0.01);
		int y2 = (int) floor(0.25*corner2.m_y-0.01);
		
		if (x2-x1>2 || y2-y1>2)
			return true;
		
		return false;
	}
	else
	{
		// dynamische Objekte duerfen maximal einen Radius von 4 haben
		return (getShape()->getOuterRadius()>4);
	}
	return false;
}

bool WorldObject::moveTo(Vector newpos)
{
	if (World::getWorld()==0 || getRegion()==0)
	{
		setPosition(newpos);

		return true;
	}
	else
	{
		return getRegion()->moveObject(this, newpos);
	}
}


bool  WorldObject::destroy()
{
	return true;
}

void WorldObject::toString(CharConv* cv)
{
	DEBUG5("worldobject::tostring");
	GameObject::toString(cv);
	cv->toBuffer<short>(m_fraction);
	cv->toBuffer<short>(m_category);
	
}

void WorldObject::fromString(CharConv* cv)
{
	GameObject::fromString(cv);
	short tmp;
	cv->fromBuffer<short>(tmp);
	setFraction( (Fraction) tmp);
	
	cv->fromBuffer<short>(tmp);
	setCategory( (Category) tmp);
}



int WorldObject::getValue(std::string valname)
{
		if (valname == "fraction")
		{
			if (getFraction() >=FRAC_PLAYER_PARTY)
			{
				std::stringstream stream;
				stream << "player" << (getFraction() - FRAC_PLAYER_PARTY);
				lua_pushstring(EventSystem::getLuaState(),stream.str().c_str());
			}
			else
			{
				static std::string frac[10] = {"none","human","demon","undead","dwarf","summoner","monster","","neutral","hostile"};
				lua_pushstring(EventSystem::getLuaState(),frac[getFraction()].c_str());
			}
			return 1;
		}
		else
		{
			return GameObject::getValue(valname);
		}

		return 0;
}


bool WorldObject::setValue(std::string valname)
{
	
	if (valname == "fraction")
	{
		if (getType() == "PLAYER")
		{
			DEBUG("You cant change the fraction of a player");
			return false;
		}
		std::string fraction  = lua_tostring(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		if (fraction == "human") setFraction(FRAC_HUMAN);
		else if (fraction == "demon") setFraction(FRAC_DEMON);
		else if (fraction == "undead") setFraction(FRAC_UNDEAD);
		else if (fraction == "dwarf") setFraction(FRAC_DWARF);
		else if (fraction == "summoner") setFraction(FRAC_SUMMONER);
		else if (fraction == "monster") setFraction(FRAC_MONSTER);
		else if (fraction == "neutral") setFraction(FRAC_NEUTRAL_TO_ALL);
		else if (fraction.find("player") != std::string::npos)
		{
 			std::string num = fraction.substr(6);
			std::stringstream stream(num);
			int number;
			stream >> number;
			setFraction((GameObject::Fraction) (FRAC_PLAYER_PARTY + number));
			
		}
		DEBUG("fraction is now %i",getFraction());
		return true;
	}
	else if (valname == "position")
	{
		Vector pos = EventSystem::getVector(EventSystem::getLuaState(),-1);
		
		getRegion()->getFreePlace (getShape(), getLayer(), pos);
		moveTo(pos);
		return true;
	}
	else 
	{
		return GameObject::setValue(valname);
	}
	
	return false;
}

void WorldObject::setFraction(Fraction fr)
{
	m_fraction = fr;
}

void WorldObject::setCategory(Category cat)
{
	m_category = cat;
}

bool WorldObject::takeDamage(Damage* damage)
{
	Trigger* tr = new Trigger("object_hit");
	tr->addVariable("defender",getId());
	tr->addVariable("attacker",damage->m_attacker_id);
	getRegion()->insertTrigger(tr);
	
	return true;
}

bool  WorldObject::reactOnUse (int id)
{
	Trigger* tr = new Trigger("object_use");
	tr->addVariable("used_object",getId());
	tr->addVariable("user",id);
	getRegion()->insertTrigger(tr);
	
	return true;
}

void WorldObject::writeNetEvent(NetEvent* event, CharConv* cv)
{
	GameObject::writeNetEvent(event,cv);
}

void WorldObject::processNetEvent(NetEvent* event, CharConv* cv)
{
	GameObject::processNetEvent(event,cv);
}
