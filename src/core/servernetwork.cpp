#include "servernetwork.h"

NetworkSlot::NetworkSlot(SystemAddress client_address,RakPeerInterface* peer)
{
	m_peer = peer;
	m_system_address = client_address;
	m_status = NET_CONNECTED;
}

NetworkSlot::~NetworkSlot()
{
	while (numberMessages()!=0)
	{
		m_peer->DeallocatePacket(popReceivedPacket());
	}
}

void NetworkSlot::pushReceivedPacket(Packet* packet)
{
	m_received_packets.push(packet);

}

Packet* NetworkSlot::popReceivedPacket()
{
	Packet* tmp = m_received_packets.front();
	m_received_packets.pop();
	return tmp;
}

int NetworkSlot::numberMessages()
{
	return m_received_packets.size();
}




ServerNetwork::ServerNetwork(int max_slots)
	: Network()
{
	m_max_slots = max_slots;

}

NetStatus ServerNetwork::init( int auth_port )
{
	SocketDescriptor sock(auth_port,0);

	m_peer->Startup(m_max_slots /* max. Anzahl Verbindungen*/, 10/*sleep time*/, &sock, 1);
	m_peer->SetMaximumIncomingConnections(m_max_slots);

	m_slots = new NetworkSlot*[m_max_slots];
	for (int i=0;i< m_max_slots;i++)
	{
		m_slots[i] =0;
	}

	return NET_OK;
}

ServerNetwork::~ServerNetwork()
{
	for (int i=0;i< m_max_slots;i++)
	{
		if (m_slots[i]!=0)
			delete m_slots[i];
	}
	delete[] m_slots;
	m_peer->Shutdown(100);
}


unsigned char ServerNetwork::getPacketIdentifier(Packet *p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	else
		return (unsigned char) p->data[0];
}



void ServerNetwork::update()
{
    if (m_peer==0)
        return;


	Packet* packet = m_peer->Receive();
	unsigned char id;
	int slot;
	while (packet !=0)
	{
		id = getPacketIdentifier(packet);
		slot = getSlotByAddress(packet->systemAddress);

		if (id <  ID_USER_PACKET_ENUM)
		{
			switch(id)
			{
				case ID_NEW_INCOMING_CONNECTION:
					slot = insertNewSlot(packet->systemAddress);
					pushNewLoginSlot(slot);
					DEBUG("connection accepted");
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

				default:
					DEBUG("unknown id: %i",id);
					break;
			}

			deallocatePacket(packet);
		}
		else
		{

			if (slot !=-1)
			{
				//DEBUG("packet received");
				m_slots[slot]->pushReceivedPacket(packet);
			}
			else
			{
				DEBUG("fehler: unbekannter absender\n");
				deallocatePacket(packet);
			}
		}
		packet = m_peer->Receive();
	}
}

int  ServerNetwork::insertNewSlot(SystemAddress address)
{
	for (int i=0;i< m_max_slots;i++)
	{
		if (m_slots[i] ==0)
		{
			m_slots[i] = new NetworkSlot(address,m_peer);
			return i;
		}
	}
	return -1;
}

int ServerNetwork::getSlotByAddress(SystemAddress adress)
{
	for (int i=0;i< m_max_slots;i++)
	{
		if (m_slots[i] != 0  && m_slots[i]->getSystemAddress() == adress)
		{
			return i;
		}
	}
	return -1;
}

int ServerNetwork::numberSlotMessages( int slot )
{
	if (m_slots[slot] == 0)
	{
		return 0;
	}
	else
	{
		return m_slots[slot]->numberMessages();
	}
}


void ServerNetwork::popSlotMessage( Packet* &data, int slot)
{

	if (m_slots[slot] ==0)
	{
		data =0;
		return;
	}

	if (m_slots[slot]->numberMessages() ==0)
	{
		data =0;
		return;
	}

	data = m_slots[slot]->popReceivedPacket();

}

void ServerNetwork::pushSlotMessage( RakNet::BitStream * bitStream,int slot, PacketPriority prio,PacketReliability reliability )
{
	if (m_slots[slot] ==0)
		return;

	m_peer->Send(bitStream,prio,reliability , 0,m_slots[slot]->getSystemAddress(), false);
}

int ServerNetwork::popNewLoginSlot()
{
	if (m_new_login_slots.empty())
	{
		return -1;
	}
	else
	{
		int tmp = m_new_login_slots.front();
		m_new_login_slots.pop();
		return tmp;
	}
}

void ServerNetwork::pushNewLoginSlot(int slot)
{
	m_new_login_slots.push(slot);
}

NetStatus ServerNetwork::getSlotStatus( int slot )
{
	if (m_slots[slot] ==0)
	{
		return NET_CLOSE;
	}
	return m_slots[slot]->getStatus();
}


