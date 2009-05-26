#ifndef GRAPHIC_OBJECT_RENDER_INFO_H
#define GRAPHIC_OBJECT_RENDER_INFO_H

#include "geometry.h"

struct ActionRenderpart
{
	enum Type
	{
		ANIMATION = 1,
		ROTATION = 2,
		MOVEMENT = 3,
		SCALE = 4,
		DETACH = 10,
	};
	
	
	std::string m_objectname;
	std::string m_animation;
	
	
	float m_start_time;
	float m_end_time;
	
	float m_start_val;
	float m_end_val;
	
	Vector m_start_vec;
	Vector m_end_vec;
	
	Type m_type;
};

struct AttachedMovableObject
{
	enum Type
	{
		
		ENTITY = 1,
		PARTICLE_SYSTEM = 2,
 		BILLBOARD_SET = 3,
		BILLBOARD_CHAIN = 4,
	};
	
	Type m_type;
	std::string m_objectname;
	std::string m_source;
	std::string m_bone;
	Ogre::MovableObject* m_object;
};

/**
 * \struct ActionRenderInfo
 * \brief Struktur mit den Informationen zum Rendern einer einzelnen Aktion
 */
struct ActionRenderInfo
{
	/**
	 * \var std::list< ActionRenderpart > m_renderparts
	 * \brief einzelne Bestandteile des Rendervorgangs
	 */
	std::list< ActionRenderpart > m_renderparts;
	
	/**
	 * \var std::list< std::pair<float,AttachedMovableObject> > m_new_objects
	 * \brief Waehrend dieser Aktion neu angehaengte erzeugte Elemente
	 */
	std::list< std::pair<float,AttachedMovableObject> > m_new_objects;
};

/**
 * \struct GraphicRenderInfo
 * \brief Struktur mit den Informationen zum Rendern eines GraphicObject
 */
struct GraphicRenderInfo
{
	/**
	 * \var std::list<AttachedMovableObject> m_objects
	 * \brief Graphische Grundbestandteile des Objekts
	 */
	std::list<AttachedMovableObject> m_objects;
	
	/**
	 * \var std::multimap<std::string, ActionRenderInfo> m_action_infos
	 * \brief Informationen, wie Aktionen zu rendern sind
	 */
	std::multimap<std::string, ActionRenderInfo> m_action_infos;
};

#endif