#ifndef GRAPHIC_MANAGER_H
#define GRAPHIC_MANAGER_H


#include "OgreSceneManager.h"
#include "OgreRoot.h"
#include "OgreParticleSystem.h"
#include "OgreEntity.h"
#include "OgreMovableObject.h"
#include "graphicobject.h"
#include "graphicobjectrenderinfo.h"

#include "../tinyxml/tinyxml.h"
#include <string>
#include <list>
#include <map>

#include <OgreRenderQueueListener.h> 
#include <OgreRenderSystem.h>
#include <OgreRenderQueue.h>
#include <OgreRenderQueueListener.h> 

#ifndef RENDER_QUEUE_MAIN
#define RENDER_QUEUE_MAIN 50
#endif

#define RENDER_QUEUE_HIGHLIGHT_MASK	RENDER_QUEUE_MAIN + 1
#define RENDER_QUEUE_HIGHLIGHT_OBJECTS	RENDER_QUEUE_MAIN + 2
#define LAST_STENCIL_OP_RENDER_QUEUE	RENDER_QUEUE_HIGHLIGHT_OBJECTS

#define STENCIL_VALUE_FOR_OUTLINE_GLOW 1
#define STENCIL_VALUE_FOR_FULL_GLOW 2
#define STENCIL_FULL_MASK 0xFFFFFFFF

class StencilOpQueueListener : public Ogre::RenderQueueListener 
{ 
	public: 
		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
		

		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);

}; 


class GraphicManager
{
	public:
		static void init();
	
		static void cleanup();
	
		static void loadRenderInfoData(const char* pFilename);
		
		static GraphicObject* createGraphicObject(GraphicObject::Type type, std::string name, int id);
		
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
		
		static void detachMovableObject(Ogre::MovableObject* obj);
		
		static void registerGraphicMapping(std::string, GraphicObject::Type graphic);
		
		static GraphicObject::Type getGraphicType(std::string objecttype);
		
		static GraphicRenderInfo* getRenderInfo(std::string name);
		
	private:
		static void registerRenderInfo(std::string name, GraphicRenderInfo* info)
		{
			if (m_render_infos.count(name) ==0)
			{
				m_render_infos[name] = info;
			}
		}
		
		/**
		 * \fn Ogre::ParticleSystem* getParticleSystem(std::string type)
		 * \brief Gibt aus dem Pool ein passendes Partikelsystem aus
		 * \param type Typ des Partikelsystems
		 */
		static Ogre::ParticleSystem* getParticleSystem(std::string type);
	
		/**
		 * \fn void putBackParticleSystem(Ogre::ParticleSystem* part)
		 * \brief Fuegt das Partikelsystem wieder in den Pool ein
	 	*/
		static void putBackParticleSystem(Ogre::ParticleSystem* part);
		
		static std::map<std::string, GraphicRenderInfo*> m_render_infos;
		
		static std::map<std::string, GraphicObject::Type> m_graphic_mapping;
		
		static void loadRenderInfos(TiXmlNode* node);
		
		static void loadRenderInfo(TiXmlNode* node, GraphicRenderInfo* info);
		
		static void loadMovableObjectInfo(TiXmlNode* node, MovableObjectInfo* info);
		
		static void loadActionRenderInfo(TiXmlNode* node, ActionRenderInfo* info);
		
		static Ogre::SceneManager* m_scene_manager;
		
		static StencilOpQueueListener* m_stencil_listener;
		
		/**
		 * \var std::multimap<std::string, Ogre::ParticleSystem*> m_particle_system_pool
		 * \brief interner Pool von Partikelsystem
		 */
		static std::multimap<std::string, Ogre::ParticleSystem*> m_particle_system_pool;
};

#endif

