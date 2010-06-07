#include "Fireball.h"
#include "D3DHeaders.h"
#include <assert.h>
#include "..\Common\Include\d3dutil.h"
#include "LightSource.h"
#include "LightSourceManager.h"

//#include "MemoryManager.h"

Fireball::Fireball()
{
	// The light in front of the enemy castle door
	lightSource = lightSourceManager->GetLightSource();
	lightSource->lightProperties.Type=D3DLIGHT_POINT;
	
//	lightSource->lightProperties.Diffuse=D3DXCOLOR(245.0f/255.0f, 252.0f/255.0f,78.0f/255.0f,.0f); // RGBA
	lightSource->lightProperties.Diffuse=D3DXCOLOR(.3,.3,.3,.3); // RGBA
	lightSource->lightProperties.Ambient=D3DXCOLOR(0,0,0,0); // RGBA
	lightSource->lightProperties.Specular=D3DXCOLOR(0,0,0,0); // RGBA

	lightSource->lightProperties.Falloff=1.0f;
	lightSource->lightProperties.Theta =D3DX_PI/16.0f;
	lightSource->lightProperties.Phi =D3DX_PI/8.0f;
	lightSource->lightProperties.Range=600.0f;
	lightSource->lightProperties.Attenuation0=0.0f;
	lightSource->lightProperties.Attenuation1=0.00f;
	lightSource->lightProperties.Attenuation2=.000032f;

	lightSource->lightProperties.Position.x=oMatrix._41;
	lightSource->lightProperties.Position.y=oMatrix._42;
	lightSource->lightProperties.Position.z=oMatrix._43;
	lightSource->Update();
	lightSource->Activate();
}

Fireball::~Fireball()
{
	lightSourceManager->ReleaseLightSource(lightSource);
}

void Fireball::Start(int Team)
{
	team = Team;
	Projectile::Start();
	//remoteParticleSystem->StartParticle(ParticleSystem::g_clrColor[COLOR_BLUE], ParticleSystem::g_clrColorFade[COLOR_BLUE], D3DXVECTOR3(oMatrix._41, oMatrix._42, oMatrix._43), velocity);
}

void Fireball::Render(long time)
{
	Projectile::Render(time);
	//remoteParticleSystem->DrawParticleReflections();
}

void Fireball::Update(long time)
{
	Projectile::Update(time);
	lightSource->lightProperties.Position.x=oMatrix._41;
	lightSource->lightProperties.Position.y=oMatrix._42;
	lightSource->lightProperties.Position.z=oMatrix._43;
	lightSource->Update();
}