
#include "nlfgnetworkpacket.h"

NLFGNetworkPacket::NLFGNetworkPacket()
	: m_data(NULL)
{
	m_timestamp = time(NULL);
    m_packet = new NLFG_Message;
    nlfg_init_packet(m_packet);
    
	nlfg_writeByte(m_packet, ID_TIMESTAMP);
	nlfg_writeInteger(m_packet, m_timestamp);
	nlfg_setId(m_packet, ID_USER_PACKET_ENUM);
}

// NLFGNetworkPacket does not copy the data from RakNet packet
// direct content from packet is used instead
NLFGNetworkPacket::NLFGNetworkPacket(NLFG_Message* packet)
{
	m_data = packet->data;
	m_length = packet->size;

	if (nlfg_readByte(packet) == ID_TIMESTAMP)
	{
		m_timestamp = nlfg_readInteger(packet);
	}
	else
	{
		m_timestamp = time(NULL);
	}
	
	m_version = nlfg_readInteger(packet);	// this is written by Network::createPacket
	
	m_packet = packet;
}
