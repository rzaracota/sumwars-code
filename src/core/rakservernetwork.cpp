

#include "rakservernetwork.h"

///////////////////////////////////////////
// RakServerNetwork functions

RakServerNetwork::RakServerNetwork(int max_slots)
	: ServerNetwork(max_slots)
{
	
}

NetStatus RakServerNetwork::init( int auth_port )
{
	m_peer = RakNetworkFactory::RakNetworkFactory::GetRakServerInterface();
	m_peer->StartOccasionalPing ();
	m_peer->Start(m_max_slots ,0, 10/*sleep time*/, auth_port);

	return NET_OK;
}

RakServerNetwork::~RakServerNetwork()
{
	kill();
}

void RakServerNetwork::kill()
{
	if (m_peer != 0)
	{
		m_peer->Disconnect(0);
		RakNetworkFactory::DestroyRakServerInterface(m_peer);
		m_peer = 0;
	}
	ServerNetwork::kill();
}

void RakServerNetwork::clientDisconnect(int slot)
{
	if (isConnectedSlot(slot))
	{
		m_peer->Kick(static_cast<RakNetworkSlot*>(m_slots[slot])->getPlayerID());
		delete m_slots[slot];
		m_slots[slot]=0;
	}
}

void RakServerNetwork::update()
{
    if (m_peer==0)
        return;


	Packet* packet = m_peer->Receive();
	unsigned char id;
	int slot;
	while (packet !=0)
	{
		id = getPacketIdentifier(packet);
		slot = getSlotByAddress(packet->playerId);

		if (id <  ID_USER_PACKET_ENUM)
		{
			switch(id)
			{
				case ID_NEW_INCOMING_CONNECTION:
					slot = insertNewSlot(packet->playerId);
					pushNewLoginSlot(slot);
					DEBUG("connection accepted for slot %i", slot);
					break;

				case ID_DISCONNECTION_NOTIFICATION:
					DEBUG("slot %i disconnected",slot);
					delete m_slots[slot];
					m_slots[slot]=0;
					break;
					
				case ID_CONNECTION_LOST:
					DEBUG("connection to slot %i lost",slot);
					delete m_slots[slot];
					m_slots[slot]=0;
					break;
					
				case ID_RECEIVED_STATIC_DATA:
					break;

				default:
					DEBUG("unknown id: %i",id);
					break;
			}

			m_peer->DeallocatePacket(packet);
		}
		else
		{
			DEBUGX("received packet %i",id);
			if (slot != NOSLOT)
			{
				// create a new NetPacket from the Raknet packet
				// must not delete packet, because netpacket uses the packets internal data
				NetworkPacket* netpacket = new RakNetworkPacket(packet);
				m_slots[slot]->pushReceivedPacket(netpacket);
			}
			else
			{
				ERRORMSG("Received message from unknown sender");
				m_peer->DeallocatePacket(packet);
			}
		}
		packet = m_peer->Receive();
	}
}

int  RakServerNetwork::insertNewSlot(PlayerID address)
{
	for (int i=0;i< m_max_slots;i++)
	{
		if (m_slots[i] ==0)
		{
			m_slots[i] = new RakNetworkSlot(this,address,m_peer);
			m_slots[i]->setStatus(NET_CONNECTED);
			return i;
		}
	}
	return NOSLOT;
}

int RakServerNetwork::getSlotByAddress(PlayerID adress)
{
	for (int i=0;i< m_max_slots;i++)
	{
		if (m_slots[i] != 0  && static_cast<RakNetworkSlot*>(m_slots[i])->getPlayerID() == adress)
		{
			return i;
		}
	}
	return NOSLOT;
}


void RakServerNetwork::pushSlotMessage(const NetworkPacket* data, int slot, NetReliability reliability)
{
	if (!isConnectedSlot(slot))
	{
		return;
	}

	// check correct type
	const RakNetworkPacket* rnpacket = dynamic_cast<const RakNetworkPacket*>(data);
	if (rnpacket == 0)
	{
		ERRORMSG("Tried to send a NetworkPacket that is not a RakNetworkPacket");
		return;
	}
	
	// wont modify the bitstream, but the function is nonconst ...
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
	
	m_peer->Send(&bs,MEDIUM_PRIORITY,rel, 0,static_cast<RakNetworkSlot*>(m_slots[slot])->getPlayerID(), false);
}


NetworkPacket* RakServerNetwork::createPacket()
{
	NetworkPacket* rnpacket = new RakNetworkPacket();
	
	// set the version number and write it to the packet
	rnpacket->setVersion(getPacketVersion());
	rnpacket->toBuffer(getPacketVersion()); 	// This number is read by the RakNetworkPacket(packet) constructor
	
	return rnpacket;
}

void RakServerNetwork::deallocatePacket(NetworkPacket* packet)
{
	// check correct type
	RakNetworkPacket* rnpacket = dynamic_cast<RakNetworkPacket*>(packet);
	if (rnpacket == 0)
	{
		ERRORMSG("Tried to delete a NetworkPacket that is not a RakNetworkPacket");
		delete packet;	// really delete it?
		return;
	}
	
	// first delete Raknet Packet, and then the object itself
	Packet* p = rnpacket->getRakPacket();
	m_peer->DeallocatePacket(p);
	delete packet;
}


