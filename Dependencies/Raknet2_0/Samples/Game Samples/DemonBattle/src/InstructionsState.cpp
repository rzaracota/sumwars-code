#include "InstructionsState.h"
#include "MainMenuState.h"
#include "ManagerState.h"
#include "InputSystem.h"
#include "..\Common\Include\d3dutil.h"
#include "ModelEnum.h"
#include "ModelContainer.h"
#include "User.h"

//#include "MemoryManager.h"

InstructionsState InstructionsState::I;

InstructionsState::InstructionsState()
{
}

InstructionsState::~InstructionsState()
{
	Unload();
}

void InstructionsState::Init(void)
{
	user->ShowMouseCursor(false);
}

void InstructionsState::DeInit(void)
{
	user->ShowMouseCursor(true);
}

void InstructionsState::Render(long time)
{
	backdrop.Render(((int)manager->GetScreenWidth() - backdrop.GetWidth()) / 2, ((int)manager->GetScreenHeight() - backdrop.GetHeight()) / 2);
//	mainMenu.Render(time);
}

void InstructionsState::Update(long time)
{

/*
	// Update (reset) all the textWidgets
	mainMenu.Update(time);
	
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

void InstructionsState::Load(void)
{
	// Load the backdrop image
	backdrop.Load("Graphic\\JPG\\Backdrops\\InstructionsBackdrop.jpg");

	// Lets load all the textWidgets
//	mainMenu.Load(0, 0, 36, "Arial", m_pd3dDevice, MOUSE_NOT_OVER_BUTTON_COLOR, MOUSE_OVER_BUTTON_COLOR, MOUSE_PRESSED_BUTTON_COLOR, "Main Menu");
//	mainMenu.SetLocation(((int)manager->GetScreenWidth()-mainMenu.GetWidth())/2,((int)manager->GetScreenHeight()-mainMenu.GetHeight())/2);
}

void InstructionsState::Unload(void)
{
	backdrop.Free();
//	mainMenu.Unload();
}

void InstructionsState::KeyJustPressed(int key)
{
	if (key==DIK_ESCAPE)
		manager->SetState(mainMenuState);
}

void InstructionsState::KeyJustReleased(int key)
{
}

void InstructionsState::MouseButtonJustPressed(int button)
{
}

void InstructionsState::MouseButtonJustReleased(int button)
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