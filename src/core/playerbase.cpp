#include "playerbase.h"
#include "world.h"

void PlayerLook::operator=(PlayerLook other)
{
	m_gender = other.m_gender;
	m_render_info = other.m_render_info;
	m_emotion_set = other.m_emotion_set;
	m_name = other.m_name;
	m_hair = other.m_hair;
	m_item_suffix = other.m_item_suffix;
}

void PlayerLook::toString(CharConv* cv)
{
	cv->toBuffer<char>(m_gender);
	cv->toBuffer(m_render_info);
	cv->toBuffer(m_emotion_set);
	if (cv->getVersion() >= 13)
	{
		cv->toBuffer(m_hair);
		cv->toBuffer(m_item_suffix);
	}
}

void PlayerLook::fromString(CharConv* cv)
{
	char tmp;
	cv->fromBuffer(tmp);
	m_gender = (Gender) tmp;
	cv->fromBuffer(m_render_info);
	cv->fromBuffer(m_emotion_set);
	if (cv->getVersion() >= 13)
	{
		cv->fromBuffer(m_hair);
		cv->fromBuffer(m_item_suffix);
	}

}

