#include "network.h"


unsigned char  Network::getPacketIdentifier(Packet *p)
{
#ifndef NO_RAKNET
	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	else
		return (unsigned char) p->data[0];
#else
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
#endif
}

