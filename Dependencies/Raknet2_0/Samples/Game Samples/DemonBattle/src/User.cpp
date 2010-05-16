#include "User.h"
#include "RakClientInterface.h"
#include "ManagerState.h"
#include "D3DHeaders.h"
#include "InputSystem.h"
#include "..\Common\Include\d3dutil.h"
#include "NetworkStructures.h"
#include "DemonBattleTypes.h"

#include <windows.h>
#include <D3dx9math.h>
#include <D3d9types.h>

//#include "MemoryManager.h"

User User::I;

extern RakClientInterface* rakClient;

// The server and the local player is team 1.  The remote client is team 2
int User::GetTeam(void)
{
	if (rakClient->IsConnected())
		return 2;
	else
		return 1;
}

int User::GetOppositeTeam(void)
{
	/*
	int team = GetTeam();
	if (team==2) return 1;
	else return 2;
	*/
	if (rakClient->IsConnected())
		return 1;
	else
		return 2;
}

void User::Update(long time)
{
	HandleInput(time);
	if (showMouseCursor)
		mouseCursor.Update(time);
}

void User::Render(long time)
{
	if (showMouseCursor)
		mouseCursor.Render(time);
}

void User::Load(void)
{
	mouseCursor.Load(CD2000_TEXTURE);

	FILE *fp;

	char tempUserName[200], tempLastClientIPAddress[200];
	
	userName.Load(0, 0, 20 * GetResolutionMultiplier(), "Arial", m_pd3dDevice, MOUSE_NOT_OVER_BUTTON_COLOR, MOUSE_OVER_BUTTON_COLOR, MOUSE_PRESSED_BUTTON_COLOR, 0);
	userName.SetPrefixText("Name: ");
	userName.SetDefaultText("Player");
	userName.SetMaximumTextLength(SCDS_MAXIMUM_NAME_LENGTH);

	clientIPAddress.Load(0, 0, 20 * GetResolutionMultiplier(), "Arial", m_pd3dDevice, MOUSE_NOT_OVER_BUTTON_COLOR, MOUSE_OVER_BUTTON_COLOR, MOUSE_PRESSED_BUTTON_COLOR, 0);
	clientIPAddress.SetPrefixText("IP Address: ");
	clientIPAddress.SetDefaultText("0.0.0.0");
	clientIPAddress.SetMaximumTextLength(24);

	// Load the player name and last used IP address
	if ((fp=fopen("multisettings.cfg", "rt")) != 0)
	{
		if (fgets(tempUserName, SCDS_MAXIMUM_NAME_LENGTH-1, fp)!=0)
		{
			// Kill the newline
			if (tempUserName[strlen(tempUserName)-1]=='\n')
				tempUserName[strlen(tempUserName)-1]=0;

			if (strlen(tempUserName)>0)
			{
				userName.SetText(tempUserName);
				if (fgets(tempLastClientIPAddress, 30, fp)!=0)
				{
					// Kill the newline
					if (tempLastClientIPAddress[strlen(tempLastClientIPAddress)-1]=='\n')
						tempLastClientIPAddress[strlen(tempLastClientIPAddress)-1]=0;

					if (strlen(tempLastClientIPAddress)>=7)
					{
						clientIPAddress.SetText(tempLastClientIPAddress);
					}
				}
			}
		}

		fclose(fp);
	}
}

void User::Unload(void)
{
	mouseCursor.Unload();

	FILE *fp;
	// Save the player name and last used IP address
	if ((fp=fopen("multisettings.cfg", "wt")) != 0 && userName.GetUserInput() && clientIPAddress.GetUserInput())
	{
		fputs(userName.GetUserInput(), fp);
		fputs("\n", fp);
		fputs(clientIPAddress.GetUserInput(), fp);
		fclose(fp);
	}

	userName.Unload();
	clientIPAddress.Unload();
}

void User::HandleInput(long time)
{
	inputSystem->UpdateMouse (time);
	inputSystem->UpdateKeyboard (time);

	for (int key=0; key < 255; key++)
	{
		if (inputSystem->KeyJustPressed(key))
			manager->GetState()->KeyJustPressed(key);
		else if (inputSystem->KeyJustReleased(key))
			manager->GetState()->KeyJustReleased(key);
	}

	for (int button=0; button < 3; button++)
	{
		if (inputSystem->MouseButtonJustPressed(button))
			manager->GetState()->MouseButtonJustPressed(button);
		else if (inputSystem->MouseButtonJustReleased(button))
			manager->GetState()->MouseButtonJustReleased(button);
	}
}

void User::SetCamera(D3DXVECTOR3 pos)
{
	camera.SetViewParams(pos, camera.GetLookatPt(), D3DXVECTOR3(0,1,0));
//	cameraPos = pos;
//	SetViewPos (cameraPos, lookAt);
	camera.SetViewMatrix();
}

void User::SetCamera(D3DXVECTOR3 pos, D3DXVECTOR3 LookAt)
{
	//cameraPos = pos;
	//lookAt=LookAt;
	camera.SetViewParams(pos, LookAt, D3DXVECTOR3(0,1,0));
	//SetViewPos (cameraPos, lookAt);
	camera.SetViewMatrix();
}

void User::GetCameraDirection(D3DXVECTOR3 *result)
{
	*result=camera.GetLookatPt() - camera.GetEyePt();
}

void User::SetCameraDirection(D3DXVECTOR3 *direction)
{
	//lookAt=cameraPos+*direction;
	camera.SetViewParams(camera.GetEyePt(), camera.GetEyePt() + *direction, D3DXVECTOR3(0,1,0));
	//SetViewPos (cameraPos, lookAt);
	camera.SetViewMatrix();
}

int User::GetResolutionMultiplier(void)
{
	return (int) manager->GetScreenWidth() / 640;
}

D3DXVECTOR3 const User::GetCameraLookAt(void) {return camera.GetLookatPt();}
D3DXVECTOR3 const User::GetCameraEye(void) {return camera.GetEyePt();}