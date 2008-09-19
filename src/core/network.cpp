#include "network.h"


Network::Network()
{
	m_peer = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor sock;
	
	m_peer->Startup(1,10,&sock, 1);
	
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

