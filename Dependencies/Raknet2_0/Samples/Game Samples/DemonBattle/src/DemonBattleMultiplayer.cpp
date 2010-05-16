#include "DemonBattleMultiplayer.h"
#include "ManagerState.h"
#include "State.h"
#include <assert.h>

//#include "MemoryManager.h"

DemonBattleMultiplayer DemonBattleMultiplayer::I;

DemonBattleMultiplayer::DemonBattleMultiplayer()
{
}

DemonBattleMultiplayer::~DemonBattleMultiplayer()
{
}

void DemonBattleMultiplayer::ProcessUnhandledPacket(Packet *p, unsigned char packetIdentifier)
{
	// Lets just have the state handle all packets
	manager->GetState()->ProcessPacket(p, packetIdentifier);
}