#include <windows.h>
#include "MainMenuState.h"
#include "User.h"
#include "CreditsState.h"
#include "InstructionsState.h"
#include "GameState.h"
#include "HostGameState.h"
#include "JoinGameState.h"
#include "ManagerState.h"
#include "InputSystem.h"
#include "..\Common\Include\d3dutil.h"
#include "ModelContainer.h"
#include "TextManager.h"
#include "RakServerInterface.h"
#include "RakClientInterface.h"
#include "SoundContainer.h"

extern RakServerInterface* rakServer;
extern RakClientInterface* rakClient;

#include "FileLoadedVisibleEntity.h"

//#include "MemoryManager.h"

static char* VERSION_TEXT = "Version .95";

MainMenuState MainMenuState::I;

MainMenuState::MainMenuState()
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::Load(void)
{
	// Lets load all the textWidgets
	singlePlayer.Load(0, 0, "Graphic\\JPG\\Buttons\\MainMenuSinglePlayerRed.jpg", "Graphic\\JPG\\Buttons\\MainMenuSinglePlayerBlue.jpg");
	hostGame.Load(0, 0, "Graphic\\JPG\\Buttons\\MainMenuHostGameRed.jpg", "Graphic\\JPG\\Buttons\\MainMenuHostGameBlue.jpg");
	joinGame.Load(0, 0, "Graphic\\JPG\\Buttons\\MainMenuJoinGameRed.jpg", "Graphic\\JPG\\Buttons\\MainMenuJoinGameBlue.jpg");
	instructions.Load(0, 0, "Graphic\\JPG\\Buttons\\MainMenuInstructionsRed.jpg", "Graphic\\JPG\\Buttons\\MainMenuInstructionsBlue.jpg");
	credits.Load(0, 0, "Graphic\\JPG\\Buttons\\MainMenuCreditsRed.jpg", "Graphic\\JPG\\Buttons\\MainMenuCreditsBlue.jpg");
	quit.Load(0, 0, "Graphic\\JPG\\Buttons\\MainMenuQuitRed.jpg", "Graphic\\JPG\\Buttons\\MainMenuQuitBlue.jpg");

	// Load the backdrop image
	backdrop.Load("Graphic\\JPG\\Backdrops\\MainMenuBackdrop.jpg");

	version.Create(12, "Arial", m_pd3dDevice);
}

void MainMenuState::Unload(void)
{
	backdrop.Free();
	singlePlayer.Unload();
	hostGame.Unload();
	joinGame.Unload();
	credits.Unload();
	instructions.Unload();
	quit.Unload();
	version.Unload();
}

void MainMenuState::Init(void)
{
	user->SetCamera(D3DXVECTOR3(300,0,0), D3DXVECTOR3(0,0,0));

	// Stop multiplayer
	rakClient->Disconnect();
	rakServer->Disconnect();
}

void MainMenuState::DeInit(void)
{
}

void MainMenuState::Render(long time)
{
	// Place the widgets centered down the screen
	int y=122 + ((int)manager->GetScreenHeight() - backdrop.GetHeight()) / 2;
	
	singlePlayer.SetLocation(((int)manager->GetScreenWidth()-singlePlayer.GetWidth())/2,y);
	y+=singlePlayer.GetHeight();

	hostGame.SetLocation(((int)manager->GetScreenWidth()-hostGame.GetWidth())/2,y);
	y+=hostGame.GetHeight();

	joinGame.SetLocation(((int)manager->GetScreenWidth()-joinGame.GetWidth())/2,y);
	y+=joinGame.GetHeight();
	
	instructions.SetLocation(((int)manager->GetScreenWidth()-instructions.GetWidth())/2,y);
	y+=instructions.GetHeight();

	credits.SetLocation(((int)manager->GetScreenWidth()-credits.GetWidth())/2,y);
	y+=credits.GetHeight();

	quit.SetLocation(((int)manager->GetScreenWidth()-quit.GetWidth())/2,y);
	//y+=quit.GetHeight();

	backdrop.Render(((int)manager->GetScreenWidth() - backdrop.GetWidth()) / 2, ((int)manager->GetScreenHeight() - backdrop.GetHeight()) / 2);
	singlePlayer.Render(time);
	hostGame.Render(time);
	joinGame.Render(time);
	instructions.Render(time);
	credits.Render(time);	
	quit.Render(time);

	int height, width;
	version.GetClippedTextDimensions(VERSION_TEXT, 5, 0, &width, &height);
	version.Render(VERSION_TEXT, 5, (int)manager->GetScreenHeight() - 5 - height, 255, 255, 255, 255, true);
}

void MainMenuState::Update(long time)
{
	/*
	if (singlePlayer.GetMouseStateIndex()==MOUSE_NOT_OVER)
		wasOverSinglePlayer=true;
	if (hostGame.GetMouseStateIndex()==MOUSE_NOT_OVER)
		wasOverHostGame=true;
	if (joinGame.GetMouseStateIndex()==MOUSE_NOT_OVER)
		wasOverJoinGame=true;
	if (credits.GetMouseStateIndex()==MOUSE_NOT_OVER)
		wasOverCredits=true;
	if (instructions.GetMouseStateIndex()==MOUSE_NOT_OVER)
		wasOverInstructions=true;
	if (quit.GetMouseStateIndex()==MOUSE_NOT_OVER)
		wasOverQuit=true;

			*/
	// Update (reset) all the textWidgets
	singlePlayer.Update(time);
	hostGame.Update(time);
	joinGame.Update(time);
	credits.Update(time);
	instructions.Update(time);
	quit.Update(time);

	if (singlePlayer.GetLastMouseStateIndex()==MOUSE_NOT_OVER && singlePlayer.GetMouseStateIndex()==MOUSE_OVER)
		directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);
	else if (hostGame.GetLastMouseStateIndex()==MOUSE_NOT_OVER && hostGame.GetMouseStateIndex()==MOUSE_OVER)
		directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);
	else if (joinGame.GetLastMouseStateIndex()==MOUSE_NOT_OVER && joinGame.GetMouseStateIndex()==MOUSE_OVER)
		directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);
	else if (credits.GetLastMouseStateIndex()==MOUSE_NOT_OVER && credits.GetMouseStateIndex()==MOUSE_OVER)
		directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);
	else if (instructions.GetLastMouseStateIndex()==MOUSE_NOT_OVER && instructions.GetMouseStateIndex()==MOUSE_OVER)
		directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);
	else if (quit.GetLastMouseStateIndex()==MOUSE_NOT_OVER && quit.GetMouseStateIndex()==MOUSE_OVER)
		directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);

/*
	if (singlePlayer.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
		if (inputSystem->MouseButtonPressed(0))
			singlePlayer.SetMousePressed();
		else
			singlePlayer.SetMouseOver();
	else
		singlePlayer.SetMouseNotOver();



	if (inputSystem->MouseButtonPressed(0))
	{	
		// Check to see if the mouse cursor is over each textWidget, and if so tell the textWidget so it can change its image
		if (singlePlayer.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
			singlePlayer.SetMousePressed();
		else if (hostGame.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
			hostGame.SetMousePressed();
		else if (joinGame.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
			joinGame.SetMousePressed();
		else if (credits.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
			credits.SetMousePressed();
		else if (instructions.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
			instructions.SetMousePressed();
		else if (quit.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
			quit.SetMousePressed();
	}
	else
	{
		// Check to see if the mouse cursor is over each textWidget, and if so tell the textWidget so it can change its image
		if (singlePlayer.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
		{
			if (singlePlayer.GetMouseStateIndex()==MOUSE_NOT_OVER)
				directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);

			singlePlayer.SetMouseOver();
		}
		else if (hostGame.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
		{
			if (hostGame.GetMouseStateIndex()==MOUSE_NOT_OVER)
				directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);

			hostGame.SetMouseOver();
		}
		else if (joinGame.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
		{
			if (joinGame.GetMouseStateIndex()==MOUSE_NOT_OVER)
				directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);

			joinGame.SetMouseOver();
		}
		else if (credits.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
		{
			if (credits.GetMouseStateIndex()==MOUSE_NOT_OVER)
				directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);

			credits.SetMouseOver();
		}
		else if (instructions.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
		{
			if (instructions.GetMouseStateIndex()==MOUSE_NOT_OVER)
				directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);

			instructions.SetMouseOver();
		}
		else if (quit.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
		{
			if (quit.GetMouseStateIndex()==MOUSE_NOT_OVER)
				directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);

			quit.SetMouseOver();
		}
	}
	*/
}

void MainMenuState::KeyJustPressed(int key)
{
	if (key==DIK_ESCAPE)
		SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
	else if (key==DIK_SPACE)
		manager->SetState(gameState);
}

void MainMenuState::KeyJustReleased(int key)
{
}

void MainMenuState::MouseButtonJustPressed(int button)
{
}

void MainMenuState::MouseButtonJustReleased(int button)
{
	if (button==0)
	{
		// Check to see if the mouse cursor is over each textWidget, and if so tell the textWidget so it can change its image
		if (singlePlayer.GetMouseStateIndex()==MOUSE_PRESSED)
			manager->SetState(gameState);
		else if (hostGame.GetMouseStateIndex()==MOUSE_PRESSED)
			manager->SetState(hostGameState);
		else if (joinGame.GetMouseStateIndex()==MOUSE_PRESSED)
			manager->SetState(joinGameState);
		else if (credits.GetMouseStateIndex()==MOUSE_PRESSED)
			manager->SetState(creditsState);
		else if (instructions.GetMouseStateIndex()==MOUSE_PRESSED)
			manager->SetState(instructionsState);
		else if (quit.GetMouseStateIndex()==MOUSE_PRESSED)
			SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
	}
}