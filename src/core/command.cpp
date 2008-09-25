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
		
		
bool Command::operator!=(Command& other)
{
	return (m_type != other.m_type || m_goal_object_id != other.m_goal_object_id 
			|| m_goal_coordinate_x != other.m_goal_coordinate_x
			|| m_goal_coordinate_y != other.m_goal_coordinate_y);
			
	
}



