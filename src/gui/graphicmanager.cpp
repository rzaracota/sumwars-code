#include "graphicmanager.h"
#include "debug.h"
#include "elementattrib.h"

double GraphicManager::g_global_scale = 1;

std::map<std::string, GraphicRenderInfo*> GraphicManager::m_render_infos;
Ogre::SceneManager* GraphicManager::m_scene_manager;
std::map<std::string, GraphicObject::Type> GraphicManager::m_graphic_mapping;
StencilOpQueueListener* GraphicManager::m_stencil_listener;
std::multimap<std::string, Ogre::ParticleSystem*> GraphicManager::m_particle_system_pool;
std::string GraphicManager::m_filename;

void StencilOpQueueListener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation) 
{ 
			
	if (queueGroupId == RENDER_QUEUE_HIGHLIGHT_OBJECTS) // outline glow object 
	{ 
		Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 

		rendersys->clearFrameBuffer(Ogre::FBT_STENCIL); 
		rendersys->setStencilCheckEnabled(true); 
		rendersys->setStencilBufferParams(Ogre::CMPF_NOT_EQUAL,
										  STENCIL_VALUE_FOR_OUTLINE_GLOW, STENCIL_FULL_MASK, 
			Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
	} 
			/*
	if (queueGroupId == RENDER_QUEUE_OUTLINE_GLOW_GLOWS)  // outline glow
	{ 
	Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
	rendersys->setStencilCheckEnabled(true); 
	rendersys->setStencilBufferParams(Ogre::CMPF_NOT_EQUAL,
	STENCIL_VALUE_FOR_OUTLINE_GLOW, STENCIL_FULL_MASK, 
	Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
} 
	if (queueGroupId == RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW)  // full glow - alpha glow
	{ 
	Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
	rendersys->setStencilCheckEnabled(true); 
	rendersys->setStencilBufferParams(Ogre::CMPF_ALWAYS_PASS,
	STENCIL_VALUE_FOR_FULL_GLOW,STENCIL_FULL_MASK, 
	Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
} 

	if (queueGroupId == RENDER_QUEUE_FULL_GLOW_GLOW)  // full glow - glow
	{ 
	Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
	rendersys->setStencilCheckEnabled(true); 
	rendersys->setStencilBufferParams(Ogre::CMPF_EQUAL,
	STENCIL_VALUE_FOR_FULL_GLOW,STENCIL_FULL_MASK, 
	Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_ZERO,false);       
} 
			*/
} 

void StencilOpQueueListener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation) 
{ 
	if (( queueGroupId == LAST_STENCIL_OP_RENDER_QUEUE )

	   ) 
	{ 
		Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
		rendersys->setStencilCheckEnabled(false); 
		rendersys->setStencilBufferParams(); 
	} 
} 

void GraphicManager::init()
{
	m_scene_manager = Ogre::Root::getSingleton().getSceneManager("DefaultSceneManager");
	m_stencil_listener = new StencilOpQueueListener;
	
	m_scene_manager->addRenderQueueListener(m_stencil_listener);

}

void GraphicManager::cleanup()
{
	std::map<std::string, GraphicRenderInfo*>::iterator it;
	for (it = m_render_infos.begin(); it != m_render_infos.end(); ++it)
	{
		delete it->second;
	}
	m_render_infos.clear();
	
	clearParticlePool();
}

void GraphicManager::clearParticlePool()
{
	std::multimap<std::string, Ogre::ParticleSystem*>::iterator pt;
	for (pt = m_particle_system_pool.begin(); pt != m_particle_system_pool.end(); ++pt)
	{
		m_scene_manager->destroyMovableObject(pt->second);
	}
	m_particle_system_pool.clear();
}

GraphicRenderInfo* GraphicManager::getRenderInfo(std::string type)
{
	std::map<std::string, GraphicRenderInfo*>::iterator it;
	it = m_render_infos.find(type);
	GraphicRenderInfo* rinfo =0;
	if (it != m_render_infos.end())
	{
		rinfo = it->second;
	}
	else
	{
		// If the name contains some # symbols, the last # and all subsequent characters are removed
		// abc#def#ghi -> abc#def -> abc -> fail if nothing was found
		size_t pos = type.find_last_of('#');
		if (pos != std::string::npos)
		{
			type.erase(pos);
			rinfo = getRenderInfo(type);
		}
	}
	return rinfo;
}

GraphicObject* GraphicManager::createGraphicObject(GraphicObject::Type type, std::string name, int id)
{
	DEBUGX("creating object %s type %s id %i",name.c_str(), type.c_str(),id);
	GraphicRenderInfo* rinfo =  getRenderInfo(type);
	
	GraphicObject* go = new GraphicObject(type, rinfo,name,id);
	return go;
}


void GraphicManager::destroyGraphicObject(GraphicObject* obj)
{
	if (obj != 0)
	{
		DEBUGX("removing object %s",obj->getName().c_str());
		delete obj;
	}
}

void GraphicManager::detachMovableObject(Ogre::MovableObject* obj)
{
	Ogre::Node* node = obj->getParentNode();
	Ogre::SceneNode* snode = dynamic_cast<Ogre::SceneNode*>(node);
	Ogre::TagPoint* tag = dynamic_cast<Ogre::TagPoint*>(node);
		
	if (snode != 0)
	{
		snode->detachObject(obj);
	}
	else if (tag != 0)
	{
			
		tag->getParentEntity()->detachObjectFromBone(obj);
	}
}

Ogre::MovableObject* GraphicManager::createMovableObject(MovableObjectInfo& info, std::string name)
{
	// this additional ID ensures that name given to OGRE are unique
	static int id =0;
	std::ostringstream stream;
	stream << id;
	name += stream.str();
	id ++;
	
	Ogre::MovableObject* obj=0;
	
	if (info.m_type == MovableObjectInfo::ENTITY)
	{
		Ogre::Entity* obj_ent;
		try
		{
			obj_ent = m_scene_manager->createEntity(name, info.m_source);
			// TODO: really always shadow caster ?
			//obj_ent->setCastShadows(true);
		}
		catch (Ogre::Exception& e)
		{
			WARNING("can't create mesh %s", info.m_source.c_str());
			obj_ent = m_scene_manager->createEntity(name, "dummy_r.mesh");
		}
		obj= static_cast<Ogre::MovableObject*>(obj_ent);
	}
	else if (info.m_type == MovableObjectInfo::PARTICLE_SYSTEM)
	{
		Ogre::ParticleSystem* part =0;
		try
		{
			part = getParticleSystem(info.m_source);
		}
		catch (Ogre::Exception& e)
		{	
			WARNING("can't create particle system %s", info.m_source.c_str());
		}
		obj= static_cast<Ogre::MovableObject*>(part);
	}
	
	if (obj != 0)
	{
		obj->setQueryFlags(0);
	}
	
	return obj;
}

void GraphicManager::destroyMovableObject(Ogre::MovableObject* obj)
{
	if (obj->getMovableType() == "ParticleSystem")
	{
		putBackParticleSystem(static_cast<Ogre::ParticleSystem*>(obj));
	}
	else
	{
		m_scene_manager->destroyMovableObject(obj);
	}
}

SoundName GraphicManager::getDropSound(std::string objecttype)
{
	 // Get the renderinfo
	 GraphicObject::Type gtype = getGraphicType(objecttype);
	 GraphicRenderInfo* ri = getRenderInfo(gtype);
	 if (ri == 0)
		 return "";
	 
	 // search for a drop action
	 ActionRenderInfo* actionri = ri->getActionRenderInfo("drop");
	 if (actionri == 0)
		 return "";
	 
	 // return the first sound found there
	 std::list< ActionRenderpart >::iterator it;
	 for (it = actionri->m_renderparts.begin(); it != actionri->m_renderparts.end(); ++it)
	 {
		 ActionRenderpart& apart = *it;
		 if (apart.m_type == ActionRenderpart::SOUND)
		 {
			 return apart.m_animation;
		 }
	 }
	 return "";
}

void GraphicManager::loadRenderInfoData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		m_filename = pFilename;
		loadRenderInfos(&doc);
	}
	else
	{
		ERRORMSG("Failed to load file %s", pFilename);
	}
}

void GraphicManager::loadRenderInfos(TiXmlNode* node)
{
	TiXmlNode* child;
	GraphicRenderInfo* info;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "RenderInfo"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		std::string name,parent, inherit;
		attr.getString("name",name);
		attr.getString("parent",parent);
		attr.getString("inherit",inherit);
		
		info = new GraphicRenderInfo(parent);
		
		if (inherit != "")
		{
			unsigned int mask =0;
			if (inherit.find("animation") != std::string::npos)
				mask |= GraphicRenderInfo::INHERIT_ANIMATION;
			if (inherit.find("rotation") != std::string::npos)
				mask |= GraphicRenderInfo::INHERIT_ROTATION;
			if (inherit.find("movement") != std::string::npos)
				mask |= GraphicRenderInfo::INHERIT_MOVEMENT;
			if (inherit.find("scale") != std::string::npos)
				mask |= GraphicRenderInfo::INHERIT_SCALE;
			if (inherit.find("transform") != std::string::npos)
				mask |= GraphicRenderInfo::INHERIT_TRANSFORM;
			if (inherit.find("sound") != std::string::npos)
				mask |= GraphicRenderInfo::INHERIT_SOUND;
			if (inherit.find("object") != std::string::npos)
				mask |= GraphicRenderInfo::INHERIT_OBJECTS;
				
			info->setInheritMask(mask);
		}
		
		DEBUGX("registering renderinfo for %s",name.c_str());
		loadRenderInfo(node,info);
		
		registerRenderInfo(name,info);
	}
	else
	{
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadRenderInfos(child);
		}
	}
}

void GraphicManager::loadRenderInfo(TiXmlNode* node, GraphicRenderInfo* info)
{
	TiXmlNode* child;
	ElementAttrib attr;
	for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
	{
		if (child->Type()==TiXmlNode::ELEMENT)
		{
			
			attr.parseElement(child->ToElement());
			
			if ((!strcmp(child->Value(), "Entity")) || (!strcmp(child->Value(), "ParticleSystem")) || (!strcmp(child->Value(), "BillboardSet")) || (!strcmp(child->Value(), "BillboardChain")) || (!strcmp(child->Value(), "Subobject")) || (!strcmp(child->Value(), "Soundobject")))
			{
				MovableObjectInfo minfo;
				loadMovableObjectInfo(child,&minfo);
				DEBUGX("registering object %s",minfo.m_objectname.c_str());
				info->addObject(minfo);
			}
			else if (!strcmp(child->Value(), "Action"))
			{
				std::string actname,refact;
				attr.getString("name",actname);
				attr.getString("reference",refact);
				if (refact != "")
				{
					DEBUGX("found reference %s %s",actname.c_str(), refact.c_str());
					info->addActionReference(actname, refact);
				}
				else if (actname != "")
				{
					ActionRenderInfo* ainfo = new ActionRenderInfo;
					
					loadActionRenderInfo(child,ainfo);
				
					
					DEBUGX("registering action %s",actname.c_str());
					info->addActionRenderInfo(actname,ainfo);
				}
			}
			else if (!strcmp(child->Value(), "State"))
			{
				attr.parseElement(child->ToElement());
				std::string state;
				attr.getString("name",state);
				
				TiXmlNode* child2;
				for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
				{
					if (child2->Type()==TiXmlNode::ELEMENT)
					{
						if (!strcmp(child2->Value(), "Activation") || !strcmp(child2->Value(), "Active") || !strcmp(child2->Value(), "Deactivation"))
						{
							attr.parseElement(child2->ToElement());
							
							float defaulttime=10;
							
							std::string act= state;
							std::string prefix;
							if (!strcmp(child2->Value(), "Activation"))
							{
								defaulttime = 10;
								prefix ="activate:";
							}
							else if (!strcmp(child2->Value(), "Active"))
							{
								defaulttime = 1000;
								 prefix = "active:";
							}
							else if (!strcmp(child2->Value(), "Deactivation"))
							{
								defaulttime = 10;
								prefix = "deactivate:";
							}
							prefix += act;
							act = prefix;
							
							ActionRenderInfo* ainfo = new ActionRenderInfo;
							
							attr.getFloat("time",ainfo->m_time,defaulttime);
							loadActionRenderInfo(child2,ainfo);
							info->addActionRenderInfo(act,ainfo);
							
							DEBUGX("registering state action %s",act.c_str());
						}
					}
				}
			}
			else if (child->Type()!=TiXmlNode::COMMENT)
			{
				WARNING("%s : unexpected element of <RenderInfo>: %s",m_filename.c_str(),child->Value());
			}
		}
	}
}

void GraphicManager::loadActionRenderInfo(TiXmlNode* node, ActionRenderInfo* ainfo)
{
	ElementAttrib attr;
	TiXmlNode* child;
	ActionRenderpart arpart;
	for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
	{
		arpart.m_type = ActionRenderpart::NONE;
						
		attr.parseElement(child->ToElement());
						
		if ((!strcmp(child->Value(), "Entity")) || (!strcmp(child->Value(), "ParticleSystem")) || (!strcmp(child->Value(), "BillboardSet")) || (!strcmp(child->Value(), "BillboardChain")) || (!strcmp(child->Value(), "Subobject")) ||  (!strcmp(child->Value(), "Soundobject")) )
		{
			MovableObjectInfo minfo;
			loadMovableObjectInfo(child,&minfo);
							
			float time;
			attr.getFloat("time",time,0.0);
			DEBUGX("registering dynamic object %s",minfo.m_objectname.c_str());
			ainfo->m_new_objects.push_back(std::make_pair(time,minfo));
		}
		else if (!strcmp(child->Value(), "Animation"))
		{
			arpart.m_type = ActionRenderpart::ANIMATION;
		}
		else if (!strcmp(child->Value(), "Rotation"))
		{
			arpart.m_type = ActionRenderpart::ROTATION;
		}
		else if (!strcmp(child->Value(), "Movement"))
		{
			arpart.m_type = ActionRenderpart::MOVEMENT;
		}
		else if (!strcmp(child->Value(), "Scale"))
		{
			arpart.m_type = ActionRenderpart::SCALE;
		}
		else if (!strcmp(child->Value(), "Visibility"))
		{
			arpart.m_type = ActionRenderpart::VISIBILITY;
		}
		else if (!strcmp(child->Value(), "Detach"))
		{
			arpart.m_type = ActionRenderpart::DETACH;
		}
		else if (!strcmp(child->Value(), "Sound"))
		{
			arpart.m_type = ActionRenderpart::SOUND;
		}
						
		if (arpart.m_type != ActionRenderpart::NONE)
		{
			attr.getString("name",arpart.m_animation);
			attr.getString("objectname",arpart.m_objectname);
			attr.getFloat("start",arpart.m_start_time,0.0);
			attr.getFloat("end",arpart.m_end_time,1.0);
			attr.getFloat("start_val",arpart.m_start_val,0.0);
			attr.getFloat("end_val",arpart.m_end_val,0.0);
							
							
			std::string startvec,endvec;
							
			attr.getString("start_vec",startvec);
			attr.getString("end_vec",endvec);
			if (startvec!= "")
			{
				std::stringstream stream;
				stream.str(startvec);
				stream >> arpart.m_start_vec[0] >> arpart.m_start_vec[1] >> arpart.m_start_vec[2];
				DEBUGX("Startvector %f %f %f", arpart.m_start_vec[0],arpart.m_start_vec[1], arpart.m_start_vec[2]);
			}
			if (endvec!= "")
			{
							
				std::stringstream stream;
				stream.str(endvec);
				stream >> arpart.m_end_vec[0] >> arpart.m_end_vec[1] >> arpart.m_end_vec[2];
				DEBUGX("endvector %f %f %f ",arpart.m_end_vec[0], arpart.m_end_vec[1], arpart.m_end_vec[2]);
			}
							
		}
						
		if (!strcmp(child->Value(), "Detach"))
		{
			attr.getFloat("time",arpart.m_start_time,0.0);
		}
		
		if (!strcmp(child->Value(), "Visibility"))
		{
			attr.getFloat("time",arpart.m_start_time,0.0);
			bool vis;
			attr.getBool("visible",vis,true);
			
			if (vis == false)
				arpart.m_start_val =0;
			else
				arpart.m_start_val = 1;
		}
						
		DEBUGX("adding renderpart %s",arpart.m_animation.c_str());
		ainfo->m_renderparts.push_back(arpart);
	}
}

void GraphicManager::loadMovableObjectInfo(TiXmlNode* node, MovableObjectInfo* info)
{
	ElementAttrib attr;
	attr.parseElement(node->ToElement());
	
	if (!strcmp(node->Value(), "Entity"))
	{
		info->m_type = MovableObjectInfo::ENTITY;
	}
	else if (!strcmp(node->Value(), "ParticleSystem"))
	{
		info->m_type = MovableObjectInfo::PARTICLE_SYSTEM;
	}
	else if (!strcmp(node->Value(), "BillboardSet"))
	{
		info->m_type = MovableObjectInfo::BILLBOARD_SET;
	}
	else if (!strcmp(node->Value(), "BillboardChain"))
	{
		info->m_type = MovableObjectInfo::BILLBOARD_CHAIN;
	}
	else if (!strcmp(node->Value(), "Subobject"))
	{
		info->m_type = MovableObjectInfo::SUBOBJECT;
	}
	else if (!strcmp(node->Value(), "Soundobject"))
	{
		info->m_type = MovableObjectInfo::SOUNDOBJECT;
	}
	
	attr.getString("objectname",info->m_objectname,"mainmesh");
	attr.getString("source",info->m_source);
	attr.getString("bone",info->m_bone);
	attr.getFloat("scale",info->m_scale,1.0);
	
	std::string pos,rot;
							
	attr.getString("position",pos);
	attr.getString("rotation",rot);
	attr.getBool("highlightable",info->m_highlightable,true);
	if (pos!= "")
	{
		std::stringstream stream;
		stream.str(pos);
		stream >> info->m_position[0] >>  info->m_position[1] >>  info->m_position[2];
		
	}
	else
	{
		info->m_position[0] =  info->m_position[1] = info->m_position[2] =0;
	}
	
	if (rot!= "")
	{
		std::stringstream stream;
		stream.str(rot);
		stream >> info->m_rotation[0] >> info->m_rotation[1] >>info->m_rotation[2];
	}
	else
	{
		info->m_rotation[0] = info->m_rotation[1] = info->m_rotation[2] =0;
	}
	
}

void GraphicManager::registerGraphicMapping(std::string objecttype, GraphicObject::Type renderinfo)
{
	DEBUGX("registered graphic %s for object type %s",graphic.c_str(), objecttype.c_str());
	m_graphic_mapping[objecttype] = renderinfo;
}

GraphicObject::Type GraphicManager::getGraphicType(std::string objecttype)
{
	std::map<std::string, GraphicObject::Type>::iterator it;
	it = m_graphic_mapping.find(objecttype);
	
	if (it != m_graphic_mapping.end())
	{
		return it->second;
	}
	
	WARNING("No graphic type for object type %s",objecttype.c_str());
	return "";
}

Ogre::ParticleSystem* GraphicManager::getParticleSystem(std::string type)
{
	// search the pool for a fitting particle system
	std::multimap<std::string, Ogre::ParticleSystem*>::iterator it;
	it = m_particle_system_pool.find(type);

	Ogre::ParticleSystem* part=0;
	static int count =0;

	if (it == m_particle_system_pool.end())
	{
		// no particle system found -> create new one
		std::ostringstream name;
		name << "ParticleSystem"<<count;
		count ++;

		part = m_scene_manager->createParticleSystem(name.str(), type);
		// type is stored in the OGRE any Attribute
		part->setUserAny(Ogre::Any(type));
		DEBUGX("created particlesystem %p %s for type %s",part, name.str().c_str(), type.c_str());
	}
	else
	{
		// take particle system from pool
		part = it->second;
		m_particle_system_pool.erase(it);
		DEBUGX("took particlesystem %s for type %s",part->getName().c_str(), type.c_str());
	}

	part->clear();
	return part;
}

void GraphicManager::putBackParticleSystem(Ogre::ParticleSystem* part)
{
	// store type of the particle system in OGRE any
	std::string type;
	type = Ogre::any_cast<std::string>(part->getUserAny());

	DEBUGX("put back particlesystem %p %s for type %s",part, part->getName().c_str(), type.c_str());

	m_particle_system_pool.insert(std::make_pair(type,part));
}


