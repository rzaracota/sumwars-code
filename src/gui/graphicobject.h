#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H

#include "OgreMovableObject.h"
#include "OgreSceneNode.h"
#include "OgreTagPoint.h"
#include "OgreBone.h"
#include "OgreSceneManager.h"

#include <string>
#include "gameobject.h"
#include "graphicobjectrenderinfo.h"
#include "sound.h"

class GraphicObject;

struct AttachedMovableObject
{
	MovableObjectInfo m_object_info;
	Ogre::MovableObject* m_object;
};

struct AttachedGraphicObject
{ 
	Ogre::TagPoint* m_tagpoint;
	Ogre::Entity* m_entity;
	GraphicObject* m_object;
};

struct ObjectDependency
{
	std::set<std::string> m_children;
	
	std::string m_parent;
};

struct AttachedAction
{
	AttachedAction()
	{
		m_arinfo =0;
	}
	
	ActionRenderInfo* m_arinfo;
	
	std::list<ActionRenderpart*> m_active_parts;
	
	std::string m_current_action;
		
	float m_current_percent;
	
	float m_time;
};


struct AttachedState
{
	enum Type
	{
		ACTIVATE =1,
 		ACTIVE = 2,
 		DEACTIVATE = 3,
	};
	
	Type m_type;
	
	AttachedAction m_attached_action;
};

/**
 * \class GraphicObject
 * \brief Klasse fuer alle gerenderten Objekte
 */
class GraphicObject
{
	public:
		typedef std::string Type;
		
		GraphicObject(Type type, GraphicRenderInfo* render_info, std::string name, int id);
		
		~GraphicObject();
		
		void updateAction(std::string action, float percent);
		
		Ogre::SceneNode* getTopNode()
		{
			return m_top_node;
		}
		
		void updateState(std::string state, bool active);
		
		void updateAllStates(std::set<std::string>& states);
		
		void update(float time);
		
		bool updateSubobject(MovableObjectInfo& object); 
		
		Type getType()
		{
			return m_type;
		}
		
		void setQueryMask(unsigned int mask);
		
		std::string getName()
		{
			return m_name;
		}
		
	private:
		void addObjectsFromRenderInfo(GraphicRenderInfo* info);
		
		void addMovableObject(MovableObjectInfo& object);
		
		void removeMovableObject(std::string name);
		
		void addActiveRenderPart(ActionRenderpart* part);
		
		void removeActiveRenderPart(ActionRenderpart* part);
		
		void updateAttachedAction(AttachedAction& attchaction, std::string action, float percent);
		
		void updateRenderPart(ActionRenderpart* part, float relpercent);
		
		Ogre::MovableObject* getMovableObject(std::string name);
		
		Ogre::Entity* getEntity(std::string name);
		
		Ogre::Node* getParentNode(std::string name);
		
		void initAttachedAction(AttachedAction& attchaction, std::string action);
		
		/**
		 * \var Type m_type
		 * \brief Typ des Objekts
		 */
		Type m_type;
		
		/**
		 * \var std::map<std::string, AttachedMovableObject> m_attached_objects
		 * \brief Ogre Objekte, die zu dem GraphicsObjekt gehoeren
		 */
		std::map<std::string, AttachedMovableObject> m_attached_objects;
		
		/**
		 * \var std::map<std::string, AttachedGraphicObject > m_subobjects
		 * \brief UnterObjekte des aktuellen GraphicsObjekts
		 */
		std::map<std::string, AttachedGraphicObject > m_subobjects;
		
		/**
		 * \var std::map<std::string, SoundObject* > m_soundobjects
		 * \brief Soundobjekte des aktuellen GraphicsObjekts
		 */
		std::map<std::string, SoundObject* > m_soundobjects;
		
		/**
		 * \var std::map<std::string, std::list<std::string> > m_dependencies
		 * \brief Enthaelt zu jedem Objekt die Liste der abhaengigen Unterobjekte
		 */
		std::map<std::string, ObjectDependency > m_dependencies;
		
		/**
		 * \var GraphicRenderInfo* m_render_info
		 * \brief Informationen, wie dieses Objekt zu rendern ist
		 */
		GraphicRenderInfo* m_render_info;
		
		
		/**
		 * \var std::map<std::string,  AttachedState> m_attached_states
		 * \brief angehaengte Statuus
		 */
		std::map<std::string,  AttachedState> m_attached_states;
		
		Ogre::SceneNode* m_top_node;
		
		std::string m_name;
		
		AttachedAction m_action;
		
		int m_id;
};


#endif
