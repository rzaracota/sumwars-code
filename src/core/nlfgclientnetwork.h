#ifndef NLFGCLIENTNETWORK_H
#define NLFGCLIENTNETWORK_H

#include "clientnetwork.h"
#include "nlfgnetworkpacket.h"

/**
 * \brief client-side network abstraction layer implemented with RakNet
 */
class NLFGClientNetwork : public ClientNetwork
{
	public:
		/**
		 * \brief Constructor
		 */
		NLFGClientNetwork();
	
		/**
		 * \brief Destructor
		 */
		virtual ~NLFGClientNetwork();
	
	
		/**
		 * \brief Establishes the connection to the server
		 * \param hostname hostname or IP
		 * \param req_port port to connect to
		 *
		 * The success of the operation can be queried by getSlotStatus()
		 */
		virtual void serverConnect(const char* hostname, int req_port );
	
		/**
		 * \brief Closes the connection to the server
		 */
		virtual void serverDisconnect();
		
		/**
		 * \brief Shuts down Network library and does all cleanup work
		 */
		virtual void kill();
			
		/**
		 * \fn  void update()
		 * \brief Updates internal state of the wrapper. This function is called once per frame.
		 */
		virtual void update();
			
		/**
		 * \brief Sends a packet
		 * \param data Network packet to be sent
		 * \param slot always pass 0 on client side (value is ignored)
		 * \param reliability Reliability of the packet
		 * 
		 * Sends the packet to the server. The packet is still valid after this operation and must be deleted with \ref deallocatePacket at the end.
		 */
		virtual void pushSlotMessage(const NetworkPacket* data, int slot=0, NetReliability reliability= NET_RELIABLE);
		
		/**
		 * \brief creates a new NetworkPacket
		 * \return new NetworkPacket
		 */
		virtual NetworkPacket* createPacket();
		
		/**
		 * \param packet packet to be deleted
		 * \brief Deletes the packet.
		 * Deletes the packet. Always use this function to delete NetworkPackets. The pointer is invalid after this operation.
		 */
		virtual void deallocatePacket(NetworkPacket* packet);
		
	private:
		/**
		 * \brief Returns Identifier of an NLFG Packet
		 * \param p Packet
		 */
		unsigned char getPacketIdentifier(NLFG_Message *p)
		{
            unsigned char timestamp = nlfg_readByte(p);
			if (timestamp == ID_TIMESTAMP)
				return nlfg_readInteger(p);
			else
				return timestamp;
		}

		/**
		 * \brief address of the server
		 */
		NLFG_Address m_server_address;
};


#endif

