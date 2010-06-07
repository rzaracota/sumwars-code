#include "State.h"
#include "Multiplayer.h"
#include "ManagerState.h"
#include <assert.h>
#include "NetworkStructures.h"

//#include "MemoryManager.h"

bool State::ProcessPacket(Packet* p, unsigned char packetIdentifier)
{
#ifdef _DEBUG
	unsigned char switchItem = Multiplayer::GetPacketIdentifier(p);

	manager->LogWrite("ProcessPacket not overridden in derived state.  Unknown packet error: type %i with length %i", switchItem, p->length);

	assert(1);
#endif
	return true;
}