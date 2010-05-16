// ----------------------------------------------------------------------
// This API and the code herein created by and wholly and privately owned by Kevin Jenkins except where specifically indicated otherwise.
// Licensed under the "RakNet" brand by Rakkarsoft LLC and subject to the terms of the relevant licensing agreement available at http://www.rakkarsoft.com
// PacketPriority.h
// File created January 24, 2003
// Enums for packet priority and reliability levels.  Don't change this file
// ----------------------------------------------------------------------

#ifndef __PACKET_PRIORITY_H
#define __PACKET_PRIORITY_H

enum PacketPriority
{
	HIGH_PRIORITY,
	MEDIUM_PRIORITY,
	LOW_PRIORITY,
	NUMBER_OF_PRIORITIES
};

// Note to self: I write this with 3 bits in the stream!
enum PacketReliability
{
	UNRELIABLE,
	UNRELIABLE_SEQUENCED,
	RELIABLE,
	RELIABLE_ORDERED,
	RELIABLE_SEQUENCED
};

#endif
