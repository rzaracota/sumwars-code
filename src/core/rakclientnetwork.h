#ifndef RAK_CLIENT_NETWORK
#define RAK_CLIENT_NETWORK


#include "clientnetwork.h"
#include "raknetworkpacket.h"

/**
 * \brief client-side network abstraction layer implemented with RakNet
 */
class RakClientNetwork : public ClientNetwork
{
	public:
		/**
		 * \brief Constructor
		 */
		RakClientNetwork();
	
		/**
		 * \brief Destructor
		 */
		virtual ~RakClientNetwork();
	
	
		/**
		 * \brief Establishes the connection to the server
		 * \param hostname hostname or IP
		 * \param req_port port to connect to
		 *
		 * The succes of the operation can be queried by getSlotStatus()
		 */
		virtual void  serverConnect(const char* hostname, int req_port );
	
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
		 * \brief Returns Identifier of a Raknet Packet
		 * \param p Raknet Package
		 */
		unsigned char  getPacketIdentifier(Packet *p)
		{
			if ((unsigned char)p->data[0] == ID_TIMESTAMP)
				return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
			else
				return (unsigned char) p->data[0];
		}
		
		/**
		 * \brief Raknet client side network abstraction object
		 */
		RakClientInterface* m_peer;

		/**
		 * \brief address of the server
		 */
		PlayerID m_server_address;
};


#endif


