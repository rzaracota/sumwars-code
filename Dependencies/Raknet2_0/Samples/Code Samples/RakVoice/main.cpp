#include <stdio.h>
#include <stdlib.h>

#include "RakPeerInterface.h"
#include "RakVoice/RakVoiceInterface.h"
#include "RakNetworkFactory.h"
#include "RakVoice/RakVoiceFactory.h"
#include <conio.h>
#include <assert.h>
#include <memory.h>
#include "portaudio.h"

RakVoiceInterface* rakVoice;
RakPeerInterface *rakPeer;

/*
** Note that many of the older ISA sound cards on PCs do NOT support
** full duplex audio (simultaneous record and playback).
** And some only support full duplex at lower sample rates.
*/
#define SAMPLE_RATE          (8000)
#define NUM_SECONDS              (5)
#define SAMPLES_PER_FRAME        (1)
#define FRAME_SIZE					(160)
#define NUMBER_OF_BLOCKS					(10)
#define FRAMES_PER_BUFFER        (FRAME_SIZE*NUMBER_OF_BLOCKS)
#define BLOCK_SIZE (FRAMES_PER_BUFFER * SAMPLES_PER_FRAME)
#define SAMPLE_TYPE paInt8
#define SAMPLE_TYPE_SIZE 1
const int MAX_PLAYERS=4;

bool muted;
// Just so we can remember where the packet came from
bool isServer;

static int PACallback( void *inputBuffer, void *outputBuffer,
						  unsigned long framesPerBuffer,
						  PaTimestamp outTime, void *userData )
{
	static int i;
	static PlayerID sender;
	static PlayerID remoteSystems[8];
	unsigned short numberOfSystems;

	assert(BLOCK_SIZE==framesPerBuffer);

	// For testing - send to ourselves
	// rakVoice->EncodeSoundPacket((char*)inputBuffer,rakPeer->GetInternalID());

	if (!muted)
	{
		numberOfSystems=8;
		rakPeer->GetConnectionList(remoteSystems, &numberOfSystems);

		for (i=0; i < numberOfSystems; i++)
			// Since we are broadcasting, and EncodeSoundPacket has to specify a particular target, loop through everyone we know about.
			rakVoice->EncodeSoundPacket((char*)inputBuffer,remoteSystems[i]);
	}

	// Writes to outputBuffer if a sound packet is waiting.  Otherwise returns false and does nothing
	// Not using sender here, but if we had the interface for it we could use it show who is speaking
	if (rakVoice->GetSoundPacket((char*)outputBuffer, &sender)==false)
	{
		// If we didn't fill in anything, fill in silence
		memset(outputBuffer, 0, BLOCK_SIZE);
	}

	return 0;
}

#include "Multiplayer.h"
template <class InterfaceType>
class RakVoiceMultiplayer : public Multiplayer<InterfaceType>
{
public:
	void ReceiveVoicePacket(Packet *packet,InterfaceType *interfaceType);

	void ShowDialogText(char *str);
private:
};

template <class InterfaceType>
void RakVoiceMultiplayer<InterfaceType>::ReceiveVoicePacket(Packet *packet,InterfaceType *interfaceType)
{
	// Put a packet in the queue
	rakVoice->DecodeAndQueueSoundPacket((char*)packet->data, packet->length);
}

template <class InterfaceType>
void RakVoiceMultiplayer<InterfaceType>::ShowDialogText(char *str)
{
	printf("%s",str);
}


/*******************************************************************/
int main(void);
int main(void)
{
	int i;
	char ch;
	RakVoiceMultiplayer<RakPeerInterface> rakVoiceMultiplayer;
	rakPeer = RakNetworkFactory::GetRakPeerInterface();
	rakVoice = RakVoiceFactory::GetRakVoiceInterface();

	// Crude interface
	// Holds user data
	char ip[30], serverPort[30], clientPort[30];
	bool b;
	char c;

	// A client
	isServer=false;

	// Get our input
	puts("Enter the local listening port");
	gets(clientPort);
	
	b=rakPeer->Initialize(8, atoi(clientPort), 0);
	

	if (b)
		puts("Starting RakPeer");
	else
	{
		puts("Failed to start RakPeer.  Terminating.");
		exit(1);
	}

	rakPeer->SetMaximumIncomingConnections(8);
	

	rakVoice->Init(SAMPLE_RATE, 8, rakPeer);


	// Set the number of bytes we read and write at a time
	rakVoice->SetBlockSize(BLOCK_SIZE);

	assert(160==rakVoice->GetFrameSize());

	PaError    err;
	PortAudioStream *stream;
	err = Pa_Initialize();
	if( err != paNoError ) goto error;
	muted=false;

	/* Record some audio. -------------------------------------------- */
	err = Pa_OpenStream(
		&stream,
		Pa_GetDefaultInputDeviceID(),
		SAMPLES_PER_FRAME,
		SAMPLE_TYPE,
		NULL,
		Pa_GetDefaultOutputDeviceID(),
		SAMPLES_PER_FRAME,
		SAMPLE_TYPE,
		NULL,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,            /* frames per buffer */
		0,               /* number of buffers, if zero then use default minimum */
		0, /* paDitherOff, // flags */
		PACallback,
		0);
	if( err != paNoError ) goto error;

	err = Pa_StartStream( stream );
	if( err != paNoError ) goto error;
	printf("Now recording!!\n"); fflush(stdout);

	puts("Hit 'm' to toggle mute (default off).\nHit 'c' to connect to another system.\nHit 's' to show network statistics.\nHit 'p' to get the connected player list.\nHit 'q' to quit.");
	ch=0;
	while( ch!='q')
	{
		if (kbhit())
		{
			ch=getch();
			if (ch=='m')
			{
				muted=!muted;
				if (muted)
					puts("Microphone muted.");
				else
					puts("Microphone no longer muted.");
				ch=0;
			}
			else if (ch=='c')
			{
				puts("Enter IP to connect to");
				gets(ip);
				puts("Enter the port to connect to");
				gets(serverPort);
				// Connecting the client is very simple.  0 means we don't care about
				// a connectionValidationInteger, and false for low priority threads
				bool b = rakPeer->Connect(ip, atoi(serverPort),  0, 0);
				if (b==false)
					puts("Connection call failed");
				else
					puts("Connection call complete");

				c=0;
			}
			else if (ch=='p')
			{
				PlayerID remoteSystems[8];
				unsigned short numberOfSystems;

				numberOfSystems=8;
				rakPeer->GetConnectionList(remoteSystems, &numberOfSystems);

				if (numberOfSystems>0)
				{
					printf("Connected players:\n");
					for (i=0; i < numberOfSystems; i++)
						printf("%i. %s:%i\n", i+1, rakPeer->PlayerIDToDottedIP(remoteSystems[i]), remoteSystems[i].port);
				}
				else
					printf("Nobody connected.\n");				

				ch=0;
			}
			else if (ch=='s')
			{
				char buffer[2048];		
				StatisticsToString(rakPeer->GetStatistics(rakPeer->GetPlayerIDFromIndex(0)),buffer,1);
				printf("%s",buffer);
				ch=0;
			}
		}
		rakVoiceMultiplayer.ProcessPackets(rakPeer);
		Pa_Sleep(30);
	}

	err = Pa_CloseStream( stream );
	if( err != paNoError ) goto error;

	rakVoice->Deinit();
	RakVoiceFactory::DestroyRakVoiceInterface(rakVoice);
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);

	printf("Full duplex sound test complete.\n" );
	fflush(stdout);
	return 0;

error:
	Pa_Terminate();
	fprintf( stderr, "An error occured while using the portaudio stream\n" );
	fprintf( stderr, "Error number: %d\n", err );
	fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
	return -1;
}
