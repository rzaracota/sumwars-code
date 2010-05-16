#ifndef __USER_H
#define __USER_H

#include "Bitmap.h"
#include <D3dx9math.h>
#include "d3dutil.h"
#include "MouseCursor.h"
#include "TextInputWidget.h"

// Represents the human in front of the computer.  Handles stuff that has to do with the local player, like score and input.
class User
{
public:
	static inline User* Instance(void) {return &I;}
	// The server and the local player is team 1.  The remote client is team 2
	int GetTeam(void);
	int GetOppositeTeam(void);
	void Update(long time);
	void SetCamera(D3DXVECTOR3 pos);
	void SetCamera(D3DXVECTOR3 pos, D3DXVECTOR3 LookAt);
	D3DXVECTOR3 const GetCameraLookAt(void);
	D3DXVECTOR3 const GetCameraEye(void);
	void GetCameraDirection(D3DXVECTOR3 *result);
	void SetCameraDirection(D3DXVECTOR3 *direction);
	void Render(long time);
	void Load(void);
	void Unload(void);
	int GetResolutionMultiplier(void); // Returns how many times the screen width is larger than 640
	void ShowMouseCursor(bool b) {showMouseCursor=b;}

	~User() {}
	CD3DCamera camera;
	MouseCursor mouseCursor;
	TextInputWidget userName, clientIPAddress;
private:
	void HandleInput(long time);
	User() {showMouseCursor=true;}
	static User I;
	bool showMouseCursor;
};

#define user User::Instance()

#endif