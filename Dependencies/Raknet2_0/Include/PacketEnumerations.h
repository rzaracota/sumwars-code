/* -*- mode: c++; c-file-style: raknet; tab-always-indent: nil; -*- */
/**
 * @ingroup RALNET_MESSAGE_ID
 * @file 
 * @brief Define RakNet internal packet identifier 
 *
 * This file is part of RakNet Copyright 2003, 2004
 * Rakkarsoft LLC and Kevin Jenkins.
 *
 * Usage of Raknet is subject to the appropriate licence agreement.
 * "Shareware" Licensees with Rakkarsoft LLC are subject to the
 * shareware license found at
 * http://www.rakkarsoft.com/shareWareLicense.html which you agreed to
 * upon purchase of a "Shareware license" "Commercial" Licensees with
 * Rakkarsoft LLC are subject to the commercial license found at
 * http://www.rakkarsoft.com/sourceCodeLicense.html which you agreed
 * to upon purchase of a "Commercial license" All other users are
 * subject to the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * Refer to the appropriate license agreement for distribution,
 * modification, and warranty rights.
 */
#ifndef __PACKET_ENUMERATIONS_H
#define __PACKET_ENUMERATIONS_H 
/**
 * @defgroup RAKNET_MESSAGE_ID Message Identifier 
 * 
 * You should not edit the file PacketEnumerations.h as it is a part of RakNet static library
 * To define your own message id, define an enum following the code example that follows. 
 *
 * @code
 * enum {
 *   ID_MYPROJECT_MSG_1 = ID_RESERVED_9 + 1, 
 *   ID_MYPROJECT_MSG_2, 
 *    ... 
 * };
 * @endcode 
 *
 * @note RakNet define message ID as a 1 byte information. If you
 * require more than 256 message IDs do not reuse the function
 * Multiplayer::GetPacketIdentifier and replace the existing one by a
 * correct version of this function the version. In this case you can
 * integrate in the code of your project the file Multiplayer.h and
 * edit this file to fits your needs.
 */

enum {
	//
	// RESERVED TYPES - DO NOT CHANGE THESE
	//
	// Ignore these:
	ID_PING,   //!< 0: Ping (internal use only)
	ID_PING_OPEN_CONNECTIONS,   //!< 1: Only reply to the unconnected ping if we have open connections
	ID_REQUEST_STATIC_DATA,   //!< 2: Someone asked for our static data (internal use only)
	ID_CONNECTION_REQUEST,   //!< 3: Asking for a new connection (internal use only)
	ID_SECURED_CONNECTION_RESPONSE,   //!< 4: Connecting to a secured server/peer
	ID_SECURED_CONNECTION_CONFIRMATION,   //!< 5: Connecting to a secured server/peer
	ID_RPC,   //!< 6: Remote procedure call (internal use only)
	ID_BROADCAST_PINGS,   //!< 7: Server / Client only - The server is broadcasting the pings of all players in the game (internal use only)
	ID_SET_RANDOM_NUMBER_SEED,   //!< 8: Server / Client only - The server is broadcasting a random number seed (internal use only)
	ID_RPC_WITH_TIMESTAMP,   //!< 9: Same as RPC, but treat the first 4 bytes as a timestamp
	//Handle these below.  Possible recipients in [...]
	ID_PONG,   //!< [CLIENT|PEER] 10: Pong.  Returned if we ping a system we are not connected so.  First byte is ID_PONG, second 4 bytes is the ping, following bytes is system specific enumeration data.
	ID_RSA_PUBLIC_KEY_MISMATCH,   //!< [CLIENT|PEER] 11: We preset an RSA public key which does not match what the system we connected to is using.
	ID_REMOTE_DISCONNECTION_NOTIFICATION,   //!< [CLIENT] 12: In a client/server environment, a client other than ourselves has disconnected gracefully.  Packet::playerID is modified to reflect the playerID of this client.
	ID_REMOTE_CONNECTION_LOST,   //!< [CLIENT] 13: In a client/server environment, a client other than ourselves has been forcefully dropped. Packet::playerID is modified to reflect the playerID of this client.
	ID_REMOTE_NEW_INCOMING_CONNECTION,   //!< [CLIENT] 14: In a client/server environment, a client other than ourselves has connected.  Packet::playerID is modified to reflect the playerID of this client.
	ID_REMOTE_EXISTING_CONNECTION,   //!< [CLIENT] 15: On our initial connection to the server, we are told of every other client in the game.  Packet::playerID is modified to reflect the playerID of this client.
	ID_REMOTE_STATIC_DATA,   //!< [CLIENT] - 16: Got the data for another client
	ID_CONNECTION_BANNED,   //!< [PEER|CLIENT] 17: We are banned from the system we attempted to connect to.
	ID_CONNECTION_REQUEST_ACCEPTED,   //!< [PEER|CLIENT] 18: In a client/server environment, our connection request to the server has been accepted.
	ID_NEW_INCOMING_CONNECTION,   //!< [PEER|SERVER] 19: A remote system has successfully connected.
	ID_NO_FREE_INCOMING_CONNECTIONS,   //!< [PEER|CLIENT] 20: The system we attempted to connect to is not accepting new connections.
	ID_DISCONNECTION_NOTIFICATION,   //!< [PEER|SERVER|CLIENT] 21: The system specified in Packet::playerID has disconnected from us.  For the client, this would mean the server has shutdown.
	ID_CONNECTION_LOST,   //!< [PEER|SERVER|CLIENT] 22: Reliable packets cannot be delivered to the system specifed in Packet::playerID.  The connection to that system has been closed.
	ID_TIMESTAMP,   //!< [PEER|SERVER|CLIENT] 23: The four bytes following this byte represent an unsigned int which is automatically modified by the difference in system times between the sender and the recipient. Requires that you call StartOccasionalPing.
	ID_RECEIVED_STATIC_DATA,   //!< [PEER|SERVER|CLIENT] 24: We got a bitstream containing static data.  You can now read this data. This packet is transmitted automatically on connections, and can also be manually sent.
	ID_INVALID_PASSWORD,   //!< [PEER|CLIENT] 25: The remote system is using a password and has refused our connection because we did not set the correct password.
	ID_MODIFIED_PACKET,   //!< [PEER|SERVER|CLIENT] 26: A packet has been tampered with in transit.  The sender is contained in Packet::playerID.
	ID_REMOTE_PORT_REFUSED,   //!< [PEER|SERVER|CLIENT] 27: The remote host is not accepting data on this port.  This only comes up when connecting to yourself on the same computer and there is no bound socket on that port.
	ID_VOICE_PACKET,   //!< [PEER] 28: This packet contains voice data.  You should pass it to the RakVoice system.
	ID_UPDATE_DISTRIBUTED_NETWORK_OBJECT,   //!< [CLIENT|SERVER] 29: Indicates creation or update of a distributed network object.  Pass to DistributedNetworkObjectManager::Instance()->HandleDistributedNetworkObjectPacket
	ID_DISTRIBUTED_NETWORK_OBJECT_CREATION_ACCEPTED,   //!< [CLIENT] 30: Client creation of a distributed network object was accepted.  Pass to DistributedNetworkObjectManager::Instance()->HandleDistributedNetworkObjectPacketCreationAccepted
	ID_DISTRIBUTED_NETWORK_OBJECT_CREATION_REJECTED,   //!< [CLIENT] 31: Client creation of a distributed network object was rejected.  Pass to DistributedNetworkObjectManager::Instance()->HandleDistributedNetworkObjectPacketCreationRejected
	ID_AUTOPATCHER_REQUEST_FILE_LIST,   //!< [PEER|SERVER|CLIENT] 32: Request for a list of downloadable files. Pass to Autopatcher::SendDownloadableFileList
	ID_AUTOPATCHER_FILE_LIST,   //!< [PEER|SERVER|CLIENT] 33: Got a list of downloadable files. Pass to Autopatcher::OnAutopatcherFileList
	ID_AUTOPATCHER_REQUEST_FILES,   //!< [PEER|SERVER|CLIENT] 34: Request for a particular set of downloadable files. Pass to Autopatcher::OnAutopatcherRequestFiles
	ID_AUTOPATCHER_SET_DOWNLOAD_LIST,   //!< [PEER|SERVER|CLIENT] 35: Set the list of files that were approved for download and are incoming. Pass to Autopatcher::OnAutopatcherSetDownloadList
	ID_AUTOPATCHER_WRITE_FILE,   //!< [PEER|SERVER|CLIENT] 36: Got a file that we requested for download.  Pass to Autopatcher::OnAutopatcherWriteFile
	ID_QUERY_MASTER_SERVER,   //!< [MASTERSERVER] 37: Request to the master server for the list of servers that contain at least one of the specified keys
	ID_MASTER_SERVER_DELIST_SERVER,   //!< [MASTERSERVER] 38: Remove a game server from the master server.
	ID_MASTER_SERVER_UPDATE_SERVER,   //!< [MASTERSERVER|MASTERCLIENT] 39: Add or update the information for a server.
	ID_MASTER_SERVER_SET_SERVER,   //!< [MASTERSERVER|MASTERCLIENT] 40: Add or set the information for a server.
	ID_RELAYED_CONNECTION_NOTIFICATION,   //!< [MASTERSERVER|MASTERCLIENT] 41: This message indicates a game client is connecting to a game server, and is relayed through the master server.
	ID_ADVERTISE_SYSTEM,   //!< [PEER|SERVER|CLIENT] 42: Inform a remote system of our IP/Port.
	ID_RESERVED5,   //!< For future versions
	ID_RESERVED6,   //!< For future versions
	ID_RESERVED7,   //!< For future versions
	ID_RESERVED8,   //!< For future versions
	ID_RESERVED9,   //!< For future versions
	//-------------------------------------------------------------------------------------------------------------
        
        
	//
	// YOUR TYPES HERE!
	// WARNING - By default it is assumed that the packet identifier is one byte (unsigned char)
	// In the unlikely event that you need more than 256 types, including the built-in types, then you'll need
	// to request a special edition with larger identifiers, or change it yourself
	//
        
        
        
};

#endif
