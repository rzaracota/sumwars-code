#ifndef __LIGHT_SOURCE_H
#define __LIGHT_SOURCE_H

#include <windows.h>
#include <D3d9types.h>

class LightSource
{
public:
	virtual void AssignLightIndex(int LightIndex); // Call this first to assign a unique index number to this light.  Start at 0
	D3DLIGHT9 lightProperties; // Fill this out second
	virtual void Update(void); // Call this third to send the light properties to directX.  Call this every time the light properties changes
	virtual void Activate(void); // Call this fourth to turn the light on.  Call Deactivate to turn it off
	virtual void Deactivate(void); // Turns the light off.  Call Activate to turn it back on

	LightSource();
	~LightSource();
protected:
	int lightIndex;
};

#endif