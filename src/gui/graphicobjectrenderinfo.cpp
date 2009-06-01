#include "graphicobjectrenderinfo.h"

#include "graphicmanager.h"

GraphicRenderInfo::GraphicRenderInfo(std::string parent)
	:m_objects(),m_action_infos(), m_action_references()
{
	m_parent = parent;
	m_parent_ptr =0;
}

GraphicRenderInfo::~GraphicRenderInfo()
{
	std::multimap<std::string, ActionRenderInfo*>::iterator it;
	for (it =m_action_infos.begin(); it != m_action_infos.end(); ++it)
	{
		delete	it->second;
	}
}

ActionRenderInfo* GraphicRenderInfo::getActionRenderInfo(std::string action)
{
	ActionRenderInfo* info = getInheritedActionRenderInfo(action);
	
	// Falls keine Informationen gefunden: Aktion reduzieren (Anhaenge mit # weglassen)
	size_t pos = action.find_last_of('#');
	while (info == 0 && pos != std::string::npos)
	{
		action.erase(pos);
		DEBUG5("reduced action %s",action.c_str());
		
		info = getInheritedActionRenderInfo(action);
		pos = action.find_last_of('#');
	}
	return info;
}

ActionRenderInfo* GraphicRenderInfo::getOwnActionRenderInfo(std::string action)
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
			return getOwnActionRenderInfo(rt->second);
		}
		else
		{
			return 0;
		}
	}

	// unter allen gleichmaessig einen auswuerfeln
	ActionRenderInfo* info = it->second;

	int count =2;
	++it;
	while (it != jt)
	{
		if (rand() %count ==0)
		{
			info = it->second;
		}
		count ++;
		++it;

	}

	return info;	
}

ActionRenderInfo* GraphicRenderInfo::getInheritedActionRenderInfo(std::string action)
{
	DEBUG5("get info for %s (%p)",action.c_str(),this);
	ActionRenderInfo*  info = getOwnActionRenderInfo(action);
	if (info ==0 && m_parent !="")
	{
		if (m_parent_ptr == 0)
		{
			m_parent_ptr = GraphicManager::getRenderInfo(m_parent);
			DEBUG5("parent render info %s %p",m_parent.c_str(), m_parent_ptr);
		}
		
		if (m_parent_ptr != 0)
		{
			info = m_parent_ptr->getInheritedActionRenderInfo(action);
		}
	}
	return info;
}

