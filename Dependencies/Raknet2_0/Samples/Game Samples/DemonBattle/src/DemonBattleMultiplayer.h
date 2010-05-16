#ifndef __DEMON_BATTLE_MULTIPLAYER_H
#define __DEMON_BATTLE_MULTIPLAYER_H

#include "Multiplayer.h"

struct Packet;

class DemonBattleMultiplayer : public Multiplayer
{
public:
	static DemonBattleMultiplayer* Instance() {return &I;}
	~DemonBattleMultiplayer();
	
	void ProcessUnhandledPacket(Packet *p, unsigned char packetIdentifier);

protected:
	DemonBattleMultiplayer();
	static DemonBattleMultiplayer I;
};

#define demonBattleMultiplayer DemonBattleMultiplayer::Instance()

#endif