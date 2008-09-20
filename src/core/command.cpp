#include "command.h"

void Command::toString(CharConv* cv)
{
	cv->toBuffer((short) m_type);
	cv->toBuffer( m_goal_coordinate_x);
	cv->toBuffer(m_goal_coordinate_y);
	cv->toBuffer(m_goal_object_id);
	cv->toBuffer(m_range);	
}

void Command::fromString(CharConv* cv)
{
	short tmp;
	cv->fromBuffer(tmp);
	m_type = (Action::ActionType) tmp;
	cv->fromBuffer( m_goal_coordinate_x);
	cv->fromBuffer(m_goal_coordinate_y);
	cv->fromBuffer(m_goal_object_id);
	cv->fromBuffer(m_range);
	m_damage_mult =1;	
}


