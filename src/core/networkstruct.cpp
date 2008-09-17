#include "networkstruct.h"
#include "charconv.h"

void ServerHeader::toString(CharConv* cv)
{
	
	cv->toBuffer<unsigned char>((unsigned char) m_content);
	cv->toBuffer(m_objects);
	cv->toBuffer(m_projectiles);
	cv->toBuffer(m_items);
	cv->toBuffer(m_drop_items);	
	cv->toBuffer(m_chatmessage);
	cv->toBuffer(m_trade);
	cv->toBuffer(m_detailed_item);
	
	
}

void ServerHeader::fromString(CharConv* cv)
{
	unsigned char tmp;
	cv->fromBuffer(tmp);
	m_content = (PackageType) tmp;
	cv->fromBuffer<short>(m_objects);
	cv->fromBuffer<short>(m_projectiles);	
	cv->fromBuffer<short>(m_items);
	cv->fromBuffer<short>(m_drop_items);
	cv->fromBuffer<bool>(m_chatmessage);
	cv->fromBuffer<bool>(m_trade);
	cv->fromBuffer<short>(m_detailed_item);
	
	
}
	
	
void ClientHeader::toString(CharConv* cv)
{
	cv->toBuffer<unsigned char>((unsigned char) m_content);
	cv->toBuffer(m_chatmessage);
	
}


void ClientHeader::fromString(CharConv* cv)
{
	unsigned char tmp;
	cv->fromBuffer(tmp);
	m_content = (PackageType) tmp;
	cv->fromBuffer<bool>(m_chatmessage);
	
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





