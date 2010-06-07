#include "LightedSphere.h"
#include "..\Common\Include\d3dutil.h"

#include "LightSourceManager.h"

LightedSphere::LightedSphere()
{
	m_pSphereMesh=0;
	impactPointLightTimer=0.0f;
}

LightedSphere::~LightedSphere()
{
}

void LightedSphere::Init(void)
{
	impactPointLight->Activate();
	
	D3DXMatrixIdentity(&oMatrix);
}

void LightedSphere::Deinit(void)
{
	if (impactPointLight)
		impactPointLight->Deactivate();
}

void LightedSphere::Load(void)
{
	impactPointLight = lightSourceManager->GetLightSource();
	impactPointLight->lightProperties.Type=D3DLIGHT_POINT;
	
	impactPointLight->lightProperties.Diffuse=D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // RGBA
	impactPointLight->lightProperties.Ambient=D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // RGBA
	impactPointLight->lightProperties.Specular=D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // RGBA

	impactPointLight->lightProperties.Falloff=1.0f;
	//impactPointLight->lightProperties.Theta =D3DX_PI/16.0f;
	impactPointLight->lightProperties.Theta =0.0f;
	impactPointLight->lightProperties.Phi =D3DX_PI/8.0f;
	impactPointLight->lightProperties.Range=50.0f;
	impactPointLight->lightProperties.Attenuation0=0.0f;
//	impactPointLight->lightProperties.Attenuation1=1.0f;
	impactPointLight->lightProperties.Attenuation2=.0032f;
//	impactPointLight->lightProperties.Attenuation2=0.0f;
	
	/*
	 impactPointLight->lightProperties.Theta        = 0.5f;
            impactPointLight->lightProperties.Phi          = 1.0f;
            impactPointLight->lightProperties.Falloff      = 1.0f;
            impactPointLight->lightProperties.Attenuation0 = 1.0f;
	impactPointLight->lightProperties.Direction=D3DXVECTOR3(0.0f, -1.0, 0);
	*/
	impactPointLight->Update();
	// Create sphere and cone meshes to represent the lights
    D3DXCreateSphere(m_pd3dDevice,LIGHTED_SPHERE_RADIUS/2.0f, 20, 20, &m_pSphereMesh, NULL);

	//SetMaterial(1.0f,1.0f,1.0f,1.0f);

}

void LightedSphere::Unload(void)
{
	if ( m_pSphereMesh )
	{
		m_pSphereMesh->Release();
		m_pSphereMesh=0;
	}

	
	if (impactPointLight)
		lightSourceManager->ReleaseLightSource(impactPointLight);
	impactPointLight=0;
}

void LightedSphere::Render(long time)
{
	// Draw the sphere that indicates your impact point
	impactPointLight->lightProperties.Position.x = oMatrix._41;
	impactPointLight->lightProperties.Position.y = oMatrix._42 + LIGHTED_SPHERE_RADIUS + 10.0;
	impactPointLight->lightProperties.Position.z = oMatrix._43;

//	 m_pd3dDevice->SetTexture(0, modelContainer->textures[PARTICLE_TEXTURE_1] );
//	m_pd3dDevice->SetMaterial (&mat);

	VisibleEntity::Render(time);
	
	m_pSphereMesh->DrawSubset(0);
}

void LightedSphere::Update(long time)
{
	// Lets change the light color over time so it is easier to see
	impactPointLightTimer += time / 1000.0f;
	impactPointLight->lightProperties.Diffuse=D3DXCOLOR(1.0f, 0.0f, (float)(cos(impactPointLightTimer) + 1.0f) / 2.0f, 1.0f); // RGBA
	// Find the impact point of your fireball and move the impactPointLight there
	impactPointLight->Update();
}