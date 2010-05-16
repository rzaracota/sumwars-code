// ----------------------------------------------------------------------
// RakNet version 1.032
// MasterServer
// Created by Rakkar Software (rakkar@rakkarsoftware.com) February 17, 2003
// Self-contained class for a master server list - a server that maintains a
// list of active game servers
// ----------------------------------------------------------------------

#ifndef __MASTER_SERVER_H
#define __MASTER_SERVER_H

#include "RakServerInterface.h"
#include "RakNetworkFactory.h"
#include "MasterCommon.h"

struct Packet;

class MasterServer : public MasterCommon
{
public:
	MasterServer();
	~MasterServer();

	// Start up the master server with or without high priority threads
	void Start(bool highPriority, int serverPort);

	// Stop the master server
	void Disconnect(void);

	// Update the master server.  Call this once per cycle
	void Update(void);
protected:
	// The sending server requested that it be removed from the active server list
	void HandleRemoveFromMasterGameServer(Packet* packet);
	// Send a bunch of reliable packets at the caller, along with a header specifying how many packets are to be sent
	void HandleQueryMasterGameServer(Packet* packet);
	// Add or update existing server data
	void HandleUpdateMasterGameServer(Packet* packet);

	RakServerInterface *rakServer;
};

#endif