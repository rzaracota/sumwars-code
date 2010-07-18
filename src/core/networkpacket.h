#ifndef NETWORKPACKET_H

#define NETWORKPACKET_H

#include "charconv.h"

/**
 * \class NetworkPacket
 * Class for data that was received or  should be sent via network
 */
class NetworkPacket : public CharConv
{
	public:
	
		/**
		* \brief Destructor
		*/
		virtual ~NetworkPacket()
		{
			
		}
};


#endif

