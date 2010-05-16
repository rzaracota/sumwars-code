#include "LightSource.h"
#include "..\Common\Include\d3dutil.h"
#include <assert.h>

//#include "MemoryManager.h"

LightSource::LightSource()
{
	#ifdef _DEBUG
	lightIndex=-1;
	#endif
	ZeroMemory( &lightProperties, sizeof(D3DLIGHT9) );
}

LightSource::~LightSource()
{
}

void LightSource::Activate(void)
{
	assert(lightIndex>=0);
	if (m_pd3dDevice)
		m_pd3dDevice->LightEnable( lightIndex, TRUE);
}

void LightSource::Deactivate(void)
{
	if (m_pd3dDevice)
		m_pd3dDevice->LightEnable( lightIndex, FALSE);
}

void LightSource::AssignLightIndex(int LightIndex)
{
	assert(LightIndex>=0);
	lightIndex=LightIndex;
}

void LightSource::Update(void)
{
	assert(lightIndex>=0);  // Make sure the user filled out lightIndex first
	if (m_pd3dDevice)
		m_pd3dDevice->SetLight( lightIndex, &lightProperties );
}