#include "networkstruct.h"
#include "charconv.h"


	
	
void PackageHeader::toString(CharConv* cv)
{
	cv->toBuffer((unsigned char) m_content);
	cv->toBuffer(m_number);
	
}


void PackageHeader::fromString(CharConv* cv)
{
	unsigned char tmp;
	cv->fromBuffer(tmp);
	m_content = (PackageType) tmp;
	
	cv->fromBuffer(m_number);
}

void ClientCommand::toString(CharConv* cv)
{
	cv->toBuffer((char) m_button);
	cv->toBuffer(m_action);
	cv->toBuffer(m_goal.m_x);
	cv->toBuffer(m_goal.m_y);
	cv->toBuffer(m_id);
	cv->toBuffer(m_number);
	cv->toBuffer(m_direction);
	
}


void ClientCommand::fromString(CharConv* cv)
{
	char ctmp;
	cv->fromBuffer(ctmp);
	m_button = (Button) ctmp;
	cv->fromBuffer(m_action);
	cv->fromBuffer(m_goal.m_x);
	cv->fromBuffer(m_goal.m_y);
	cv->fromBuffer(m_id);
	cv->fromBuffer(m_number);
	cv->fromBuffer(m_direction );
	
}





