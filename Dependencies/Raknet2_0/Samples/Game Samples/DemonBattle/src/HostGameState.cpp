#include "HostGameState.h"
#include "MainMenuState.h"
#include "ManagerState.h"
#include "InputSystem.h"
#include "..\Common\Include\d3dutil.h"
#include "ModelEnum.h"
#include "ModelContainer.h"
#include "RakServerInterface.h"
#include "RakClientInterface.h"
#include "PacketEnumerations.h"
#include "NetworkStructures.h"
#include "Multiplayer.h"
#include "DemonBattleTypes.h"
#include "GameState.h"
#include "User.h"
#include <assert.h>

//#include "MemoryManager.h"

extern RakServerInterface* rakServer;
extern RakClientInterface* rakClient;

HostGameState HostGameState::I;

HostGameState::HostGameState()
{
}

HostGameState::~HostGameState()
{
	Unload();
}

void HostGameState::Init(void)
{
	StartServer();
	
	user->userName.Init();
}

void HostGameState::DeInit(void)
{
	user->userName.Deinit();
}

void HostGameState::Render(long time)
{
	mainMenu.SetLocation(((int)manager->GetScreenWidth()-mainMenu.GetWidth())/2,((int)manager->GetScreenHeight()-mainMenu.GetHeight())/2);
	user->userName.SetLocation(((int)manager->GetScreenWidth()-mainMenu.GetWidth())/2,((int)manager->GetScreenHeight()+mainMenu.GetHeight())/2);

	backdrop.Render(((int)manager->GetScreenWidth() - backdrop.GetWidth()) / 2, ((int)manager->GetScreenHeight() - backdrop.GetHeight()) / 2);
	mainMenu.Render(time);
	user->userName.Render(time);
	statusBar.SetLocation(((int)manager->GetScreenWidth()-statusBar.GetWidth())/2,((int)manager->GetScreenHeight()-mainMenu.GetHeight()-5-statusBar.GetHeight()));
	statusBar.Render(time);
}

void HostGameState::Update(long time)
{
	D3DXVECTOR3 rayDir, rayOrg;
	POINT ptCursor;
	ptCursor.x=inputSystem->MouseX(); ptCursor.y=inputSystem->MouseY(); 
	GetClickPos (rayDir, rayOrg, ptCursor);

	// Update (reset) all the textWidgets
	mainMenu.Update(time);
	statusBar.Update(time);
	user->userName.Update(time);

	if (statusBar.GetLastMouseStateIndex()==MOUSE_PRESSED && statusBar.GetMouseStateIndex()==MOUSE_OVER && rakServer->IsActive()==false)
	{
		// Server is not running and we just released the mouse button over the status bar text
		StartServer();
	}

	/*
	if (inputSystem->MouseButtonPressed(0))
	{	
		// Check to see if the mouse cursor is over each textWidget, and if so tell the textWidget so it can change its image
		if (mainMenu.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
			mainMenu.SetMousePressed();
	}
	else
	{
		// Check to see if the mouse cursor is over each textWidget, and if so tell the textWidget so it can change its image
		if (mainMenu.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
			mainMenu.SetMouseOver();
	}
	*/
}

void HostGameState::Load(void)
{
	// Load the backdrop image
	backdrop.Load("Graphic\\JPG\\Backdrops\\HostGameBackdrop.jpg");

	// Lets load all the textWidgets
	mainMenu.Load(0, 0, 28 * user->GetResolutionMultiplier(), "Arial", m_pd3dDevice, MOUSE_NOT_OVER_BUTTON_COLOR, MOUSE_OVER_BUTTON_COLOR, MOUSE_PRESSED_BUTTON_COLOR, "Back to main menu");
	statusBar.Load(0, 0, 20 * user->GetResolutionMultiplier(), "Arial", m_pd3dDevice, MOUSE_NOT_OVER_BUTTON_COLOR, MOUSE_OVER_BUTTON_COLOR, MOUSE_PRESSED_BUTTON_COLOR, "Connection Pending");
}

void HostGameState::Unload(void)
{
	backdrop.Free();
	mainMenu.Unload();
	statusBar.Unload();
}

void HostGameState::KeyJustPressed(int key)
{
	bool handled = user->userName.KeyJustPressed(key);
	if (handled)
	{
		EnumerationDataStruct s;
		// Copy what we enetered into the TextInputWidget instance into the client multiplayer static data
		strncpy(s.serverName,user->userName.GetUserInput(),39);
		s.serverName[39]=0;
		
		if (s.serverName[0]==0)
		{
			strcpy(s.serverName, "Player");
		}

		rakServer->SetStaticServerData((char*)&s, sizeof(EnumerationDataStruct));
	}
	if (handled==false && key==DIK_ESCAPE)
		manager->SetState(mainMenuState);
}

void HostGameState::KeyJustReleased(int key)
{
	user->userName.KeyJustReleased(key);
}

void HostGameState::MouseButtonJustPressed(int button)
{
}

void HostGameState::MouseButtonJustReleased(int button)
{
	if (button==0)
	{
		// Check to see if the mouse cursor is over each textWidget, and if so tell the textWidget so it can change its image
		if (mainMenu.GetMouseStateIndex()==MOUSE_PRESSED)
			manager->SetState(mainMenuState);
	}
}

void HostGameState::StartServer(void)
{
	// Copy what we enetered into the TextInputWidget instance into the client multiplayer static data
	EnumerationDataStruct s;
	strncpy(s.serverName,user->userName.GetUserInput(),39);
	s.serverName[39]=0;

	if (s.serverName[0]==0)
	{
		strcpy(s.serverName, "Player");
	}
	rakServer->SetStaticServerData((char*)&s, sizeof(EnumerationDataStruct));

	#ifdef _EXE_SIZE_CHECKING
	rakServer->Start(1, manager->GetFileSize("DemonBattle.exe"), false, DEMON_BATTLE_SERVER_PORT);
	#else
	rakServer->Start(1, 0, false, DEMON_BATTLE_SERVER_PORT);
	#endif
	if (rakServer->IsActive())
	{
		char text[300];

		strcpy(text, "Server active on ");
		int numberOfAddresses=rakServer->GetNumberOfAddresses();
		for (int i=0; i < numberOfAddresses; i++)
		{
			strcat(text, rakServer->GetServerIP(i));
			if (i+1 < numberOfAddresses) // more to show
				strcat(text, ", ");
		}

		strcat(text, ". Waiting for players.");
		statusBar.SetText(text);
	}
	else
		statusBar.SetText("Error starting server.  Click here to try again.");
}

bool HostGameState::ProcessPacket(Packet* p, unsigned char packetIdentifier)
{
	if (packetIdentifier==ID_DISCONNECTION_NOTIFICATION)
	{
		statusBar.SetText("Client has disconnected.");
	}
	else if (packetIdentifier==ID_NEW_CLIENT)
	{
		statusBar.SetText("Client attempting to connect.");
	}
	else if (packetIdentifier==ID_RESPONSIVE_CLIENT)
	{
		manager->SetState(gameState);
	}
	else if (packetIdentifier==ID_CONNECTION_LOST)
	{
		statusBar.SetText("Connection lost from client.");
	}
	else if (packetIdentifier==ID_MODIFIED_PACKET)
	{
		statusBar.SetText("A modified packet arrived from this client (cheating client).");
		rakServer->Kick(p->playerId);
	}
	else
	{
		manager->LogWrite("Unknown packet type in HostGameState::ProcessPacket.  Type %i with length %i", packetIdentifier, p->length);

		assert(1);
	}

	return true;
}