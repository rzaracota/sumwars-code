#include "servernetwork.h"

NetworkSlot::~NetworkSlot()
{
	while (!m_received_packets.empty())
	{
		m_server_network->deallocatePacket(m_received_packets.front());
		m_received_packets.pop();
	}
}

NetworkPacket* NetworkSlot::popReceivedPacket()
{
	NetworkPacket* data;
	
	if (!m_received_packets.empty())
	{
		data = m_received_packets.front();
		m_received_packets.pop();
	}
	else
	{
		data = 0;
	}
	return data;
}

///////////////////////////////////////////
// ServerNetwork functions

ServerNetwork::ServerNetwork(int max_slots)
	: Network()
{
    DEBUG("Max slots %d", max_slots);
	m_slots.resize(max_slots);
	m_max_slots = max_slots;
	
	for (int i=0;i< m_max_slots;i++)
	{
		m_slots[i] =0;
	}
}


ServerNetwork::~ServerNetwork()
{
	
}

void ServerNetwork::kill()
{
	for (int i=0;i< m_max_slots;i++)
	{
		if (m_slots[i]!=0)
		{
			delete m_slots[i];
			m_slots[i] = 0;
		}
	}
}

int ServerNetwork::popNewLoginSlot()
{
	if (m_new_login_slots.empty())
	{
		return NOSLOT;
	}
	else
	{
		int tmp = m_new_login_slots.front();
		m_new_login_slots.pop();
		return tmp;
	}
}


