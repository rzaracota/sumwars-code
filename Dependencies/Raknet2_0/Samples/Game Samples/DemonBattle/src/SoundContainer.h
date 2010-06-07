#ifndef __SOUND_CONTAINER_H
#define __SOUND_CONTAINER_H

#include <windows.h>
#include "SoundEnum.h"
#include "DirectSound.h"

class SoundContainer
{
public:
	SoundContainer();
	~SoundContainer();
	static SoundContainer* Instance() {return &I;}
	void LoadAll(void);
	void UnloadAll(void);
	void LoadSound(int i);
	void UnloadSound(int i);

	int sounds[NUMBER_OF_SOUNDS];

private:
	static SoundContainer I;
};

#define soundContainer SoundContainer::Instance()

#endif