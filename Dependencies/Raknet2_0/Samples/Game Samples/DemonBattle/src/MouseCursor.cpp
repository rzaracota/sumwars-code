#include "MouseCursor.h"
#include "InputSystem.h"
#include "User.h"
#include <assert.h>
#include "LightSource.h"
#include "LightSourceManager.h"
#include "TextManager.h"

//#include "MemoryManager.h"

MouseCursor::MouseCursor() : PROJECTION_DISTANCE(175.0f)
{
	lightSource=0;
	SetScale(.5f);
}

MouseCursor::~MouseCursor()
{
}

void MouseCursor::Load(int texture)
{
	SetRemoteTexture(modelContainer->textures[texture]);
	/*

	lightSource = lightSourceManager->GetLightSource();
	lightSource->lightProperties.Type=D3DLIGHT_SPOT;
	
	lightSource->lightProperties.Diffuse=D3DXCOLOR(.9f, .9f, .9f, 1.0f); // RGBA
	lightSource->lightProperties.Ambient=D3DXCOLOR(.9f, .9f, .9f, 1.0f); // RGBA
	lightSource->lightProperties.Specular=D3DXCOLOR(.9f, .9f, .9f, 1.0f); // RGBA

	lightSource->lightProperties.Falloff=1.0f;
	
	lightSource->lightProperties.Phi =D3DX_PI/8.0f;
	lightSource->lightProperties.Theta =lightSource->lightProperties.Phi;
	lightSource->lightProperties.Range=3000.0f;
	lightSource->lightProperties.Attenuation0=1.0f;
	lightSource->lightProperties.Attenuation1=0.00f;
	lightSource->lightProperties.Attenuation2=0.0f;

	lightSource->lightProperties.Position.x=oMatrix._41;
	lightSource->lightProperties.Position.y=oMatrix._42;
	lightSource->lightProperties.Position.z=oMatrix._43;
	lightSource->lightProperties.Direction=D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	lightSource->Update();


//	lightSource->Activate();
	lightSource->Deactivate();
*/
	SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);

	SetRemoteAnimation(&modelContainer->animations[CD2000_MODEL], &modelContainer->models[CD2000_MODEL]);
	remoteAnimation->ComputeMeshData(0L);
}

void MouseCursor::Unload(void)
{
	/*
	assert(lightSource);
	lightSourceManager->ReleaseLightSource(lightSource);
	*/
}

void MouseCursor::Update(long time)
{
	// Position the cursor and make it look around
	D3DXVECTOR3 rayDir, rayOrg;
	POINT ptCursor;
	ptCursor.x=inputSystem->MouseX(); ptCursor.y=inputSystem->MouseY(); 
	GetClickPos (rayDir, rayOrg, ptCursor);
	SetLocation(rayOrg.x + rayDir.x*PROJECTION_DISTANCE,rayOrg.y + rayDir.y*PROJECTION_DISTANCE,rayOrg.z + rayDir.z*PROJECTION_DISTANCE);
	LookAt(D3DXVECTOR3(user->GetCameraEye().x*.5f,user->GetCameraEye().y,0.0f));
	
	/*
	lightSource->lightProperties.Position.x=oMatrix._41;
	lightSource->lightProperties.Position.y=oMatrix._42;
	lightSource->lightProperties.Position.z=oMatrix._43;
	lightSource->lightProperties.Direction=D3DXVECTOR3(rayOrg.x + rayDir.x,rayOrg.y + rayDir.y,rayOrg.z + rayDir.z);
	lightSource->Update();
	*/
}

void MouseCursor::Render(long time)
{
	assert(remoteAnimation);
	assert(remoteAnimation->GetMeshDataPointer());

	// Update the animation for the mouse cursor
	remoteAnimation->ComputeMeshData(time);
	FileLoadedVisibleEntity::Render(time);
}

void MouseCursor::PlayAttackAnimation(void)
{
	remoteAnimation->SetSingleAnimation("attack", false);
}