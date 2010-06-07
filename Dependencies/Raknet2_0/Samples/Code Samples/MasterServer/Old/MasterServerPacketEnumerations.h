// ----------------------------------------------------------------------
// RakNet version 1.032
// Filename MasterServerPacketEnumerations.h
// Created by Rakkar Software (rakkar@rakkarsoftware.com) January 24, 2003
// This is a sample implementation of this file for a Master Server
// For your own implementation, move the specific types starting with ID_MASTER_GAME_SERVER
// into your own PacketEnumerations.h file
// ----------------------------------------------------------------------

#include "PacketEnumerations.h"

#ifndef __MASTER_SERVER_PACKET_ENUMERATIONS_H
#define __MASTER_SERVER_PACKET_ENUMERATIONS_H

enum {
	// For your own implementation, move the specific types starting here
	// into your own PacketEnumerations.h file
	ID_UPDATE_MASTER_GAME_SERVER=(ID_RESERVED9+1),
	ID_QUERY_MASTER_GAME_SERVER,
	ID_REMOVE_FROM_MASTER_GAME_SERVER,
	ID_UPDATE_NUMBER_OF_SERVERS
};

#endif