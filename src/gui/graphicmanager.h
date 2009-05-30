#ifndef GRAPHIC_MANAGER_H
#define GRAPHIC_MANAGER_H


#include "OgreSceneManager.h"
#include "OgreRoot.h"
#include "OgreParticleSystem.h"
#include "OgreEntity.h"
#include "OgreMovableObject.h"
#include "graphicobject.h"
#include "graphicobjectrenderinfo.h"
#include "gameobject.h"

#include "../tinyxml/tinyxml.h"
#include <string>
#include <list>
#include <map>

class GraphicManager
{
	public:
		static void init();
	
		static void cleanup();
	
		static void loadRenderInfoData(const char* pFilename);
		
		static GraphicObject* createGraphicObject(GraphicObject::Type type, std::string name);
		
		static void destroyGraphicObject(GraphicObject* obj);
		
		static Ogre::SceneManager* getSceneManager()
		{
			return m_scene_manager;
		}
		
		static void setSceneManager(Ogre::SceneManager* scene_manager)
		{
			m_scene_manager = scene_manager;
		}
		
		static Ogre::MovableObject* createMovableObject(MovableObjectInfo& info, std::string name);
		
		static void destroyMovableObject(Ogre::MovableObject* obj);
		
		static void registerGraphicMapping(GameObject::Type objecttype, GraphicObject::Type graphic);
		
		GraphicObject::Type getGraphicType(GameObject::Type objecttype);
		
	private:
		static void registerRenderInfo(std::string name, GraphicRenderInfo* info)
		{
			if (m_render_infos.count(name) ==0)
			{
				m_render_infos[name] = info;
			}
		}
		
		static std::map<std::string, GraphicRenderInfo*> m_render_infos;
		
		static std::map<GameObject::Type, GraphicObject::Type> m_graphic_mapping;
		
		static void loadRenderInfos(TiXmlNode* node);
		
		static void loadRenderInfo(TiXmlNode* node, GraphicRenderInfo* info);
		
		static void loadMovableObjectInfo(TiXmlNode* node, MovableObjectInfo* info);
		
		static void loadActionRenderInfo(TiXmlNode* node, ActionRenderInfo* info);
		
		static Ogre::SceneManager* m_scene_manager;
};

#endif

