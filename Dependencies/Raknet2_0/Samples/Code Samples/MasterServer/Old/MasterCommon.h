// ----------------------------------------------------------------------
// RakNet version 1.032
// MasterCommon.h
// Created by Rakkar Software (rakkar@rakkarsoftware.com) February 17, 2003
// Common code between the master server and the master client
// ----------------------------------------------------------------------

#ifndef __MASTER_COMMON_H
#define __MASTER_COMMON_H

// A server will automatically be removed after this much ms on the master server if a remote
// game server does not update itself.  You should set this value
// to something higher than how long it will normally take to start a game
// Note that the better way to handle this is for servers to properly add and remove themselves,
// not for servers to be removed because of this timeout
const int MASTER_SERVER_UPDATE_FREQUENCY=1000*60*5; // (Currently at 5 minutes)

// Change these if they conflict with anything.  These should not be the same as your regular game ports.
const unsigned short MASTER_SERVER_PORT=61234;
const unsigned short MASTER_CLIENT_PORT=62234;

#include "ArrayList.h"

#pragma pack(push,1)

#define SERVER_NAME_MAXIMUM_LENGTH 50

// This is what we actually send over the network.  Update it however you wish
#pragma pack(1)
struct MasterServerDataStruct
{
	unsigned char typeId; // ID_UPDATE_MASTER_GAME_SERVER or ID_QUERY_MASTER_GAME_SERVER - required field.  Should be defined in PacketEnumerations.h
	char IP[22]; // Required field
	unsigned short port; // Required field
	char serverName[SERVER_NAME_MAXIMUM_LENGTH]; // User data
};

#pragma pack(1)
struct UpdateNumberOfServersStruct
{
	unsigned char typeId; // ID_UPDATE_NUMBER_OF_SERVERS
	unsigned short numberOfServers;
};

#pragma pack(pop)

// A node in the binary tree to hold server game data.  We need the overloaded operators
// So the tree can form properly
struct MasterServerDataListStruct
{
	MasterServerDataStruct *object;
	unsigned long timeoutTime; // When this object should be deleted (only used by the master server)
	
	MasterServerDataListStruct();
	MasterServerDataListStruct(MasterServerDataStruct *Object);

	friend int operator==(const MasterServerDataListStruct& left, const MasterServerDataListStruct& right);
	friend int operator > (const MasterServerDataListStruct& left, const MasterServerDataListStruct& right);
	friend int operator < (const MasterServerDataListStruct& left, const MasterServerDataListStruct& right);
};

// A class to hold common code between the master server and master client
class MasterCommon
{
public:
	~MasterCommon();
	
	// Clear out the server list
	void ClearMasterServerList(void);

	// Get the number of servers returned
	unsigned long GetMasterServerDataListSize(void);

	// Get the master server data at the specified index.  This may change during updates
	MasterServerDataStruct * GetMasterServerDataAtIndex(unsigned long i);
protected:
	// The list of game servers is held in a list for fast per-frame updates
	BasicDataStructures::List<MasterServerDataListStruct> masterGameDataList;
};


#endif
