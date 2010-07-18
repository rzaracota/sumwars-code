
#include "raknetworkpacket.h"

RakNetworkPacket::RakNetworkPacket()
	: m_bitstream()
{
	m_timestamp = RakNet::GetTime();
	m_bitstream.Write((char) ID_TIMESTAMP);
	m_bitstream.Write(m_timestamp);
	m_bitstream.Write((char) ID_USER_PACKET_ENUM);
	m_rak_packet = 0;
}

// RakNetworkPacket does not copy the data from RakNet packet
// direct content from packet is used instead
RakNetworkPacket::RakNetworkPacket(Packet* packet)
	: m_bitstream((const char*) packet->data, packet->length, false)
{
	char tmp;
	if (packet->data[0] == ID_TIMESTAMP)
	{
		m_bitstream.Read(tmp);		// read the ID_TIMESTAMP
		m_bitstream.Read(m_timestamp);
	}
	else
	{
		m_timestamp = RakNet::GetTime();
	}
	m_bitstream.Read(tmp);			// read the ID_USER_PACKET_ENUM
	m_bitstream.Read(m_version);	// this is written by Network::createPacket
	
	m_rak_packet =  packet;
}


