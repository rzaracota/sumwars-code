#include "Multiplayer.h"
#include "RakServer.h"
#include "RakClient.h"
#include "RakNetworkFactory.h"
#include "BitStream.h"
using namespace RakNet;

// You can parse the input data in two ways.
// Either cast input to a struct (such as if you sent a struct)
// Or create a BitStream instance with input as data such as
// BitStream myBitStream(input, (numberOfBitsOfData-1)/8+1);
// where (numberOfBitsOfData-1)/8+1 is the number of bytes of data
// If you didn't pass any data then of course you don't do either
void serverRPC(char *input, int numberOfBitsOfData, PlayerID sender)
{
	if (input)
		printf("In serverRPC Message = %s\n", input);
	else
		printf("In serverRPC no input");
}

// You can parse the input data in two ways.
// Either cast input to a struct (such as if you sent a struct)
// Or create a BitStream instance with input as data such as
// BitStream myBitStream(input, (numberOfBitsOfData-1)/8+1);
// where (numberOfBitsOfData-1)/8+1 is the number of bytes of data
// If you didn't pass any data then of course you don't do either
void clientRPC(char *input, int numberOfBitsOfData, PlayerID sender)
{
	// We send the data as a bitstream.  However, we know the first 4 bytes is an unsigned int
	// That holds a timestamp.  Use the BitStream class to avoid 
	// cross-platform Endian problems in transmitting non-ASCII data.
	RakNet::BitStream tstamp(input, sizeof(unsigned int), false);
	unsigned int timestamp;
	tstamp.Read(timestamp);

	printf("In clientRPC. Time sent: %i. Message = %s\n", timestamp, input+sizeof(unsigned int));
}

int main(void)
{
	Multiplayer<RakClientInterface> rakClientMultiplayer;
	Multiplayer<RakServerInterface> rakServerMultiplayer;
	
	int state;
	unsigned int nextActionTime;

	RakClientInterface *rakClient=RakNetworkFactory::GetRakClientInterface();
	RakServerInterface *rakServer=RakNetworkFactory::GetRakServerInterface();
	
	state=0;
	nextActionTime=RakNet::GetTime()+1000;

	// Defined in NetworkTypes.h.
	// You can register a function anytime
	// Note that although you are passing a function pointer, the name you are passing matters.  The macro uses
	// this name as a unique identifier.  Don't do *f = clientRPC and then register f
	REGISTER_AS_REMOTE_PROCEDURE_CALL(rakClient, clientRPC);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(rakServer, serverRPC);

	rakServer->Start(32,0,30,2000);
	rakClient->Connect("127.0.0.1", 2000, 2100, 0, 30);
	
	printf("RPC demo\n");
	
	while (state<4)
	{
		rakClientMultiplayer.ProcessPackets(rakClient);
		rakServerMultiplayer.ProcessPackets(rakServer);
		
		if (RakNet::GetTime() > nextActionTime)
		{
			
			if (state==1)
			{
				// This shows the first form of RPC, which is just sending a char* and the number of bits
				// used.
	
				char str[200];
				printf("Enter any text string to show on the server:\n");
				gets(str);
				// Don't forget that RPC takes bits as the data length, not bytes!
				rakClient->RPC("serverRPC",str, (int)((strlen(str)+1)*8), HIGH_PRIORITY, RELIABLE, 0, false);
			}
			else if (state==2)
			{
				// This shows the second form of RPC, which is to send using a bitstream.  We pass
				// true to the last parameter of RPC, which means the first 4 bytes is a timestamp.
				// It will be modified to be system relative on the recipient
				BitStream bitStream;
				bitStream.Write(RakNet::GetTime()); // Write the system time to the first 4 bytes
				char str[200]; // Temporary buffer to hold a string
				printf("Enter any text string to show on the client:\n");
				gets(str);
				bitStream.Write(str, (int)strlen(str)+1); // Append the string to the bitstream
				rakServer->RPC("clientRPC",&bitStream, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true, true); // broadcast to everyone, which happens to be our one client
			}
			
			nextActionTime = RakNet::GetTime() + 1000;
			state++;
		}
	}

	printf("Example complete\n");
	
	// This is not necessary since on shutdown everything is unregistered.  This is just here to show usage
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(rakClient, clientRPC);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(rakServer, serverRPC);
	
	rakClient->Disconnect(0);
	rakServer->Disconnect(0);

	RakNetworkFactory::DestroyRakClientInterface(rakClient);
	RakNetworkFactory::DestroyRakServerInterface(rakServer);

	return 0;
}

