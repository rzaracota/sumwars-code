// ----------------------------------------------------------------------
// This API and the code herein created by and wholly and privately owned by Kevin Jenkins except where specifically indicated otherwise.
// Licensed under the "RakNet" brand by Rakkarsoft LLC and subject to the terms of the relevant licensing agreement available at http://www.rakkarsoft.com
// Filename RakClientInterface.h
// File created January 24, 2003
// This is an interface for for the network client and provides primary
// functionality
// ----------------------------------------------------------------------

#ifndef __RAK_CLIENT_INTERFACE_H
#define __RAK_CLIENT_INTERFACE_H

#include "NetworkTypes.h"
#include "PacketPriority.h"

class BitStream;

class RakClientInterface
{
public:
	// Destructor
	virtual ~RakClientInterface() {}

	// Call this to connect the client to the specified host (ip or domain name) and server port.
	// This is a non-blocking connection.  You know the connection is successful when IsConnected() returns true
	// or receive gets a packet with the type identifier ID_ENUMERATION_REPLY.  If the connection is not
	// successful, such as rejected connection or no response then neither of these things will happen.
	// serverPort is which port to connect to on the remote machine.  This should match whatever the server specified
	// as the port in the constructor of RakServer. clientPort is the port you want the client to use.
	// With winsock 2, clientPort is used to read and clientPort+1 is used to write
	// Both ports must be open for UDP
	// validationInteger is an integer that the client must match to the server to be able to connect.
	// If the client does not match this number the connection will be wordlessly refused.
	// This is most useful for verifying that the exe size is the same on both the server and the client when
	// your program uses the same exe for hosting and connecting.  However you can use it for a different file
	// a checksum or
	// for any purpose you wish.  This number is also used as an encryption key, so you should use
	// a random value if you don't want to use it for anything else
	// Set highPriorityThreads to true (recommended) for a real-time responsive client.  Set it to false
	// for games where high pings don't matter, or if the network engine slows down the game too much
	// Returns true on successful initiation, false otherwise
	virtual bool Connect(char* host, unsigned short serverPort, unsigned short clientPort, unsigned long connectionValidationInteger, bool highPriorityThreads)=0;

	// Stops the client, stops synchronized data, and resets all internal data. 
	// Does nothing if the client is not connected to begin with
	virtual void Disconnect(void)=0;

	// Set the password to use when connecting to a server.  The password persists between connections.
	// Pass 0 for no password.  Only enter up to CONNECTION_VERIFICATION_PASSWORD_LENGTH characters (currently 10)
	// You can call this anytime
	virtual void SetPassword(char *_password)=0;

	// Returns true if a password was set, false otherwise
	virtual bool HasPassword(void) const=0;

	// This function only works while the client is connected (Use the Connect function).  Returns false on failure, true on success
	// Sends the data stream of length length
	// If you aren't sure what to specify for priority and reliability, use HIGH_PRIORITY and RELIABLE, 0 for ordering stream
	virtual bool Send(char *data, const long length, PacketPriority priority, PacketReliability reliability, char orderingStream)=0;

	// This function only works while the client is connected (Use the Connect function).  Returns false on failure, true on success
	// Sends the BitStream
	// If you aren't sure what to specify for priority and reliability, use HIGH_PRIORITY and RELIABLE, 0 for ordering stream
	virtual bool Send(BitStream* bitStream, PacketPriority priority, PacketReliability reliability, char orderingStream)=0;

	// Call this to get a packet from the incoming packet queue.  Use DeallocatePacket to deallocate the packet after you are done with it.
	// Check the Packet struct at the top of CoreNetworkStructures.h for the format of the struct
	// Returns 0 if no packets are waiting to be handled
	// If the client is not active this will also return 0, as all waiting packets are flushed when the client is Disconnected
	// This also updates all memory blocks associated with synchronized memory
	virtual Packet* Receive(void)=0;

	// Call this to deallocate a packet returned by Receive when you are done handling it.
	virtual void DeallocatePacket(Packet *packet)=0;

	// Send a ping request to the server.  Occasional pings are on by default (see StartOccasionalPing and StopOccasionalPing)
	// so unless you turn them off it is not necessary to call this function.  It is here for completeness if you want it
	// Does nothing if the client is not connected to begin with
	virtual void PingServer(void)=0;

	// Returns the average of all ping times read
	virtual int GetAveragePing(void)=0;

	// Returns the last ping time read for the specific player or -1 if none read yet
	virtual int GetLastPing(void) const=0;

	// Returns the lowest ping time read or -1 if none read yet
	virtual int GetLowestPing(void) const=0;

	// Returns the last ping for the specified player.  This information is broadcast by the server automatically
	// In order to save bandwidth this information is updated only infrequently and only for the first 32 players
	// Note: You can read your own ping with this method by passing your own playerId, however for more up-to-date
	// readings you should use one of the three functions above
	virtual int GetPlayerPing(PlayerID playerId)=0;

	// Ping the server every so often.  This is on by default.  In games where you don't care about ping you can call
	// StopOccasionalPing to save the bandwidth
	// This will work anytime
	virtual void StartOccasionalPing(void)=0;

	// Stop pinging the server every so often.  The server is pinged by default.  In games where you don't care about ping
	// you can call this to save the bandwidth
	// This will work anytime
	virtual void StopOccasionalPing(void)=0;

	// Returns true if the client is connected to a responsive server
	virtual bool IsConnected(void) const=0;

	// Returns a number automatically synchronized between the server and client which randomly changes every
	// 9 seconds. The time it changes is accurate to within a few ms and is best used to seed
	// random number generators that you want to usually return the same output on all systems.  Keep in mind this
	// isn't perfectly accurate as there is always a very small chance the numbers will by out of synch during
	// changes so you should confine its use to visual effects or functionality that has a backup method to
	// maintain synchronization.  If you don't need this functionality and want to save the bandwidth call
	// StopSynchronizedRandomInteger after starting the server
	virtual unsigned long GetSynchronizedRandomInteger(void) const=0;

	// Call this to automatically synchronize a block of memory.
	// Unique identifier should be an integer corresponding to the same variable between clients and the server.  This integer
	// should start at 0 and not surpass the range of UniqueIDType.  It is recommended you set this from an enum
	// memoryBlock should point to the data you want to read from or write to with size of size in bytes
	// isAuthority should be true if all other computers should match their data in memory block to yours.  This is triggered by
	// when the variable changes.  So setting it to true on both the server and one client would make it so if the synchronized
	// variable on that client changed, the server would then relay it to all clients.
	// In the current implementation, setting isAuthority to true on the server will cause changes to that variable to be broadcast to
	// all connected clients.
	// synchronizationRules is an optional function pointer defined by you.  It should
	// return true if the two passed memory blocks are sufficiently different to synchronize them.  This is an optimization so
	// data that changes rapidly, such as per-frame, can be made to not update every frame
	// The first parameter to synchronizationRules is the new data, the second is the internal copy of the old data
	// secondaryUniqueIdentifier is optional and used when you have the same unique identifier and is intended for multiple instances of a class
	// that derives from NetworkObject.
	// You can call this anytime - however if you call it before the connection is complete initial data will not by synchronized
	virtual void SynchronizeMemory(UniqueIDType uniqueIdentifier, char *memoryBlock, unsigned short size, bool isAuthority, bool (*synchronizationRules) (char*,char*)=0,ObjectID secondaryUniqueIdentifier=UNASSIGNED_OBJECT_ID)=0;

	// Call this to stop synchronization of a block of memory previously defined by uniqueIdentifier and secondaryUniqueIdentifier
	// by the call to SynchronizeMemory
	// CALL THIS BEFORE SYNCHRONIZED MEMORY IS DEALLOCATED!
	// It is not necessary to call this before disconnecting, as all synchronized states will be released then.
	virtual void DesynchronizeMemory(UniqueIDType uniqueIdentifier, ObjectID secondaryUniqueIdentifier=UNASSIGNED_OBJECT_ID)=0;

	// Call this to Desynchronize all synchronized memory
	virtual void DesynchronizeAllMemory(void)=0;

	// This is an optional function to generate the compression layer from the input frequency table.
	// You should call this twice - once with inputLayer as true and once as false.
	// The frequency table passed here with inputLayer=true should match the frequency table on the recipient with inputLayer=false.
	// Likewise, the frequency table passed here with inputLayer=false should match the frequency table on the recipient with inputLayer=true
	// Calling this function when there is an existing layer will overwrite the old layer
	// You should only call this when disconnected
	// Return value: false (failure) if connected.  Otherwise true (success)
	virtual bool GenerateCompressionLayer(unsigned long inputFrequencyTable[256], bool inputLayer)=0;

	// Delete the output or input layer as specified.  This is not necessary to call and is only valuable for freeing memory
	// You should only call this when disconnected
	// Return value: false (failure) if connected.  Otherwise true (success)
	virtual bool DeleteCompressionLayer(bool inputLayer)=0;

	// Register a C function as available for calling as a remote procedure call
	// uniqueID should be a null terminated non-case senstive string of only letters to identify this procedure
	// Parameter 2 should be the name of the C function or C++ singleton to be used as a function pointer
	// This can be called whether the client is active or not, and registered functions stay registered unless unregistered with
	// UnregisterAsRemoteProcedureCall
	virtual void RegisterAsRemoteProcedureCall(char* uniqueID, void (*functionName)(char *input, int numberOfBitsOfData, PlayerID sender))=0;

	// Unregisters a C function as available for calling as a remote procedure call that was formerly registered
	// with RegisterAsRemoteProcedureCall
	virtual void UnregisterAsRemoteProcedureCall(char* uniqueID)=0;

	// Calls a C function on the server that the server already registered using RegisterAsRemoteProcedureCall
	// Pass the data you want to pass to that function in parameters, or 0 for no data to pass
	// You can also pass a regular data stream which will be converted to a bitstream internally by passing data and bit length
	// If you want that function to return data you should call RPC from that system in the same way
	// Returns true on a successful packet send (this does not indicate the recipient performed the call), false on failure
	// The uniqueID must be composed of a string with only characters from a-z and is not case sensitive
	virtual bool RPC(char* uniqueID, char *data, long bitLength, PacketPriority priority, PacketReliability reliability, char orderingStream)=0;
	virtual bool RPC(char* uniqueID, BitStream *parameters, PacketPriority priority, PacketReliability reliability, char orderingStream)=0;

	// Handles an RPC packet.  If you get a packet with the ID ID_RPC you should pass it to this function
	// This is already done in Multiplayer.cpp, so if you use the Multiplayer class it is handled for you.
	// Returns true on success, false on a bad packet or an unregistered function
	virtual bool HandleRPCPacket(Packet* packet)=0;

	// Enables or disables frequency table tracking.  This is required to get a frequency table, which is used to generate
	// A new compression layer.
	// You can call this at any time - however you SHOULD only call it when disconnected.  Otherwise you will only track
	// part of the values sent over the network.
	// This value persists between connect calls and defaults to false (no frequency tracking)
	virtual void SetTrackFrequencyTable(bool b)=0;

	// Returns the frequency of outgoing bytes into outputFrequencyTable
	// The purpose is to save to file as either a master frequency table from a sample game session for passing to
	// GenerateCompressionLayer.
	// You should only call this when disconnected
	// Requires that you first enable data frequency tracking by calling SetTrackFrequencyTable(true)
	// Return value: false (failure) if connected or if frequency table tracking is not enabled.  Otherwise true (success)
	virtual bool GetSendFrequencyTable(unsigned long outputFrequencyTable[256])=0;

	// Returns the compression ratio.  A low compression ratio is good.  Compression is for outgoing data
	virtual float GetCompressionRatio(void) const=0;

	// Returns the decompression ratio.  A high decompression ratio is good.  Decompression is for incoming data
	virtual float GetDecompressionRatio(void) const=0;

	// The server internally maintains a data struct that is automatically sent to clients when the connect.
	// This is useful to contain data such as the server name or message of the day.  Access that struct with these
	// functions.
	// The data is entered as an array and stored and returned as a BitStream.  
	// Everytime you call GetStaticServerData it resets the read pointer to the start of the bitstream.  To do multiple reads without reseting the pointer
	// Maintain a pointer copy to the bitstream as in
	// BitStream *copy = ...->GetStaticServerData(...);
	// To store a bitstream, use the GetData() and GetNumberOfBytesUsed() methods
	// of the bitstream for the 2nd and 3rd parameters
	// Note that the server may change at any time the
	// data contents and/or its length!
	virtual BitStream* GetStaticServerData(void)=0;
	virtual void SetStaticServerData(char *data, const long length)=0;

	// The client internally maintains a data struct that is automatically sent to the server on connection
	// This is useful to contain data such as the player name.  Access that struct with these
	// function. Pass UNASSIGNED_PLAYER_ID for playerId to reference your internal data.  A playerId value to access the data of another player.
	// Pass by value.
	// *** NOTE ***
	// If you change any data in the struct the server won't reflect this change unless you manually update it
	// Do so by calling SendStaticClientDataToServer
	// The data is entered as an array and stored and returned as a BitStream.
	virtual BitStream* GetStaticClientData(PlayerID playerId)=0;
	virtual void SetStaticClientData(PlayerID playerId, char *data, const long length)=0;

	// Send the static server data to the server
	// The only time you need to call this function is to update clients that are already connected when you change the static
	// server data by calling GetStaticServerData and directly modifying the object pointed to.  Obviously if the
	// connected clients don't need to know the new data you don't need to update them, so it's up to you
	// The server must be active for this to have meaning
	virtual void SendStaticClientDataToServer(void)=0;

	// Return the player number the server has assigned to you.  You don't usually need to know this
	virtual PlayerID GetPlayerID(void) const=0;

	// Low level utility to get the socket.  The socket isn't initialized until a connection is established
	//virtual SOCKET GetSocket(void)=0;

	// Put a packet back at the end of the receive queue in case you don't want to deal with it immediately
	virtual void PushBackPacket(Packet *packet)=0;

	// Change the MTU size in order to improve performance when sending large packets
	// This can only be called when not connected.
	// Returns false on failure (we are connected).  True on success.  Maximum allowed size is MAXIMUM_MTU_SIZE
	// A too high of value will cause packets not to arrive at worst and be fragmented at best.
	// A too low of value will split packets unnecessarily.
	// Set according to the following table:
	// 1500. The largest Ethernet packet size; it is also the default value.
	// This is the typical setting for non-PPPoE, non-VPN connections. The default value for NETGEAR routers, adapters and switches. 
	// 1492. The size PPPoE prefers. 
	// 1472. Maximum size to use for pinging. (Bigger packets are fragmented.) 
	// 1468. The size DHCP prefers. 
	// 1460. Usable by AOL if you don't have large email attachments, etc. 
	// 1430. The size VPN and PPTP prefer. 
	// 1400. Maximum size for AOL DSL. 
	// 576. Typical value to connect to dial-up ISPs. (Default)
	virtual bool SetMTUSize(int size)=0;

	// Returns the current MTU size
	virtual int GetMTUSize(void) const=0;

	// Network statistics.
	virtual double GetPacketlossPercentile(void)=0; // The average outgoing packetloss percentile
	virtual unsigned long GetTransmittedPacketCount(void)=0; // How many packets were sent
	virtual unsigned long GetTransmittedFrameCount(void)=0; // How many frames were sent	
	virtual unsigned long GetLostPacketCount(void)=0; // How many packets were lost
	virtual unsigned long GetReceivedPacketCount(void)=0; // How many packets were received
	virtual unsigned long GetBytesSent(void)=0; // How many bytes have been sent.
	virtual unsigned long GetBytesReceived(void)=0; // How many bytes have been received.
	virtual unsigned long GetUnacknowledgedSentPacketListSize(void)=0; // How many reliable packets are waiting for remote acknowledgement.  This is a debugging statistic.
	virtual unsigned long GetBytesSentPerSecond(void) const=0;  // How many bytes were sent in the last histogram block.
	virtual unsigned long GetBytesReceivedPerSecond(void) const=0; // How many bytes were received in the last histogram block.
	virtual unsigned long GetMaximumWindowSize(void)=0; // Returns the size of the flow control window, in bytes.  Smaller values correlate with higher throughput and less packetloss
	virtual unsigned long GetPacketOutputBufferSize(void)=0; // Returns the number of waiting packets on the send queue
	virtual unsigned long GetConnectionTime(void)=0; // Returns the time elapsed since you connected
};

#endif
