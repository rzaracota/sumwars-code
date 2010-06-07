#ifndef __LIGHT_SOURCE_MANAGER_H
#define __LIGHT_SOURCE_MANAGER_H

#include "LinkedList.h"

class LightSource;

class LightSourceManager
{
public:
	LightSourceManager();
	~LightSourceManager();
	static LightSourceManager* Instance(void) {return &I;}
	// Call GetParticleSystem to get a particle system
	LightSource* GetLightSource(void);
	void Unload(void);

	// Call ReleaseParticleSystem When you are done with your particle
	void ReleaseLightSource(LightSource* light);
private:
	static LightSourceManager I;
	BasicDataStructures::LinkedList<LightSource*> usedLightSourceList;
	BasicDataStructures::LinkedList<LightSource*> unusedLightSourceList;
	int lightIndex;
};

#define lightSourceManager LightSourceManager::Instance()

#endif