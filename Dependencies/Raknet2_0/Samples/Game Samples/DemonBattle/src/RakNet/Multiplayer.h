// ----------------------------------------------------------------------
// This API and the code herein created by and wholly and privately owned by Kevin Jenkins except where specifically indicated otherwise.
// Licensed under the "RakNet" brand by Rakkarsoft LLC and subject to the terms of the relevant licensing agreement available at http://www.rakkarsoft.com
// Multiplayer.h
// Written by Kevin Jenkins (rakkar@rakkar.org) January 24, 2003
// The multiplayer class is a simple illustrative class designed to show how to allocate the server
// and client and parse the native packet types. As you can see from the code, it has the following limitations: 
//
// 1. It will allocate globally a server and a client, while you may only need one or the other. 
// 2. You may not want to globally allocate something. 
// 3. It can only be instantiated once (as you pointed out). 
// 4. It is not efficient, checking server packet types for the client and vice-versa.
//
// For sophisticated games, you will want to allocate the server and client when you actually need it, based
// on your own code framework, and write your own switch/case statements that only check for the packet types
// you actually need
// ----------------------------------------------------------------------

#ifndef __MULTIPLAYER_H
#define __MULTIPLAYER_H

struct Packet;

class Multiplayer
{
public:
	Multiplayer();
	virtual ~Multiplayer();

	// -----------------------------------------------------------------
	// CALL THIS EVERY FRAME
	// Reads any packets from the network, handles the native messages, and sends user defined messages to ProcessUnhandledPacket
	// -----------------------------------------------------------------
	virtual void ProcessPackets(void);

	// -----------------------------------------------------------------
	//  *** YOU PROBABLY WANT TO OVERRIDE THESE FOR YOUR OWN APP ***
	// -----------------------------------------------------------------
	virtual void ReceiveDisconnectionNotification(Packet *packet);
	virtual void ReceiveNewClient(Packet *packet);
	virtual void ReceiveResponsiveClient(Packet *packet);
	virtual void ReceiveServerFull(Packet *packet);
	virtual void ReceiveInvalidPassword(Packet *packet);
	virtual void ReceiveKickedByServer(Packet *packet);
	virtual void ReceiveEnumerationReply(Packet *packet);
	virtual void ReceiveUnableToConnect(Packet *packet);
	virtual void ReceiveModifiedPacket(Packet *packet);
	virtual void ReceiveConnectionLost(Packet *packet);
	virtual void ReceiveVoicePacket(Packet *packet);
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	// Given a packet, returns the packet identifier
	// -----------------------------------------------------------------
	static unsigned char GetPacketIdentifier(Packet *packet);

protected:
	// -----------------------------------------------------------------
	// *** YOU MUST OVERRIDE THIS FOR YOUR OWN APP ***
	// -----------------------------------------------------------------
	// All user defined packets are sent to this function.
	virtual void ProcessUnhandledPacket(Packet *packet, unsigned char packetIdentifier);
	// -----------------------------------------------------------------
	
	// -----------------------------------------------------------------
	// YOU PROBABLY WONT NEED TO OVERRIDE ANY OF THE FOLLOWING FUNCTIONS
	// -----------------------------------------------------------------
	// Gets a packet from the server or the client, whoever has one ready.
	virtual Packet* GetPacketFromServerOrClient(bool &fromServer);
	// Handle a remote procedure call packet
	virtual void ReceiveRPC(Packet *packet,bool fromServer);
	// -----------------------------------------------------------------

private:
};

#define DEMON_BATTLE_SERVER_PORT 60004
#define DEMON_BATTLE_CLIENT_PORT 60005

#endif
