#include "clientmplayer.h"

ClientMPlayer::ClientMPlayer(int id) : ClientWObject(id)
{
	m_equipement=new Equipement(5,14,30);
}

ClientMPlayer::~ClientMPlayer()
{
	DEBUG5("delete m_equipement %p",m_equipement);
	delete m_equipement;
	DEBUG5("done");
}

void ClientMPlayer::fromString(CharConv* cv)
{
	ClientWObject::fromString(cv);
	cv->fromBuffer<char>(m_level);
	cv->fromBuffer<float>(m_max_health);
	cv->fromBuffer<float>(m_health);
	cv->fromBuffer<short>(m_strength);
	cv->fromBuffer<short>(m_dexterity);
	cv->fromBuffer<short>(m_magic_power);
	cv->fromBuffer<short>(m_willpower);
	cv->fromBuffer<short>(m_attribute_points);
	
	int i;
	for (i=0;i<4;i++)
		cv->fromBuffer<short>(m_resistances[i]);
	for (i=0;i<4;i++)
		cv->fromBuffer<short>(m_resistances_cap[i]);
	
	
	cv->fromBuffer<float>(m_max_experience);
	cv->fromBuffer<float>(m_experience);
	cv->fromBuffer<short>(m_armor);
	cv->fromBuffer<short>(m_block);
	cv->fromBuffer<short>(m_attack);
	cv->fromBuffer<short>(m_attack_speed);
	cv->fromBuffer<float>(m_attack_range);
	cv->fromBuffer<float>(m_power);
	
	m_base_damage.fromString(cv);
	m_left_damage.fromString(cv);
	m_right_damage.fromString(cv);
	
	/*
	for (i=0;i<4;i++)
	{
		printf("%s %f - %f\n",Damage::getDamageTypeName((Damage::DamageType) i).c_str(),m_left_damage.m_min_damage[i],m_left_damage.m_max_damage[i]);
		printf("%s %f - %f\n",Damage::getDamageTypeName((Damage::DamageType) i).c_str(),m_right_damage.m_min_damage[i],m_right_damage.m_max_damage[i]);		
	}
	*/
	
	m_base_damage.normalize();
	m_left_damage.normalize();
	m_right_damage.normalize();
	
	
	
	for (i=0;i<6;i++)
		cv->fromBuffer<int>(m_abilities[i]);	
	cv->fromBuffer<float>(m_timer1_perc);
	cv->fromBuffer<float>(m_timer2_perc);
	

}

bool ClientMPlayer::checkAbility(Action::ActionType at)
{
	if (at<0 || at >=192)
		return false;
	
	int nr = at / 32;
	int mask = 1 << (at % 32);
		
	if ((m_abilities[nr] & mask) !=0)
	{
		return true;
	}
	return false;
}

bool ClientMPlayer::checkAbilityLearnable(Action::ActionType at)
{
	if (checkAbility(at))
	{
		// Faehigkeit ist schon erlernt
		return false;
	}
	
	Action::ActionInfo* aci = Action::getActionInfo(at);
	
	if (aci->m_req_level > m_level)
	{
		// Levelvorraussetzung nicht erfuellt
		return false;
	}
		
	
	for (int i=0;i<3;i++)
	{
		if (!checkAbility(aci->m_req_ability[i]))
		{
			// Faehigkeiten Vorraussetzung nicht erfuellt	
			return false;
		}
	}
	return true;
}


