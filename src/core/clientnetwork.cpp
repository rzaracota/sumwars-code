#include "clientnetwork.h"

#ifndef NO_RAKNET
ClientNetwork::ClientNetwork()
	: Network()
{
	m_peer = RakNetworkFactory::GetRakClientInterface();
	m_server_address = UNASSIGNED_PLAYER_ID;
	m_status = NET_CLOSE;
}

ClientNetwork::~ClientNetwork()
{
	RakNetworkFactory::DestroyRakClientInterface(m_peer);
}

void ClientNetwork::serverConnect( char* hostname, int req_port )
{
	bool success = m_peer ->Connect(hostname, req_port, req_port+1, 0,0);
	if (!success)
	{
		m_status = NET_ERROR;
	}
}

void ClientNetwork::serverDisconnect()
{
	DEBUG("closing connection");
	m_peer->Disconnect(0);
	// warten, damit die disconnect meldung ankommt
#ifdef WIN32
	Sleep( 30 );
#else
	usleep(30000 );
#endif
	m_status = NET_CLOSE;
}

void ClientNetwork::kill()
{
	m_peer->Disconnect(0);
	RakNetworkFactory::DestroyRakClientInterface(m_peer);
	m_peer =0;
}


void ClientNetwork::update()
{
	static int nr=0;
	Packet* packet = m_peer->Receive();
	unsigned char id;
	while (packet !=0)
	{
		nr ++;
		DEBUGX("packets allocated: %i",nr);
		id = getPacketIdentifier(packet);
		
		if (id <  ID_USER_PACKET_ENUM)
		{
			switch(id)
			{
// bei altem Raknet nicht definiert
//				case ID_ALREADY_CONNECTED:
				case ID_CONNECTION_REQUEST_ACCEPTED:
					m_status = NET_CONNECTED;
					while (!m_received_packets.empty())
						m_received_packets.pop();
					m_server_address = packet->playerId;
					DEBUG("connection accepted");
					break;
// bei altem Raknet nicht definiert
					/*
				case ID_CONNECTION_ATTEMPT_FAILED:
					DEBUG("connection refused");
					m_status =NET_REJECTED;
					m_server_address = UNASSIGNED_PLAYER_ID;
					break;
					*/
					
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					DEBUG("no free slots on server");
					m_status =NET_SLOTS_FULL;
					m_server_address = UNASSIGNED_PLAYER_ID;
					break;
				
				case ID_CONNECTION_LOST:
				case ID_DISCONNECTION_NOTIFICATION:
					DEBUG("connection timed out");
					m_status =NET_TIMEOUT;
					m_server_address = UNASSIGNED_PLAYER_ID;
					break;
					
				default:
					DEBUG("unknown id: %i",id);
					break;
			}
			
			deallocatePacket(packet);
		}
		else
		{
			
			DEBUGX("packet received");
			m_received_packets.push(packet);

		}
		packet = m_peer->Receive();
	}
}


int ClientNetwork::numberSlotMessages(int slot)
{
	return m_received_packets.size();
}


void ClientNetwork::popSlotMessage( Packet* &data,int slot)
{
	data = m_received_packets.front();
	m_received_packets.pop();
}

void ClientNetwork::pushSlotMessage( RakNet::BitStream * data,int slot,PacketPriority prio,PacketReliability reliability)
{
	m_peer->Send(data,prio,reliability , true);
}

NetStatus ClientNetwork::getSlotStatus(int slot)
{
	return m_status;
}

#else // ifndef NO_RAKNET
ClientNetwork::ClientNetwork()
	: Network()
{
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
}

ClientNetwork::~ClientNetwork()
{
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
}

void ClientNetwork::serverConnect( char* hostname, int req_port )
{
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
}

void ClientNetwork::serverDisconnect()
{
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
}

void ClientNetwork::kill()
{
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
}


void ClientNetwork::update()
{
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
}


int ClientNetwork::numberSlotMessages(int slot)
{
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
}


void ClientNetwork::popSlotMessage( Packet* &data,int slot)
{
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
}

void ClientNetwork::pushSlotMessage( RakNet::BitStream * data,int slot,PacketPriority prio,PacketReliability reliability)
{
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
}

NetStatus ClientNetwork::getSlotStatus(int slot)
{
	return m_status;
}


#endif

