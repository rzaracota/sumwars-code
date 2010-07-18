#ifdef RAK_RakServerNetwork_H
#else
#define RAK_RakServerNetwork_H

#include "servernetwork.h"
#include "raknetworkpacket.h"

#include <queue>
#include <map>

// Debuggin
#include "debug.h"

/**
 * \brief Class for Raknet server-client connection
 */
class RakNetworkSlot : public NetworkSlot
{
	public:
		/**
		 * \brief Initialises the slot
		 * \param client_address Network address of the client
		 * \param peer Raknet server interface class
		 * \param server_network server network wrapper class
		 */
		RakNetworkSlot(ServerNetwork* server_network, PlayerID client_address,RakServerInterface* peer)
			: NetworkSlot(server_network)
		{
			m_peer = peer;
			m_system_address = client_address;
		}

		/**
		 * \fn ~RakNetworkSlot()
		 * \brief Destructor
		 */
		~RakNetworkSlot()
		{
		}

		
		/**
		 * \fn PlayerID getPlayerID()
		 * \brief Gibt die Adresse des mit diesem Slot verbundenen Client aus
		 */
		PlayerID getPlayerID()
		{
			return m_system_address;
		}

	private:
		/**
		 * \brief Network address of the client that is connected to this slot
		 */
		PlayerID m_system_address;

		/**
		 * \brief Raknet network interface
		 */
		RakServerInterface* m_peer;
};

/**
 * \class RakServerNetwork
 * \brief \brief server-side network abstraction layer implemented with RakNet
 */
class RakServerNetwork: public ServerNetwork
{
	public:
		/**
		 * \fn RakServerNetwork(int max_slots)
		 * \brief Constructor
		 * \param max_slots maximal number of client connections that are accepted by the server
		 */
		RakServerNetwork(int max_slots);

		/**
		 * \fn ~RakServerNetwork()
		 * \brief Destructor
		 */
		virtual ~RakServerNetwork();

		/**
		 * \brief Initializes network interface, starts listening on the port
		 * \param auth_port port for listening
		 * \return network status
		 *
		 */
		virtual NetStatus init( int auth_port );

		/**
		 * \fn void kill()
		 * \brief Shuts down network connections
		 */
		virtual void kill();
		
		/**
		 * \brief Closes a connection to a client
		 * \param slot slot number
		 */
		virtual void clientDisconnect(int slot);

		/**
		 * \fn  void update()
		 * \brief Updates internal state of the wrapper. This function is called once per frame.
		 */
		virtual void update();

		/**
		 * \brief Sends a packet to the specified slot
		 * \param data Network packet to be sent
		 * \param slot Slot the packet is sent to
		 * \param reliability Reliability of the packet
		 * 
		 * Sends the packet to the specified slot. The slot number is only relevant on server side, client network class will ignore it and send the packet to the server. The packet is still valid after this operation and may be sent to other slots.
		 */
		virtual void pushSlotMessage(const NetworkPacket* data, int slot=0, NetReliability reliability= NET_RELIABLE) ;
		
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
		unsigned char getPacketIdentifier(Packet *p)
		{
			
			if ((unsigned char)p->data[0] == ID_TIMESTAMP)
				return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
			else
				return (unsigned char) p->data[0];
		}

		/**
		 * \fn int insertNewSlot(PlayerID addr)
		 * \brief Fuegt einen neuen Client hinzu
		 * \param addr Netzwerkadresse des Client
		 */
		int insertNewSlot(PlayerID addr);

		/**
		 * \fn int getSlotByAddress(PlayerID address)
		 * \brief ermittelt auf der Netzwerkadresse einen Client den Slot auf dem er angemeldet ist
		 * \param address Netzwerkadresse des Client
		 */
		int getSlotByAddress(PlayerID address);

		
		/**
		 * \brief Raknet Server-side network layer object
		 */
		RakServerInterface* m_peer;
};


#endif

