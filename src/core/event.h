#ifndef EVENT_H
#define EVENT_H

#include "charconv.h"
#include <list>

/**
 * \struct Event
 * \brief beschreibt ein Ereignis in der Spielwelt
 */
struct Event
{
	/**
	 * \enum EventType
	 * \brief verschiedene Eventarten
	 */
	enum EventType
	{
		OBJECT_CREATED =1,
		OBJECT_DESTROYED = 2,
		OBJECT_STAT_CHANGED =3,
		
		PROJECTILE_CREATED = 5,
		PROJECTILE_DESTROYED = 6,
 		PROJECTILE_STAT_CHANGED = 7,

		PLAYER_JOINED = 10,
		PLAYER_QUIT = 11,
		PLAYER_CHANGED_REGION = 12,
  
		PLAYER_ITEM_EQUIPED = 20,
		PLAYER_NOITEM_EQUIPED = 21,
		PLAYER_ITEM_PICKED_UP = 22,
		
		ITEM_DROPPED = 30,
		ITEM_REMOVED = 31,
		
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
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
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
	 * \param buf Objekt als String
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	void fromString(CharConv* cv)
	{
		char ctmp;
		cv->fromBuffer(ctmp);
		m_type = (EventType) ctmp;
		cv->fromBuffer(m_data);
		cv->fromBuffer(m_id);
	}
	
};

typedef std::list<Event> EventList;

#endif

