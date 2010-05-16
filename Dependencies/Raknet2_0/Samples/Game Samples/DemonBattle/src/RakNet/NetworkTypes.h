// ----------------------------------------------------------------------
// This API and the code herein created by and wholly and privately owned by Kevin Jenkins except where specifically indicated otherwise.
// Licensed under the "RakNet" brand by Rakkarsoft LLC and subject to the terms of the relevant licensing agreement available at http://www.rakkarsoft.com
// NetworkTypes.h
// File created January 24, 2003
// Contains user-exposed enumerations, structures, and definitions
// ----------------------------------------------------------------------

#ifndef __NETWORK_TYPES_H
#define __NETWORK_TYPES_H

typedef unsigned short ObjectID;
typedef unsigned short PlayerID;
typedef unsigned char UniqueIDType;

#pragma pack(push) 
#pragma pack(1) 
struct Packet
{
	PlayerID playerId;
	unsigned long length;
	unsigned long bitSize; // Same as length but represents bits.  Length is obsolete and retained for backwards compatibility
	char* data;
};
#pragma pack(pop) 

const PlayerID SERVER_SENDER_ID=65535;
const PlayerID UNASSIGNED_PLAYER_ID=65535;
const ObjectID UNASSIGNED_OBJECT_ID=65535;

//-----------------------------------------------------
// RPC FUNCTION IMPLEMENTATION
// --------------------------------------------------------
//
// Use the following C function prototype for your callbacks
// void functionName(char *input, int numberOfBitsOfData, PlayerID sender);
//
// If you pass input data, you can parse the input data in two ways.
//
// 1.
// Cast input to a struct (such as if you sent a struct)
// i.e. MyStruct *s = (MyStruct*) input;
// Make sure that the sizeof(MyStruct) is equal to the number of bytes passed!
//
// 2.
// Create a BitStream instance with input as data and the number of bytes
// i.e. BitStream myBitStream(input, (numberOfBitsOfData-1)/8+1)
//
// (numberOfBitsOfData-1)/8+1 is how convert from bits to bytes
//
// Full example:
//
// void MyFunc(char *input, int numberOfBitsOfData, PlayerID sender) {}
// RakClient *rakClient;
// REGISTER_AS_REMOTE_PROCEDURE_CALL(rakClient, MyFunc);
// This would allow MyFunc to be called from the server using  (for example)
// rakServer->RPC("MyFunc", 0, clientID, false);

#define REGISTER_AS_REMOTE_PROCEDURE_CALL(networkObject, functionName) (networkObject)->RegisterAsRemoteProcedureCall((#functionName),(functionName))
// Unregisters a remote procedure call
#define UNREGISTER_AS_REMOTE_PROCEDURE_CALL(networkObject,functionName) (networkObject)->UnregisterAsRemoteProcedureCall((#functionName))

#endif

