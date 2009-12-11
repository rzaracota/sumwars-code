#include "network.h"


Network::Network()
{
	m_peer = RakNetworkFactory::GetRakPeerInterface();

	m_peer->SetOccasionalPing(true);

	// m_peer->ApplyNetworkSimulator( 0, 100, 0);
	// m_peer->ApplyNetworkSimulator( 0, 100, 500);
	// m_peer->ApplyNetworkSimulator( 0, 100, 2000);
	DEBUG5("raknet enum %i", ID_USER_PACKET_ENUM);
}

Network::~Network()
{
	delete m_peer;
}

void Network::kill()
{
	m_peer->Shutdown(0);
	RakNetworkFactory::DestroyRakPeerInterface(m_peer);
}

unsigned char  Network::getPacketIdentifier(Packet *p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	else
		return (unsigned char) p->data[0];
}

