#include "CreditsState.h"
#include "MainMenuState.h"
#include "ManagerState.h"
#include "InputSystem.h"
#include "..\Common\Include\d3dutil.h"
#include "ModelEnum.h"
#include "ModelContainer.h"
#include "User.h"

//#include "MemoryManager.h"

CreditsState CreditsState::I;

CreditsState::CreditsState()
{
}

CreditsState::~CreditsState()
{
	Unload();
}

void CreditsState::Init(void)
{
	user->ShowMouseCursor(false);

}

void CreditsState::DeInit(void)
{
	user->ShowMouseCursor(true);
}

void CreditsState::Render(long time)
{
	backdrop.Render(((int)manager->GetScreenWidth() - backdrop.GetWidth()) / 2, ((int)manager->GetScreenHeight() - backdrop.GetHeight()) / 2);
	//mainMenu.Render(time);
}

void CreditsState::Update(long time)
{


	// Update (reset) all the textWidgets
//	mainMenu.Update(time);
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

void CreditsState::Load(void)
{
	// Load the backdrop image
	backdrop.Load("Graphic\\JPG\\Backdrops\\CreditsBackdrop.jpg");

	/*
	// Lets load all the textWidgets
	mainMenu.Load(0, 0, 36, "Arial", m_pd3dDevice, MOUSE_NOT_OVER_BUTTON_COLOR, MOUSE_OVER_BUTTON_COLOR, MOUSE_PRESSED_BUTTON_COLOR, "Main Menu");
	mainMenu.SetLocation((int)manager->GetScreenWidth()-mainMenu.GetWidth())/2,((int)manager->GetScreenHeight()-mainMenu.GetHeight())/2);
	*/
}

void CreditsState::Unload(void)
{
	backdrop.Free();
//	mainMenu.Unload();
}

void CreditsState::KeyJustPressed(int key)
{
	if (key==DIK_ESCAPE)
		manager->SetState(mainMenuState);
}

void CreditsState::KeyJustReleased(int key)
{
}

void CreditsState::MouseButtonJustPressed(int button)
{
}

void CreditsState::MouseButtonJustReleased(int button)
{
	manager->SetState(mainMenuState);

	/*
	if (button==0)
	{
		// Check to see if the mouse cursor is over each textWidget, and if so tell the textWidget so it can change its image
		if (mainMenu.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
			manager->SetState(mainMenuState);
	}
	*/
}