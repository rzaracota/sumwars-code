#ifndef RAKNETWORKPACKET_H

#define RAKNETWORKPACKET_H

#include "PacketEnumerations.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "NetworkTypes.h"
#include "BitStream.h"
#include "RakClientInterface.h"
#include "RakServerInterface.h"
#include "GetTime.h"

#define ID_USER_PACKET_ENUM  100

#include "networkpacket.h"

/**
 * Class for data sent or received with RakNet
 */
class RakNetworkPacket : public NetworkPacket
{
	public:
		/**
		 * \brief Constructor
		 * Creates a new packet and writes the current time as timestamp
		 */
		RakNetworkPacket();
		
		/**
		* \brief Creates a new packet from a RakNet::Packet
		* \param packet RakNet Packet
		*/
		RakNetworkPacket(Packet* packet);
		
		/**
		* \brief writes raw data to the buffer
		* \param data raw data
		* \param size length of data in bytes
		*/
		virtual void toBuffer(const char* data, unsigned int size)
		{
			m_bitstream.Write(data,size);
		}
			
			
		/**
		* \brief reads raw data from buffer
		* \param data output buffer for raw data
		* \param size length of data in bytes
		*/
		virtual void fromBuffer(char* data, unsigned int size)
		{
			m_bitstream.Read(data,size);
		}
			
			
		/**
		* \brief writes a string to the buffer
		* \param s String
		*/
		virtual void toBuffer(const std::string s)
		{
			toBuffer(static_cast<int>(s.size()));
			toBuffer(s.data(),s.size());
		}
			
			
		/**
		* \brief reads a string from the buffer
		* \param s String
		*/
		virtual void fromBuffer(std::string& s)
		{
			int len;
			fromBuffer(len);
			
			char * data = new char[len];
			fromBuffer(data,len);
			s.assign(data,len);
			delete data;
		}
			
		/**
		* \brief writes a double to the buffer
		* \param d double
		*/
		virtual void toBuffer(const double d)
		{
			m_bitstream.Write(d);
		}
			
			
		/**
		* \brief reads a double from the buffer
		* \param d double
		*/
		virtual void fromBuffer(double& d)
		{
			m_bitstream.Read(d);
		}
			
		/**
		* \brief writes a float to the buffer
		* \param f float
		*/
		virtual void toBuffer(const float f)
		{
			m_bitstream.Write(f);
		}
			
			
		/**
		* \brief reads a float from the buffer
		* \param f float
		*/
		virtual void fromBuffer(float& f)
		{
			m_bitstream.Read(f);
		}
			
		/**
		* \brief writes an int to the buffer
		* \param i int
		*/
		virtual void toBuffer(const int i)
		{
			m_bitstream.Write(i);
		}
			
			
		/**
		* \brief reads an int from the buffer
		* \param i int
		*/
		virtual void fromBuffer(int& i)
		{
			m_bitstream.Read(i);
		}
			
		/**
		* \brief writes a short to the buffer
		* \param s short
		*/
		virtual void toBuffer(const short s)
		{
			m_bitstream.Write(s);
		}
			
			
		/**
		* \brief reads a short from the buffer
		* \param s short
		*/
		virtual void fromBuffer(short& s)
		{
			m_bitstream.Read(s);
		}
			
		/**
		* \brief writes a char to the buffer
		* \param c char
		*/
		virtual void toBuffer(const char c)
		{
			m_bitstream.Write(c);
		}
			
			
		/**
		* \brief reads a char from the buffer
		* \param c char
		*/
		virtual void fromBuffer(char& c)
		{
			m_bitstream.Read(c);
		}
		
		/**
		* \brief writes an unsigned char to the buffer
		* \param c char
		*/
		virtual void toBuffer(const unsigned char c)
		{
			m_bitstream.Write(c);
		}
		
		
		/**
		* \brief reads an unsigned char from the buffer
		* \param c char
		*/
		virtual void fromBuffer(unsigned char& c)
		{
			m_bitstream.Read(c);
		}
		
		/**
		* \brief returns the number of bits read from stream so far
		*/
		virtual int readBits()
		{
			return m_bitstream.GetReadOffset();
		}
		
		
		/**
		* \brief returns the number of bits written to stream so far
		*/
		virtual int writeBits()
		{
			return m_bitstream.GetNumberOfBitsUsed();
		}
		
		/**
		* \brief returns the delay of the packet in milliseconds
		*/
		virtual float getDelay()
		{
			return std::max(0.0f,static_cast<float>(RakNet::GetTime())-static_cast<float>(m_timestamp));
		}
		
		/**
		 * \brief Returns the RakNet packet that was used to create the object
		 */
		Packet* getRakPacket() const
		{
			return m_rak_packet;
		}
		
		/**
		 * \brief Returns the underlying bitstream (for internal use only)
		 */
		RakNet::BitStream& getBitStream()
		{
			return m_bitstream;
		}
	private:
		/**
		* \brief stream for reading and writing the data
		*/
		RakNet::BitStream m_bitstream;
		
		/**
		 * \brief Time when the packet was created
		 */
		unsigned int m_timestamp;
		
		/**
		 * \brief Pointer to the RakNet Packet that was used to create this object
		 */
		Packet* m_rak_packet;
};

#endif

