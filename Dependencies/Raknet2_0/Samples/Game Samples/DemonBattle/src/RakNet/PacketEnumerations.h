// ----------------------------------------------------------------------
// RakNet version 1.042
// Filename PacketEnumerations.h
// Created by Rakkar Software (rakkar@rakkarsoftware.com) January 24, 2003
// The various packet identifiers go here.  Do not touch the built in
// Identifiers.  Add your own to the bottom of the list
// ----------------------------------------------------------------------

#ifndef __PACKET_ENUMERATIONS_H
#define __PACKET_ENUMERATIONS_H

enum {
	//-------------------------------------------------------------------------------------------------------------
	//
	// RESERVED TYPES - DO NOT CHANGE THESE
	//
	ID_PING, // Ignore this
	ID_PONG, // Ignore this
	ID_SET_CLIENT_DATA, // Ignore this
	ID_DISCONNECTION_NOTIFICATION, // User terminates connection
	ID_CONNECTION_LOST, // Internet terminates connection :)
	ID_KICKED_BY_SERVER, // You were kicked out.  The network code will not stop the connection, that is up to you to do gracefully.
	ID_BANNED_FROM_SERVER, // For clients only
	ID_ENUMERATION_REQUEST, ID_ENUMERATION_REPLY,  // Used for getting connection data without actually connecting
	ID_SERVER_FULL,  // A message to both the game and the client that the server is full.  The network code will not stop the connection, that is up to you to do gracefully.
	ID_NEW_CLIENT, // A message to the game that someone has connected to a socket.  This client may or may not be responsive and may or may not be a valid player
	ID_TIMESTAMP, // Use this to indicate there is a timestamp.  Follow immediately with the timestamp
	ID_MODIFIED_PACKET,  // This is sent to the game to indicate an incoming packet was modified.  The player it is for is specified in the packet
	ID_SET_RANDOM_NUMBER_SEED, // Used to synchronize a random number between the server and the client
	ID_PLAYER_PINGS,  // Used by the server to periodically broadcast the pings of connected sockets
	ID_GET_STATIC_CLIENT_DATA, // Used by the server to request the static client data during handshaking
	ID_RESPONSIVE_CLIENT, // A message to the game that the client sent the static client data.  You can assume the client is valid and send your own data to it
	ID_CONNECTION_REQUEST, // Ignore this
	ID_CONNECTION_VERIFICATION, // Ignore this
 	ID_CONNECTION_ACCEPT, // Ignore this
	ID_SYNCHRONIZE_MEMORY, // Synchronize memory
	ID_RPC, // Remote procedure call
	ID_INVALID_PASSWORD, // Invalid Password
	ID_UNABLE_TO_CONNECT_TO_REMOTE_HOST, // Connection is blocked
	ID_DESIGNATE_REMOTE_PORT, // Ignore this
	ID_VOICE_PACKET, // A voice packet
	ID_RESERVED6, // For future versions
	ID_RESERVED7, // For future versions
	ID_RESERVED8, // For future versions
	ID_RESERVED9, // For future versions
 	//-------------------------------------------------------------------------------------------------------------
 	

	//
	// YOUR TYPES HERE!
	// WARNING - By default it is assumed that the packet identifier is one byte (unsigned char)
	// In the unlikely event that you need more than 256 types, including the built-in types, then you'll need
	// to request a special edition with larger identifiers, or purchase an open-source license and change it yourself
	//
	// Local system is the authority on when demons are summoned.  Timestamped
	ID_SUMMON_CREATURE,
	
	// Local system is the authority on demons dying from your own fireballs.  Local system ignores death from remote fireballs
	// Local system is the authority on demons dying from your own demons.  Local system ignores death from remote demons
	ID_KILL_CREATURE,
	
	// Local system is the authority on creatures changing teams due to local triggers (i.e. fireballs)
	// Local system ignores team changes by other team's triggers (i.e. fireballs)
	ID_SET_CREATURE_TEAM,
	
	// Local system is the authority on perks being chosen.
	ID_CHOOSE_PERK, 
	
	// Local system is the authority on fireballs being created
	ID_CREATE_FIREBALL,
	
	// Local system is the authority on your own castle's health.  This is broadcast once per second unless the value didn't change since
	// your last broadcast.  Also sent when you die.  Local systems ignore death to the enemy castle
	ID_SET_CASTLE_HEALTH,
	
	// No comment
	ID_CHAT_MESSAGE,
	
	// This is a single byte send so doesn't have an associated struct
	ID_RESTART_GAME,


};

#endif
