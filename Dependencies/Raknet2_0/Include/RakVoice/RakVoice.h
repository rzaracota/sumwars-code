/* -*- mode: c++; c-file-style: raknet; tab-always-indent: nil; -*- */
/**
* @file
* @ingroup RAKNET_RAKVOICE
* @brief RakVoice class declaration 
*
* This file is part of RakNet Copyright 2003, 2004 Rakkarsoft LLC and
* Kevin Jenkins.
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
#ifndef __RAK_VOICE_H
#define __RAK_VOICE_H

#include "speex_bits.h"
#include "RakPeerInterface.h"
#include "NetworkTypes.h"
#include "Queue.h"
#include "SimpleMutex.h"
#include "ArrayList.h"
#include "RakVoiceInterface.h"

/**
* @ingroup RAKNET_RAKVOICE
* This is a float in speex 1.0.3 and a short in 1.1.x
*/
typedef short speex_encoding_type;
/**
* @ingroup RAKNET_RAKVOICE
* Size of the internal queue to hold sound data.  This should be written to the sound buffer at a fixed rate.
*/
#define RAK_VOICE_INPUT_LIST_BUFFER_SIZE 200

/**
* @ingroup RAKNET_RAKVOICE
* @brief Maintain association between communicating peer
*
* Each paired player permutation requires its own encoder and decoder
*/

struct CoderStateWithPlayerIDMapStruct
{
	/**
	* State of the decoder
	*/
	void *decoderState;
	/**
	* State of the encoder
	*/
	void *encoderState;
	/**
	* last time an operation occured for this association 
	*/
	unsigned long lastUsageTime;
	/**
	* Remote player id 
	*/
	PlayerID playerId;
	/**
	* Number of packet receive from the last decoding operation 
	*/
	unsigned short lastReceivedPacketNumber;
	/**
	* next packet number
	*/
	unsigned short nextPacketNumber;
};

/**
* @ingroup RAKNET_RAKVOICE
* @brief RakVoice Class declaration 
* 
* The RakVoice class provide services for the RakVoice subsystem. 
* You will not directly used RakVoice object. Instead you will use 
* RakVoiceInterface object provided by the RakVoiceFactory class. 
*/

class RakVoice : public RakVoiceInterface
{

public:
	/**
	* Default constructor
	*/
	RakVoice();
	/**
	* Destructor 
	*/
	virtual ~RakVoice();
	/**
	* Call this before using voice packets.  Use the server
	* version to send packets through the server, client version
	* to send packets through the client _blockSize is the size
	* of each block that you want to process at a time.  Each
	* network packet will have this size before compression.  It
	* must be a multiple of frame_size.  The best _blockSize is
	* what would be compressed to slightly under your MTU.  You
	* can safely assume the compression rate is 50% (actually
	* it's closer to 75%).
	*
	* @param samplingRate The rate to use, the higher the better
	* quality and the bigger the size of the data ...
	* @param bitsPerSample The number of bits used for one 
	* @param _peer RakPeer to use for communication it might be a
	* peer, a server or a client.
	*/
	void Init( int samplingRate, int bitsPerSample, RakPeerInterface *_peer );
	/**
	* Valid to call after a call to Init.  Returns the frame size
	* used by the encoder in bytes It is best to send input to
	* EncodeSoundData that matches this frame size
	*
	* @return the frame size used by the encoder in bytes
	*/
	int GetFrameSize( void ) const;
	/**
	* Whenever a player disconnects we need to know about it.
	* Otherwise we will be using old values for our encoding.
	* Passing an id that has never been used is ok, it will be
	* ignored.  
	*
	* @param id discard this player from the RakVoice
	* communication group.
	*/
	void Disconnect( PlayerID id );
	/**
	* Set the block size that EncodeSoundPacket will read and
	* GetSoundPacket will write.  If you don't call this, it will
	* default to GetFrameSize() You should only call this after
	* calling Init.  It is reset every call to Init.  This must
	* be a multiple of GetFrameSize().
	*
	* @param _blockSize The size of a RakVoice packet.
	*/
	void SetBlockSize( int _blockSize );
	/**
	* Call this before shutting down
	*/
	void Deinit( void );
	/**
	* When you have raw sound data, pass it to this function.
	* Input must be of size blockSize that you specified in Init
	* This will encode and send in another thread the data as a
	* packet Because of the way encoding works, you cannot
	* broadcast voice data.  You must specify a recipient If you
	* want to send to everyone, you have to call this once for
	* each recipient Use UNASSIGNED_PLAYER_ID to send to the
	* server (if you are a client).
	*
	* @param input a byte array of block size containing the raw voice data. 
	*
	* @param recipient the targeted peer or UNASSIGNED_PLAYER_ID
	* if you want to send to the server beeing a client.
	*/
	void EncodeSoundPacket( char *input, PlayerID recipient );
	/**
	* When you get a packet with the type ID_VOICE_PACKET, Pass
	* the data and length to this function.  This will decode the
	* data and put it in the internal queue, or simply relay the
	* data if This is the server and the target is not the server
	* 
	* @param data byte data from the Packet object 
	* @param length size of data from the Packet object too. 
	*/
	void DecodeAndQueueSoundPacket( char* data, int length );
	/**
	* This will get the next sound data packet and write it to output
	* Returns false if no packets are waiting.
	* The originator of the packet is written to sender
	* @param output The resulting raw voice data 
	* @param sender The sender of this data 
	* @return false if no packets are waiting 
	*/
	bool GetSoundPacket( char *output, PlayerID *sender );
	/**
	* Gives you the size, in bytes, of the next sound packet, or 0 for nothing left
	* @return the size, in bytes, of the next sound packet, or 0 for nothing left
	*/
	int GetNextSoundPacketSize( void );
	/**
	* This will tell you the total number of bytes in the output buffer
	* @return the number of remaining byte in the buffer. 
	*/
	int GetOutputBufferSize( void );

private:
	/**
	* Internal initialisation procedure 
	* @param samplingRate the rate to use 
	* @param bitsPerSample the bits to use per sample 
	*/
	void Init( int samplingRate, int bitsPerSample );
	/**
	* Creates encoders and decoders 
	*/
	CoderStateWithPlayerIDMapStruct* CreateCoderStateWithPlayerIDMapStruct( int samplingRate, PlayerID playerId, bool decoder );
	/**
	* Creates encoders and decoders 
	*/
	void *CreateCoderState( int samplingRate, bool decoder );

	/**
	* Retrieve the coder struct for a player
	*/
	CoderStateWithPlayerIDMapStruct *GetCoderFromPlayerID( unsigned short sr, PlayerID id, bool decoder );
	/**
	* true if a previous call to init occured 
	*/
	bool init;
	/**
	* Encoding calls are buffered to this input list
	* The thread will then parse this list to actually create the encoded data and send it through the network
	*/
	PlayerID targetedSendRecipient[ RAK_VOICE_INPUT_LIST_BUFFER_SIZE ];
	/**
	*/
	speex_encoding_type inputList[ RAK_VOICE_INPUT_LIST_BUFFER_SIZE ][ 160 ];
	/**
	* The position of the next writing operation 
	*/
	int writeCursor;
	/**
	* The position of the next reading operation 
	*/
	int readCursor;
	/**
	* bits per sample 
	*/
	unsigned char bps;
	/**
	* 
	*/
	SpeexBits bits;
	/**
	* Frame size 
	*/
	int frame_size;
	/**
	* @brief Raw Data to encode or to play 
	* 
	* Store raw information coming from the sound input device 
	* or going to the sound card 
	*/

	struct PCMDataStruct
	{
		/**
		* data the buffer array (of blockSize) 
		*/
		char *data;
		/**
		* id of the target or sender 
		*/
		PlayerID sender;
	};


#ifdef _WIN32
	/**
	* RakVoice Thread function to use 
	*/
	friend unsigned __stdcall rakVoiceThread( LPVOID arguments );
#else
	/**
	* RakVoice Thread function to use
	*/
	friend void* rakVoiceThread( void* arguments );
#endif
	/**
	* Store PCM Data
	*/
	BasicDataStructures::Queue<PCMDataStruct*> PCMQueue;
	/**
	* Pool of PCM Data ready to be used 
	*/
	BasicDataStructures::Queue<PCMDataStruct*> PCMQueuePool;
	/**
	* List of coder. Each party involved in the voice communication requires
	* one coder state 
	*/
	BasicDataStructures::List<CoderStateWithPlayerIDMapStruct*> coderStateList;
	/**
	* Prevent multiple access at the same time to PCM queue 
	*/
	SimpleMutex PCMQueueMutex;
	/**
	* Prevent multiple access to the PCM pool 
	*/
	SimpleMutex coderStateListMutex;
	/**
	* Communication end point to use 
	*/
	RakPeerInterface *peer;
	/**
	* ample rate 
	*/
	unsigned short sampleRate;
	/**
	* Must be a multiple of frame_size
	*/
	int blockSize;
};

#endif
