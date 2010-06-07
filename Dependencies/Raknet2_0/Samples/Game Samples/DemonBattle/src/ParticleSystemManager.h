#ifndef __PARTICLE_SYSTEM_MANAGER_H
#define __PARTICLE_SYSTEM_MANAGER_H

#include "LinkedList.h"

class ParticleSystem;

class ParticleSystemManager
{
public:
	ParticleSystemManager();
	~ParticleSystemManager();
	static ParticleSystemManager* Instance(void) {return &I;}
	// Call GetParticleSystem to get a particle system
	ParticleSystem* GetParticleSystem(void);
	void Unload(void);

	// Call ReleaseParticleSystem When you are done with your particle
	void ReleaseParticleSystem(ParticleSystem* pSystem);
private:
	static ParticleSystemManager I;
	BasicDataStructures::LinkedList<ParticleSystem*> usedParticleSystemList;
	BasicDataStructures::LinkedList<ParticleSystem*> unusedParticleSystemList;
};

#define particleSystemManager ParticleSystemManager::Instance()

#endif