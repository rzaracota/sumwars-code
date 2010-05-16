#ifndef __DIRECT_SOUND_H
#define __DIRECT_SOUND_H

#include "..\Common\Include\dsutil.h"
#include "ArrayList.h"

// Wrapper for the directsound wrapper.  You know Microsoft programming sucks when you need a wrapper class to use another wrapper class
class DirectSound
{
public:
	~DirectSound();
	static DirectSound* Instance(void) {return &I;}
	// Call this first
	void Load(void);
	// Call this last
	void Unload(void);

	// Creates the sound specified and returns the array index you can use to access it
	unsigned long CreateSound(LPTSTR filename, int maximumConcurrentPlayInstances);
	void PlaySound(unsigned long soundIndex, bool looped);
	
private:
	DirectSound();
	CSoundManager* soundManager;
	BasicDataStructures::List<CSound*> soundList;
	static DirectSound I;
	bool soundDisabled;
};

#define directSound DirectSound::Instance()

#endif