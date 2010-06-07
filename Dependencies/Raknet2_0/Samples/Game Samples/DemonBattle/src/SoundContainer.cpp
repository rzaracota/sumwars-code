#include <assert.h>

#include "SoundContainer.h"
#include "DirectSound.h"
#include "SoundEnum.h"
#include <tchar.h>

//#include "MemoryManager.h"

SoundContainer SoundContainer::I;

SoundContainer::SoundContainer()
{
}

SoundContainer::~SoundContainer()
{
	UnloadAll();
}

void SoundContainer::LoadAll(void)
{
	int i;

	for (i=0; i < NUMBER_OF_SOUNDS; i++)
		LoadSound(i);
}

void SoundContainer::UnloadAll(void)
{
	int i;
	
	for (i=0; i < NUMBER_OF_SOUNDS; i++)
		UnloadSound(i);
}

void SoundContainer::LoadSound(int i)
{
	assert(i < NUMBER_OF_SOUNDS);

	// Load the sound
	sounds[i]=directSound->CreateSound((LPTSTR)soundPaths[i], _maximumConcurrentPlayInstances[i]);
}

void SoundContainer::UnloadSound(int i)
{
}
