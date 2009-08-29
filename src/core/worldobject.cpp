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
	
	m_fraction = Fraction::NOFRACTION;
	m_race = "";
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
	cv->toBuffer(m_fraction);
	cv->toBuffer(m_race);
	
}

void WorldObject::fromString(CharConv* cv)
{
	GameObject::fromString(cv);
	cv->fromBuffer(m_fraction);	
	cv->fromBuffer(m_race);
	
}



int WorldObject::getValue(std::string valname)
{
		if (valname == "fraction")
		{
			if (getFraction() < Fraction::NEUTRAL_TO_ALL)
			{
				std::stringstream stream;
				stream << "player_" << getFraction();
				lua_pushstring(EventSystem::getLuaState(),stream.str().c_str());
			}
			else
			{
				Fraction* frac = World::getWorld()->getFraction( getFraction() );
				static std::string fractype = "";
				if (frac != 0)
				{
					fractype = frac->getType();
				}
				lua_pushstring(EventSystem::getLuaState(),fractype.c_str());
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
		Fraction::Id id = World::getWorld()->getFractionId(fraction);
		setFraction(id);
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

void WorldObject::setFraction(Fraction::Id fr)
{
	m_fraction = fr;
}

void WorldObject::setRace(Race race)
{
	m_race = race;
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
