#include "clientwobject.h"

ClientWObject::ClientWObject(int id) : WorldObject(id)
{
	m_health_perc =1;
	
}

void ClientWObject::fromString(CharConv* cv)
{
	
	WorldObject::fromString(cv);
	
	if (getTypeInfo()->m_type != TypeInfo:: TYPE_FIXED_OBJECT)
	{
		char tmp;
		cv->fromBuffer<char>(tmp);
		getTypeInfo()->m_category = (WorldObject::TypeInfo::Category) tmp;
		cv->fromBuffer<char>(tmp);
		getTypeInfo()->m_fraction = (WorldObject::TypeInfo::Fraction) tmp;
		m_action.fromString(cv);
		cv->fromBuffer<float>(m_health_perc);
		cv->fromBuffer<char>(m_status_mods);
		cv->fromBuffer<char>(m_effects);
		if (getTypeInfo()->m_type == TypeInfo:: TYPE_PLAYER)
		{
			char name[32];
			cv->fromBuffer(name,32);
			name[31]=0;
			m_name.assign(name);
		}
		else
		{
			// TODO: Name anhand Typ/ Subtyp setzen
			m_name = WorldObject::getName();
		}
	}
	else
	{
		m_health_perc=1;
		m_status_mods =0;
		m_action.m_type = Action::NOACTION;
	}
	//DEBUG("bytes read: %i",bp-buf);

}

