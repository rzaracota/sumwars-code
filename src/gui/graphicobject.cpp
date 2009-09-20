#include "graphicobject.h"
#include "graphicmanager.h"

GraphicObject::GraphicObject(Type type, GraphicRenderInfo* render_info, std::string name, int id)
	: m_attached_objects(), m_subobjects(), m_dependencies()
{
	m_id = id;
	m_type = type;
	m_render_info = render_info;
	m_name = name;
	
	m_action.m_current_action ="";
	m_action.m_current_percent=1.0;
	
	m_top_node = GraphicManager::getSceneManager()->getRootSceneNode()->createChildSceneNode();
	
	if (m_render_info != 0)
	{
		addObjectsFromRenderInfo(render_info);
	}
	else
	{
		MovableObjectInfo mainmesh;
		mainmesh.m_source = type;
		mainmesh.m_type = MovableObjectInfo::ENTITY;
		addMovableObject(mainmesh);
	}
}

GraphicObject::~GraphicObject()
{
	/*
	DEBUG("dependencies");
	std::map<std::string, ObjectDependency >::iterator it;
	std::set<std::string>::iterator jt;
	
	for (it = m_dependencies.begin(); it !=  m_dependencies.end(); ++it)
	{
		DEBUG("* %s",it->first.c_str());
		for (jt = it->second.m_children.begin(); jt != it->second.m_children.end(); ++jt)
		{
			DEBUG("  %s",jt->c_str());
		}
	}
	*/
	DEBUG5("destroying %s",m_name.c_str());
	while (! m_dependencies.empty())
	{
		removeMovableObject(m_dependencies.begin()->first);
	}
	m_top_node->getCreator()->destroySceneNode(m_top_node->getName());
}

void GraphicObject::addObjectsFromRenderInfo(GraphicRenderInfo* info)
{
	std::list<MovableObjectInfo>& objs = info->getObjects();
	std::list<MovableObjectInfo>::iterator it;
		
	for (it = objs.begin(); it != objs.end(); ++it)
	{
		addMovableObject(*it);
	}
	
	GraphicRenderInfo* pinfo = info->getParentInfo();
	if (pinfo != 0)
		addObjectsFromRenderInfo(pinfo);
}

Ogre::MovableObject* GraphicObject::getMovableObject(std::string name)
{
	if (name == "")
		name ="mainmesh";
	
	if (m_attached_objects.count(name) == 0)
		return 0;
	
	return m_attached_objects[name].m_object;
}

Ogre::Entity* GraphicObject::getEntity(std::string name)
{
	return dynamic_cast<Ogre::Entity*>(getMovableObject(name));
}

Ogre::Node* GraphicObject::getParentNode(std::string name)
{
	if (name =="")
		name = "mainmesh";
	
	Ogre::MovableObject* obj = getMovableObject(name);
	Ogre::Node* node=0;
	if (obj != 0)
	{
		node = obj->getParentNode();
	}
	else
	{
			
		std::map<std::string, AttachedGraphicObject >::iterator it;
		it = m_subobjects.find(name);
			
		if (it != m_subobjects.end())
		{
			if (it->second.m_tagpoint !=0)
			{
				node = it->second.m_tagpoint;
			}
			else
			{
				node = it->second.m_object->getTopNode();
			}
		}
	}
	return node;
}

void GraphicObject::setQueryMask(unsigned int mask)
{
	DEBUG5("setting mask %i to %s",mask,m_name.c_str());
	std::map<std::string, AttachedMovableObject>::iterator it;
	Ogre::Entity* ent;
	for (it = m_attached_objects.begin(); it != m_attached_objects.end(); ++it)
	{
		ent = dynamic_cast<Ogre::Entity*>(it->second.m_object);
		if (ent != 0)
		{
			ent->setQueryFlags(mask);
		}
	}
	
	std::map<std::string, AttachedGraphicObject  >::iterator gt;
	GraphicObject* obj;
	for (gt = m_subobjects.begin(); gt != m_subobjects.end(); ++gt)
	{
		obj = gt->second.m_object;
		obj->setQueryMask(mask);
	}
}

void GraphicObject::addMovableObject(MovableObjectInfo& object)
{
	DEBUG5("adding object %s type %i",object.m_objectname.c_str(), object.m_type);
	
	// Meshname und Knochen an den angefuegt wird
	std::string bone;
	std::string mesh ="";
	int pos = object.m_bone.find(':');
	if (pos == -1)
	{
		bone = object.m_bone;
		if (bone != "")
		{
			mesh = "mainmesh";
		}
	}
	else
	{
		mesh = object.m_bone.substr(0,pos);
		bone = object.m_bone.substr(pos+1);
	}
	
	Ogre::Node* node=0;
	
	if (object.m_type == MovableObjectInfo::SUBOBJECT)
	{
		// Unterobjekt einfuegen
		if (m_subobjects.count(object.m_objectname) >0)
		{
			WARNING("graphicobject %s: subobject %s already exists",m_name.c_str(), object.m_objectname.c_str());
			return;
		}
		
		GraphicObject* obj = GraphicManager::createGraphicObject(object.m_source, object.m_objectname, m_id);
		node = obj->getTopNode();
		node->getParent()->removeChild(node);
		
		AttachedGraphicObject attchobj;
		attchobj.m_object = obj;
		attchobj.m_entity =0;
	
		Ogre::TagPoint* tag =0;
		if (object.m_bone == "")
		{
			// Anfuegen an TopKnoten
			getTopNode()->addChild(node);
			
			node->setInheritScale(true);
		}
		else
		{
			// Anfuegen an einen Knochen
			Ogre::Entity* ent = getEntity(mesh);
			if (ent !=0)
			{
				// Anfuegen an den Knoten an dem die Entity haengt
				// und verfolgen des neu angelegten TagPoint
				Ogre::SkeletonInstance* skel= ent->getSkeleton();
				if (skel == 0)
				{
					ERRORMSG("could not attach %s to %s (entity has no skeleton)",object.m_objectname.c_str(),mesh.c_str());
					return;
				}

				Ogre::Bone* boneptr = skel->getBone(bone);
				if (boneptr == 0)
				{
					ERRORMSG("could not attach %s to %s (skeleton has no bone %s",object.m_objectname.c_str(),mesh.c_str(),bone.c_str());
					return;
				}
				
				tag = skel->createTagPointOnBone (boneptr);
				attchobj.m_entity = ent;
				tag->setInheritScale(false);
				
				ent->getParentSceneNode()->addChild(node);
				node = tag;
			}
			else
			{
				ERRORMSG("could not attach %s to %s (entity does not exist)",object.m_objectname.c_str(),mesh.c_str());
				return;
			}
		}
		attchobj.m_tagpoint = tag;
		m_subobjects[object.m_objectname] = attchobj;
		DEBUG5("adding subobject %p with name %s", m_subobjects[object.m_objectname].m_object,  object.m_objectname.c_str());
	}
	else if (object.m_type == MovableObjectInfo::SOUNDOBJECT)
	{
		static int counter;
		std::stringstream sname;
		sname << m_name << ":" << counter << ":"<<object.m_objectname;
		std::string name = sname.str();
		counter++;
		
		if (m_soundobjects.count(object.m_objectname) >0)
		{
			WARNING("soundobject %s: subobject %s already exists",m_name.c_str(), object.m_objectname.c_str());
			return;
		}
		
		SoundObject* obj = SoundSystem::createSoundObject(name);
		
		m_soundobjects[object.m_objectname] = obj;
		DEBUG5("adding soundobject with name %s (%s)",object.m_objectname.c_str(),name.c_str());
	}
	else 
	{
		// normales Ogre::MovableObject anfuegen
		if (m_attached_objects.count(object.m_objectname) >0)
		{
			WARNING("graphicobject %s: subobject %s already exists",m_name.c_str(), object.m_objectname.c_str());
			return;
		}
		
		std::ostringstream ostr;
		ostr << m_name << ":"<<object.m_objectname;
		
		Ogre::MovableObject* obj;
		obj = GraphicManager::createMovableObject(object,ostr.str());
		
		// Objekt mit ID versehen
		obj->setUserAny(Ogre::Any(m_id));
		
		if (object.m_bone == "")
		{
			// Anfuegen an TopKnoten
			Ogre::SceneNode* snode = m_top_node->createChildSceneNode();
			snode->attachObject(obj);
			
			snode->setInheritScale(true);
			node = snode;
			DEBUG5("node %p parent %p",node,m_top_node );
		}
		else
		{
			// Anfuegen an einen Knochen
			Ogre::Entity* ent = getEntity(mesh);
			if (ent !=0)
			{
				try
				{
					Ogre::TagPoint* tag = ent->attachObjectToBone(bone, obj);
					tag->setInheritScale(false);
					tag->setScale(Ogre::Vector3(1,1,1));
					node = tag;
				}
				catch (std::exception& e)
				{
					ERRORMSG("could not attach %s to %s (no skeleton or no bone %s)",object.m_objectname.c_str(),mesh.c_str(),bone.c_str());
					return;
				}
			}
			else
			{
				ERRORMSG("could not attach %s to %s (entity does not exist)",object.m_objectname.c_str(),mesh.c_str());
				return;
			}
		}
		
		m_attached_objects[object.m_objectname].m_object_info = object;
		m_attached_objects[object.m_objectname].m_object = obj;
	}
	
	// StartPosition und -Rotation setzen
	if (node != 0)
	{
		node->setPosition(object.m_position*50);
		node->setOrientation(1,0,0,0);
		node->pitch(Ogre::Degree(object.m_rotation[0]));
		node->yaw(Ogre::Degree(object.m_rotation[1]));
		node->roll(Ogre::Degree(object.m_rotation[2]));
	}
	
	// Abhaengigkeiten eintragen
	ObjectDependency& dep = m_dependencies[object.m_objectname];
	dep.m_parent = mesh;
	if (mesh != "")
	{
		m_dependencies[mesh].m_children.insert(object.m_objectname);
	}
	
}

void GraphicObject::removeMovableObject(std::string name)
{
	DEBUG5("removing object %s",name.c_str());
	while (! m_dependencies[name].m_children.empty())
	{
		DEBUG5("recursion");
		removeMovableObject(*(m_dependencies[name].m_children.begin()));
	}
	
	Ogre::MovableObject* obj = getMovableObject(name);
	if (obj != 0)
	{
		// normales MovableObject entfernen
		Ogre::Node* node = obj->getParentNode();
		Ogre::SceneNode* snode = dynamic_cast<Ogre::SceneNode*>(node);
		Ogre::TagPoint* tag = dynamic_cast<Ogre::TagPoint*>(node);
		
		if (snode != 0)
		{
			snode->detachObject(obj);
			snode->getCreator()->destroySceneNode(snode->getName());
		}
		else if (tag != 0)
		{
			
			tag->getParentEntity()->detachObjectFromBone(obj);
		}
		
		GraphicManager::destroyMovableObject(obj);
		
		std::map<std::string, AttachedMovableObject>::iterator it;
		it = m_attached_objects.find(name);
		if (it != m_attached_objects.end())
		{
			m_attached_objects.erase(it);
		}
	}
	else if (m_soundobjects.find(name) != m_soundobjects.end())
	{
		SoundSystem::deleteSoundObject(m_soundobjects[name]);
		m_soundobjects.erase(name);
		DEBUG5("removing Soundobject %s",name.c_str());
	}
	else
	{
		// SubObject entfernen
		std::map<std::string, AttachedGraphicObject >::iterator jt;
		jt = m_subobjects.find(name);
		if (jt != m_subobjects.end())
		{
			Ogre::TagPoint* tag;
			GraphicObject* obj;
	
			tag = jt->second.m_tagpoint;
			obj = jt->second.m_object;
			
			if (tag !=0 && jt->second.m_entity!=0)
			{
				DEBUG5("tag %p parent %p",tag, jt->second.m_entity);
				jt->second.m_entity->getSkeleton()->freeTagPoint(tag);
			}
			GraphicManager::destroyGraphicObject(obj);
			
			m_subobjects.erase(jt);
		}
	}
	
	
	if (m_dependencies[name].m_parent != "")
	{
		DEBUG5("removing dependency %s -> %s",m_dependencies[name].m_parent.c_str(), name.c_str());
		m_dependencies[m_dependencies[name].m_parent].m_children.erase(name);
	}
	m_dependencies.erase(name);
	
}

void GraphicObject::initAttachedAction(AttachedAction& attchaction, std::string action)
{
	attchaction.m_current_action = action;
	attchaction.m_current_percent = -0.00001;
	
	attchaction.m_arinfo = m_render_info->getActionRenderInfo(action);
	if (attchaction.m_arinfo !=0)
	{
		attchaction.m_time = attchaction.m_arinfo->m_time;
		DEBUG5("action %s  time %f",action.c_str(), attchaction.m_time);
	}
}

void GraphicObject::updateAction(std::string action, float percent)
{
	if (m_render_info ==0)
		return;
	
	DEBUG5("update %s action %s %f",m_name.c_str(),action.c_str(),percent);
	updateAttachedAction(m_action,action,percent);
	
	// weitergeben der Animation
	std::map<std::string, AttachedGraphicObject  >::iterator gt;
	GraphicObject* obj;
	for (gt = m_subobjects.begin(); gt != m_subobjects.end(); ++gt)
	{
		obj = gt->second.m_object;
		obj->updateAction(action,percent);
	}
}

void GraphicObject::updateAttachedAction(AttachedAction& attchaction, std::string action, float percent)
{
	// vorhergehende Aktion beenden
	if (action != attchaction.m_current_action || percent < attchaction.m_current_percent)
	{
		if (attchaction.m_current_percent<1.0)
		{
			updateAttachedAction(attchaction, attchaction.m_current_action, 1.0);
		}
		
		while (!attchaction.m_active_parts.empty())
		{
			removeActiveRenderPart(attchaction.m_active_parts.front());
			attchaction.m_active_parts.pop_front();
		}
		
		initAttachedAction(attchaction,action);
	}
	
	DEBUG5("update action %s %f -> %f", action.c_str(), attchaction.m_current_percent, percent);
	
	// Suche nach neu dazu gekommenen Aktionen
	ActionRenderInfo* arinfo;
	arinfo = attchaction.m_arinfo;
	
	if (arinfo !=0)
	{
		std::list< ActionRenderpart >::iterator it;
		for (it = arinfo->m_renderparts.begin(); it != arinfo->m_renderparts.end(); ++it)
		{
			DEBUG5("render part %s",it->m_animation.c_str());
			if (attchaction.m_current_percent < it->m_start_time && percent >= it->m_start_time)
			{
				DEBUG5("action %s: adding part %s",action.c_str(), it->m_animation.c_str());
				addActiveRenderPart(&(*it));
				if (it ->m_type != ActionRenderpart::DETACH)
				{
					attchaction.m_active_parts.push_back(&(*it));
				}
			}
		}
		
		
		// Suche nach neuen grafischen Objekten
		std::list< std::pair<float,MovableObjectInfo> >:: iterator mt;
		for (mt = arinfo->m_new_objects.begin(); mt != arinfo->m_new_objects.end(); ++mt)
		{
			if (attchaction.m_current_percent < mt->first && percent >= mt->first)
			{
				DEBUG5("adding object %s",mt->second.m_objectname.c_str());
				addMovableObject(mt->second);
			}
		}
		
		// aktive Renderparts aktualisieren
		std::list<ActionRenderpart*>::iterator jt;
		float relpercent;
		for (jt = attchaction.m_active_parts.begin(); jt !=attchaction.m_active_parts.end(); )
		{
			if (percent >= (*jt)->m_end_time)
			{
				removeActiveRenderPart(*jt);
				jt = attchaction.m_active_parts.erase(jt);
			}
			else
			{
				relpercent = (percent - (*jt)->m_start_time) / ( (*jt)->m_end_time -(*jt)->m_start_time);
				updateRenderPart(*jt, relpercent);
				jt++;
			}
		}
		
	}
	else if (action!= "")
	{
		DEBUG5("no action render information for %s",action.c_str());
	}
	
	attchaction.m_current_percent = percent;
}

void GraphicObject::updateState(std::string state, bool active)
{
	if (m_render_info ==0)
		return;

	std::string actionname = state;
	std::map<std::string,  AttachedState>::iterator it;
	it = m_attached_states.find(state);

	if (active)
	{
		actionname +=":activate";
		if (it == m_attached_states.end() || it->second.m_type == AttachedState::DEACTIVATE)
		{
			DEBUG5("activated state %s",state.c_str());
			// Status aktuell noch nicht gesetzt
			AttachedState& astate  = m_attached_states[state];
			astate.m_type = AttachedState::ACTIVATE;
			updateAttachedAction(astate.m_attached_action, actionname, 0.0);
		}
	}
	else
	{
		// Status ist nicht bekannt und soll auch nicht gesetzt werden
		if (it == m_attached_states.end())
			return;
		
		if (it->second.m_type != AttachedState::DEACTIVATE)
		{
			
			DEBUG5("deactivated state %s",state.c_str());
			// Status ist intern noch aktiv
			// aktuelle Aktion beenden
			actionname +=":deactivate";
			updateAttachedAction(it->second.m_attached_action, actionname, 0.0);
			it->second.m_type = AttachedState::DEACTIVATE;
		}
	}
	
	// weitergeben der Animation
	std::map<std::string, AttachedGraphicObject  >::iterator gt;
	GraphicObject* obj;
	for (gt = m_subobjects.begin(); gt != m_subobjects.end(); ++gt)
	{
		obj = gt->second.m_object;
		obj->updateState(state,active);
	}
}

void GraphicObject::updateAllStates(std::set<std::string>& states)
{
	std::set<std::string>::iterator it = states.begin();
	std::map<std::string,  AttachedState>::iterator jt = m_attached_states.begin(),jtold;
	
	while (it != states.end() || jt != m_attached_states.end())
	{
		if (it == states.end() || (jt != m_attached_states.end() && *it > jt->first))
		{
			// Status deaktivieren
			updateState(jt->first,false);
			++jt;
			
		}
		else if (jt == m_attached_states.end() || (it != states.end() && *it < jt->first))
		{
			// Status auf true setzen
			updateState(*it,true);
			++it;
		}
		else if (*it == jt->first)
		{
			if (jt->second.m_type == AttachedState::DEACTIVATE)
			{
				updateState(*it,true);
			}
			// Status ist korrekt gesetzt
			++it;
			++jt;
		}
	}
}

bool GraphicObject::updateSubobject(MovableObjectInfo& object)
{
	std::map<std::string, AttachedGraphicObject >::iterator it;
	it = m_subobjects.find(object.m_objectname);
	
	bool ret = false;
	if (it == m_subobjects.end() && object.m_source != "")
	{
		// Objekt existiert bisher nicht, soll aber angehaengt sein
		addMovableObject(object);
		ret = true;
	}
	else if (it != m_subobjects.end() && object.m_source=="")
	{
		// Objekt ist angehaengt, soll aber nicht angehaengt sein
		removeMovableObject(it->first);
		ret = true;
	}
	else if (it != m_subobjects.end () && it->second.m_object->getType() != object.m_source)
	{
		// angehaengtes Objekt hat nicht den korrekten Typ;
		removeMovableObject(it->first);
		addMovableObject(object);
		ret = true;
	}
	
	return ret;
}

void GraphicObject::update(float time)
{
	if (m_render_info ==0)
		return;
	
	std::map<std::string,  AttachedState>::iterator it;
	float abstime;
	std::string act;
	AttachedAction* attch;
	for (it = m_attached_states.begin();it != m_attached_states.end();)
	{
		attch = &(it->second.m_attached_action);
		abstime =attch->m_time * attch->m_current_percent;
		abstime += time;
		
		act = attch->m_current_action;
		
		if (abstime > attch->m_time)
		{
			if (it->second.m_type == AttachedState::DEACTIVATE)
			{
				// Deaktivierung abgeschlossen
				DEBUG5("deleting attached action %s",it->first.c_str());
				updateAttachedAction(*attch,act,1.0);
				m_attached_states.erase(it++);
				continue;
			}
			else
			{
				abstime = 0;
				if (it->second.m_type == AttachedState::ACTIVATE)
				{
					// Aktivierung abgeschlossen
					it->second.m_type = AttachedState::ACTIVE;
					act = it->first;
					act += ":active";
				}
			}
		}
		DEBUG5("time %f  abstime %f percent %f  type %i",time,abstime,abstime/attch->m_time,it->second.m_type) ;
		
		updateAttachedAction(*attch,act,abstime/attch->m_time);
		++it;
	}
	
	// an Knochen angehaengte Subobjekte aktualisieren
	std::map<std::string, AttachedGraphicObject  >::iterator gt;
	Ogre::TagPoint* tag;
	GraphicObject* obj;
	for (gt = m_subobjects.begin(); gt != m_subobjects.end(); ++gt)
	{
		tag = gt->second.m_tagpoint;
		obj = gt->second.m_object;
		if (tag != 0)
		{
			// Verfolgen eines Tagpoint
			const Ogre::Vector3& pos = tag->_getDerivedPosition();
			const Ogre::Quaternion& dir = tag ->_getDerivedOrientation();
			const Ogre::Vector3& scal = tag->_getDerivedScale();
			obj->getTopNode()->setPosition(pos);
			obj->getTopNode()->setOrientation(dir);
			obj->getTopNode()->setScale(scal);
		}
		
		obj->update(time);
	}

	
	// SoundObjekte aktualisieren
	Ogre::Vector3 opos = getTopNode()->_getDerivedPosition();
	Vector pos(opos.x/50, opos.z / 50);
	std::map<std::string, SoundObject* >::iterator st;
	for (st = m_soundobjects.begin(); st != m_soundobjects.end(); ++st)
	{
		st->second->setPosition(pos);
		st->second->update();
		DEBUG5("setting sound %s position to %f %f",st->first.c_str(),pos.m_x, pos.m_y);
	}
}

void GraphicObject::addActiveRenderPart(ActionRenderpart* part)
{
	
	if (part->m_type == ActionRenderpart::DETACH)
	{
		removeMovableObject(part->m_objectname);
	}
	else if (part->m_type == ActionRenderpart::SOUND)
	{
		std::map<std::string, SoundObject* >::iterator it;
		it = m_soundobjects.find(part->m_objectname);
		if (it != m_soundobjects.end())
		{
			it->second->setSound(part->m_animation);
			DEBUG5("setting sound object %s to sound %s",it->first.c_str(), part->m_animation.c_str());
		}
	}
}

void GraphicObject::removeActiveRenderPart(ActionRenderpart* part)
{
	DEBUG5("removing part %s",part->m_animation.c_str());
	if (part->m_type == ActionRenderpart::ANIMATION)
	{
		// Animation deaktivieren
		Ogre::Entity* ent = getEntity(part->m_objectname);
		if (ent != 0)
		{
			Ogre::AnimationState* anim;
			Ogre::AnimationStateSet* anim_set;
			
			anim_set = ent->getAllAnimationStates();
			
			if (anim_set!= 0 && anim_set->hasAnimationState(part->m_animation))
			{
				anim = ent->getAnimationState(part->m_animation);
				if (anim != 0)
				{
					anim->setEnabled(false);
				}
			}
		}
	}
	else if (part->m_type == ActionRenderpart::SCALE)
	{
		Ogre::MovableObject* obj = getMovableObject(part->m_objectname);
		if (obj != 0)
		{
			Ogre::Node* node = obj->getParentNode();
			Ogre::SceneNode* snode = dynamic_cast<Ogre::SceneNode*>(node);
			if (snode != 0)
			{
				snode->setScale(Ogre::Vector3(part->m_end_val,part->m_end_val,part->m_end_val));
			}
		}
	}
	else if (part->m_type == ActionRenderpart::MOVEMENT || part->m_type == ActionRenderpart::ROTATION)
	{
		Ogre::Node* node = getParentNode(part->m_objectname);
		if (node != 0)
		{
			Ogre::Vector3 pos = part->m_end_vec ;
		
			if (part->m_type == ActionRenderpart::MOVEMENT)
			{
				node->setPosition(pos*50);
			}
			if (part->m_type == ActionRenderpart::ROTATION)
			{
				node->setOrientation(1,0,0,0);
				node->pitch(Ogre::Degree(pos[0]));
				node->yaw(Ogre::Degree(pos[1]));
				node->roll(Ogre::Degree(pos[2]));
			}
		}
	}
}

void GraphicObject::updateRenderPart(ActionRenderpart* part,float  relpercent)
{
	DEBUG5("updating part %s to %f",part->m_animation.c_str(),relpercent);
	if (part->m_type == ActionRenderpart::ANIMATION)
	{
		Ogre::Entity* ent = getEntity(part->m_objectname);
		if (ent != 0)
		{
			DEBUG5("anim %s perc %f",part->m_animation.c_str(), relpercent);
			Ogre::AnimationState* anim;
			Ogre::AnimationStateSet* anim_set;
			
			anim_set = ent->getAllAnimationStates();
			
			if (anim_set != 0 && anim_set->hasAnimationState(part->m_animation))
			{
				
				anim = ent->getAnimationState(part->m_animation);
				if (anim != 0)
				{
					anim->setEnabled(true);
					anim->setTimePosition(relpercent*anim->getLength());
				}
			}
		}
	}
	else if (part->m_type == ActionRenderpart::SCALE)
	{
		Ogre::Node* node = getParentNode(part->m_objectname);
		
		if (node != 0)
		{
			float scal = part->m_start_val + relpercent*(part->m_end_val - part->m_start_val);
			node->setScale(Ogre::Vector3(scal,scal,scal));
			DEBUG5("Scale Node %p %f",node,scal);
		}
	}
	else if (part->m_type == ActionRenderpart::MOVEMENT || part->m_type == ActionRenderpart::ROTATION)
	{
		Ogre::Node* node = getParentNode(part->m_objectname);
		if (node != 0)
		{
			Ogre::Vector3 pos = part->m_start_vec + relpercent*(part->m_end_vec - part->m_start_vec);
			DEBUG5("start %f %f %f   end %f %f %f",part->m_start_vec[0],part->m_start_vec[1],part->m_start_vec[2],part->m_end_vec[0],part->m_end_vec[1],part->m_end_vec[2]);
			DEBUG5("Movement %f %f %f relpercent %f",pos[0], pos[1],pos[2],relpercent);
			if (part->m_type == ActionRenderpart::MOVEMENT)
			{
				node->setPosition(pos*50);
			}
			if (part->m_type == ActionRenderpart::ROTATION)
			{
				node->setOrientation(1,0,0,0);
				node->pitch(Ogre::Degree(pos[0]));
				node->yaw(Ogre::Degree(pos[1]));
				node->roll(Ogre::Degree(pos[2]));
			}
		}
	}
}
