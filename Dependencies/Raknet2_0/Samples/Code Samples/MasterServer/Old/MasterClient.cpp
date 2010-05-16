// ----------------------------------------------------------------------
// RakNet version 1.032
// MasterClient.cpp
// Created by Rakkar Software (rakkar@rakkarsoftware.com) February 17, 2003
// Self-contained class for a master server client - a client that updates
// a master game server
// ----------------------------------------------------------------------

#include "MasterClient.h"
#include "GetTime.h"
#include "NetworkTypes.h"
#include "MasterServerPacketEnumerations.h"


MasterClient::MasterClient()
{
	rakClient=0;
	numberOfGameServers=0;
	receivedNumberOfServers=false;
}

MasterClient::~MasterClient()
{
	Disconnect();
	ClearMasterServerList();
}

// Disconnect internally, but don't clear data received
void MasterClient::Disconnect(void)
{
	if (rakClient)
	{
		rakClient->Disconnect();
		RakNetworkFactory::DestroyRakClientInterface(rakClient);
		rakClient=0;
		receivedNumberOfServers=false;
		numberOfGameServers=0;
	}
}

// Start connection sequence internally
bool MasterClient::Connect(char* host, int masterServerPort, int masterClientPort)
{
	Disconnect();

	// This should always be true, but lets be safe in case other people incorrectly change the code
	if (rakClient==0)
	{
		rakClient=RakNetworkFactory::GetRakClientInterface();
		rakClient->StopOccasionalPing();
	}

	return rakClient->Connect(host, masterServerPort, masterClientPort, 0, false);
}

// Ask for a data stream from the master server.  If this returns true, you can expect the list of servers to be updated
// until GetNumberOfGameServers() == GetMasterServerDataListSize()
// connectionTimeout is in ms and usually needs to be no more than 2000
// After calling this you should call Update until GetNumberOfGameServers() == GetMasterServerDataListSize()
// To make sure all the servers in the list arrive unless you cancel the process with Disconnect()
// Returns false on a failure to connect to the master server, true otherwise
bool MasterClient::QueryMasterGameServer(char* host, int masterServerPort, int masterClientPort, unsigned long connectionTimeout)
{
	Disconnect();

	unsigned long timeoutTime = getTime() + connectionTimeout;

	if (rakClient==0)
	{
		if (Connect(host,masterServerPort,masterClientPort)==false)
			return false;
    
		while (getTime() < timeoutTime && rakClient->IsConnected()==false)
		{
			Update(); // Wait for the packet telling us how many servers are active, or until we timeout
		}
	}

	if (rakClient->IsConnected()==false)
	{
		Disconnect();
		return false;
	}

	// Clear the list in preparation for new data
	ClearMasterServerList();
	receivedNumberOfServers=false;
	numberOfGameServers=0;

	// We're connected so send a query packet
	unsigned char c = ID_QUERY_MASTER_GAME_SERVER;
	rakClient->Send((char*) &c, sizeof(unsigned char), HIGH_PRIORITY, RELIABLE, 0);

	/*
	while (getTime() < timeoutTime && receivedNumberOfServers==false)
	{
		Update(); // Wait for the packet telling us how many servers are active, or until we timeout
	}

	if (receivedNumberOfServers==false)
	{
		Disconnect();
		return false; // Couldn't connect in time
	}
	*/

	// Leave the connection open
	
	return true;
}

// Register or update the server with the data passed to localData.  IP, port, and typeId are automatically filled in for you
// connectionTimeout is in ms and usually needs to be no more 300
// Returns false on failure or timeout, true otherwise
// You need to call this at least every MASTER_SERVER_UPDATE_FREQUENCY ms to keep your
// game listed on the master server (you should do it slightly faster to compensate for ping)
bool MasterClient::UpdateMasterGameServer(char* host, int masterServerPort, int masterClientPort, unsigned long connectionTimeout, MasterServerDataStruct* localData)
{
	Disconnect();

	if (Connect(host,masterServerPort,masterClientPort)==false)
		return false;
	
	unsigned long timeoutTime = getTime() + connectionTimeout;

	while (getTime() < timeoutTime && rakClient->IsConnected()==false)
	{
		Update(); // Wait for the connection
	}

	if (rakClient->IsConnected()==false)
	{
		Disconnect();
		return false; // Couldn't connect in time
	}

	// In case the user forgot to set this
	localData->typeId=ID_UPDATE_MASTER_GAME_SERVER;

	// We're connected, so send the data
	return rakClient->Send((char*)localData, sizeof(MasterServerDataStruct), HIGH_PRIORITY, RELIABLE, 0);

	// Note that the server will kick us off once it gets this packet, so we don't need to disconnect ourselves
}

// Remove ourselves from the master game server list.  This should be called when we don't
// want anyone else connecting to our game.
// connectionTimeout is in ms and usually needs to be no more than 300
bool MasterClient::RemoveFromMasterGameServer(char* host, int masterServerPort, int masterClientPort, unsigned long connectionTimeout)
{
	Disconnect();

	if (Connect(host,masterServerPort,masterClientPort)==false)
		return false;
    
	unsigned long timeoutTime = getTime() + connectionTimeout;

	while (getTime() < timeoutTime && rakClient->IsConnected()==false)
	{
		Update(); // Wait for the connection
	}

	if (rakClient->IsConnected()==false)
	{
		Disconnect();
		return false; // Couldn't connect in time
	}
	
	// Tell the server to remove us
	unsigned char c = ID_REMOVE_FROM_MASTER_GAME_SERVER;
	rakClient->Send((char*) &c, sizeof(unsigned char), HIGH_PRIORITY, RELIABLE, 0);
	
	// Note that the server will kick us off once it gets this packet, so we don't need to disconnect ourselves

	return true;
}


// Returns how many game server description packets we should receive.  Valid after a successful
// call to QueryMasterGameServer
unsigned short MasterClient::GetNumberOfGameServers(void)
{
	if (rakClient && rakClient->IsConnected() && receivedNumberOfServers==false)
		return 65535;

	return numberOfGameServers;
}


// Will return true while waiting for UpdateMasterGameServer to finish.
// During this time you need to call Update
bool MasterClient::IsConnected(void)
{
	if (rakClient)
		return rakClient->IsConnected();
	return false;
}

// Call this once per frame while you are updating the server list
void MasterClient::Update(void)
{
	if (rakClient==0)
		return;

	Packet* packet;
	unsigned char packetIdentifier;

	packet = rakClient->Receive();
	while (packet) // We got a packet, which should be either a message from the network engine or another game
	{
		// Don't check for a timestamp because we never use one
		packetIdentifier= (unsigned char) packet->data[0];

		switch (packetIdentifier)
		{
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
			case ID_INVALID_PASSWORD:
			case ID_MODIFIED_PACKET:
			case ID_REMOTE_PORT_REFUSED:
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			rakClient->DeallocatePacket(packet);
			Disconnect();
			break;
			// We only care about 3 kinds of identifiers.  Ignore anything else
			case ID_UPDATE_MASTER_GAME_SERVER:
				HandleUpdateMasterGameServer(packet);
			break;
			case ID_UPDATE_NUMBER_OF_SERVERS:
				HandleUpdateNumberOfServers(packet);
			break;
		}

		if (rakClient==0) // From Disconnect
			return;

		rakClient->DeallocatePacket(packet);

		// As long as more packets are waiting to be handled go ahead and handle them
		packet = rakClient->Receive();
	}
}

void MasterClient::HandleUpdateMasterGameServer(Packet* packet)
{
	assert(packet->length == sizeof(MasterServerDataStruct)); // Shouldn't be sending structs of other lengths with this ID
	if (packet->length != sizeof(MasterServerDataStruct))
		return;

	// Allocate the item and add it to the list
	MasterServerDataListStruct insertionData;
	insertionData.object = new MasterServerDataStruct;
	memcpy(insertionData.object, (MasterServerDataStruct *) packet->data, sizeof(MasterServerDataStruct));
	insertionData.timeoutTime = getTime() + MASTER_SERVER_UPDATE_FREQUENCY; // Set when to delete this item

	// Add the item to the back of the list.  The list is unordered.
	masterGameDataList.insert(insertionData);

	//printf("masterGameDataList.size()=%i\n",masterGameDataList.size());

	// Disconnect us if we are done downloading the list
	if (numberOfGameServers == masterGameDataList.size())
	{
		rakClient->DeallocatePacket(packet);
		Disconnect();
	}
}

void MasterClient::HandleUpdateNumberOfServers(Packet* packet)
{
	assert(packet->length == sizeof(UpdateNumberOfServersStruct)); // Shouldn't be sending structs of other lengths with this ID
	if (packet->length != sizeof(UpdateNumberOfServersStruct))
		return;

	// Cast the data into a struct for easy access
	UpdateNumberOfServersStruct * dataStruct = (UpdateNumberOfServersStruct *) packet->data;
	numberOfGameServers = dataStruct->numberOfServers;
	receivedNumberOfServers=true;

	if (numberOfGameServers==0)
		Disconnect();
}
