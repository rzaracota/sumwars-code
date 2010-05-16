// ----------------------------------------------------------------------
// RakNet version 1.032
// MasterClient.h
// Created by Rakkar Software (rakkar@rakkarsoftware.com) February 17, 2003
// Self-contained class for a master server client - a client that updates
// a master game server
// ----------------------------------------------------------------------



#ifndef __MASTER_CLIENT_H
#define __MASTER_CLIENT_H

#include "RakClientInterface.h"
#include "RakNetworkFactory.h"
#include "MasterCommon.h"

struct Packet;

class MasterClient : public MasterCommon
{
public:
	MasterClient();
	~MasterClient();

	// Ask for a data stream from the master server.  If this returns true, you can expect the list of servers to be updated
	// until GetNumberOfGameServers() == GetMasterServerDataListSize()
	// connectionTimeout is in ms and usually needs to be no more than 2000
	// After calling this you should call Update until GetNumberOfGameServers() == GetMasterServerDataListSize()
	// To make sure all the servers in the list arrive unless you cancel the process with Disconnect()
	// Returns false on a failure to connect to the master server, true otherwise
	bool QueryMasterGameServer(char* host, int masterServerPort, int masterClientPort, unsigned long connectionTimeout);

	// Register or update the server with the data passed to localData.  IP, port, and typeId are automatically filled in for you
	// connectionTimeout is in ms and usually needs to be no more 2000
	// Returns false on failure or timeout, true otherwise
	// You need to call this at least every MASTER_SERVER_UPDATE_FREQUENCY ms to keep your
	// game listed on the master server (you should do it slightly faster to compensate for ping)
	// While the Update is in progress, IsConnected will return true.
	bool UpdateMasterGameServer(char* host, int masterServerPort, int masterClientPort, unsigned long connectionTimeout, MasterServerDataStruct* localData);

	// Remove ourselves from the master game server list.  This should be called when we don't
	// want anyone else connecting to our game.
	// connectionTimeout is in ms and usually needs to be no more than 300
	bool RemoveFromMasterGameServer(char* host, int masterServerPort, int masterClientPort, unsigned long connectionTimeout);

	// Returns how many game server description packets we should receive.  Valid after a successful
	// call to QueryMasterGameServer.  Returns 65535 for unknown
	unsigned short GetNumberOfGameServers(void);

	// Call this once per frame while you are updating the server list after a call to
	// QueryMasterGameServer
	void Update(void);

	// Call this to stop getting servers from a call to QueryMasterGameServer.  You should also call it when
	// you are done with this class in case any connections were kept open due to packetloss
	void Disconnect(void);

	// Will return true while waiting for UpdateMasterGameServer to finish.
	// During this time you need to call Update
	bool IsConnected(void);

	// Implement pinging servers?
protected:
	// Add or update existing server data
	void HandleUpdateMasterGameServer(Packet* packet);
	void HandleUpdateNumberOfServers(Packet* packet);
	unsigned short numberOfGameServers;

	// Start connection sequence internally
	bool Connect(char* host, int masterServerPort, int masterClientPort);
	
	RakClientInterface *rakClient;
	bool receivedNumberOfServers;
};

#endif