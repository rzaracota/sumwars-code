#include "JoinGameState.h"
#include "ModelContainer.h"
#include "MainMenuState.h"
#include "ManagerState.h"
#include "InputSystem.h"
#include "RakClientInterface.h"
#include "RakServerInterface.h"
#include "..\Common\Include\d3dutil.h"
#include "NetworkStructures.h"
#include "PacketEnumerations.h"
#include "GameState.h"
#include "User.h"
#include "Multiplayer.h"
#include "DemonBattleTypes.h"
#include <stdlib.h>
#include <assert.h>

//#include "MemoryManager.h"

extern RakServerInterface* rakServer;
extern RakClientInterface* rakClient;

JoinGameState JoinGameState::I;

JoinGameState::JoinGameState()
{
}

JoinGameState::~JoinGameState()
{
}

void JoinGameState::Init(void)
{
	mainMenu.SetLocation(((int)manager->GetScreenWidth()-mainMenu.GetWidth())/2,((int)manager->GetScreenHeight()-mainMenu.GetHeight())/2);
	user->userName.Init();
	user->userName.SetLocation(((int)manager->GetScreenWidth()-mainMenu.GetWidth())/2,((int)manager->GetScreenHeight()+mainMenu.GetHeight())/2);
	user->clientIPAddress.Init();
	user->clientIPAddress.SetLocation(((int)manager->GetScreenWidth()-mainMenu.GetWidth())/2,((int)manager->GetScreenHeight()+mainMenu.GetHeight()+user->userName.GetHeight()*2)/2);
	statusBar.SetText("Connect Now");
	connectionAttemptionTimer=-1L;
}

void JoinGameState::DeInit(void)
{
	user->userName.Deinit();
	user->clientIPAddress.Deinit();
}

void JoinGameState::Render(long time)
{
	backdrop.Render(((int)manager->GetScreenWidth() - backdrop.GetWidth()) / 2, ((int)manager->GetScreenHeight() - backdrop.GetHeight()) / 2);
	mainMenu.Render(time);

	user->userName.Render(time);
	user->clientIPAddress.Render(time);
	statusBar.SetLocation(((int)manager->GetScreenWidth()-statusBar.GetWidth())/2,((int)manager->GetScreenHeight()-mainMenu.GetHeight()-5-statusBar.GetHeight()));
	statusBar.Render(time);

	if (connectionAttemptionTimer >= 0L)
	{
		connectionAttemptionTimer+=time;

		char str[200];
		sprintf(str, "Attempting to connect to %s.  %i seconds elapsed.", tempConnectionIP, connectionAttemptionTimer/1000L);
		statusBar.SetText(str);
	}
}

void JoinGameState::Update(long time)
{
	// Update (reset) all the textWidgets
	mainMenu.Update(time);
	statusBar.Update(time);
	
	user->userName.Update(time);
	user->clientIPAddress.Update(time);
	
	if (statusBar.GetLastMouseStateIndex()==MOUSE_PRESSED && statusBar.GetMouseStateIndex()==MOUSE_OVER)
	{
		// User wants to start connection
		rakClient->Disconnect();
		connectionAttemptionTimer=-1L;
		if (VerifyIPString(user->clientIPAddress.GetUserInput())==false)
		{
			statusBar.SetText("Invalid IP.  Click to attempt new connection.");
		}
		else
		{
			ConnectClient();
		}
	}
}

void JoinGameState::Load(void)
{
	// Load the backdrop image
	backdrop.Load("Graphic\\JPG\\Backdrops\\JoinGameBackdrop.jpg");

	// Lets load all the textWidgets
	mainMenu.Load(0, 0, 28, "Arial", m_pd3dDevice, MOUSE_NOT_OVER_BUTTON_COLOR, MOUSE_OVER_BUTTON_COLOR, MOUSE_PRESSED_BUTTON_COLOR, "Back to main menu");
	statusBar.Load(0, 0, 22 * user->GetResolutionMultiplier(), "Arial", m_pd3dDevice, MOUSE_NOT_OVER_BUTTON_COLOR, MOUSE_OVER_BUTTON_COLOR, MOUSE_PRESSED_BUTTON_COLOR, "Connect Now");
}

void JoinGameState::Unload(void)
{
	backdrop.Free();
	mainMenu.Unload();
	statusBar.Unload();
}

void JoinGameState::KeyJustPressed(int key)
{
	bool handled = user->userName.KeyJustPressed(key);
	if (handled==false)
		user->clientIPAddress.KeyJustPressed(key);
	if (handled==false && key==DIK_ESCAPE)
		manager->SetState(mainMenuState);
}

void JoinGameState::KeyJustReleased(int key)
{
	user->userName.KeyJustReleased(key);
	user->clientIPAddress.KeyJustReleased(key);
}

void JoinGameState::MouseButtonJustPressed(int button)
{
}

void JoinGameState::MouseButtonJustReleased(int button)
{
	if (button==0)
	{
		// Check to see if the mouse cursor is over each textWidget, and if so tell the textWidget so it can change its image
		if (mainMenu.GetMouseStateIndex()==MOUSE_PRESSED)
			manager->SetState(mainMenuState);
	}
}

//------------------------------------------------------------
// Verify the IP string is of the form a.b.c.d where 0 <= a,b,c,d <=255
// If so, then true.  Otherwise returns false
//------------------------------------------------------------
bool JoinGameState::VerifyIPString(const char* IPString)
{
	if (IPString==0 || IPString[0]==0)
		return false;

	/*
	// Don't allow local connections
	if (strcmp(IPString, "0.0.0.0")==0 || strcmp(IPString, "127.0.0.1")==0)
		return false;
		*/

	int numberOfPeriods=0, consequtiveDigits=0, counter;

	for (counter=0; counter < 16 && (IPString[counter]!=0); counter++)
	{
		if (IPString[counter]=='.')
		{
			consequtiveDigits=0;
			if (counter==0)
				return false;
			if ((!(IsDigit(IPString[counter-1]))) || (!(IsDigit(IPString[counter+1]))))  
				return false;
			if (++numberOfPeriods==5)
				return false;
		}
		else if (IsDigit((IPString[counter])))
		{
			if (++consequtiveDigits == 4)
				return false;
		}
		else return false;
	}

	if (numberOfPeriods!=3)
		return false;

	unsigned char IP[4];
	IPStringToIPDigits(IPString, IP);

	for (counter=0; counter<4; counter++)
		if (IP[counter]>255)
			return false;

	return true;
}

bool JoinGameState::IsDigit(char c)
{
	return c >= '0' && c <='9';
}


//------------------------------------------------------------
// Derives the four bytes that comprise an IP String
//------------------------------------------------------------
void JoinGameState::IPStringToIPDigits(const char* IPString, unsigned char* IP)
{
	char IPTriple[4];
	int counter1, counter2, counter3;

	// Counter 1 increments for each IP triple.  Counter2 increments along the string.  Counter3 increments along the 3 characters of each triple
	for (counter1=0, counter2=0; counter1<4; counter1++)
	{
		counter3=0;

		while (IPString[counter2] != '.' && IPString[counter2] != 0)
		{
			// copy the digit
			IPTriple[counter3++]=IPString[counter2++];
		}

		IPTriple[counter3]=0; // Null terminate the string
		IP[counter1]=atoi(IPTriple);

		counter2++; // go past the '.'
	}
}

void JoinGameState::ConnectClient(void)
{
	rakClient->Disconnect();

	StaticClientDataStruct s;
	// Copy what we enetered into the TextInputWidget instance into the client multiplayer static data
	strncpy(s.name,user->userName.GetUserInput(),39);
	s.name[39]=0;
	rakClient->SetStaticClientData(UNASSIGNED_PLAYER_ID,(char*)&s, sizeof(StaticClientDataStruct));

	strcpy(tempConnectionIP, (char*)user->clientIPAddress.GetUserInput());
	#ifdef _EXE_SIZE_CHECKING
	bool b = rakClient->Connect(tempConnectionIP, DEMON_BATTLE_SERVER_PORT, DEMON_BATTLE_CLIENT_PORT, manager->GetFileSize("DemonBattle.exe"), false));
	#else
	bool b = rakClient->Connect(tempConnectionIP, DEMON_BATTLE_SERVER_PORT, DEMON_BATTLE_CLIENT_PORT, 0, false);
	#endif

	if (b==false)
		statusBar.SetText("Connection attempt failed");
	else
		connectionAttemptionTimer=0L;
}

bool JoinGameState::ProcessPacket(Packet* p, unsigned char packetIdentifier)
{
	// We got a packet so we know the connection is ok
	connectionAttemptionTimer=-1L;

	if (packetIdentifier==ID_ENUMERATION_REPLY)
	{
		manager->SetState(gameState);
	}
	else if (packetIdentifier==ID_KICKED_BY_SERVER)
	{
		statusBar.SetText("You were kicked out by the server.");
	}
	else if (packetIdentifier==ID_SERVER_FULL)
	{
		statusBar.SetText("This server already has a game in progress.");
	}
	else if (packetIdentifier==ID_CONNECTION_LOST)
	{
		statusBar.SetText("Connection lost.");
	}
	else if (packetIdentifier==ID_MODIFIED_PACKET)
	{
		statusBar.SetText("A modified packet arrived from this server (cheating server).");
		rakClient->Disconnect();
	}
	else
	{
		manager->LogWrite("Unknown packet type in JoinGameState::ProcessPacket.  Type %i with length %i", packetIdentifier, p->length);

		assert(1);
	}

	return true;
}