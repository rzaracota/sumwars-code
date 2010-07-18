#ifdef CLIENTNETWORK_H
#else
#define CLIENTNETWORK_H

#include "network.h"
#include <queue>
#include "debug.h"
#include "charconv.h"


/**
 * \class ClientNetwork
 * \brief Abstract class for Network representation on client side
 */
class ClientNetwork : public Network
{
	public:
		/**
		* \brief Constructor
		*/
		ClientNetwork()
			: Network()
		{
		}
	
		/**
		* \brief Destructor
		*/
		virtual ~ClientNetwork()
		{
			clearMessageQueue();
		}
	
	
		/**
		* \brief Establishes the connection to the server
		* \param hostname hostname or IP
		* \param req_port port to connect to
		*
		* The succes of the operation can be queried by getSlotStatus()
		*/
		virtual void  serverConnect(const char* hostname, int req_port )=0;
	
		/**
		* \brief Closes the connection to the server
		*/
		virtual void serverDisconnect() =0;
		
	
	
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
		 * \param slot always pass 0 on client side (value is ignored)
		 *
		 * Returns the Number of packets that are in the receive queue. The slot number is ignored.
		 */
		virtual int numberSlotMessages(int slot=0)
		{
			return m_received_packets.size();
		}
		
		
		/**
		 * \brief Returns the next received packet from the specified slot and removes it from the queue.
		 * \param data Received NetworkPacket, returns a NULL pointer, if packet queue is empty
		 * \param slot always pass 0 on client side (value is ignored)
		 * 
		 * Returns the next received packet removes it from the queue. If no packet was received, a NULL pointer is returned. Delete the Packet with \ref deallocatePacket at the end. 
		 */
		virtual void popSlotMessage( NetworkPacket* &data, int slot=0)
		{
			if (!m_received_packets.empty())
			{
				data = m_received_packets.front();
				m_received_packets.pop();
			}
			else
			{
				data = 0;
			}
		}
		
		/**
		 * \brief Return the status of the connection
		 * \param slot always pass 0 on client side (value is ignored)
		 * \return status of a slot
		 *
		 */
		virtual NetStatus getSlotStatus( int slot=0 )
		{
			return m_status;
		}
		
		/**
		 * \brief deallocates all message in the receive queue and clears the queue
		 */
		void clearMessageQueue()
		{
			while (!m_received_packets.empty())
			{
				deallocatePacket(m_received_packets.front());
				m_received_packets.pop();
			}
		}


	protected:
		/**
		* \brief Liste der empfangenen Packete
		*/
		PacketQueue m_received_packets;



		/**
		* \brief Status der Netzwerkschnittstelle
		*/
		NetStatus m_status;
};

#endif

