#include "ParticleSystemManager.h"
#include "D3DHeaders.h"
#include "ParticleSystem.h"
#include "..\Common\Include\d3dutil.h"
#include <assert.h>

//#include "MemoryManager.h"

ParticleSystemManager ParticleSystemManager::I;

ParticleSystemManager::ParticleSystemManager()
{
}

ParticleSystemManager::~ParticleSystemManager()
{
	Unload();
}

void ParticleSystemManager::Unload(void)
{
	ParticleSystem *particle;

	// Free all the particles
	while (usedParticleSystemList.size())
	{
		particle = usedParticleSystemList.pop();
		particle->InvalidateDeviceObjects();
		delete particle;
	}
	while (unusedParticleSystemList.size())
	{
		particle = unusedParticleSystemList.pop();
		particle->InvalidateDeviceObjects();
		delete particle;
	}
}

// Call GetParticleSystem to get a particle system
ParticleSystem* ParticleSystemManager::GetParticleSystem(void)
{
	ParticleSystem* particle;

	if (unusedParticleSystemList.size()>0)
	{
		particle=unusedParticleSystemList.pop();
	}
	else
	{
		// Make a new particle system
		particle = new ParticleSystem( 512, 32768);
		HRESULT hr=particle->RestoreDeviceObjects(m_pd3dDevice);
		assert(hr==S_OK);
	}

	usedParticleSystemList.add(particle);
	//particle->RestoreDeviceObjects(m_pd3dDevice);
	return particle;
}

// Call ReleaseParticleSystem When you are done with your particle
void ParticleSystemManager::ReleaseParticleSystem(ParticleSystem* pSystem)
{
	// Just move lists
	bool found = usedParticleSystemList.find(pSystem);
	assert(found);
	if (found)
	{
//		pSystem->InvalidateDeviceObjects();
		unusedParticleSystemList.add(usedParticleSystemList.pop());
	}
}