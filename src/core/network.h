#ifdef NETWORK_H
#else
#define NETWORK_H

/*
#include "raknet/MessageIdentifiers.h"
#include "raknet/RakNetworkFactory.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/RakNetTypes.h"
#include "raknet/BitStream.h"
*/
#include "PacketEnumerations.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "NetworkTypes.h"
#include "BitStream.h"
#include "RakClientInterface.h"
#include "RakServerInterface.h"

#define ID_USER_PACKET_ENUM  80


#include "debug.h"
#include "networkpacket.h"
#include <queue>



/**
 * \enum NetStatus
 * \brief Network Status values
 */
enum NetStatus
{
	NET_REJECTED = -5,	/*! Connection or Request was rejected */
	NET_SLOTS_FULL=-4,	/*!< Server has no more slots for connections */
	NET_TIMEOUT=-3,		/*!< Request timed out */
	NET_CLOSE=-2,		/*!< Connections was closed */
	NET_ERROR=-1,		/*!< Unspecified error */
	NET_OK=0,		/*!< Operation or Request was succesful */
	NET_CONNECTED		/*!< connections is established */
};



/**
 * \enum SpecialSlot
 * \brief Special Slotnumbers
 * \sa NetworkSlot
 */
enum SpecialSlot
{
	LOCAL_SLOT = -1, //! Used for packets that were not sent by network, but created locally
 	NOSLOT = -2,
};

/**
 * \enum NetReliability
 * \brief Packet transmission quality types
 */
enum NetReliability
{
	NET_UNRELIABLE,			//! Packet might get lost
	NET_RELIABLE,			//!	Packet is guaranteed to be delivered, but packet order might change
	NET_RELIABLE_ORDERED 	//! Packet is guarenteed to be delivered and packet order will be preserved
};

/**
 * \class Network
 * \brief Virtual base class for network
 */
class Network
{
	public:
		
		
		/**
		 * \brief Constructor
		 */
		Network()
		{
			
		}
		
		/**
		 * \brief Destructor
		 */
		virtual ~Network()
		{
			
		}
		
	
		/**
		 * \brief Shuts down Network library and does all cleanup work
		 */
		virtual void kill() =0;
		
		/**
		 * \fn  void update()
		 * \brief Updates internal state of the wrapper. This function is called once per frame.
		 */
		virtual void update()=0;
		
		/**
		 * \brief Returns the Number of packets that are in the receive queue for the specified slot
		 * \return number of packets in the specified slot
		 * \param slot ID of a Slot
		 *
		 * Returns the Number of packets that are in the receive queue for the specified slot. The slot number is server relevant on host side, client network class will ignore it and return the number of packets received from server.
		 */
		virtual int numberSlotMessages(int slot=0) =0;
		
		
		/**
		 * \brief Returns the next received packet from the specified slot and removes it from the queue.
		 * \param data Received NetworkPacket, returns a NULL pointer, if packet queue is empty
		 * \param slot ID of a Slot
		 * 
		 * Returns the next received packet from the specified slot and removes it from the queue. If no packet was received from the specified slot, a NULL pointer is returned. The slot number is only relevant on server side, client network class will ignore it and return the next packet received from server. Delete the Packet with \ref deallocatePacket at the end. 
		 */
		virtual void popSlotMessage( NetworkPacket* &data, int slot=0) =0;
		
		/**
		 * \brief Sends a packet to the specified slot
		 * \param data Network packet to be sent
		 * \param slot Slot the packet is sent to
		 * \param reliability Reliability of the packet
		 * 
		 * Sends the packet to the specified slot. The slot number is only relevant on server side, client network class will ignore it and send the packet to the server. The packet is still valid after this operation and may be sent to other slots.
		 */
		virtual void pushSlotMessage(const NetworkPacket* data, int slot=0, NetReliability reliability= NET_RELIABLE) =0;
		
		/**
		 * \brief creates a new NetworkPacket
		 * \return new NetworkPacket
		 */
		virtual NetworkPacket* createPacket() = 0;
		
		/**
		 * \param packet packet to be deleted
		 * \brief Deletes the packet.
		 * Deletes the packet. Always use this function to delete NetworkPackets. The pointer is invalid after this operation.
		 */
		virtual void deallocatePacket(NetworkPacket* packet)
		{
			delete packet;
		}
	
		/**
		 * \brief Return the status of a NetworkSlot
		 * \param slot ID of a slot
		 * \return status of a slot
		 *
		 */
		virtual NetStatus getSlotStatus( int slot=0 ) =0;
		
			
		/**
		 * \brief Sets the version number, that is written to the packets
		 * \param version version number
		 */
		void setPacketVersion(int version)
		{
			m_packet_version = version;
		}
		
		/**
		 * \brief returns the version number, that is written to the packets
		 */
		int getPacketVersion()
		{
			return m_packet_version;
		}
		
	private:
		/**
		 * \brief version number that is written to the Packets
		 */
		int m_packet_version;
};

/**
 * Queue of NetworkPackets
 */
typedef std::queue<NetworkPacket*> PacketQueue;

#endif

