
#include "rakclientnetwork.h"

#ifdef WIN32
#include <windows.h>
#endif

#include "math.h"

RakClientNetwork::RakClientNetwork()
	: ClientNetwork()
{
	m_peer = RakNetworkFactory::GetRakClientInterface();
	m_server_address = UNASSIGNED_PLAYER_ID;
	m_status = NET_CLOSE;
}

RakClientNetwork::~RakClientNetwork()
{
	kill();
}

void RakClientNetwork::serverConnect(const char* hostname, int req_port )
{
	// the second port is the outgoing port on client side
	int portadd = 1;
	srand ( time(NULL) );
	// this (hacky) line allows to start two clients for testing with 99% prob 
	// portadd = rand() % 100;
	
	bool success = m_peer ->Connect((char*) hostname, req_port, req_port+portadd, 0,0);
	if (!success)
	{
		m_status = NET_ERROR;
	}
	else
	{
		m_status = NET_CONNECTED;
	}
}

void RakClientNetwork::serverDisconnect()
{
	DEBUG("closing connection");
	m_peer->Disconnect(0);
	// wait to allow Raknet to deliver disconnect message
#ifdef WIN32
	Sleep( 30 );
#else
	usleep(30000 );
#endif
	m_status = NET_CLOSE;
}

void RakClientNetwork::kill()
{
	if (m_peer != 0)
	{
		m_peer->Disconnect(0);
		RakNetworkFactory::DestroyRakClientInterface(m_peer);
		m_peer =0;
	}
}


void RakClientNetwork::update()
{
	// Receive all packets from server
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
			// Raknet internal message
			switch(id)
			{

				case ID_CONNECTION_REQUEST_ACCEPTED:
					m_status = NET_CONNECTED;
					// Clear queue, because this data belongs to old connection
					clearMessageQueue();
					m_server_address = packet->playerId;
					DEBUG("connection accepted");
					break;

					
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
					
				case ID_RECEIVED_STATIC_DATA:
				case ID_REMOTE_NEW_INCOMING_CONNECTION:
				case ID_REMOTE_EXISTING_CONNECTION:
					break;
					
				default:
					DEBUG("unknown id: %i",id);
					break;
			}
			
			m_peer->DeallocatePacket(packet);
		}
		else
		{
			
			DEBUGX("packet received");
			// create a new NetPacket from the Raknet packet
			// must not delete packet, because netpacket uses the packets internal data
			NetworkPacket* netpacket = new RakNetworkPacket(packet);
			m_received_packets.push(netpacket);
		}
		packet = m_peer->Receive();
	}
}


void RakClientNetwork::pushSlotMessage(const NetworkPacket* data, int slot, NetReliability reliability) 
{
	// check correct type
	const RakNetworkPacket* rnpacket = dynamic_cast<const RakNetworkPacket*>(data);
	if (rnpacket == 0)
	{
		ERRORMSG("Tried to send a NetworkPacket that is not a RakClientNetwork");
		return;
	}
	
	RakNet::BitStream& bs = const_cast<RakNetworkPacket*>(rnpacket)->getBitStream();
	
	// convert reliability into raknet enums
	PacketReliability rel;
	if (reliability == NET_RELIABLE)
	{
		rel = RELIABLE;
	}
	else if (reliability == NET_RELIABLE_ORDERED)
	{
		rel = RELIABLE_ORDERED;
	}
	else if (reliability == NET_UNRELIABLE)
	{
		rel = UNRELIABLE;
	}
	else
	{
		ERRORMSG("invalid value for reliability");
		return;
	}
	
	// send it!
	m_peer->Send(&bs,MEDIUM_PRIORITY,rel , true);
}

NetworkPacket* RakClientNetwork::createPacket()
{
	NetworkPacket* rnpacket = new RakNetworkPacket();
	
	// set the version number and write it to the packet
	rnpacket->setVersion(getPacketVersion());
	rnpacket->toBuffer(getPacketVersion()); 	// This number is read by the RakClientNetwork(packet) constructor
	
	return rnpacket;
}

void RakClientNetwork::deallocatePacket(NetworkPacket* packet)
{
	// check correct type
	RakNetworkPacket* rnpacket = dynamic_cast<RakNetworkPacket*>(packet);
	if (rnpacket == 0)
	{
		ERRORMSG("Tried to delete a NetworkPacket that is not a RakClientNetwork");
		delete packet;	// really delete it?
		return;
	}
	
	// first delete Raknet Packet, and then the object itself
	Packet* p = rnpacket->getRakPacket();
	m_peer->DeallocatePacket(p);
	delete packet;
}



