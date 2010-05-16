// ----------------------------------------------------------------------
// RakNet version 1.032
// MasterServer.cpp
// Created by Rakkar Software (rakkar@rakkarsoftware.com) February 17, 2003
// Self-contained class for a master server list - a server that maintains a
// list of active game servers
// ----------------------------------------------------------------------

#include "MasterServer.h"
#include "GetTime.h"
#include "NetworkTypes.h"
#include "MasterServerPacketEnumerations.h"

MasterServer::MasterServer()
{
	rakServer=0;
}

MasterServer::~MasterServer()
{
	Disconnect();
}

void MasterServer::Start(bool highPriority, int serverPort)
{
	if (rakServer==0)
		rakServer = RakNetworkFactory::GetRakServerInterface();

	rakServer->StopSynchronizedRandomInteger();
	rakServer->StopOccasionalPing();

	rakServer->Start(32, 0, highPriority, serverPort);
	
	rakServer->StopSynchronizedRandomInteger(); // Don't need this
	rakServer->StopOccasionalPing(); // Don't need this either.
	rakServer->SetRelayStaticClientData(false); // Nor this
}

void MasterServer::Disconnect(void)
{
	if (rakServer)
	{
		rakServer->Disconnect();
		RakNetworkFactory::DestroyRakServerInterface(rakServer);
		rakServer=0;
		ClearMasterServerList();
	}
}

void MasterServer::Update(void)
{
	Packet* packet;
	unsigned char packetIdentifier;

	packet = rakServer->Receive();
	while (packet) // We got a packet, which should be either a message from the network engine or another game
	{
		// Don't check for a timestamp because we never use one
		packetIdentifier= (unsigned char) packet->data[0];

		switch (packetIdentifier)
		{
			// We only care about 3 kinds of identifiers.  Ignore anything else
			case ID_UPDATE_MASTER_GAME_SERVER:
				HandleUpdateMasterGameServer(packet);
			break;
			case ID_QUERY_MASTER_GAME_SERVER:
				HandleQueryMasterGameServer(packet);
			break;
			case ID_REMOVE_FROM_MASTER_GAME_SERVER:
				HandleRemoveFromMasterGameServer(packet);
			break;
		}

		rakServer->DeallocatePacket(packet);

		// As long as more packets are waiting to be handled go ahead and handle them
		packet = rakServer->Receive();
	}

	unsigned long time=getTime();
	unsigned long i=0;

	// Go through the list and clear out expired game servers
	while (i<GetMasterServerDataListSize())
	{
		if (masterGameDataList[i].timeoutTime < time)
		{
			// Expired item.  List deletions aren't efficient but they happen rarely compared to traversals
			delete masterGameDataList[i].object;
			masterGameDataList.del(i);
		}
		else
		{
			i++;
		}
	}

	// Go through all connections and kick anyone connected too long with no waiting data
	for (i=0; i < rakServer->GetConnectedPlayers(); i++)
	{
		if (rakServer->GetConnectionTime(rakServer->GetPlayerIDFromIndex(i)) > 10000 && rakServer->GetPacketOutputBufferSize()==0)
			rakServer->Kick(rakServer->GetPlayerIDFromIndex(i));
	}
}

// Add or update existing server data
void MasterServer::HandleUpdateMasterGameServer(Packet* packet)
{
	assert(packet->length == sizeof(MasterServerDataStruct)); // Shouldn't be sending structs of other lengths with this ID
	if (packet->length != sizeof(MasterServerDataStruct))
		return;

	// Cast the data into a struct for easy access
	MasterServerDataStruct * dataStruct = (MasterServerDataStruct *) packet->data;

	// Fill in the IP/port data from the sending client
	rakServer->GetPlayerIPFromID(packet->playerId, dataStruct->IP, &dataStruct->port);

	// Disconnect the sending client
	rakServer->Kick(packet->playerId);

	// See if this element is already in the list.  If so, update that element.  Otherwise add it to the list
	for (unsigned long i=0; i < GetMasterServerDataListSize(); i++)
	{
		if (strcmp(masterGameDataList[i].object->IP , dataStruct->IP)==0 && masterGameDataList[i].object->port==dataStruct->port)
		{
			// Item is in the list.  Update it
			memcpy(masterGameDataList[i].object, dataStruct, sizeof(MasterServerDataStruct));
			masterGameDataList[i].timeoutTime = getTime() + MASTER_SERVER_UPDATE_FREQUENCY; // Reset when to delete this item
			return;
		}
	}


	// The item is not in the list so allocate and add it
	MasterServerDataListStruct insertionData;
	insertionData.object = new MasterServerDataStruct;
	memcpy(insertionData.object, (MasterServerDataStruct *) packet->data, sizeof(MasterServerDataStruct));
	insertionData.timeoutTime = getTime() + MASTER_SERVER_UPDATE_FREQUENCY; // Set when to delete this item

	// Add the item to the back of the list.  The list is unordered.
	masterGameDataList.insert(insertionData);

//	printf("masterGameDataList.size()=%i\n",masterGameDataList.size());
}

// Send a bunch of reliable packets at the caller, along with a header specifying how many packets are to be sent
void MasterServer::HandleQueryMasterGameServer(Packet* packet)
{
	UpdateNumberOfServersStruct numberOfServersStruct;
	MasterServerDataStruct masterServerDataStruct;

	// Tell the remote game server how many servers we are going to send
	numberOfServersStruct.typeId = ID_UPDATE_NUMBER_OF_SERVERS;
	numberOfServersStruct.numberOfServers = (unsigned short) GetMasterServerDataListSize();
	rakServer->Send((char*)&numberOfServersStruct, sizeof(UpdateNumberOfServersStruct), HIGH_PRIORITY, RELIABLE, 0, packet->playerId, false, false);
	
	// Now send the list of servers
	for (unsigned long i=0; i < GetMasterServerDataListSize(); i++)
	{
	//	printf("Sending server #%i\n", i);
		memcpy(&masterServerDataStruct, masterGameDataList[i].object, sizeof(MasterServerDataStruct));
		rakServer->Send((char*)&masterServerDataStruct, sizeof(MasterServerDataStruct), MEDIUM_PRIORITY, RELIABLE, 0, packet->playerId, false, false);
	}
}

// The sending server requested that it be removed from the active server list
void MasterServer::HandleRemoveFromMasterGameServer(Packet* packet)
{
	char ip[22];
	unsigned short port;

	ip[0]=0;

	// Get the IP/port of who this is from
	rakServer->GetPlayerIPFromID(packet->playerId, ip, &port);

	if (ip[0]==0)
	{
		// Couldn't determine who this was from so just ignore it
		return;
	}

	// See if this element is in the list.
	for (unsigned int i=0; i < GetMasterServerDataListSize(); i++)
	{
		if (strcmp(masterGameDataList[i].object->IP, ip)==0 && masterGameDataList[i].object->port==port)
		{
			// Item is in the list.  Delete it
			delete masterGameDataList[i].object;
			masterGameDataList.del(i);
			return;
		}
	}

	// They are removed now, so disconnect the sending client
	rakServer->Kick(packet->playerId);
}