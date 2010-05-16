#include "DirectSound.h"
#include <windows.h>
#include "ManagerState.h"
#include <assert.h>
#include <dxerr9.h>

////#include "MemoryManager.h"

extern HWND m_hWnd;

DirectSound DirectSound::I;

DirectSound::DirectSound()
{
	soundDisabled=true;
	soundManager=0;
}

DirectSound::~DirectSound()
{
	Unload();
}

void DirectSound::Load(void)
{
	HRESULT hr;
	soundManager = new CSoundManager();
	hr = soundManager->Initialize( m_hWnd, DSSCL_PRIORITY );
	if (FAILED(hr))
	{
		manager->LogWrite(DXGetErrorString9(hr));
		soundDisabled=true;
	}
	else
	{
		hr = soundManager->SetPrimaryBufferFormat( 8, 22050, 16 );
		if (FAILED(hr))
		{
			manager->LogWrite(DXGetErrorString9(hr));		
			soundDisabled=true;
		}
	}

	// We initialized correctly so enable the sound
	soundDisabled=false;
}

void DirectSound::Unload(void)
{
	for (unsigned long i=0; i < soundList.size(); i++)
	{
		soundList[i]->Stop();
		delete soundList[i];
	}

	soundList.clear();

	if (soundManager)
	{
		delete soundManager;
		soundManager=0;
	}
}

// Creates the sound specified and returns the array index you can use to access it
unsigned long DirectSound::CreateSound(LPTSTR filename, int maximumConcurrentPlayInstances)
{
	CSound *sound=0;
	assert(soundManager);
	assert(filename);
	HRESULT hr = soundManager->Create( &sound, filename, 0, GUID_NULL, maximumConcurrentPlayInstances);
	assert(!FAILED(hr));
	assert(sound);

	if (FAILED(hr) || sound==0)
	{
		if (FAILED(hr))
			manager->LogWrite(DXGetErrorString9(hr));
		soundDisabled=true;
	}
	
	soundList.insert(sound);
	return soundList.size()-1L;
}

void DirectSound::PlaySound(unsigned long soundIndex, bool looped)
{
	assert(soundManager);
	assert(soundIndex < soundList.size());

	if (soundDisabled==true)
		return;

	DWORD dwFlags = looped ? DSBPLAY_LOOPING : 0L;

	if (soundList[soundIndex])
		soundList[soundIndex]->Play( 0, dwFlags );
}