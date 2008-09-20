#include "networkstruct.h"
#include "charconv.h"


	
	
void PackageHeader::toString(CharConv* cv)
{
	cv->toBuffer<unsigned char>((unsigned char) m_content);
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
	cv->toBuffer<char>((char) m_button);
	cv->toBuffer<short>((short) m_action);
	cv->toBuffer(m_coordinate_x);
	cv->toBuffer(m_coordinate_y);
	cv->toBuffer(m_id);
	cv->toBuffer(m_number);
	cv->toBuffer<short>(m_direction);
	
}


void ClientCommand::fromString(CharConv* cv)
{
	char ctmp;
	short tmp;
	cv->fromBuffer<char>(ctmp);
	m_button = (Button) ctmp;
	cv->fromBuffer<short>(tmp);
	m_action = (Action::ActionType) tmp;
	cv->fromBuffer<float>(m_coordinate_x);
	cv->fromBuffer<float>(m_coordinate_y);
	cv->fromBuffer<int>(m_id);
	cv->fromBuffer<int>(m_number);
	cv->fromBuffer<short>(m_direction );
	
}





