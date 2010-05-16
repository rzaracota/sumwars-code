#include "LightSourceManager.h"
#include "D3DHeaders.h"
#include "LightSource.h"
#include "..\Common\Include\d3dutil.h"
#include <assert.h>

//#include "MemoryManager.h"

LightSourceManager LightSourceManager::I;

LightSourceManager::LightSourceManager()
{
	lightIndex=0;
}

LightSourceManager::~LightSourceManager()
{
	Unload();
}

void LightSourceManager::Unload(void)
{
	LightSource *light;

	// Free all the lights
	while (usedLightSourceList.size())
	{
		light = usedLightSourceList.pop();
		light->Deactivate();
		delete light;
	}
	while (unusedLightSourceList.size())
	{
		light = unusedLightSourceList.pop();
		light->Deactivate();
		delete light;
	}
	lightIndex=0;
}

// Call GetLightSource to get a particle system
LightSource* LightSourceManager::GetLightSource(void)
{
	LightSource* light;

	if (unusedLightSourceList.size()>0)
	{
		light=unusedLightSourceList.pop();
	}
	else
	{
		// Make a new particle system
		light = new LightSource;
		light->AssignLightIndex(lightIndex++);
	}

	light->Activate();
	usedLightSourceList.add(light);
	//particle->RestoreDeviceObjects(m_pd3dDevice);
	return light;
}

// Call ReleaseLightSource When you are done with your particle
void LightSourceManager::ReleaseLightSource(LightSource* light)
{
	// Just move lists
	bool found = usedLightSourceList.find(light);
	assert(found);

//	pSystem->InvalidateDeviceObjects();
	unusedLightSourceList.add(usedLightSourceList.pop());
	light->Deactivate();
}