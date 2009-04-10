#ifndef NETEVENT_H
#define NETEVENT_H

#include "charconv.h"
#include <list>

/**
 * \struct NetEvent
 * \brief beschreibt ein Ereignis in der Spielwelt das ueber das Netzwerk mitgeteilt wird
 */
struct NetEvent
{
	
	/**
	 * \enum EventType
	 * \brief verschiedene Eventarten
	 */
	enum EventType
	{
		NOEVENT =0,
		OBJECT_CREATED =1,
		OBJECT_DESTROYED = 2,
		OBJECT_STAT_CHANGED =3,
		
		PROJECTILE_CREATED = 5,
		PROJECTILE_DESTROYED = 6,
 		PROJECTILE_STAT_CHANGED = 7,

		PLAYER_JOINED = 10,
		PLAYER_QUIT = 11,
		PLAYER_CHANGED_REGION = 12,
  
		PLAYER_WAYPOINT_DISCOVERED = 15,
  
		PLAYER_ITEM_INSERT = 20,
		PLAYER_ITEM_REMOVE = 21,
		PLAYER_ITEM_SWAP = 22,
  
		PLAYER_PARTY_CHANGED = 24,
		PLAYER_PARTY_CANDIDATE = 25,

 		PARTY_RELATION_CHANGED = 28,
		
		ITEM_DROPPED = 30,
		ITEM_REMOVED = 31,
  
		TRADER_INVENTORY_REFRESH = 50,
		TRADER_ITEM_BUY = 51,
		TRADER_ITEM_SELL = 52,
  
		REGION_CUTSCENE = 40,
		
	};
	
	/**
	 * \var EventType m_type
	 * \brief Gibt die Art des Events an
	 */
	EventType m_type;
	
	/**
	 * \enum ObjectData
	 * \brief Objektdaten die mit dem Event versendet werden
	 */
	enum ObjectData
	{
		DATA_COMMAND =0x1,
		DATA_ACTION =0x2,
		DATA_HP = 0x4,
		DATA_MAX_HP = 0x8,
		DATA_EFFECTS = 0x10,
		DATA_STATUS_MODS = 0x20,
		DATA_TIMER = 0x40,
 		DATA_STATE = 0x80,
		DATA_WALK_SPEED = 0x100,
		DATA_ATTACK_SPEED = 0x200,
		DATA_NEXT_COMMAND = 0x400,
		DATA_ABILITIES = 0x800,
		DATA_ATTRIBUTES	 = 0x1000,
  		DATA_FLAGS = 0x2000,
		DATA_EXPERIENCE=0x4000,
 		DATA_MOVE_INFO = 0x8000,
 		DATA_LEVEL = 0x10000,
		DATA_SPEAK_TEXT = 0x20000,
		DATA_REVIVE_LOCATION = 0x40000,
 		DATA_SKILL_ATTR_POINTS = 0x80000,
		DATA_TRADE_INFO = 0x100000,
		DATA_DIALOGUE = 0x200000,
		DATA_WAYPOINT = 0x400000,
		DATA_FIGHT_STAT= 0x800000,
	};
	
	/**
	 * \enum ProjectileData
	 * \brief Projektildaten, die mit dem Event versendet werden
	 */
	enum ProjectileData
	{
		DATA_GEOMETRY = 0x1,
		DATA_SPEED = 0x2,
		DATA_PROJ_STATE = 0x4,
		DATA_GOAL_OBJECT = 0x8,
		DATA_TYPE = 0x10,
 		DATA_PROJ_TIMER = 0x20,
		DATA_MAX_RADIUS = 0x40,
	};
	
	/**
	 * \var int m_data
	 * \brief Bitmaske die angibt welche Daten gesendet werden
	 */
	int m_data;
	
	/**
	 * \var int m_id
	 * \brief ID des Objektes, das das Ereignis erzeugt hat
	 */
	int m_id;
		
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	 */
	void toString(CharConv* cv)
	{
		cv->toBuffer((char) m_type);
		cv->toBuffer(m_data);
		cv->toBuffer(m_id);
	}
	
	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	void fromString(CharConv* cv)
	{
		char ctmp;
		cv->fromBuffer(ctmp);
		m_type = (EventType) ctmp;
		cv->fromBuffer(m_data);
		cv->fromBuffer(m_id);
	}
	
	/**
	 * \fn NetEvent()
	 * \brief Konstruktor
	 */
	NetEvent()
	{
		m_id =0;
		m_data =0;
		m_type = NOEVENT;
	}
	
};

typedef std::list<NetEvent> NetEventList;

#endif

