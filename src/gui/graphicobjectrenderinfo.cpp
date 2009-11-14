#include "graphicobjectrenderinfo.h"

#include "graphicmanager.h"

GraphicRenderInfo::GraphicRenderInfo(std::string parent)
	:m_objects(),m_action_infos(), m_action_references()
{
	m_parent = parent;
	m_parent_ptr =0;
	m_inherit_mask = INHERIT_ALL;
}

GraphicRenderInfo::~GraphicRenderInfo()
{
	std::multimap<std::string, ActionRenderInfo*>::iterator it;
	for (it =m_action_infos.begin(); it != m_action_infos.end(); ++it)
	{
		delete	it->second;
	}
}

ActionRenderInfo* GraphicRenderInfo::getActionRenderInfo(std::string action,int random_action_nr)
{
	ActionRenderInfo* info = getInheritedActionRenderInfo(action, random_action_nr);
	
	// Falls keine Informationen gefunden: Aktion reduzieren (Anhaenge mit # weglassen)
	size_t pos = action.find_last_of('#');
	while (info == 0 && pos != std::string::npos)
	{
		action.erase(pos);
		DEBUG5("reduced action %s",action.c_str());
		
		info = getInheritedActionRenderInfo(action,random_action_nr);
		pos = action.find_last_of('#');
	}
	return info;
}

ActionRenderInfo* GraphicRenderInfo::getOwnActionRenderInfo(std::string action,int random_action_nr)
{
	std::multimap<std::string, ActionRenderInfo*>::iterator it,jt;
	
	it = m_action_infos.lower_bound(action);
	jt = m_action_infos.upper_bound(action);

	// keine Daten vorhanden
	if ( m_action_infos.count(action) ==0)
	{
		// pruefen, ob eine Referenz existiert
		std::map<std::string, std::string>::iterator rt;
		rt = m_action_references.find(action);
		if (rt != m_action_references.end() && rt->second != action)
		{
			return getOwnActionRenderInfo(rt->second, random_action_nr);
		}
		else
		{
			return 0;
		}
	}

	// unter allen gleichmaessig einen auswuerfeln
	ActionRenderInfo* info;

	int count =0;
	std::multimap<std::string, ActionRenderInfo*>::iterator nt = it;
	while (nt != jt)
	{
		count ++;
		++nt;
	}
	
	int nr = random_action_nr % count;
	for (int i=0; i<nr; i++)
	{
		++it;
	}
	info = it->second;

	return info;	
}

ActionRenderInfo* GraphicRenderInfo::getInheritedActionRenderInfo(std::string action,int  random_action_nr)
{
	DEBUG5("get info for %s (%p)",action.c_str(),this);
	ActionRenderInfo*  info = getOwnActionRenderInfo(action, random_action_nr);
	if (info ==0 && m_parent !="")
	{
		if (m_parent_ptr == 0)
		{
			m_parent_ptr = GraphicManager::getRenderInfo(m_parent);
			DEBUG5("parent render info %s %p",m_parent.c_str(), m_parent_ptr);
		}
		
		if (m_parent_ptr != 0)
		{
			info = m_parent_ptr->getInheritedActionRenderInfo(action, random_action_nr);
		}
	}
	return info;
}

GraphicRenderInfo* GraphicRenderInfo::getParentInfo()
{
	if (m_parent =="")
		return 0;
	
	if (m_parent_ptr == 0)
	{
		m_parent_ptr = GraphicManager::getRenderInfo(m_parent);
		DEBUG5("parent render info %s %p",m_parent.c_str(), m_parent_ptr);
	}
	return m_parent_ptr;
	
}

bool GraphicRenderInfo::checkActionInheritMask(ActionRenderpart::Type arpart)
{
	if (m_inherit_mask == INHERIT_ALL)
		return true;
	
	unsigned int mask = 0;
	if (arpart == ActionRenderpart::ANIMATION)
		mask = INHERIT_ANIMATION;
	if (arpart == ActionRenderpart::ROTATION)
		mask = INHERIT_ROTATION;
	if (arpart == ActionRenderpart::MOVEMENT)
		mask = INHERIT_MOVEMENT;
	if (arpart == ActionRenderpart::SCALE)
		mask = INHERIT_SCALE;
	if (arpart == ActionRenderpart::SOUND)
		mask = INHERIT_SOUND;
	
	return checkInheritMask(mask);
}
